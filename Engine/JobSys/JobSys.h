#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>

#include "./HashedString.h"
#include "./JobRunner.h"
#include "./JobQueue.h"
#include "Debugger.h"


namespace Engine
{

struct JobFlowManager
{
	bool isAuto;
	bool isTooMany = false;
	bool isTooFew  = false;

	static const DWORD interval = 100;
	static const uint32_t upperTHR = 25;

	JobFlowManager(bool isAuto) : isAuto(isAuto)
	{}

	~JobFlowManager() = default;
};


struct JobQueueManager
{
	JobSys::JobQueue				jobQueue;
	JobSys::JobStatus				jobStatus;
	std::vector<JobSys::JobRunner*>	jobRunnerList;
	JobFlowManager					jobFlowManager;

	JobQueueManager(const std::string& queueName, bool autoFlowControl) : 
		jobQueue(queueName),
		jobFlowManager(JobFlowManager(autoFlowControl))
	{}
};


/** TODO: @brief The job system will have one private job queue by default, to run flow 
 *		  controller routine. The default job queue has one job runner only.
 */
class JobSystem
{
private:
	bool												stopRequested = false;
	JobSys::HashedString								defaultQueueName;
	std::map<JobSys::HashedString, JobQueueManager*>	jobQueueMap;

	void JobFlowControl();

public:
	JobSystem() = default;
	~JobSystem() = default;

	/* @brief Create a default job queue with the queue name of "Default" and 2 job runner. */
	void Init();

	/* @brief Create a new job queue with the given name and return the hashed job queue name.
	 *		  The hashed name serves as a unique identifier for the new job queue. If a job 
	 *		  queue with the same hashed name already exists, return the hashed name directly
	 *		  instead. 
	 *		  A job queue must have at least one job runner. If the user creates a job queue 
	 *		  with a "runnerNum" value of 0, this function will automatically create a job runner. */
	JobSys::HashedString CreateQueue(const std::string& queueName, unsigned int runnerNum = 1, bool autoFlowControl = false);

	/* @brief Add a job runner thread to the specified job queue. */
	void AddRunnerToQueue(JobQueueManager* manager);

	/* @brief Add a job runner thread to the specified job queue. Return true if the job queue
	 *		  exists and the adding is successful. Otherwise, return false. */
	bool AddRunnerToQueue(const JobSys::HashedString& queueName);

	/* @brief Register a job to the specified job queue. Returen true if the job queue exists   
	 *		  and the adding is successful. Otherwise, return false. */
	bool AddJobToQueue(const JobSys::HashedString& queueName, std::function<void()> jobFunction, const std::string& jobName = std::string());

	/* @brief Remove the first job runner from the specified job queue. The job queue must have 
	 *		  at least one job runner; otherwise, the removal will have no effect and return 
	 *		  false. */
	bool RemoveRunnerFromQueue(JobQueueManager* manager);

	/* @brief Remove the first job runner from the specified job queue. The job queue must have 
	 *		  at least one job runner; otherwise, the removal will have no effect and return 
	 *		  false. Return true if the job queue exists and the removal is successful. 
	 *		  Otherwise, return false. */
	bool RemoveRunnerFromQueue(const JobSys::HashedString& queueName);

	/* @brief Remove the specified job queue from the job system. Return true if the job queue
	 *		  exists and the removal is successful. Otherwise, return false. */
	bool RemoveQueue(const JobSys::HashedString& queueName);

	/* @brief Get the specified job queue with given queue hashed name. Return a null pointer 
	 *		  if the job queue does not exist. */
	JobQueueManager* GetQueue(const JobSys::HashedString& queueName);

	/* @brief Check if the specified job queue exists and has unfinished jobs. */
	bool IsQueueHasJobs(const JobSys::HashedString& queueName);

	void RequestStop();
	bool IsStopped();
};



namespace JobSys
{
/********************************* Unit tests **************************************/

#if defined (_DEBUG)
#include "./Sync/Mutex.h"
#include "./Sync/ScopeLock.h"

inline void JobSystemUnitTest()
{
	JobSystem jobSystem;
	jobSystem.Init();

	/* Test 1: Testing non-blocking tasks in job system. Testing ordinary job system operations. */
	DEBUG_PRINT("Starting Test 1 \n");
	{
		JobSys::HashedString queueName = jobSystem.CreateQueue("SimulPrinter", 2, false);

		for (int num = 0; num < 4; num++)
		{
			/* Remove one job runner from default queue. Sleep 100ms to make sure won't remove 
			 * the job runner immediately before both runners has taken responsibility of at 
			 * least one job */
			if (num == 2)
			{
				Sleep(100);
				jobSystem.RemoveRunnerFromQueue(queueName);

			}

			/* Add jobs to job queue. */
			bool success = jobSystem.AddJobToQueue(
				queueName,
				[num]() {
					for (int i = 0; i < (6 + 4 * num); i++)
					{
						DEBUG_PRINT("$ Wahoo! No.%d $ \n", num);
						std::cout << "Wahoo! No." << num << "\n";
						Sleep(100);
					}
				},
				"WAHOO " + std::to_string(num)
			);

			assert(success == true);
		}

		jobSystem.GetQueue(queueName)->jobStatus.WaitForZeroJobsLeft();

		bool success = jobSystem.RemoveQueue(queueName);
		assert(success == true);
	}

	/* Test 2: Test blocking tasks in job system. Testing components: Mutex, ScopLock, etc. */
	DEBUG_PRINT("\n\nStarting Test 2 \n");
	{
		/* Prepare for testing data */
		struct JobSysTester
		{
			std::string name;

			JobSysTester(const std::string& name) : name(name)
			{}
		};

		std::vector<JobSysTester*>* allTester = new std::vector<JobSysTester*>();
		std::vector<JobSysTester*>* newTester = new std::vector<JobSysTester*>();
		Engine::Mutex* mutex = new Engine::Mutex();
		int testerCount = 0;

		std::function<void()> adder = [mutex, newTester, &testerCount]() {
			Engine::ScopeLock lock(mutex);

			for (int i = 0; i < 7; i++)
			{
				DEBUG_PRINT("Creating Obj %d \n", testerCount);
				newTester->push_back(new JobSysTester("Obj " + std::to_string(testerCount)));
				testerCount++;
				Sleep(50);
			}
		};

		std::function<void()> mover = [mutex, allTester, newTester]() {
			Engine::ScopeLock lock(mutex);

			if (!newTester->empty())
			{
				for (std::vector<JobSysTester*>::iterator iter = newTester->begin(); iter != newTester->end(); iter++)
				{
					DEBUG_PRINT("Moving Obj %s \n", (*iter)->name.c_str());
					allTester->push_back((*iter));
					Sleep(10);
				}

				newTester->clear();
			}
		};

		/* Proceed testing */
		JobSys::HashedString queueName = jobSystem.CreateQueue("TesterLoader", 2, false);
		
		for (int i = 0; i < 5; i++)
		{
			jobSystem.AddJobToQueue(
				queueName,
				adder,
				"Add Tester, Batch: " + std::to_string(i)
			);

			jobSystem.AddJobToQueue(
				queueName,
				mover,
				"Move Tester, Batch: " + std::to_string(i)
			);
		}

		jobSystem.GetQueue(queueName)->jobStatus.WaitForZeroJobsLeft();
		
		/* Clean up remaining data */ 
		mover();
		for (std::vector<JobSysTester*>::iterator iter = allTester->begin(); iter != allTester->end(); iter++)
		{
			delete (*iter);
		}

		delete allTester;
		delete newTester;
		delete mutex;
		bool success = jobSystem.RemoveQueue(queueName);
		assert(success == true);
	}

	/* Test 3: Test dynamic job flow controller. */
	DEBUG_PRINT("\n\nStarting Test 3 \n");
	{
		std::function<void()> job = []() {
			Sleep(150);
		};

		JobSys::HashedString queueName = jobSystem.CreateQueue("JobFlowController", 1, true);

		
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 35; j++)
			{
				jobSystem.AddJobToQueue(
					queueName, 
					job, 
					"Batch: " + std::to_string(i) + ", Job ID: " + std::to_string(j));
			}

			Sleep(3000);
		}

		jobSystem.GetQueue(queueName)->jobStatus.WaitForZeroJobsLeft();
	}

	jobSystem.RequestStop();
}

#endif


}//Namespace JobSys
}//Namespace Engine








