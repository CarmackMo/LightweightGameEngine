#pragma once
#include <string>
#include <cassert>
#include "Debugger.h"
#include "./JobQueue.h"

namespace Engine
{
namespace JobSys
{

/**
 *	@brief `JobRunner` object is an elementary job executer of the specific job queue it 
 *		   assigned to. 
 *
 *		   The job runner object serves as the control module for the thread that performs 
 *		   the job execution. Each job runner object stores essential data of the thread, 
 *		   including the handler to the threa, thread ID, and a pointer to the associated job
 *		   queue. 
 *
 *		   The job runner object also serves as the thread input when creating a new thread 
 *		   for job execution, while the instance of the job runner object is managed by the 
 *		   job system in the main thread.
 */
struct JobRunner
{
	JobQueue*		jobQueue;
	HANDLE			threadHandle;
	DWORD			threadID;
	bool			stopRequested;
#ifdef _DEBUG
	std::string		threadName;
#endif

	JobRunner(JobQueue& jobQueue) :
		jobQueue(&jobQueue),
		threadHandle(INVALID_HANDLE_VALUE),
		threadID(-1),
		stopRequested(false)
	{}

	inline void RequestStop()
	{
		stopRequested = true;
	}

	inline bool IsStopped()
	{
		return stopRequested;
	}
};


/*	@brief Execute the job runner routine, which involves continuously retrieving jobs from 
 *		   the shared job queue and executing them until the job queue or job runner is 
 *		   shutted down. If there are no jobs in the queue, this function will wait until 
 *		   new jobs are added.
 * 
 *		   This function also serves as the starting address for the job runner thread. 
 * 
 *	@param "threadInput": the external data passed to this function using "CreateThread()". */
inline DWORD WINAPI JobRunnerRoutine(void* threadInput)
{
	JobRunner* input = reinterpret_cast<JobRunner*>(threadInput);
	bool isStopped = false;

#if defined (_DEBUG)
	assert(threadInput);
	assert(input->jobQueue);
	const char* threadName = input->threadName.c_str();
	DEBUG_PRINT("JobRunner \"%s\": Starting Queue \"%s\". \n", threadName, input->jobQueue->GetName().c_str());
#endif

	do
	{
		Job* job = input->jobQueue->Get();
		if (job)
		{
#if defined (_DEBUG)
			std::string jobName = job->jobName;
			DEBUG_PRINT("JobRunner \"%s\": Starting Job \"%s\" on Processor %d. \n", threadName, jobName.c_str(), GetCurrentProcessorNumber());
#endif

			input->jobQueue->StartingJob(job);
			job->action();
			input->jobQueue->FinishedJob(job);

#if defined (_DEBUG)
			DEBUG_PRINT("JobRunner \"%s\": Finished Job \"%s\". \n", threadName, jobName.c_str());
#endif
		}

		isStopped = input->stopRequested || input->jobQueue->IsStopped();

	} while (isStopped == false);

#ifdef _DEBUG
	DEBUG_PRINT("JobRunner \"%s\": Shutting down. \n", threadName);
#endif
	return 0;
}


}//Namespace Engine
}//Namespace JobSys
