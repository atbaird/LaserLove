#pragma once

#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <mutex>

struct page_node
{
	page_node* next;
};

struct buffer
{
	buffer* node;
};

template <typename T>
class ObjectPool
{
private:
	std::mutex mtx;
	page_node* m_free_stack;
	size_t m_count_per_block = 0;
	size_t currentNumOfFreeBlocks = 0;
	T* m_buffers;
	T* m_first;
public:
	void init(const size_t& count_per_block);
	void exit();

	template <typename... Args>
	T* Alloc(Args... args)
	{
		mtx.lock();
		T* t = (T*)m_free_stack;
		page_node* temp = m_free_stack->next;
		m_free_stack = nullptr;
		m_free_stack = temp;
		new (t) T(args...);
		currentNumOfFreeBlocks--;
		mtx.unlock();
		return t;
	}

	void Dealloc(T* t);
};

//-----------------------------------------------------------------------------------------------------------
template <typename T>
void ObjectPool<T>::init(const size_t& count_per_block)
{
	mtx.lock();
	currentNumOfFreeBlocks += count_per_block;
	m_count_per_block += count_per_block;
	size_t sizeOfT = sizeof(T);
	size_t buffer_size = sizeOfT * count_per_block;
	ASSERT_OR_DIE(sizeOfT >= sizeof(page_node), "ObjectPool, sizeof T is less than size of page node!");
	m_buffers = (T*)malloc(buffer_size);
	m_first = m_buffers;
	m_free_stack = nullptr;
	for (int i = count_per_block - 1; i >= 0; i--)
	{
		T* ptr = &m_buffers[i];
		page_node* node = (page_node*)ptr;
		node->next = m_free_stack;
		m_free_stack = nullptr;
		m_free_stack = node;
	}
	mtx.unlock();
}

//-----------------------------------------------------------------------------------------------------------
template <typename T>
void ObjectPool<T>::exit()
{
	mtx.lock();
	currentNumOfFreeBlocks = 0;
	m_count_per_block = 0;
	free(m_first);
	m_buffers = nullptr;
	m_first = nullptr;
	m_free_stack = nullptr;
	mtx.unlock();
}

//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
template <typename T>
void ObjectPool<T>::Dealloc(T* t)
{
	mtx.lock();
	page_node* page = (page_node*)t;
	t->~T();
	page->next = m_free_stack;
	m_free_stack = nullptr;
	m_free_stack = page;
	currentNumOfFreeBlocks++;
	mtx.unlock();
}

#endif