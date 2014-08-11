#pragma once

#include <vector>
#include <assert.h>

#define ALLOC_BULK_ITEMS	100000

template <class T>
class MemAllocator	// Bulk memory allocator: thread unsafe (one thread - one object)
{
public:
	MemAllocator();
	~MemAllocator();
	MemAllocator(int bulkAllocSz);

	T* Alloc();

private:
	int m_bulkAllocSz;

	std::vector <T*> m_aPointers;
	int m_pos;
};

template <class T>
MemAllocator<T>::~MemAllocator()
{
	for (auto pt : m_aPointers)
		delete[] pt;

	m_aPointers.clear();
}

template <class T>
MemAllocator<T>::MemAllocator()
	: MemAllocator<T>::MemAllocator(ALLOC_BULK_ITEMS)
{
	
}

template <class T>
MemAllocator<T>::MemAllocator(int bulkAllocSz)
{
	m_bulkAllocSz = bulkAllocSz;
	m_aPointers.reserve(m_bulkAllocSz);
	
	assert(m_aPointers.empty());
	m_pos = 0;

	// Buffers init
	m_aPointers.push_back(new T[m_bulkAllocSz]);
}

template <class T>
T* MemAllocator<T>::Alloc()
{
	assert(m_pos <= m_bulkAllocSz);

	if (m_pos < m_bulkAllocSz)
	{
		T *ptT = m_aPointers.back() + m_pos;
		m_pos++;
		return ptT;
	}

	// Allocate new bulk for T objects
	m_aPointers.push_back(new T[m_bulkAllocSz]);
	m_pos = 1;

	return m_aPointers.back();
}