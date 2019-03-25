cmake_minimum_required(VERSION 3.10)
project(humanAnimation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CATCH_BUILD_TESTING OFF CACHE BOOL "")
set(CATCH_BUILD_EXAMPLES OFF CACHE BOOL "")
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "")
set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "")
set(BUILD_TESTING OFF CACHE BOOL "")

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)
find_package(glm REQUIRED)

include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(./DTrack_src/include)
include_directories(./json/single_include/nlohmann)
include_directories(./catch2/single_include)

set(LINKER_FLAGS
		-Wl,--sort-common,--as-needed)

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

		# Compilateur GNU
	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
		set(COMPILER_OPTIONS
				PRIVATE -pipe
				-Wall
				-Wextra
				-march=native)

		set(LINKER_FLAGS
				${LINKER_FLAGS}
				-lstdc++)

	endif ()

elseif (${CMAKE_HOST_WIN32}) # Partie pour Windows

	# Compilateur Microsoft
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
		# Tu te débrouilles.
		set(COMPILER_OPTIONS
				PRIVATE /Wall)

	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
		set(COMPILER_OPTIONS
				PRIVATE -pipe
				-Wall
				-Wextra
				-march=native)

		set(LINKER_FLAGS
				${LINKER_FLAGS})
	endif ()
endif ()

set(SRC_FILES
		humanAnimation.cpp
		humanAnimation.h
		IUnityInterface.h
		bodyParts.cpp
		bodyParts.h
		for_windows.h)

set(DTRACK_FILES
		./DTrack_src/include/DTrackDataTypes.h
		./DTrack_src/include/DTrackNet.h
		./DTrack_src/include/DTrackParse.hpp
		./DTrack_src/include/DTrackParser.hpp
		./DTrack_src/include/DTrackSDK.hpp
		./DTrack_src/src/DTrackNet.cpp
		./DTrack_src/src/DTrackParse.cpp
		./DTrack_src/src/DTrackParser.cpp
		./DTrack_src/src/DTrackSDK.cpp)

set(TESTS_FILES
		Tests/catch2Include.cpp Tests/mainTests.cpp)

add_subdirectory(json)
add_subdirectory(catch2)

add_library(humanAnimation SHARED ${SRC_FILES} ${DTRACK_FILES})
target_compile_options(humanAnimation ${COMPILER_OPTIONS} ${OpenMP_CXX_FLAGS})
target_link_libraries(humanAnimation ${LINKER_FLAGS} ${OpenMP_CXX_LIBRARIES} ${OpenCV_LIBS} nlohmann_json::nlohmann_json)

add_executable(humanAnimationTests ${TESTS_FILES})
add_dependencies(humanAnimationTests humanAnimation)
target_compile_options(humanAnimationTests ${COMPILER_OPTIONS})
target_link_libraries(humanAnimationTests ${LINKER_FLAGS} Catch2::Catch2 ${PROJECT_BINARY_DIR}/libhumanAnimation.so)