#pragma once
#include <string>
#include <queue>
#include <functional>

#include "HashedString.h"
#include "JobStatus.h"
#include "./Sync/AtomicOperations.h"

using namespace std;

namespace Engine
{
namespace JobSystem
{


/**
 *	@brief This struct implements a handler that represents a job, which will be pushed
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
 *	@brief This class implements a queue that stores jobs. Jobs need to be synchronously
 *		   retrieved from the queue. If the there is no available job in the queue
 *		   and a get-job method is invoked, the thread owns the queue will sleep and
 *		   wait until new jobs are added to the queue. 
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
	/*	@brief Retrieve the first job from the job queue. If there are no available 
	 *		   jobs in the queue, the current thread will be put to sleep until new 
	 *		   jobs are added to the queue or the current thread is terminated. This
	 *		   will not return until current thread is waked up.
	 *		   Even if the current thread has been terminated, other threads can 
	 *		   still fetch jobs from it using this function. */
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


#include "SharedJobQueue.inl"
