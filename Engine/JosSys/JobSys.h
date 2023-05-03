#pragma once
#include "Dependency.h"

class HashedString;
class JobSys;



class HashedString
{
public:
	HashedString();
	~HashedString();

	HashedString(const char* i_string);
	HashedString(const HashedString& i_other);
	HashedString& operator=(const HashedString& i_other);

	unsigned int Get() const;

	bool operator==(const HashedString& i_other) const;
	bool operator<(const HashedString& i_other) const;

	static unsigned int Hash(const char* i_string);
	static unsigned int Hash(const void* i_bytes, size_t i_count);
private:
	unsigned int 	m_Hash;

#ifdef DEBUG_KEEP_STRING
	const char* m_pString;
#endif
};


class QueuedJob
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






class JobStatus
{
public:
	JobStatus(unsigned int i_InitialJobCount = 0) :
		m_JobCount(i_InitialJobCount)
	{}

	uint32_t JobsLeft() const { return m_JobCount; }

	void WaitForZeroJobsLeft(int i_WaitMS = Engine::WaitableObject::WaitInfinite)
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



class JobSys
{
public:
	JobSys();
	~JobSys();

	void Init();


};


#include "JobSys.inl"