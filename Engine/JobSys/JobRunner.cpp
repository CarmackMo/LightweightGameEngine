#include "JobRunner.h"

namespace Engine
{
namespace JobSystem
{


DWORD WINAPI JobRunner(void* i_pThreadInput)
{
	assert(i_pThreadInput);

	JobRunnerInput* pInput = reinterpret_cast<JobRunnerInput*>(i_pThreadInput);
	assert(pInput->m_pQueue);

	Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Queue \"%s\". \n", pInput->m_ThreadName.c_str(), pInput->m_pQueue->GetName().c_str());

	bool bDone = false;

	do
	{
		Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Acquiring Job. \n", pInput->m_ThreadName.c_str());
		Job* pJob = pInput->m_pQueue->Get();
		if (pJob)
		{
			Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Job \"%s\" on Processor %d. \n", pInput->m_ThreadName.c_str(), pJob->jobName.c_str(), GetCurrentProcessorNumber());
			pInput->m_pQueue->StartingJob(pJob);
			pJob->action();

			Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Finished Job \"%s\". \n", pInput->m_ThreadName.c_str(), pJob->jobName.c_str());
			pInput->m_pQueue->FinishedJob(pJob);
		}

		bDone = pInput->m_pQueue->IsStopped();

	} while (bDone == false);

#ifdef _DEBUG
	Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Shutting down. \n", pInput->m_ThreadName.c_str());
#endif
	return 0;
}

}//Namespace Engine
}//Namespace JobSystem