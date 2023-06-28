#pragma once
#include <map>
#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>
#include "./HashedString.h"
#include "./JobRunner.h"
#include "./JobQueue.h"
#include "Debugger.h"



namespace Engine
{

struct JobQueueManager
{
	JobSys::JobQueue				jobQueue;
	JobSys::JobStatus				jobStatus;
	std::vector<JobSys::JobRunner*>	jobRunnerList;

	JobQueueManager(const std::string& queueName) : jobQueue(queueName)
	{}
};



class JobSystem
{
private:
	bool												stopRequested = false;
	std::map<JobSys::HashedString, JobQueueManager*>	jobQueueMap;

public:
	JobSystem() = default;
	~JobSystem() = default;

	/* @brief Create a default job queue with the queue name of "Default" and 2 job runner. */
	void Init();

	/* @brief Create a new job queue with the given name and return the hashed job queue name.
	 *		  The hashed name serves as a unique identifier for the new job queue. If a job 
	 *		  queue with the same name already exists, return the hashed name of the existed 
	 *		  job queue instead. */
	JobSys::HashedString CreateQueue(const std::string& queueName, unsigned int runnerNum);

	/* @brief Add a job runner thread to the specified job queue. */
	void AddRunnerToQueue(JobQueueManager* manager);

	/* @brief Add a job runner thread to the specified job queue. Return true if the job queue
	 *		  exists and the adding is successful. Otherwise, return false. */
	bool AddRunnerToQueue(const JobSys::HashedString& queueName);

	/* @brief Register a job to the specified job queue. Returen true if the job queue exists   
	 *		  and the adding is successful. Otherwise, return false. */
	bool AddJobToQueue(const JobSys::HashedString& queueName, std::function<void()> jobFunction, const std::string& jobName = std::string());

	/* @brief Check if the specified job queue exists and has unfinished jobs. */
	bool IsQueueHasJobs(const JobSys::HashedString& queueName);

	void RequestStop();
	bool IsStopped();

	JobSys::HashedString GetDefaultQueue();

	JobQueueManager* GetQueue(const JobSys::HashedString& queueName);
};





/********************************* Unit tests **************************************/

namespace JobSys
{

#if defined (_DEBUG)
#include "./Sync/Mutex.h"
#include "./Sync/ScopeLock.h"

inline void JobSystemUnitTest()
{
	JobSystem jobSystem;
	jobSystem.Init();

	/* Test 1: Testing non-blocking tasks in job system. Testing ordinary job system operations. */
	DEBUG_PRINT("Starting Test 1");
	{

		for (int num = 0; num < 4; num++)
		{
			bool success = jobSystem.AddJobToQueue(
				jobSystem.GetDefaultQueue(),
				[num]() {
					for (int i = 0; i < (6 + 4 * num); i++)
					{
						DEBUG_PRINT("$ Wahoo! No.%d $ \n", num);
						std::cout << "Wahoo! No." << num << "\n";
						Sleep(400);
					}
				},
				"WAHOO " + std::to_string(num)
			);

			assert(success == true);
		}

		jobSystem.GetQueue(jobSystem.GetDefaultQueue())->jobStatus.WaitForZeroJobsLeft();
		
	}


	/* Test 2: Test blocking tasks in job system. Testing components: Mutex, ScopLock, etc. */
	DEBUG_PRINT("\n\nStarting Test 2");
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
				Sleep(600);
			}
		};

		std::function<void()> mover = [mutex, allTester, newTester]() {
			Engine::ScopeLock lock(mutex);

			if (!newTester->empty())
			{
				for (std::vector<JobSysTester*>::iterator iter = newTester->begin(); iter != newTester->end(); iter++)
				{
					//Engine::Debugger::DEBUG_PRINT("Moving Obj: %s \n", (*iter)->name.c_str());
					DEBUG_PRINT("Moving Obj \n");
					allTester->push_back((*iter));
					Sleep(100);
				}

				newTester->clear();
			}
		};

		/* Proceed testing */
		JobSys::HashedString queuName = jobSystem.CreateQueue("TesterLoader", 2);
		
		for (int i = 0; i < 5; i++)
		{
			jobSystem.AddJobToQueue(
				queuName,
				adder,
				"Add Tester, Batch: " + std::to_string(i)
			);

			jobSystem.AddJobToQueue(
				queuName,
				mover,
				"Move Tester, Batch: " + std::to_string(i)
			);
		}

		jobSystem.GetQueue("TesterLoader")->jobStatus.WaitForZeroJobsLeft();
		
		/* Clean up remaining data */ 
		mover();
		for (std::vector<JobSysTester*>::iterator iter = allTester->begin(); iter != allTester->end(); iter++)
		{
			delete (*iter);
		}

		delete allTester, newTester, mutex;
	}

	jobSystem.RequestStop();
}

#endif

}



}//Namespace Engine








