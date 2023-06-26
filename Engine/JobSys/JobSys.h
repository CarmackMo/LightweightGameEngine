#pragma once
#include "Dependency.h"

#include <processthreadsapi.h>
#include "HashedString.h"
#include "HashedString.cpp"
#include "./JobRunner.h"
#include "./JobQueue.h"

using namespace std;
using namespace Engine::JobSystem;

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
};








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