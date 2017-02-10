#pragma once

#ifndef JOBCONSUMER_HPP
#define JOBCONSUMER_HPP
#include "Engine/Utils/ThreadSafe/threadsafe_queue.hpp"
#include "Engine/Core/JobSystem/Job.hpp"
#include <vector>

class JobConsumer
{
private:
	bool ShutDownMode;
	size_t m_categoryID;
	std::vector<threadsafe_queue<Job*>*> my_queues;
	bool m_busy;
protected:
public:
	JobConsumer(size_t jobCategoryID = 0);
	JobConsumer(const JobConsumer& other);
	virtual ~JobConsumer();

	//Consume
	void ConsumeAll();
	void ConsumeAllTimed(float secondsBeforeStop);
	const bool Consume();

	//Setters
	void ClearAndSetQueue(threadsafe_queue<Job*>* queue);
	void ChangeCategoryFocus(size_t categoryID);
	void SetShutdownMode(bool mode);

	//Getters
	const size_t GetCategoryID() const;
	const std::vector<threadsafe_queue<Job*>*>* GetQueues() const;
	const bool GetBusy() const;

	//static methods

	//operators
};
#endif

/*
void GenericJobThread(void*)
{
JobConsumer cConsumer(Generic_Slow,Generic);
While(JObSystemIsRunning())
{
Consumer.Consume_all();
ThreadYield();
}
}
*/