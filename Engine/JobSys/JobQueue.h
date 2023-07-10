#pragma once
#include <queue>
#include <string>
#include <functional>

#include "./HashedString.h"
#include "./Sync/Event.h"
#include "./Sync/AtomicOperations.h"
#include "./Sync/WaitableObject.h"


namespace Engine
{
namespace JobSys
{

/* Forwared declaration */
struct Job;
class JobStatus;
class JobQueue;


/**
 *	TODO: @brief This struct implements a handler that represents a job, which will be pushed
 *		   job queue and executed by the job system.
 * 
 *		   Does not allow creating a new job object by copy construct or assign from an existing
 *		   job object, to prevent undefined behavior.
 */
struct Job
{
	std::function<void()>	action;
	HashedString			queueName;
	std::string				jobName;
	JobStatus*				jobStatus;

	Job(std::function<void()> function, const HashedString& queueName, const std::string& jobName, JobStatus* jobStatus = nullptr) :
		action(function),
		queueName(queueName),
		jobName(jobName),
		jobStatus(jobStatus)
	{}

	Job(const Job& other) = delete;
	Job& operator=(const Job& other) = delete;
};


/**
 *	TODO: @brief This class serves as the control block that manage the status of all 
 *		   queued jobs. Job count is consist of both waiting jobs and executing jobs.
 
 *		   When all registered jobs are finished (i.e. jobCount is zero), 
 *		   JobStatus class will signal a event to tell all waiting thread that current
 *		   job queue is empty.
 */
class JobStatus
{
private:
	uint32_t		jobCount;
	AutoResetEvent	jobsFinishedEvent;

	uint32_t IncJobCount();
	uint32_t DecJobCount();

public:
	friend class JobQueue;

	JobStatus(unsigned int jobCount = 0);
	~JobStatus() = default;
	
	JobStatus(const JobStatus& other) = delete;
	JobStatus& operator=(const JobStatus& other) = delete;

	uint32_t JobsLeft() const;

	/* @brief Set the calling thread waiting until there is no job in the job queue. */
	void WaitForZeroJobsLeft(int waitMS = INFINITE);
};


/** 
 *	TODO: Need to do research on "CONDITION_VARIABLE", "CRITICAL_SECTION", mutable, and 
 *		  mutex.
 * 
 *	@brief This class implements a queue that stores jobs. 
 
 *		   Jobs need to be synchronously retrieved from the queue. If the there is no 
 *		   available job in the queue and a get-job method is invoked, the thread owns 
 *		   the queue will sleep and wait until new jobs are added to the queue. 
 */
class JobQueue
{
private:
	std::string					queueName;
	std::queue<Job*>			jobQueue;
	uint32_t					jobsRunning;
	bool						stopRequested;

	CONDITION_VARIABLE			queueNotEmpty;
	mutable CRITICAL_SECTION	queueLock;

public:
	JobQueue(const std::string& queueName);
	~JobQueue() = default;

	JobQueue(const JobQueue&) = delete;
	JobQueue& operator=(const JobQueue&) = delete;

	bool Add(Job* job);

	/*	@brief Retrieve the first job from the job queue. If there is no job in the queue,  
	 *		   the calling thread will wait until other thread add job to the queue, or 
	 *		   the queue is shutted down. */
	Job* Get();

	void StartingJob(Job* job);
	void FinishedJob(Job* job);

	void RequestStop();
	bool IsStopped() const;
	bool HasJobs() const;

	void WakeRunners();

	std::string GetName() const;
};

}//Namespace Engine
}//Namespace JobSystem
