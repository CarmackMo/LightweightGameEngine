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
 *	@brief Job is a data structure that stores a specific function to be executed by one of 
 *		   the job runners in the job system. Any attempt to duplicate an existing job object 
 *		   will result in undefined behavior.
 */
struct Job
{
	std::function<void()>	action;
	JobStatus*				jobStatus;
#if defined (_DEBUG)
	std::string				jobName;
#endif

	Job(std::function<void()> function, JobStatus* jobStatus = nullptr) :
		action(function),
		jobStatus(jobStatus)
	{}

	Job(const Job& other) = delete;
	Job& operator=(const Job& other) = delete;
};


/**
 *	@brief JobStatus serves as a control block for each job queue in the job system. Note 
 *		   that "jobCount" is the total count of awaiting jobs and executing jobs within the 
 *		   job queue. When jobCount is zero, the "JobStatus "jobsFinishedEvent" will signal 
 *		   all waiting thread that current job queue is empty.
 * 
 *		   Note that each job queue is bound to a unique job status object. Any attempt to 
 *		   duplicate an existing job status object will result in undefined behavior.
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
 *	@brief JobQueue is a data structure that responsible for storing and managing jobs. The 
 *		   jobs within the job queue object are executed in a first-in, first-out (FIFO) order.
 *
 *		   The job queue object is designed to be a shared resource for job runners. To 
 *		   facilitate synchronization between job runners working on the same job queue, the
 *		   job queue object maintains a `CONDITION_VARIABLE` object and a `CRITICAL_SECTION` 
 *		   object. These ensure that jobs are added to and retrieved from the queue in a 
 *		   synchronized manner.
 * 
 *		   If the there is no available job in the queue. The thread calls the "Get()" function
 *		   sleep and wait until either new jobs are added to the queue or it is wake manually.
 *
 *		   Note that due to the unique dependencies (e.g. `JobStatus`) of the job queue object, 
 *		   a job queue object cannot be created using copy constructor or assignment operation. 
 *		   Any attempt to duplicate an existing job queue object will result in undefined 
 *		   behavior.
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
