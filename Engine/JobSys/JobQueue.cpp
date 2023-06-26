#include <cassert>
#include "./JobQueue.h"

namespace Engine
{
namespace JobSys
{

#pragma region JobStatus

JobStatus::JobStatus(unsigned int jobCount) :
	jobCount(jobCount)
{ }


uint32_t JobStatus::IncJobCount()
{
	return AtomicIncrement(jobCount);
}


uint32_t JobStatus::DecJobCount()
{
	assert(jobCount > 0);

	uint32_t newJobCount = AtomicDecrement(jobCount);
	if (newJobCount == 0)
		jobsFinishedEvent.Signal();

	return newJobCount;
}


uint32_t JobStatus::JobsLeft() const 
{ 
	return jobCount; 
}


void JobStatus::WaitForZeroJobsLeft(int waitMS)
{
	jobsFinishedEvent.Wait(DWORD(waitMS));
}

#pragma endregion


#pragma region SharedJobQueue

JobQueue::JobQueue(const string& queueName) :
	queueName(queueName),
	jobsRunning(0),
	stopRequested(false),
	queueNotEmpty(CONDITION_VARIABLE_INIT)
{
	InitializeCriticalSection(&queueLock);
}


bool JobQueue::Add(Job* job)
{
	assert(job);
	bool isAdded = false;

	/* Blocks until the calling thread can take ownership of the specified critical section. The 
	 * function returns when the calling thread is granted ownership. */
	EnterCriticalSection(&queueLock);
	if (stopRequested == false)
	{
		if (job->jobStatus)
			job->jobStatus->IncJobCount();

		jobQueue.push(job);
		isAdded = true;
	}
	/* Releases ownership of the specified critical section object. There is no 
	 * guarantee about the order in which waiting threads will acquire ownership of 
	 * the critical section. */
	LeaveCriticalSection(&queueLock);

	/* Wake other threads that is waiting the condition variable. */
	if (isAdded)
		WakeConditionVariable(&queueNotEmpty);

	return isAdded;
}


Job* JobQueue::Get()
{
	EnterCriticalSection(&queueLock);

	/* If job queue is empty, current therad will sleep and wait until new jobs are added to 
	 * the queue or the queue is shutted down. */
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

	/* Retrieve job from job queue. Note that this action can be proceeded even if the current
	 * thread is shutted down. */
	Job* job = nullptr;
	if (!jobQueue.empty())
	{
		job = jobQueue.front();
		jobQueue.pop();
	}

	LeaveCriticalSection(&queueLock);
	return job;
}


void JobQueue::StartingJob(Job* job)
{
	AtomicIncrement(jobsRunning);
}


void JobQueue::FinishedJob(Job* job)
{
	if (job->jobStatus)
		job->jobStatus->DecJobCount();

	delete job;

	AtomicDecrement(jobsRunning);
}


void JobQueue::RequestStop()
{
	stopRequested = true;
	/* If the current queue is waiting to acquire available jobs, wake it. */
	WakeAllConditionVariable(&queueNotEmpty);
}


bool JobQueue::IsStopped() const
{
	return stopRequested;
}


bool JobQueue::HasJobs() const
{
	EnterCriticalSection(&queueLock);
	bool isFinished = jobQueue.empty() && (jobsRunning == 0);
	LeaveCriticalSection(&queueLock);

	return !isFinished;
}


string JobQueue::GetName() const
{
	return queueName;
}

#pragma endregion



}//Namespace Engine
}//Namespace JobSystem