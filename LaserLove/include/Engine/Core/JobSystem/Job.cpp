#include "Engine/Core/JobSystem/Job.hpp"

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Constructors
Job::Job(size_t jobCategoryID, job_cb job, void* dataStructure, bool cameFromJobSystemObjectPool, bool immediatlyDetach)
	: m_jobCategoryID(jobCategoryID),
	m_jobFunction(job),
	m_dataStructure(dataStructure),
	m_CameFromJobSystemObjectPool(cameFromJobSystemObjectPool),
	m_immediatlyDetach(immediatlyDetach)
{
	m_referenceCount = 1;
}
Job::~Job()
{
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Setters
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Getters
job_cb Job::GetJobFunction() const
{
	return m_jobFunction;
}

size_t Job::GetJobCategoryID() const
{
	return m_jobCategoryID;
}

const bool Job::GetCameFromJobSystemObjectPool() const
{
	return m_CameFromJobSystemObjectPool;
}

void* Job::GetDataStructureContained() const
{
	return m_dataStructure;
}

size_t Job::GetJobReferenceCount() const
{
	return m_referenceCount;
}

const bool Job::GetImmediatlyDetach() const
{
	return m_immediatlyDetach;
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//JobRef Counting
void Job::JobAcquire()
{
	m_referenceCount++;
}

void Job::JobRelease()
{
	if ((m_referenceCount) > 0)
	{
		(m_referenceCount--);
	}
}



//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Static methods
void Job::RunJob(Job* job)
{
	if (job == nullptr)
	{
		return;
	}

	job_cb func = job->GetJobFunction();
	func(job->GetDataStructureContained());
}