cmake_minimum_required(VERSION 3.10)
project(humanAnimation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")


find_package(OpenCV REQUIRED)
find_package(glm REQUIRED)

include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(../DTrack_src/include ../DTrack_src/src)
include_directories(./json/single_include/nlohmann)

# Partie Linux
if (${CMAKE_HOST_UNIX})

	set(LINKER_FLAGS
			-Wl,--sort-common,--as-needed)

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

	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
		set(COMPILER_OPTIONS
				PRIVATE -pipe
				-Wall
				-Wextra
				-march=native)

		set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG")

		set(LINKER_FLAGS
				${LINKER_FLAGS}
				-lstdc++)

	endif ()

elseif (${CMAKE_HOST_WIN32}) # Partie pour Windows

	# Compilateur Microsoft
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
		# Tu te débrouilles.
	endif ()
endif ()

add_subdirectory(json)
add_library(humanAnimation SHARED humanAnimation.cpp humanAnimation.h IUnityInterface.h bodyParts.cpp bodyParts.h for_windows.h)
target_compile_options(humanAnimation ${COMPILER_OPTIONS})
target_link_libraries(humanAnimation ${LINKER_FLAGS} ${OpenCV_LIBS} nlohmann_json::nlohmann_json)