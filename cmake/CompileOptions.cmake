add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-fno-elide-constructors>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wall>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wextra>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Winit-self>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wold-style-cast>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Woverloaded-virtual>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wuninitialized>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wmissing-declarations>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wstrict-aliasing>)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
	# enable hardware math for float's on arms
	add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-mfloat-abi=hard>)
endif()

if (FORCE_COLORED_OUTPUT)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()
