#include "Engine/Core/JobSystem/JobConsumer.hpp"
#include "Engine/Core/JobSystem/JobSystem.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Engine/Utils/ThreadSafe/CASCalls.hpp"

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Constructors
JobConsumer::JobConsumer(size_t jobCategoryID)
	: m_categoryID(jobCategoryID),
	ShutDownMode(false)
{
	ChangeCategoryFocus(m_categoryID);
}

JobConsumer::JobConsumer(const JobConsumer& other)
	: m_categoryID(other.m_categoryID),
	m_busy(other.m_busy),
	my_queues(other.my_queues),
	ShutDownMode(false)
{

}
JobConsumer::~JobConsumer()
{

}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Consume
void JobConsumer::ConsumeAll()
{
	while (Consume());
}

void JobConsumer::ConsumeAllTimed(float secondsBeforeStop)
{
	uint64_t startTime = Performance::GetCurrentPerformanceCount();
	uint64_t currentTime = startTime;
	bool keepRunning = true;
	while (keepRunning && Consume())
	{
		currentTime = Performance::GetCurrentPerformanceCount();
		uint64_t diff = currentTime - startTime;
		if ((float)Performance::PerformanceCountToSeconds(diff) >= secondsBeforeStop)
		{
			keepRunning = false;
		}
	}
}

const bool JobConsumer::Consume()
{
	for (size_t qIdx = 0; qIdx < my_queues.size(); qIdx++)
	{
		Job* job;
		if (my_queues.at(qIdx) != nullptr && my_queues.at(qIdx)->deque(&job))
		{
			if (job == nullptr)
			{
				continue;
			}
			bool detach = job->GetImmediatlyDetach();
			(CASCalls::CAS(&m_busy, true, false));
			Job::RunJob(job);
			if (detach)
			{
				JobSystem::JobDetach(job);
			}
			else
			{
				JobSystem::JobJoin(job);
			}
			(CASCalls::CAS(&m_busy, false, true));
			return true;
		}
	}
	return false;
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Setters
void JobConsumer::ClearAndSetQueue(threadsafe_queue<Job*>* queue)
{
	my_queues.clear();
	my_queues.push_back(queue);
}

void JobConsumer::ChangeCategoryFocus(size_t categoryID)
{
	bool categorySet = false;
	while (categorySet == false)
	{
		size_t curVal = m_categoryID;
		if (CASCalls::CAS(&m_categoryID, categoryID, m_categoryID) == curVal)
		{
			categorySet = true;
		}
	}
	if (JobSystem::s_jobSystem != nullptr)
	{
		my_queues.clear();
		threadsafe_queue<Job*>* focusedQue = JobSystem::GlobalGetQueueForCategory(m_categoryID);
		my_queues.push_back(focusedQue);
		std::vector<threadsafe_queue<Job*>*> otherQueues = JobSystem::GlobalGetQueueForAllCategories();
		for (size_t i = 0; i < otherQueues.size(); i++)
		{
			if (focusedQue == otherQueues.at(i) || otherQueues.at(i) == nullptr)
			{
				continue;
			}
			my_queues.push_back(otherQueues.at(i));
		}
	}
}

void JobConsumer::SetShutdownMode(bool mode)
{
	ShutDownMode = mode;
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//Getters
const size_t JobConsumer::GetCategoryID() const
{
	return m_categoryID;
}

const std::vector<threadsafe_queue<Job*>*>* JobConsumer::GetQueues() const
{
	return &my_queues;
}

const bool JobConsumer::GetBusy() const
{
	return m_busy;
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//static functions

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//operators
