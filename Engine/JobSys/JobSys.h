#pragma once
#include <map>
#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>
#include "./HashedString.h"
#include "./JobRunner.h"
#include "./JobQueue.h"


using namespace Engine::JobSys;
using namespace Engine::Debugger;

//namespace Engine
//{
//using namespace JobSys;
//using namespace Debugger;




struct JobQueueManager
{
	JobQueue			jobQueue;
	JobStatus			jobStatus;
	vector<JobRunner*>	jobRunnerList;

	JobQueueManager(const string& queueName) : jobQueue(queueName)
	{}
};



class JobSystem
{
private:
	bool								stopRequested = false;
	map<HashedString, JobQueueManager*> jobQueueMap;

public:
	JobSystem() = default;
	~JobSystem() = default;

	/* @brief Create a default job queue with the queue name of "Default" and 2 job runner. */
	void Init();

	/* @brief Create a new job queue with the given name and return the hashed job queue name.
	 *		  The hashed name serves as a unique identifier for the new job queue. If a job 
	 *		  queue with the same name already exists, return the hashed name of the existed 
	 *		  job queue instead. */
	HashedString CreateQueue(const string& queueName, unsigned int runnerNum);

	/* @brief Add a job runner thread to the specified job queue. */
	void AddRunnerToQueue(JobQueueManager* manager);

	/* @brief Add a job runner thread to the specified job queue. Return true if the job queue
	 *		  exists and the adding is successful. Otherwise, return false. */
	bool AddRunnerToQueue(const HashedString& queueName);

	/* @brief Register a job to the specified job queue. Returen true if the job queue exists   
	 *		  and the adding is successful. Otherwise, return false. */
	bool AddJobToQueue(const HashedString& queueName, function<void()> jobFunction, const string& jobName = string());

	/* @brief Check if the specified job queue exists and has unfinished jobs. */
	bool IsQueueHasJobs(const HashedString& queueName);

	void RequestStop();
	bool IsStopped();

	HashedString GetDefaultQueue();

	JobQueueManager* GetQueue(const HashedString& queueName);
};


/********************************* Unit tests **************************************/

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
				"WAHOO " + to_string(num)
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
			string name;

			JobSysTester(const string& name) : name(name)
			{}
		};

		vector<JobSysTester*>* allTester = new vector<JobSysTester*>();
		vector<JobSysTester*>* newTester = new vector<JobSysTester*>();
		Engine::Mutex* mutex = new Engine::Mutex();
		int testerCount = 0;

		function<void()> adder = [mutex, newTester, &testerCount]() {
			Engine::ScopeLock lock(mutex);

			for (int i = 0; i < 7; i++)
			{
				DEBUG_PRINT("Creating Obj %d \n", testerCount);
				newTester->push_back(new JobSysTester("Obj " + to_string(testerCount)));
				testerCount++;
				Sleep(600);
			}
		};

		function<void()> mover = [mutex, allTester, newTester]() {
			Engine::ScopeLock lock(mutex);

			if (!newTester->empty())
			{
				for (vector<JobSysTester*>::iterator iter = newTester->begin(); iter != newTester->end(); iter++)
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
		HashedString queuName = jobSystem.CreateQueue("TesterLoader", 2);
		
		for (int i = 0; i < 5; i++)
		{
			jobSystem.AddJobToQueue(
				queuName,
				adder,
				"Add Tester, Batch: " + to_string(i)
			);

			jobSystem.AddJobToQueue(
				queuName,
				mover,
				"Move Tester, Batch: " + to_string(i)
			);
		}

		jobSystem.GetQueue("TesterLoader")->jobStatus.WaitForZeroJobsLeft();
		
		/* Clean up remaining data */ 
		mover();
		for (vector<JobSysTester*>::iterator iter = allTester->begin(); iter != allTester->end(); iter++)
		{
			delete (*iter);
		}

		delete allTester, newTester, mutex;
	}

	jobSystem.RequestStop();
}

#endif




//}//Namespace Engine








