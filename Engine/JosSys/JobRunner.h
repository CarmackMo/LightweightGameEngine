#pragma once
#include "Debugger.h"
#include "SharedJobQueue.h"


struct JobRunnerInput
{
	SharedJobQueue* m_pQueue;
#ifdef _DEBUG
	std::string		m_ThreadName;
#endif

	JobRunnerInput(SharedJobQueue& i_Queue) :
		m_pQueue(&i_Queue)
	{}
};


struct JobRunnerData
{
	JobRunnerInput		m_Input;
	HANDLE				m_ThreadHandle;
	DWORD				m_ThreadID;

	JobRunnerData(SharedJobQueue& i_Queue) :
		m_Input(i_Queue),
		m_ThreadHandle(INVALID_HANDLE_VALUE),
		m_ThreadID(-1)
	{}
};


DWORD WINAPI JobRunner(void* i_pJobRunnerInput);