#include "SharedJobQueue.h"

SharedJobQueue::SharedJobQueue(const std::string& i_Name) :
	m_Name(i_Name),
	m_WakeAndCheck(CONDITION_VARIABLE_INIT),
	m_JobsRunning(0),
	m_bShutdownRequested(false)
{
	InitializeCriticalSection(&m_QueueAccess);
}

bool SharedJobQueue::Add(struct QueuedJob* i_pJob)
{
	assert(i_pJob);
	bool bAdded = false;

	EnterCriticalSection(&m_QueueAccess);
	if (m_bShutdownRequested == false)
	{
		if (i_pJob->pJobStatus)
			i_pJob->pJobStatus->AddJob();

		m_Jobs.push(i_pJob);
		bAdded = true;
	}
	LeaveCriticalSection(&m_QueueAccess);

	if (bAdded)
		WakeConditionVariable(&m_WakeAndCheck);

	return bAdded;
}

bool SharedJobQueue::HasJobs() const
{
	EnterCriticalSection(&m_QueueAccess);
	bool bFinished = m_Jobs.empty() && (m_JobsRunning == 0);
	LeaveCriticalSection(&m_QueueAccess);

	return !bFinished;
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

struct QueuedJob* SharedJobQueue::GetWhenAvailable()
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

void SharedJobQueue::RequestShutdown()
{
	m_bShutdownRequested = true;
	WakeAllConditionVariable(&m_WakeAndCheck);
}