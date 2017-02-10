#pragma once

#ifndef THREADSAFE_VECTOR_HPP
#define THREADSAFE_VECTOR_HPP
#include <vector>
#include <mutex>

template <typename type>
class threadsafe_vector : protected std::vector<type>
{
	std::mutex mtx;
public:
	void push_back(const type& val)
	{
		mtx.lock();
		//Lock(cs);
		vector::push_back(val);
		mtx.unlock();
	}

	void pop_back()
	{
		mtx.lock();
		vector::pop_back();
		mtx.unlock();
	}

	void insert(iterator position, const value_type& val)
	{
		mtx.lock();
		vector::insert(position, val);
		mtx.unlock();
	}

	void insert(iterator position, size_t n, const type& val)
	{
		mtx.lock();
		vector::insert(position, n, val);
		mtx.unlock();
	}

	void erase(iterator pos)
	{
		mtx.lock();
		vector::erase(pos);
		mtx.unlock();
	}

	void erase(iterator first, iterator last)
	{
		mtx.lock();
		vector::erase(first, last);
		mtx.unlock();
	}

	void resize(size_t n, type val = type())
	{
		mtx.lock();
		vector::resize(n);
		mtx.unlock();
	}

	void reserve(size_t n)
	{
		mtx.lock();
		vector::reserve(n);
		mtx.unlock();
	}

	void empty()
	{
		mtx.lock();
		vector::empty();
		mtx.unlock();
	}

	void clear()
	{
		mtx.lock();
		vector::clear();
		mtx.unlock();
	}

	void assign(size_t n, const type& val)
	{
		mtx.lock();
		vector::assign(n, val);
		mtx.unlock();
	}

	type operator[](size_t n)
	{
		mtx.lock();
		vector::operator[](n)
		mtx.unlock();
	}
	const type operator[](size_t n) const
	{
		mtx.lock();
		vector::operator[](n)
		mtx.unlock();
	}

	const size_t size() const
	{
		return vector::size();
	}

	type at(size_t i)
	{
		return vector::at(i);
	}
};
#endif