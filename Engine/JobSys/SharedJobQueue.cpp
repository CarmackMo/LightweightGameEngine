#include "SharedJobQueue.h"

namespace Engine
{
namespace JobSystem
{


SharedJobQueue::SharedJobQueue(const string& queueName) :
	m_Name(queueName),
	m_JobsRunning(0),
	m_bShutdownRequested(false),
	m_WakeAndCheck(CONDITION_VARIABLE_INIT)
{
	InitializeCriticalSection(&m_QueueAccess);
}


bool SharedJobQueue::Add(struct QueuedJob* job)
{
	assert(job);
	bool isAdded = false;

	/* Blocks until the thread can take ownership of the specified critical 
	 * section. The function returns when the calling thread is granted ownership. */
	EnterCriticalSection(&m_QueueAccess);
	if (m_bShutdownRequested == false)
	{
		if (job->pJobStatus)
			job->pJobStatus->AddJob();

		m_Jobs.push(job);
		isAdded = true;
	}
	/* Releases ownership of the specified critical section object. There is no 
	 * guarantee about the order in which waiting threads will acquire ownership of 
	 * the critical section. */
	LeaveCriticalSection(&m_QueueAccess);

	/* If the current queue is waiting to acquire available jobs, wake it. */
	if (isAdded)
		WakeConditionVariable(&m_WakeAndCheck);

	return isAdded;
}


bool SharedJobQueue::HasJobs() const
{
	EnterCriticalSection(&m_QueueAccess);
	bool isFinished = m_Jobs.empty() && (m_JobsRunning == 0);
	LeaveCriticalSection(&m_QueueAccess);

	return !isFinished;
}


QueuedJob* SharedJobQueue::Get()
{
	EnterCriticalSection(&m_QueueAccess);

	/* If current job queue is empty, sleep and wait until new jobs are added to 
	 * the queue or the thread is shutted down. */
	if (m_Jobs.empty() && (m_bShutdownRequested == false))
	{
		BOOL success = SleepConditionVariableCS(&m_WakeAndCheck, &m_QueueAccess, INFINITE);
		assert(success != 0);

		if (m_bShutdownRequested == true)
		{
			LeaveCriticalSection(&m_QueueAccess);
			return nullptr;
		}
	}

	/* Retrieve job from job queue. Note that this action can be proceeded even if
	 * the current thread is shutted down. */
	QueuedJob* job = nullptr;
	if (!m_Jobs.empty())
	{
		job = m_Jobs.front();
		m_Jobs.pop();
	}

	LeaveCriticalSection(&m_QueueAccess);
	return job;
}


void SharedJobQueue::StartingJob(QueuedJob* i_pJob)
{
	AtomicIncrement(m_JobsRunning);
}


void SharedJobQueue::FinishedJob(QueuedJob* i_pJob)
{
	if (i_pJob->pJobStatus)
		i_pJob->pJobStatus->FinishJob();

	delete i_pJob;

	AtomicDecrement(m_JobsRunning);
}


void SharedJobQueue::RequestShutdown()
{
	m_bShutdownRequested = true;
	/* If the current queue is waiting to acquire available jobs, wake it. */
	WakeAllConditionVariable(&m_WakeAndCheck);
}

}//Namespace Engine
}//Namespace JobSystem