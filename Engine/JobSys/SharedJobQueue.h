#pragma once
#include <string>
#include <queue>

#include "HashedString.h"
#include "JobStatus.h"
#include "AtomicOperations.h"

using namespace std;

namespace Engine
{
namespace JobSystem
{


struct QueuedJob
{
	std::function<void()>	Function;
	HashedString			QueueName;
	std::string				JobName;
	JobStatus* pJobStatus;

	QueuedJob(std::function<void()> i_Function, const HashedString& i_QueueName, const std::string& i_JobName, JobStatus* i_pJobStatus = nullptr) :
		Function(i_Function),
		QueueName(i_QueueName),
		JobName(i_JobName),
		pJobStatus(i_pJobStatus)
	{}
};


/** 
 *	@brief This class implements job queue that uses "CONDITION_VARIABLE" and
 *		   "CRITICAL_SECTION to record thread status "*/
class SharedJobQueue
{
private:
	string						m_Name;
	queue<QueuedJob*>			m_Jobs;
	uint32_t					m_JobsRunning;
	bool						m_bShutdownRequested;

	CONDITION_VARIABLE			m_WakeAndCheck;
	mutable CRITICAL_SECTION	m_QueueAccess;


	SharedJobQueue(const SharedJobQueue&) = delete;
	SharedJobQueue& operator=(const SharedJobQueue&) = delete;

public:
	SharedJobQueue(const string& queueName);

	bool Add(QueuedJob* job);
	bool HasJobs() const;
	QueuedJob* GetWhenAvailable();
	void StartingJob(QueuedJob* i_pJob);
	void FinishedJob(QueuedJob* i_pJob);

	void RequestShutdown();
	bool ShutdownRequested() const { return m_bShutdownRequested; }

	std::string GetName() const { return m_Name; }
};

}//Namespace Engine
}//Namespace JobSystem


#include "SharedJobQueue.inl"
