#include "MemBlock.h"
#include <sys/mman.h>
#include <string.h>

MemBlock::MemBlock(char* mem, int size) : m_mem(mem), m_size(size)
{
}

MemBlock::~MemBlock()
{
	munmap(m_mem, m_size);
}

void MemBlock::clear(int pattern) 
{
	memset(m_mem, pattern, m_size);
}

