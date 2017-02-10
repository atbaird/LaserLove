#pragma once

#ifndef JOBSYSTEM_HPP
#define JOBSYSTEM_HPP
#include "Engine/Core/JobSystem/Job.hpp"
#include "Engine/Utils/ThreadSafe/threadsafe_queue.hpp"
#include "Engine/Utils/Thread.hpp"
#include "Engine/Utils/ObjectPool.hpp"
#include <vector>

class JobConsumer;

class JobSystem
{
private:
	size_t categoryCount;
	int workerThreadCount;
	bool is_running;
	std::mutex mtx;
	std::vector<threadsafe_queue<Job*>*> m_jobs;
	std::vector<Thread*> m_threads;
	std::vector<JobConsumer> m_consumers;
	std::vector<size_t> jobCountPerCategory;
	std::vector<bool> whichConsumersAreBusy;
	std::vector<size_t> AssignedCategoryPerConsumer;
	ObjectPool<Job> JobPool;

	//constructors
	JobSystem(int desiredWorkerThreadCount, size_t desiredCategoryCount, size_t numberOfJobsToPreallocate = 500, bool makeGlobal = true);
	virtual ~JobSystem();
protected:
public:
	static JobSystem* s_jobSystem;

	//static methods
	static void StartupJobSystem( int numberOfWorkerThreads, size_t numberOfCategories = NUM_OF_JOB_SPEED_TYPES, size_t numberOfJobsToPreallocate = 500);
	static void ShutdownJobSystem();
	static threadsafe_queue<Job*>* GlobalGetQueueForCategory(size_t JobCategoryID);
	static std::vector < threadsafe_queue<Job*>*> GlobalGetQueueForAllCategories();
	static void JobDispatch(Job* jp, size_t category = 0);
	static void JobJoin(Job* jp);
	static void JobDetach(Job* jp);

	static void ReEnqueJob(Job* jp);
	static Job* CreateAndAddJob(size_t categoryID, job_cb function, void* data = nullptr);
	static void ChangeJobConsumerCategoryFocus(size_t jobConsumer, size_t categoryID);

	static void JobConsumerThread(void*);

	//operations

	//Getters
	threadsafe_queue<Job*>* GetQueueForCategory(size_t JobCategoryID);
	std::vector<threadsafe_queue<Job*>*> GetQueueForAllCategories();
protected:
	JobConsumer* GetConsumerAsPointer(size_t consumerIdx);
};
#endif