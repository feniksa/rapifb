#pragma once

#include <boost/noncopyable.hpp>

class MemBlock : private boost::noncopyable
{
public:
	MemBlock(char* mem, int size);
	~MemBlock();

	char* data() const noexcept { return m_mem; }
	int size() const noexcept { return m_size; }

	char& operator[](const int index) noexcept { return m_mem[index]; }

	void clear(int pattern);
private:
	char* m_mem;
	int m_size;
};

