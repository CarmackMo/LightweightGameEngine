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

	/* Wake a single thread waiting on the specified condition variable. */
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


QueuedJob* SharedJobQueue::GetWhenAvailable()
{
	EnterCriticalSection(&m_QueueAccess);

	if (m_Jobs.empty() && (m_bShutdownRequested == false))
	{
		BOOL result = SleepConditionVariableCS(&m_WakeAndCheck, &m_QueueAccess, INFINITE);
		assert(result != 0);

		if (m_bShutdownRequested == true)
		{
			LeaveCriticalSection(&m_QueueAccess);
			return nullptr;
		}
	}

	struct QueuedJob* pJob = nullptr;

	if (!m_Jobs.empty())
	{
		pJob = m_Jobs.front();
		m_Jobs.pop();
	}

	LeaveCriticalSection(&m_QueueAccess);

	return pJob;
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
	WakeAllConditionVariable(&m_WakeAndCheck);
}

}//Namespace Engine
}//Namespace JobSystem