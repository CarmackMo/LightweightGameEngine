#include "SharedJobQueue.h"

namespace Engine
{
namespace JobSystem
{

SharedJobQueue::SharedJobQueue(const string& queueName) :
	queueName(queueName),
	jobsRunning(0),
	stopRequested(false),
	queueNotEmpty(CONDITION_VARIABLE_INIT)
{
	InitializeCriticalSection(&queueLock);
}


bool SharedJobQueue::Add(struct Job* job)
{
	assert(job);
	bool isAdded = false;

	/* Blocks until the thread can take ownership of the specified critical 
	 * section. The function returns when the calling thread is granted ownership. */
	EnterCriticalSection(&queueLock);
	if (stopRequested == false)
	{
		if (job->jobStatus)
			job->jobStatus->AddJob();

		jobQueue.push(job);
		isAdded = true;
	}
	/* Releases ownership of the specified critical section object. There is no 
	 * guarantee about the order in which waiting threads will acquire ownership of 
	 * the critical section. */
	LeaveCriticalSection(&queueLock);

	/* If the current queue is waiting to acquire available jobs, wake it. */
	if (isAdded)
		WakeConditionVariable(&queueNotEmpty);

	return isAdded;
}


Job* SharedJobQueue::Get()
{
	EnterCriticalSection(&queueLock);

	/* If current job queue is empty, sleep and wait until new jobs are added to 
	 * the queue or the thread is shutted down. */
	if (jobQueue.empty() && (stopRequested == false))
	{
		BOOL success = SleepConditionVariableCS(&queueNotEmpty, &queueLock, INFINITE);
		assert(success != 0);

		if (stopRequested == true)
		{
			LeaveCriticalSection(&queueLock);
			return nullptr;
		}
	}

	/* Retrieve job from job queue. Note that this action can be proceeded even if
	 * the current thread is shutted down. */
	Job* job = nullptr;
	if (!jobQueue.empty())
	{
		job = jobQueue.front();
		jobQueue.pop();
	}

	LeaveCriticalSection(&queueLock);
	return job;
}


void SharedJobQueue::StartingJob(Job* job)
{
	AtomicIncrement(jobsRunning);
}


void SharedJobQueue::FinishedJob(Job* job)
{
	if (job->jobStatus)
		job->jobStatus->FinishJob();

	delete job;

	AtomicDecrement(jobsRunning);
}


void SharedJobQueue::RequestStop()
{
	stopRequested = true;
	/* If the current queue is waiting to acquire available jobs, wake it. */
	WakeAllConditionVariable(&queueNotEmpty);
}


bool SharedJobQueue::IsStopped() const
{
	return stopRequested;
}


bool SharedJobQueue::HasJobs() const
{
	EnterCriticalSection(&queueLock);
	bool isFinished = jobQueue.empty() && (jobsRunning == 0);
	LeaveCriticalSection(&queueLock);

	return !isFinished;
}


string SharedJobQueue::GetName() const
{
	return queueName;
}



}//Namespace Engine
}//Namespace JobSystem