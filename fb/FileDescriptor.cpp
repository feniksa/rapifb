#include "FileDescriptor.h"
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <stdexcept>

FileDescriptor::FileDescriptor(const char* path) 
{
	m_resource = open(path, O_RDWR);
	if (m_resource < 0) {
		throw std::runtime_error("Can't open " + std::string(path));
	}
}

FileDescriptor::~FileDescriptor() 
{
	close(m_resource);
}

