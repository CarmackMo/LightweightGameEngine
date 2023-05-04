#include "JobRunner.h"

DWORD WINAPI JobRunner(void* i_pThreadInput)
{
	assert(i_pThreadInput);

	JobRunnerInput* pInput = reinterpret_cast<JobRunnerInput*>(i_pThreadInput);
	assert(pInput->m_pQueue);

	Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Queue \"%s\".", pInput->m_ThreadName.c_str(), pInput->m_pQueue->GetName().c_str());

	bool bDone = false;

	do
	{
		Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Acquiring Job.", pInput->m_ThreadName.c_str());
		QueuedJob* pJob = pInput->m_pQueue->GetWhenAvailable();
		if (pJob)
		{
			Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Starting Job \"%s\" on Processor %d.", pInput->m_ThreadName.c_str(), pJob->JobName.c_str(), GetCurrentProcessorNumber());
			pInput->m_pQueue->StartingJob(pJob);
			pJob->Function();

			Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Finished Job \"%s\".", pInput->m_ThreadName.c_str(), pJob->JobName.c_str());
			pInput->m_pQueue->FinishedJob(pJob);
		}

		bDone = pInput->m_pQueue->ShutdownRequested();

	} while (bDone == false);

#ifdef _DEBUG
	Engine::Debugger::DEBUG_PRINT("JobRunner \"%s\": Shutting down.", pInput->m_ThreadName.c_str());
#endif
	return 0;
}