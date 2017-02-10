#pragma once
#ifndef JOB_HPP
#define JOB_HPP

enum JobSpeedType
{
	GENERIC,
	GENERIC_SLOW,
	NUM_OF_JOB_SPEED_TYPES
};

typedef void(*job_cb)(void*);

class Job
{
private:
	job_cb m_jobFunction;
	void* m_dataStructure;
	size_t m_jobCategoryID;
	size_t m_referenceCount; //standard atomic size_t
	bool m_CameFromJobSystemObjectPool;
	bool m_immediatlyDetach;
protected:
public:
	//Constructors
	Job(size_t jobCategoryID, job_cb job, void* dataStructure = nullptr, bool cameFromJobSystemObjectPool = false, bool immediatlyDetach = false);
	~Job();

	//Setters

	//Getters
	job_cb GetJobFunction() const;
	size_t GetJobCategoryID() const;
	const bool GetCameFromJobSystemObjectPool() const;
	void* GetDataStructureContained() const;
	size_t GetJobReferenceCount() const;
	const bool GetImmediatlyDetach() const;

	//JobRef Counting
	void JobAcquire();
	void JobRelease();

	//Static Methods
	static void RunJob(Job* job);
	
};
#endif