#include "JobSys.h"


namespace Engine
{

using std::string;
using std::function;
using std::vector;
using std::map;
using namespace JobSys;


void JobSystem::Init()
{
	CreateQueue("Default", 2);
}


HashedString JobSystem::CreateQueue(const string& queueName, unsigned int runnerNum)
{
	HashedString hashedName = HashedString(queueName.c_str());

	if (jobQueueMap.find(hashedName) == jobQueueMap.end())
	{
		JobQueueManager* manager = new JobQueueManager(queueName);
		jobQueueMap.emplace(hashedName, manager);

		for (unsigned int i = 0; i < runnerNum; i++)
			this->AddRunnerToQueue(manager);

		return hashedName;
	}
	else
	{
		DEBUG_PRINT("Job System: Warning: Trying to create a new job queue using the name of an existing job queue! \n");
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
	JobQueueManager* manager = GetQueue(queueName);
	if (manager != nullptr)
	{
		this->AddRunnerToQueue(manager);
		return true;
	}
	else
		return false;
}


bool JobSystem::AddJobToQueue(const HashedString& queueName, function<void()> jobFunction, const string& jobName)
{
	JobQueueManager* manager = GetQueue(queueName);
	if (manager != nullptr)
	{
#if defined (_DEBUG)
		DEBUG_PRINT("Job System: Adding Job to Queue \"%s\". \n", manager->jobQueue.GetName().c_str());
#endif

		manager->jobQueue.Add(new Job(jobFunction, queueName, jobName, &(manager->jobStatus)));
		return true;
	}
	else
		return false;
}


bool JobSystem::IsQueueHasJobs(const HashedString& queueName)
{
	JobQueueManager* manager = GetQueue(queueName);
	if (manager != nullptr)
	{
		return manager->jobQueue.HasJobs();
	}

	return false;
}


void JobSystem::RequestStop()
{
#if defined (_DEBUG)
	DEBUG_PRINT("Job System: Shutting down. \n");
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
	DEBUG_PRINT("Job System: Waiting for Queue runner threads to shut down. \n");
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
	DEBUG_PRINT("Job System: Shut down Successful. \n");
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


JobQueueManager* JobSystem::GetQueue(const HashedString& queueName)
{
	if (jobQueueMap.find(queueName) != jobQueueMap.end())
		return jobQueueMap[queueName];
	else
		return nullptr;
}


}//Namespace Engine
