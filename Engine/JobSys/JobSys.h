#pragma once
#include "Dependency.h"
#include "HashedString.h"
#include "HashedString.cpp"
#include "JobRunner.h"
#include "SharedJobQueue.h"

using namespace Engine::JobSystem;


struct JobQueueData
{
	SharedJobQueue		m_SharedQueue;
	std::vector<JobRunnerData*>	m_Runners;

	JobQueueData(const std::string& i_QueueName) :
		m_SharedQueue(i_QueueName)
	{
	}
};






void Init();

HashedString GetDefaultQueue();

inline const char* GetDefaultQueueName() noexcept {
	static char DefaultQueueName[] = "Default";
	return DefaultQueueName;
}

inline HashedString GetDefaultQueue() {
	return HashedString(GetDefaultQueueName());
}
HashedString CreateQueue(const std::string& i_Name, unsigned int i_numRunners);

void RunJob(const HashedString& i_QueueName, std::function<void()> i_JobFunction, JobStatus* i_pJobStatus = nullptr, const char* pJobName = nullptr);
bool HasJobs(const HashedString& i_QueueName);

void RequestShutdown();
bool ShutdownRequested();





/* Job system unit test */
void ProcessFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents, std::function<void(uint8_t*, size_t)> i_Processor);

class ProcessFile
{
public:
	ProcessFile(const char* i_pFilename, std::function<void(uint8_t*, size_t)> i_Processor, const HashedString i_QueueName = GetDefaultQueue(), JobStatus* i_pJobStatus = nullptr) :
		m_pFilename(i_pFilename),
		m_Processor(i_Processor),
		m_QueueName(i_QueueName),
		m_pJobStatus(i_pJobStatus)
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

					RunJob(
						m_QueueName,
						[pFileContents, sizeFileContents, Processor]()
					{
						ProcessFileContents(pFileContents, sizeFileContents, Processor);
					},
						m_pJobStatus,
						"ProcessFileContents"
						);
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
	JobStatus* m_pJobStatus;
};


void PrintOnInterval(std::string i_String, unsigned int i_IntervalMilliseconds, unsigned int i_Count);

void PrintFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents);

void BasicSample();

//void GameObjectSample();

#include "JobSys.inl"