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
 *	@brief This class serves as an elementary executer for a specified shared job queue in 
 *		   the job system.
 *		   
 *		   Each JobRunner instance stores essential data related to the runner thread it 
 *		   belongs to, including a handler (i.e. pointer) to the thread, the thread ID, and 
 *		   a pointer to the job queue that the instance operates on. Note that each runner 
 *		   thread can own only one JobRunner instance.
 *
 *		   This class also serves as the input parameter that is going past into the thread
 *		   function when creating a new runner thread.
 */
struct JobRunner
{
	JobQueue*		jobQueue;
	HANDLE			threadHandle;
	DWORD			threadID;
#ifdef _DEBUG
	string		threadName;
#endif

	JobRunner(JobQueue& jobQueue) :
		jobQueue(&jobQueue),
		threadHandle(INVALID_HANDLE_VALUE),
		threadID(-1)
	{}
};


/*	@brief Execute the job runner routine, which involves continuously retrieving jobs from 
 *		   the shared job queue and executing them until the job queue is shut down. If there 
 *		   are no jobs in the queue, this function will wait until new jobs are added. 
 *		   This function also serves as the starting address for the job runner thread. 
 *	@param "threadInput": the external data passed to this function using "CreateThread()". */
inline DWORD WINAPI JobRunnerRoutine(void* threadInput)
{
	JobRunner* input = reinterpret_cast<JobRunner*>(threadInput);
	bool isStopped = false;

#if defined (_DEBUG)
	assert(threadInput);
	assert(input->jobQueue);
	const char* threadName = input->threadName.c_str();
	Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Queue \"%s\". \n", threadName, input->jobQueue->GetName().c_str());
#endif

	do
	{
		Job* job = input->jobQueue->Get();
		if (job)
		{
#if defined (_DEBUG)
			std::string jobName = job->jobName;
			Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Job \"%s\" on Processor %d. \n", threadName, jobName.c_str(), GetCurrentProcessorNumber());
#endif

			input->jobQueue->StartingJob(job);
			job->action();
			input->jobQueue->FinishedJob(job);

#if defined (_DEBUG)
			Debugger::DEBUG_PRINT("JobRunner \"%s\": Finished Job \"%s\". \n", threadName, jobName.c_str());
#endif
		}

		isStopped = input->jobQueue->IsStopped();

	} while (isStopped == false);

#ifdef _DEBUG
	Debugger::DEBUG_PRINT("JobRunner \"%s\": Shutting down. \n", threadName);
#endif
	return 0;
}




}//Namespace Engine
}//Namespace JobSys
