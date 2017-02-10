#include "Engine/Core/JobSystem/JobSystem.hpp"
#include "Engine/Utils/Thread.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/JobSystem/JobConsumer.hpp"

JobSystem* JobSystem::s_jobSystem = nullptr;

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Constructors
JobSystem::JobSystem(int desiredWorkerThreadCount, size_t desiredCategoryCount, size_t numberOfJobsToPreallocate, bool makeGlobal)
	: categoryCount(desiredCategoryCount),
	workerThreadCount(desiredWorkerThreadCount),
	is_running(true)
{
	if (makeGlobal == true)
	{
		s_jobSystem = this;
	}
	if (categoryCount == 0)
	{
		categoryCount = 1;
	}
	if (workerThreadCount <= 0)
	{
		workerThreadCount = Thread::GetComputerCoreCount() + workerThreadCount;
		if (workerThreadCount <= 0)
		{
			workerThreadCount = 1;
		}
	}
	JobPool.init(numberOfJobsToPreallocate);
	m_threads.resize(workerThreadCount);
	whichConsumersAreBusy.resize(workerThreadCount);
	m_jobs.resize(categoryCount);
	for (size_t i = 0; i < categoryCount; i++)
	{
		m_jobs[i] = new threadsafe_queue<Job*>();
	}
	m_consumers.resize(m_threads.size());
	AssignedCategoryPerConsumer.resize(m_threads.size());
	jobCountPerCategory.resize(m_jobs.size());
	for (size_t i = 0; i < m_threads.size(); i++)
	{
		m_consumers[i].ChangeCategoryFocus((i % categoryCount));
		m_threads[i] = Thread::ThreadCreate(JobConsumerThread, (void*)i);
	}

}

JobSystem::~JobSystem()
{
	for (size_t i = 0; i < categoryCount; i++)
	{
		Job* job;
		while (m_jobs.at(i)->deque(&job))
		{
			JobDetach(job);
		}
		delete m_jobs[i];
	}
	m_jobs.clear();

	for (size_t i = 0; i < m_threads.size(); i = i)
	{
		if (m_threads.at(i) != nullptr)
		{
			Thread::ThreadDetach(m_threads.at(i));
		}
		m_threads.erase(m_threads.begin());
	}
	JobPool.exit();
}



//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Static Methods
void JobSystem::StartupJobSystem(int numberOfWorkerThreads, size_t numberOfCategories, size_t numberOfJobsToPreallocate)
{
	if (s_jobSystem != nullptr)
	{
		return;
	}
	
	s_jobSystem = new JobSystem(numberOfWorkerThreads, numberOfCategories, numberOfJobsToPreallocate);
}

void JobSystem::ShutdownJobSystem()
{
	if (s_jobSystem == nullptr)
	{
		return;
	}
	s_jobSystem->is_running = false;
	bool allThreadsNotJoined = true;
	while (allThreadsNotJoined)
	{
		for (size_t i = 0; i < s_jobSystem->m_threads.size(); i = i)
		{
			Thread::ThreadJoin(s_jobSystem->m_threads[i]);
			s_jobSystem->m_threads.erase(s_jobSystem->m_threads.begin());
		}
		if (s_jobSystem->m_threads.size() == 0)
		{
			allThreadsNotJoined = false;
		}
	}
	delete s_jobSystem;
	s_jobSystem = nullptr;
}

threadsafe_queue<Job*>* JobSystem::GlobalGetQueueForCategory(size_t JobCategoryID)
{
	if (s_jobSystem == nullptr)
	{
		return nullptr;
	}
	return s_jobSystem->GetQueueForCategory(JobCategoryID);
}

std::vector<threadsafe_queue<Job*>*> JobSystem::GlobalGetQueueForAllCategories()
{
	if (s_jobSystem == nullptr)
	{
		return std::vector<threadsafe_queue<Job*>*>();
	}
	return s_jobSystem->GetQueueForAllCategories();
}

void JobSystem::JobDispatch(Job* jp, size_t category)
{
	if (jp == nullptr || s_jobSystem == nullptr || category >= s_jobSystem->m_jobs.size())
	{
		return;
	}
	jp->JobAcquire();
	s_jobSystem->m_jobs.at(category)->enque(jp); 
	//Signal(gSystem->signal);
}

void JobSystem::JobJoin(Job* jp)
{
	if (jp == nullptr || s_jobSystem == nullptr)
	{
		return;
	}
	jp->JobRelease();
	if (jp->GetJobReferenceCount() > 1)
	{
		return;
	}
	//size_t catId = jp->GetJobCategoryID();

	bool isFromPool = jp->GetCameFromJobSystemObjectPool();
	if (isFromPool)
	{
		s_jobSystem->JobPool.Dealloc(jp);
	}
	else
	{
		delete jp;
	}
}
void JobSystem::JobDetach(Job* jp)
{
	if (jp == nullptr || s_jobSystem == nullptr)
	{
		return;
	}

	bool isFromPool = jp->GetCameFromJobSystemObjectPool();
	if (isFromPool)
	{
		s_jobSystem->JobPool.Dealloc(jp);
	}
	else
	{
		delete jp;
	}
}

void JobSystem::ReEnqueJob(Job* jp)
{
	if (jp == nullptr)
	{
		return;
	}
	JobDispatch(jp, jp->GetJobCategoryID());
}

Job* JobSystem::CreateAndAddJob(size_t categoryID, job_cb function, void* data)
{
	Job* job = s_jobSystem->JobPool.Alloc(categoryID, function, data, true);
	JobDispatch(job, categoryID);
	return job;
}

void JobSystem::ChangeJobConsumerCategoryFocus(size_t jobConsumer, size_t categoryID)
{
	if (s_jobSystem == nullptr)
	{
		return;
	}
	JobConsumer* consumer = s_jobSystem->GetConsumerAsPointer(jobConsumer);
	if (consumer == nullptr)
	{
		return;
	}
	consumer->ChangeCategoryFocus(categoryID);
}

void JobSystem::JobConsumerThread(void* data)
{
	size_t consumerIdx = (size_t)data;
	if (s_jobSystem == nullptr)
	{
		return;
	}
	JobConsumer* Consumer = s_jobSystem->GetConsumerAsPointer(consumerIdx);
	if (Consumer == nullptr)
	{
		return;
	}
	while (s_jobSystem != nullptr && s_jobSystem->is_running)
	{
		Consumer->ConsumeAll();
		Thread::ThreadYield();
	}
	Consumer->SetShutdownMode(true);
	Consumer->ConsumeAll();
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//operations


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Getters
threadsafe_queue<Job*>* JobSystem::GetQueueForCategory(size_t JobCategoryID)
{
	if (JobCategoryID >= m_jobs.size())
	{
		return nullptr;
	}
	return (m_jobs.at(JobCategoryID));
}

std::vector<threadsafe_queue<Job*>*> JobSystem::GetQueueForAllCategories()
{
	std::vector<threadsafe_queue<Job*>*> jobQueues;

	for (size_t i = 0; i < m_jobs.size(); i++)
	{
		jobQueues.push_back(GetQueueForCategory(i));
	}

	return jobQueues;
}


JobConsumer* JobSystem::GetConsumerAsPointer(size_t consumerIdx)
{
	if (consumerIdx >= m_consumers.size())
	{
		return nullptr;
	}
	return &(m_consumers[consumerIdx]);
}
