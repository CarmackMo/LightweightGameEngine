#include "JobSys.h"


bool bShutdownRequested = false;
std::map<HashedString, JobQueueData*>	Queues;




void RequestShutdown()
{
	Engine::Debugger::DEBUG_PRINT("Job System: Shutting down.");

	bShutdownRequested = true;

	std::vector<HANDLE>	AllThreads;

	std::map<HashedString, JobQueueData*>::iterator iter = Queues.begin();
	while (iter != Queues.end())
	{
		if (iter->second)
		{
			iter->second->m_SharedQueue.RequestStop();

			const size_t count = iter->second->m_Runners.size();
			for (size_t i = 0; i < count; i++)
			{
				JobRunnerData* pRunner = iter->second->m_Runners[i];
				if (pRunner && pRunner->m_ThreadHandle != NULL)
				{
					AllThreads.push_back(pRunner->m_ThreadHandle);
				}
			}
		}
		++iter;
	}

	Engine::Debugger::DEBUG_PRINT("Job System: Waiting for Queue runner threads to shut down.");

	DWORD result = WaitForMultipleObjects(static_cast<DWORD>(AllThreads.size()), &AllThreads[0], TRUE, INFINITE);
	assert(result == WAIT_OBJECT_0);

	iter = Queues.begin();
	while (iter != Queues.end())
	{
		if (iter->second)
		{
			const size_t count = iter->second->m_Runners.size();
			for (size_t i = 0; i < count; i++)
			{
				JobRunnerData* pRunner = iter->second->m_Runners[i];
				if (pRunner)
					delete pRunner;
			}

			delete iter->second;
		}

		++iter;
	}

	Queues.clear();

}


bool ShutdownRequested()
{
	return bShutdownRequested;
}


void AddRunner(JobQueueData& i_QueueData)
{
	size_t runners = i_QueueData.m_Runners.size();

	JobRunnerData* pNewRunner = new JobRunnerData(i_QueueData.m_SharedQueue);

#ifdef _DEBUG
	const size_t	sizeThreadName = 32;
	char			ThreadName[sizeThreadName];
	sprintf_s(ThreadName, sizeThreadName, "%s %d", i_QueueData.m_SharedQueue.GetName().c_str(), int(runners + 1));
	pNewRunner->m_Input.m_ThreadName = ThreadName;
#endif

	pNewRunner->m_ThreadHandle = CreateThread(NULL, 0, JobRunner, &pNewRunner->m_Input, CREATE_SUSPENDED, &pNewRunner->m_ThreadID);
	assert(pNewRunner->m_ThreadHandle != NULL);

	i_QueueData.m_Runners.push_back(pNewRunner);
	ResumeThread(pNewRunner->m_ThreadHandle);
}


void AddRunner(const HashedString& i_QueueName)
{
	std::map<HashedString, JobQueueData*>::iterator existing = Queues.find(i_QueueName);
	assert(existing != Queues.end());
	assert(existing->second);

	AddRunner(*existing->second);
}


HashedString CreateQueue(const std::string& i_Name, unsigned int i_numRunners)
{
	HashedString	HashedName(i_Name.c_str());

	assert(Queues.find(HashedName) == Queues.end());

	JobQueueData* pNewJobQueueData = new JobQueueData(i_Name);

	Queues[HashedName] = pNewJobQueueData;

	for (unsigned int i = 0; i < i_numRunners; i++)
		AddRunner(*pNewJobQueueData);

	return HashedName;
}


bool HasJobs(const HashedString& i_QueueName)
{
	std::map<HashedString, JobQueueData*>::iterator existing = Queues.find(i_QueueName);
	if (existing != Queues.end())
	{
		if (existing->second)
			return existing->second->m_SharedQueue.HasJobs();
	}

	return false;
}


void RunJob(const HashedString& i_QueueName, std::function<void()> i_JobFunction, JobStatus* i_pJobStatus, const char* i_pJobName)
{
	std::map<HashedString, JobQueueData*>::iterator existing = Queues.find(i_QueueName);
	assert(existing != Queues.end());
	assert(existing->second);

	Engine::Debugger::DEBUG_PRINT("Job System: Adding Job to Queue \"%s\". \n", existing->second->m_SharedQueue.GetName().c_str());
	existing->second->m_SharedQueue.Add(new Job(i_JobFunction, i_QueueName, i_pJobName ? i_pJobName : std::string(), i_pJobStatus));
}


void Init()
{
	CreateQueue(GetDefaultQueueName(), 2);
}










void ProcessFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents, std::function<void(uint8_t*, size_t)> i_Processor)
{
	if (i_pFileContents)
	{
		if (i_sizeFileContents && !ShutdownRequested())
			i_Processor(i_pFileContents, i_sizeFileContents);
	}

	delete[] i_pFileContents;

	std::cout << "ProcessFileContents finished processing file.\n";
}


void PrintOnInterval(std::string i_String, unsigned int i_IntervalMilliseconds, unsigned int i_Count)
{
	bool bCounted = i_Count > 0;

	do
	{
		Engine::Debugger::DEBUG_PRINT("$s@#$ \n", i_String.c_str());
		std::cout << i_String << "\n";
		Sleep(i_IntervalMilliseconds);
	} while ((!bCounted || (bCounted && --i_Count)) && !ShutdownRequested());
}


void PrintFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents)
{
	assert(i_pFileContents && i_sizeFileContents);

	std::cout << "File Contents:\n";
	while (i_sizeFileContents--)
		std::cout << *(i_pFileContents++);

	std::cout << "\n";
}


void BasicSample()
{
	using namespace std::placeholders;

	{
		// use a lambda function to run a job
		JobStatus JobStatus;

		RunJob(
			GetDefaultQueueName(),
			[]()
			{
				PrintOnInterval("Wahoo!!", 500, 10);
			},
			&JobStatus
		);

		JobStatus.WaitForZeroJobsLeft();
	}

	{
		JobStatus JobStatus;

		// use std::bind to bind a function to a std::function
		// _1 and _2 are placeholders that get replaced when actually executing the function
		ProcessFile ProcessFileInstance("File.txt", std::bind(PrintFileContents, _1, _2), GetDefaultQueue(), &JobStatus);

		// use std::bind to bind an instance of a class that has operator() () (a functor)
		RunJob(GetDefaultQueue(), std::bind(ProcessFileInstance), &JobStatus);

		// wait for FinishEvent to be signaled
		JobStatus.WaitForZeroJobsLeft();

		Engine::Debugger::DEBUG_PRINT("ProcessFile finished running.\n");
	}

}


//void GameObjectSample()
//{
//	using namespace std::placeholders;
//
//	{
//		const char* CustomQueueName = "GameObjectLoader";
//
//		HashedString QueueName = CreateQueue(CustomQueueName, 2);
//
//		{
//			RunJob(QueueName, std::bind(ProcessFile("GameObjects.txt", std::bind(CreateGameObjects, _1, _2))));
//
//			do
//			{
//				CheckForNewGameObjects();
//				Sleep(10);
//			} while (Engine::JobSystem::HasJobs(CustomQueueName));
//		}
//		CheckForNewGameObjects();
//	}
//
//	{
//		for (GameObject* p : AllGameObjects)
//		{
//			if (p)
//				delete p;
//		}
//
//		AllGameObjects.clear();
//	}
//}