#pragma once
#include "Dependency.h"
#include "HashedString.h"
#include "JobRunner.h"
#include "SharedJobQueue.h"

class JobSys;







class JobSys
{
public:
	JobSys();
	~JobSys();

	void Init();


};


#include "JobSys.inl"