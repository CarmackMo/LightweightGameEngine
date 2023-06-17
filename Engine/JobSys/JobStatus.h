#pragma once
#include "Dependency.h"
#include "WaitableObject.h"
#include "AtomicOperations.h"
#include "Event.h"


namespace Engine
{
namespace JobSystem
{


class JobStatus
{
public:
	JobStatus(unsigned int i_InitialJobCount = 0) :
		m_JobCount(i_InitialJobCount)
	{}

	uint32_t JobsLeft() const { return m_JobCount; }

	void WaitForZeroJobsLeft(int i_WaitMS = WaitableObject::WaitInfinite)
	{
		m_JobsFinishedEvent.Wait(DWORD(i_WaitMS));
	}
private:
	friend class SharedJobQueue;

	JobStatus(const JobStatus&) = delete;
	JobStatus& operator=(const JobStatus&) = delete;

	uint32_t AddJob()
	{
		return AtomicIncrement(m_JobCount);
	}

	uint32_t FinishJob()
	{
		assert(m_JobCount > 0);

		uint32_t NewJobCount = AtomicDecrement(m_JobCount);
		if (NewJobCount == 0)
			m_JobsFinishedEvent.Signal();

		return NewJobCount;
	}

	uint32_t	m_JobCount;
	AutoResetEvent m_JobsFinishedEvent;
};

}//Namespace Engine
}//Namespace JobSystem