cmake_minimum_required(VERSION 3.10)
project(humanAnimation)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

set(LINKER_FLAGS
		-Wl,--sort-common,--as-needed)

include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})

# Partie Linux
if (${CMAKE_HOST_UNIX})

	# Pour compilateur Intel.
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
		set(COMPILER_OPTIONS
				PRIVATE -pipe
				-Wall
				-Wextra
				-march=native
				-xHost)

		set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG")

		set(LINKER_FLAGS
				${LINKER_FLAGS}
				-shared-intel)
	endif ()

elseif (${CMAKE_HOST_WIN32}) # Partie pour Windows

	# S'il s'agit du compilateur de Microsoft
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
		# Tu te débrouilles.
	endif ()
endif ()

add_library(humanAnimation SHARED humanAnimation.cpp humanAnimation.h IUnityInterface.h bodyParts.cpp bodyParts.h)
target_compile_options(humanAnimation ${COMPILER_OPTIONS})
target_link_libraries(humanAnimation ${LINKER_FLAGS} ${OpenCV_LIBS})