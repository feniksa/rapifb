#pragma once



class FileDescriptor final
{
public:
	FileDescriptor(const char* path);
	~FileDescriptor();

	FileDescriptor(const FileDescriptor&)            = delete;
	FileDescriptor& operator=(const FileDescriptor&) = delete;

	operator int() const noexcept { return m_resource; }
	int resource() const noexcept { return m_resource; }
private:
	int m_resource;	

};

