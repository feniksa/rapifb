set(HEADERS
	Color.h  
	Error.h  
	FileDescriptor.h  
	FrameBuffer.h  
	MemBlock.h  
	Point.h  
	StbTexture.h  
	Utils.h
)

set(SOURCES
	FileDescriptor.cpp  
	FrameBuffer.cpp  
	MemBlock.cpp  
	StbTexture.cpp  
	StbiImageWriteImpl.cpp  
	StbiImpl.cpp  
	StbiResizeImpl.cpp  
	Utils.cpp
)

add_library(fb ${HEADERS} ${SOURCES})
target_include_directories(fb PUBLIC "${CMAKE_SOURCE_DIR}" "${STB_IMAGE_INCLUDE_DIR}")

