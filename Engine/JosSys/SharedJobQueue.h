#pragma once
#include "Dependency.h"
#include "HashedString.h"
#include "JobStatus.h"


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

class SharedJobQueue
{
public:
	SharedJobQueue(const std::string& i_QueueName);

	bool Add(QueuedJob* i_pJob);
	bool HasJobs() const;
	struct QueuedJob* GetWhenAvailable();
	void StartingJob(QueuedJob* i_pJob);
	void FinishedJob(QueuedJob* i_pJob);

	void RequestShutdown();
	bool ShutdownRequested() const { return m_bShutdownRequested; }

	std::string GetName() const { return m_Name; }
private:
	SharedJobQueue(const SharedJobQueue&) = delete;
	SharedJobQueue& operator=(const SharedJobQueue&) = delete;

	std::string m_Name;
	std::queue<struct QueuedJob*>	m_Jobs;
	CONDITION_VARIABLE m_WakeAndCheck;
	mutable CRITICAL_SECTION m_QueueAccess;

	uint32_t m_JobsRunning;
	bool m_bShutdownRequested;
};

#include "SharedJobQueue.inl"