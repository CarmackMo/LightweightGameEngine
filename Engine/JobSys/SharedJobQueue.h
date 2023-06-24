#pragma once
#include <string>
#include <queue>
#include <functional>

#include "./HashedString.h"
#include "./Sync/Event.h"
#include "./Sync/AtomicOperations.h"
#include "./Sync/WaitableObject.h"

using namespace std;

namespace Engine
{
namespace JobSystem
{
/* Forwared declaration */
struct Job;
class JobStatus;
class SharedJobQueue;


/**
 *	TODO: @brief This struct implements a handler that represents a job, which will be pushed
 *		   job queue and executed by the job system.
 */
struct Job
{
	function<void()>	action;
	HashedString		queueName;
	string				jobName;
	JobStatus*			jobStatus;

	Job(function<void()> function, const HashedString& queueName, const string& jobName, JobStatus* jobStatus = nullptr) :
		action(function),
		queueName(queueName),
		jobName(jobName),
		jobStatus(jobStatus)
	{}
};


/**
 *	TODO: @brief This class serves as the control block that manage the status of all 
 *		   queued jobs. 
 
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
	friend class SharedJobQueue;

	JobStatus(unsigned int jobCount = 0);
	~JobStatus() = default;
	
	JobStatus(const JobStatus&) = delete;
	JobStatus& operator=(const JobStatus&) = delete;

	uint32_t JobsLeft() const;

	/* @brief Set the current thread waiting until there is no job in the job queue. */
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
class SharedJobQueue
{
private:
	string						queueName;
	queue<Job*>					jobQueue;
	uint32_t					jobsRunning;
	bool						stopRequested;

	CONDITION_VARIABLE			queueNotEmpty;
	mutable CRITICAL_SECTION	queueLock;


	SharedJobQueue(const SharedJobQueue&) = delete;
	SharedJobQueue& operator=(const SharedJobQueue&) = delete;

public:
	SharedJobQueue(const string& queueName);
	~SharedJobQueue() = default;

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

	string GetName() const;
};

}//Namespace Engine
}//Namespace JobSystem
