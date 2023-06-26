#include "JobSys.h"


bool bShutdownRequested = false;
std::map<HashedString, JobQueueManager*>	Queues;


#pragma region JobSystem

void JobSystem::Init()
{
	CreateQueue("Default", 2);
}


HashedString JobSystem::CreateQueue(const string& queueName, unsigned int runnerNum)
{
	HashedString hashedName = HashedString(queueName.c_str());

	if (jobQueueMap.find(hashedName) != jobQueueMap.end())
	{
		JobQueueManager* manager = new JobQueueManager(queueName);
		jobQueueMap.emplace(hashedName, manager);

		for (unsigned int i = 0; i < runnerNum; i++)
			this->AddRunnerToQueue(manager);

		return hashedName;
	}
	else
	{
		Engine::Debugger::DEBUG_PRINT("Job System: Warning: Trying to create a new job queue using the name of an existing job queue! \n");
		return hashedName;
	}
}


void JobSystem::AddRunnerToQueue(JobQueueManager* manager)
{
	JobRunner* jobRunner = new JobRunner(manager->jobQueue);

#ifdef _DEBUG
	size_t runners = manager->jobRunnerList.size();
	const size_t	sizeThreadName = 32;
	char			ThreadName[sizeThreadName];
	sprintf_s(ThreadName, sizeThreadName, "%s %d", manager->jobQueue.GetName().c_str(), int(runners + 1));
	jobRunner->threadName = ThreadName;
#endif

	/* Create a thread to proceed job runner routine.
	 * NULL:				the thread handle cannot be inherited
	 * 0:					the initial size of the stack, in bytes, is zero. The new thread uses the default size for the executable.
	 * JobRunnerRoutine:	the function to be executed by the thread. Also represents the starting address of the thread.
	 * jobRunner:			A pointer to a variable to be passed to the thread.
	 * CREATE_SUSPENDED:	the thread is created in a suspended state, and does not run until user wake it. */
	jobRunner->threadHandle = CreateThread(NULL, 0, JobRunnerRoutine, jobRunner, CREATE_SUSPENDED, &jobRunner->threadID);
	assert(jobRunner->threadHandle != NULL);

	manager->jobRunnerList.push_back(jobRunner);

	/* Start the job runner thread. */
	ResumeThread(jobRunner->threadHandle);
}


bool JobSystem::AddRunnerToQueue(const HashedString& queueName)
{
	if (jobQueueMap.find(queueName) != jobQueueMap.end())
	{
		JobQueueManager* manager = jobQueueMap[queueName];
		assert(manager != nullptr);

		this->AddRunnerToQueue(manager);
		return true;
	}
	else
		return false;
}


bool JobSystem::AddJobToQueue(const HashedString& queueName, function<void()> jobFunction, const string& jobName)
{
	if (jobQueueMap.find(queueName) != jobQueueMap.end())
	{
		JobQueueManager* manager = jobQueueMap[queueName];
		assert(manager != nullptr);

		manager->jobQueue.Add(new Job(jobFunction, queueName, jobName, &(manager->jobStatus)));

#if defined (_DEBUG)
		Engine::Debugger::DEBUG_PRINT("Job System: Adding Job to Queue \"%s\". \n", manager->jobQueue.GetName().c_str());
#endif

		return true;
	}
	else
		return false;
}


bool JobSystem::IsQueueHasJobs(const HashedString& queueName)
{
	map<HashedString, JobQueueManager*>::iterator iter = jobQueueMap.find(queueName);
	if (iter != jobQueueMap.end())
	{
		if (iter->second != nullptr)
			return iter->second->jobQueue.HasJobs();
	}

	return false;
}


void JobSystem::RequestStop()
{
#if defined (_DEBUG)
	Engine::Debugger::DEBUG_PRINT("Job System: Shutting down. \n");
#endif

	stopRequested = true;
	vector<HANDLE> allThreads;
	vector<JobRunner*> allRunners;
	vector<JobQueueManager*> allManagers;

	/* Request all job queues to stop. Record all JobRunner objects, JobQueueManager objects, and 
	 * job runner threads for later termination. */
	map<HashedString, JobQueueManager*>::iterator iter = jobQueueMap.begin();
	while (iter != jobQueueMap.end())
	{
		if (iter->second != nullptr)
		{
			iter->second->jobQueue.RequestStop();

			const size_t runnerCount = iter->second->jobRunnerList.size();
			for (size_t i = 0; i < runnerCount; i++)
			{
				JobRunner* runner = iter->second->jobRunnerList[i];
				if (runner != nullptr)
					allRunners.push_back(runner);
				if (runner != nullptr && runner->threadHandle != NULL)
					allThreads.push_back(runner->threadHandle);
			}

			allManagers.push_back(iter->second);
		}
		iter++;
	}

#if defined (_DEBUG)
	Engine::Debugger::DEBUG_PRINT("Job System: Waiting for Queue runner threads to shut down. \n");
#endif

	/* Wait until all job runner thread are ternimated. */
	DWORD result = WaitForMultipleObjects(static_cast<DWORD>(allThreads.size()), &allThreads[0], TRUE, INFINITE);
	assert(result == WAIT_OBJECT_0);

	/* Delete all remaining JobRunner and JobQueueManager objects. */
	for (size_t i = 0; i < allRunners.size(); i++)
		delete allRunners[i];

	for (size_t i = 0; i < allManagers.size(); i++)
		delete allManagers[i];

	jobQueueMap.clear();

#if defined (_DEBUG)
	Engine::Debugger::DEBUG_PRINT("Job System: Shut down Successful. \n");
#endif
}


bool JobSystem::IsStopped()
{
	return stopRequested;
}


HashedString JobSystem::GetDefaultQueue()
{
	return HashedString("Default");
}

#pragma endregion



//////////////////////////////////////////////////////////////////////////////////////////////


void Init()
{
	CreateQueue(GetDefaultQueueName(), 2);
}


inline const char* GetDefaultQueueName() noexcept {
	static char DefaultQueueName[] = "Default";
	return DefaultQueueName;
}


inline HashedString GetDefaultQueue() {
	return HashedString(GetDefaultQueueName());
}


HashedString CreateQueue(const std::string& i_Name, unsigned int i_numRunners)
{
	HashedString	HashedName(i_Name.c_str());

	assert(Queues.find(HashedName) == Queues.end());

	JobQueueManager* pNewJobQueueData = new JobQueueManager(i_Name);

	Queues[HashedName] = pNewJobQueueData;

	for (unsigned int i = 0; i < i_numRunners; i++)
		AddRunnerToQueue(*pNewJobQueueData);

	return HashedName;
}


void AddRunnerToQueue(JobQueueManager& queueData)
{	
	JobRunner* jobRunner = new JobRunner(queueData.jobQueue);

#ifdef _DEBUG
	size_t runners = queueData.jobRunnerList.size();
	const size_t	sizeThreadName = 32;
	char			ThreadName[sizeThreadName];
	sprintf_s(ThreadName, sizeThreadName, "%s %d", queueData.jobQueue.GetName().c_str(), int(runners + 1));
	jobRunner->threadName = ThreadName;
#endif

	/* Create a thread to proceed job runner routine. 
	 * NULL:				the thread handle cannot be inherited
	 * 0:					the initial size of the stack, in bytes, is zero. The new thread uses the default size for the executable.
	 * JobRunnerRoutine:	the function to be executed by the thread. Also represents the starting address of the thread.
	 * jobRunner:			A pointer to a variable to be passed to the thread.
	 * CREATE_SUSPENDED:	the thread is created in a suspended state, and does not run until user wake it. */
	jobRunner->threadHandle = CreateThread(NULL, 0, JobRunnerRoutine, jobRunner, CREATE_SUSPENDED, &jobRunner->threadID);
	assert(jobRunner->threadHandle != NULL);

	queueData.jobRunnerList.push_back(jobRunner);

	/* Start the job runner thread. */
	ResumeThread(jobRunner->threadHandle);
}


bool AddRunnerToQueue(const HashedString& queueName)
{
	if (Queues.find(queueName) != Queues.end())
	{
		JobQueueManager* queueData = Queues[queueName];
		assert(queueData != nullptr);

		AddRunnerToQueue(*queueData);
		return true;
	}
	else
		return false;
}



bool AddJobToQueue(const HashedString& queueName, function<void()> jobFunction, string jobName)
{
	if (Queues.find(queueName) != Queues.end())
	{
		JobQueueManager* queueData = Queues[queueName];
		assert(queueData != nullptr);
		
		queueData->jobQueue.Add(new Job(jobFunction, queueName, jobName, &(queueData->jobStatus)));

#if defined (_DEBUG)
		Engine::Debugger::DEBUG_PRINT("Job System: Adding Job to Queue \"%s\". \n", queueData->jobQueue.GetName().c_str());
#endif

		return true;
	}
	else
		return false;
}


/* Original Code */
//void RunJob(const HashedString& queueName, function<void()> jobFunction, JobStatus* jobStatus, string jobName)
//{
//	map<HashedString, JobQueueData*>::iterator iter = Queues.find(queueName);
//	assert(iter != Queues.end());
//	assert(iter->second);
//
//	Engine::Debugger::DEBUG_PRINT("Job System: Adding Job to Queue \"%s\". \n", iter->second->jobQueue.GetName().c_str());
//	iter->second->jobQueue.Add(new Job(jobFunction, queueName, jobName, jobStatus));
//}


bool HasJobs(const HashedString& i_QueueName)
{
	std::map<HashedString, JobQueueManager*>::iterator existing = Queues.find(i_QueueName);
	if (existing != Queues.end())
	{
		if (existing->second)
			return existing->second->jobQueue.HasJobs();
	}

	return false;
}


void RequestShutdown()
{
	Engine::Debugger::DEBUG_PRINT("Job System: Shutting down. \n");

	bShutdownRequested = true;

	std::vector<HANDLE>	AllThreads;

	std::map<HashedString, JobQueueManager*>::iterator iter = Queues.begin();
	while (iter != Queues.end())
	{
		if (iter->second)
		{
			iter->second->jobQueue.RequestStop();

			const size_t count = iter->second->jobRunnerList.size();
			for (size_t i = 0; i < count; i++)
			{
				JobRunner* pRunner = iter->second->jobRunnerList[i];
				if (pRunner && pRunner->threadHandle != NULL)
				{
					AllThreads.push_back(pRunner->threadHandle);
				}
			}
		}
		++iter;
	}

	Engine::Debugger::DEBUG_PRINT("Job System: Waiting for Queue runner threads to shut down. \n");

	DWORD result = WaitForMultipleObjects(static_cast<DWORD>(AllThreads.size()), &AllThreads[0], TRUE, INFINITE);
	assert(result == WAIT_OBJECT_0);

	iter = Queues.begin();
	while (iter != Queues.end())
	{
		if (iter->second)
		{
			const size_t count = iter->second->jobRunnerList.size();
			for (size_t i = 0; i < count; i++)
			{
				JobRunner* pRunner = iter->second->jobRunnerList[i];
				if (pRunner)
					delete pRunner;
			}

			delete iter->second;
		}

		++iter;
	}

	Queues.clear();

	Engine::Debugger::DEBUG_PRINT("Job System: Shut down Successful. \n");
}


bool ShutdownRequested()
{
	return bShutdownRequested;
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
		bool success = AddJobToQueue(
			GetDefaultQueueName(),
			[]() { PrintOnInterval("Wahoo!!", 500, 10); },
			"WAHOO"
		);

		assert(success == true);

		Queues[GetDefaultQueueName()]->jobStatus.WaitForZeroJobsLeft();


		/* Original Code */
		//// use a lambda function to run a job
		//JobStatus JobStatus;

		//RunJob(
		//	GetDefaultQueueName(),
		//	[]()
		//	{
		//		PrintOnInterval("Wahoo!!", 500, 10);
		//	},
		//	&JobStatus,
		//	"Wahoo!"
		//);

		//JobStatus.WaitForZeroJobsLeft();
	}

	{
		// use std::bind to bind a function to a std::function
		// _1 and _2 are placeholders that get replaced when actually executing the function
		ProcessFile ProcessFileInstance("File.txt", std::bind(PrintFileContents, _1, _2), GetDefaultQueue());

		bool success = AddJobToQueue(GetDefaultQueue(), std::bind(ProcessFileInstance), "File.txt");

		assert(success == true);

		Queues[GetDefaultQueue()]->jobStatus.WaitForZeroJobsLeft();




		/* Original Code */
		//JobStatus JobStatus;

		//// use std::bind to bind a function to a std::function
		//// _1 and _2 are placeholders that get replaced when actually executing the function
		//ProcessFile ProcessFileInstance("File.txt", std::bind(PrintFileContents, _1, _2), GetDefaultQueue(), &JobStatus);

		//// use std::bind to bind an instance of a class that has operator() () (a functor)
		//RunJob(GetDefaultQueue(), std::bind(ProcessFileInstance), &JobStatus, "File.txt");

		//// wait for FinishEvent to be signaled
		//JobStatus.WaitForZeroJobsLeft();

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