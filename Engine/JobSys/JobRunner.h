#pragma once
#include <string>
#include "Debugger.h"
#include "./SharedJobQueue.h"

namespace Engine
{
namespace JobSystem
{

/**
 *	@brief This struct serves as the input parameter this is going past into the thread
 *		   function when creating a thread to run a new job runner.
 */
struct JobRunnerInput
{
	SharedJobQueue* jobQueue;
#ifdef _DEBUG
	std::string		threadName;
#endif

	JobRunnerInput(SharedJobQueue& jobQueue) :
		jobQueue(&jobQueue)
	{}
};


struct JobRunner
{
	JobRunnerInput	threadInput;
	HANDLE			threadHandle;
	DWORD			threadID;

	JobRunner(SharedJobQueue& queue) :
		threadInput(queue),
		threadHandle(INVALID_HANDLE_VALUE),
		threadID(-1)
	{}
};



struct JobRunnerTest
{
	SharedJobQueue* jobQueue;
	HANDLE			threadHandle;
	DWORD			threadID;
#ifdef _DEBUG
	std::string		threadName;
#endif

	JobRunnerTest(SharedJobQueue& jobQueue) :
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
	JobRunnerInput* input = reinterpret_cast<JobRunnerInput*>(threadInput);
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
			string jobName = job->jobName;
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
}//Namespace JobSystem
