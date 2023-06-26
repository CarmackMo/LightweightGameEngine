#pragma once
#include <map>
#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>
#include "./HashedString.h"
#include "./JobRunner.h"
#include "./JobQueue.h"

using namespace std;
using namespace Engine;
using namespace Engine::JobSys;


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


#if defined (_DEBUG)
#include "./Sync/Mutex.h"
#include "./Sync/ScopeLock.h"

inline void JobSystemUnitTest()
{
	JobSystem jobSystem;
	jobSystem.Init();

	/* Test 1: Testing non-blocking tasks in job system. Testing ordinary job system operations. */
	Engine::Debugger::DEBUG_PRINT("Starting Test 1");
	{

		for (int num = 0; num < 4; num++)
		{
			bool success = jobSystem.AddJobToQueue(
				jobSystem.GetDefaultQueue(),
				[num]() {
					for (int i = 0; i < (6 + 4 * num); i++)
					{
						Engine::Debugger::DEBUG_PRINT("$ Wahoo! No.%d $ \n", num);
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
	Engine::Debugger::DEBUG_PRINT("\n\nStarting Test 2");
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
		Engine::Mutex* mutex = new Mutex();
		int testerCount = 0;

		function<void()> adder = [mutex, newTester, &testerCount]() {
			Engine::ScopeLock lock(mutex);

			for (int i = 0; i < 7; i++)
			{
				Engine::Debugger::DEBUG_PRINT("Creating Obj %d \n", testerCount);
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
					Engine::Debugger::DEBUG_PRINT("Moving Obj \n");
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

		delete allTester, newTester, mutex, testerCount;
	}

	jobSystem.RequestStop();
}
#endif







void Init();

void AddRunnerToQueue(JobQueueManager& i_QueueData);

bool AddRunnerToQueue(const HashedString& queueName);

HashedString GetDefaultQueue();

inline const char* GetDefaultQueueName() noexcept;

HashedString CreateQueue(const std::string& i_Name, unsigned int i_numRunners);



bool AddJobToQueue(const HashedString& queueName, function<void()> jobFunction, string jobName = string());
//void RunJob(const HashedString& queueName, function<void()> jobFunction, JobStatus* jobStatus = nullptr, string jobName = string());
bool HasJobs(const HashedString& i_QueueName);

void RequestShutdown();
bool ShutdownRequested();





/* Job system unit test */
void ProcessFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents, std::function<void(uint8_t*, size_t)> i_Processor);

class ProcessFile
{
public:
	ProcessFile(const char* i_pFilename, std::function<void(uint8_t*, size_t)> i_Processor, const HashedString i_QueueName = GetDefaultQueue()) :
		m_pFilename(i_pFilename),
		m_Processor(i_Processor),
		m_QueueName(i_QueueName)
	{
		assert(m_pFilename);
	}

	void operator() ()
	{
		if (m_pFilename)
		{
			size_t sizeFileContents = 0;
			uint8_t* pFileContents = LoadFile(m_pFilename, sizeFileContents);

			if (pFileContents && sizeFileContents)
			{
				if (!ShutdownRequested())
				{
					std::cout << "ProcessFile finished loading file.\n";

					// this works around C++11 issue with capturing member variable by value
					std::function<void(uint8_t*, size_t)> Processor = m_Processor;


					AddJobToQueue(
						m_QueueName,
						[pFileContents, sizeFileContents, Processor]() {
						ProcessFileContents(pFileContents, sizeFileContents, Processor); },
						"ProcessFileContents"
					);


					//RunJob(
					//	m_QueueName,
					//	[pFileContents, sizeFileContents, Processor]()
					//	{
					//		ProcessFileContents(pFileContents, sizeFileContents, Processor);
					//	},
					//	m_pJobStatus,
					//	"ProcessFileContents"
					//	);
				}
				else
				{
					delete[] pFileContents;
				}
			}
		}
	}

	static uint8_t* LoadFile(const char* i_pFilename, size_t& o_sizeFile)
	{
		assert(i_pFilename != NULL);

		FILE* pFile = NULL;

		errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
		if (fopenError != 0)
			return NULL;

		assert(pFile != NULL);

		int FileIOError = fseek(pFile, 0, SEEK_END);
		assert(FileIOError == 0);

		long FileSize = ftell(pFile);
		assert(FileSize >= 0);

		FileIOError = fseek(pFile, 0, SEEK_SET);
		assert(FileIOError == 0);

		uint8_t* pBuffer = new uint8_t[FileSize];
		assert(pBuffer);

		size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
		assert(FileRead == FileSize);

		fclose(pFile);

		o_sizeFile = FileSize;

		return pBuffer;
	}

private:
	const char* m_pFilename;
	std::function<void(uint8_t*, size_t)>  m_Processor;
	HashedString m_QueueName;
};


void PrintOnInterval(std::string i_String, unsigned int i_IntervalMilliseconds, unsigned int i_Count);

void PrintFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents);

void BasicSample();

//void GameObjectSample();

#include "JobSys.inl"