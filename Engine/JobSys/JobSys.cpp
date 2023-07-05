#include "JobSys.h"


namespace Engine
{

using std::bind;
using std::function;
using std::string;
using std::vector;
using std::map;
using namespace JobSys;




void JobSystem::JobFlowControl()
{
	do
	{
		map<HashedString, JobQueueManager*>::iterator iter = jobQueueMap.begin();
		while (iter != jobQueueMap.end())
		{
			JobQueueManager* manager = iter->second;
			if (manager != nullptr && manager->jobFlowManager.isAuto == true)
			{
				if (manager->jobStatus.JobsLeft() > JobFlowManager::threshold)
				{
					if (manager->jobFlowManager.overflowFlag == true)
					{
						AddRunnerToQueue(manager);
						manager->jobFlowManager.overflowFlag = false;
					}
					else
						manager->jobFlowManager.overflowFlag = true;
				}
				else if (manager->jobStatus.JobsLeft() == 0)
				{
					if (manager->jobFlowManager.idleFlag == true)
					{
						RemoveRunnerFromQueue(manager);
						manager->jobFlowManager.idleFlag = false;
					}
					else
						manager->jobFlowManager.idleFlag = true;
				}
				else
				{
					manager->jobFlowManager.overflowFlag = false;
					manager->jobFlowManager.idleFlag = false;
				}
			}

			iter++;
		}

		Sleep(JobFlowManager::interval);

	} while (IsStopped() == false);
}


void JobSystem::Init()
{
	CreateQueue("Default", 2, false);
	AddJobToQueue(GetDefaultQueue(), bind(&JobSystem::JobFlowControl, this), "Job Flow Control Routine");
}


HashedString JobSystem::CreateQueue(const string& queueName, unsigned int runnerNum, bool autoFlowControl)
{
	HashedString hashedName = HashedString(queueName.c_str());

	if (jobQueueMap.find(hashedName) == jobQueueMap.end())
	{
		JobQueueManager* manager = new JobQueueManager(queueName, autoFlowControl);
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


bool JobSystem::RemoveRunnerFromQueue(JobQueueManager* manager)
{
	if (manager->jobRunnerList.size() > 1)
	{
		JobRunner* runner = manager->jobRunnerList[0];
		runner->RequestStop();

		DWORD result = WaitForSingleObject(runner->threadHandle, INFINITE);
		assert(result == WAIT_OBJECT_0);

		manager->jobRunnerList.erase(manager->jobRunnerList.begin());
		delete runner;

		return true;
	}
	else
		return false;
}


bool JobSystem::RemoveRunnerFromQueue(const HashedString& queueName)
{
	JobQueueManager* manager = GetQueue(queueName);

	if (manager != nullptr)
		return RemoveRunnerFromQueue(manager);
	else
		return false;


	//if (manager != nullptr && manager->jobRunnerList.size() > 1)
	//{
	//	JobRunner* runner = manager->jobRunnerList[0];
	//	runner->RequestStop();

	//	DWORD result = WaitForSingleObject(runner->threadHandle, INFINITE);
	//	assert(result == WAIT_OBJECT_0);

	//	manager->jobRunnerList.erase(manager->jobRunnerList.begin());
	//	delete runner;
	//	
	//	return true;
	//}
	//else
	//	return false;
}


bool JobSystem::RemoveQueue(const HashedString& queueName)
{
	JobQueueManager* manager = GetQueue(queueName);
	if (manager != nullptr)
	{
		vector<HANDLE> threads;
		vector<JobRunner*> runners;

		manager->jobQueue.RequestStop();

		const size_t runnerCount = manager->jobRunnerList.size();
		for (size_t i = 0; i < runnerCount; i++)
		{
			JobRunner* runner = manager->jobRunnerList[i];
			if (runner != nullptr)
			{
				runners.push_back(runner);
				if (runner->threadHandle != nullptr)
					threads.push_back(runner->threadHandle);

				runner->RequestStop();
			}
		}

		/* Wait until all job runner thread are ternimated. */
		DWORD result = WaitForMultipleObjects(static_cast<DWORD>(threads.size()), &threads[0], TRUE, INFINITE);
		assert(result == WAIT_OBJECT_0);

		/* Delete all remaining JobRunner and JobQueueManager objects. */
		for (size_t i = 0; i < runners.size(); i++)
			delete runners[i];

		delete manager;
		jobQueueMap.erase(queueName);

		return true;
	}
	else
		return false;
}


JobQueueManager* JobSystem::GetQueue(const HashedString& queueName)
{
	if (jobQueueMap.find(queueName) != jobQueueMap.end())
		return jobQueueMap[queueName];
	else
		return nullptr;
}


HashedString JobSystem::GetDefaultQueue()
{
	return HashedString("Default");
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
		JobQueueManager* manager = iter->second;
		if (manager != nullptr)
		{
			manager->jobQueue.RequestStop();

			const size_t runnerCount = manager->jobRunnerList.size();
			for (size_t i = 0; i < runnerCount; i++)
			{
				JobRunner* runner = manager->jobRunnerList[i];
				if (runner != nullptr)
				{
					allRunners.push_back(runner);
					if (runner->threadHandle != nullptr)
						allThreads.push_back(runner->threadHandle);

					runner->RequestStop();
				}
			}

			allManagers.push_back(manager);
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


}//Namespace Engine
