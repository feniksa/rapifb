#pragma once

#include <boost/core/noncopyable.hpp>
#include <boost/noncopyable.hpp>

class FileDescriptor final : private boost::noncopyable
{
public:
	FileDescriptor(const char* path);
	~FileDescriptor();

	operator int() const noexcept { return m_resource; }
	int resource() const noexcept { return m_resource; }
private:
	int m_resource;

};

