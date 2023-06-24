#include "JobRunner.h"

namespace Engine
{
namespace JobSystem
{


//DWORD WINAPI JobRunnerRountine(void* threadInput)
//{
//	assert(threadInput);
//
//	JobRunnerInput* input = reinterpret_cast<JobRunnerInput*>(threadInput);
//	assert(input->jobQueue);
//
//	Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Queue \"%s\". \n", input->threadName.c_str(), input->jobQueue->GetName().c_str());
//
//	bool isStopped = false;
//
//	do
//	{
//		Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Acquiring Job. \n", input->threadName.c_str());
//		Job* pJob = input->jobQueue->Get();
//		if (pJob)
//		{
//			Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Job \"%s\" on Processor %d. \n", input->threadName.c_str(), pJob->jobName.c_str(), GetCurrentProcessorNumber());
//			input->jobQueue->StartingJob(pJob);
//			pJob->action();
//
//			Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Finished Job \"%s\". \n", input->threadName.c_str(), pJob->jobName.c_str());
//			input->jobQueue->FinishedJob(pJob);
//		}
//
//		isStopped = input->jobQueue->IsStopped();
//
//	} while (isStopped == false);
//
//#ifdef _DEBUG
//	Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Shutting down. \n", input->threadName.c_str());
//#endif
//	return 0;
//}

}//Namespace Engine
}//Namespace JobSystem