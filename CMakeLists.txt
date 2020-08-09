cmake_minimum_required(VERSION 3.6)
# set targetPlatform, will be passed in from gradle when this sample is completed
# openGL Supportability
# platform		 status
#   (0 12)		   ES2/ES3 not supported
#   [12, 18)		 ES2 only; for ES3, app do dynamic load/detection
#					this applies to the situations that:
#						- minimum API is set to less than 18. In this case
#						  there is no ES3 header/lib support inside NDK
#						- the built APK might be running on newer API phones
#					with dynamic loading of ES3, the same APK would still be able
#					to use ES3. Otherwise, app would stuck with ES2 even phone is
#					is newer than the minimum API level (for example, Android-27 etc).
#
#   [18, 24)		 ES2 & ES3
#					If app is built to only support API-18 or later,
#					set minimum api level to 18 is good enough, NDK supprts ES3
#					with the right header and lib files. No need to use ES3 dynamic
#					detection.
#   [24, infinite)   ES2 & ES3 & Vulkan

project(glrenderer)

include(cmake/config.cmake)

if (WIN32)
  include(cmake/win_config.cmake)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory("engine/deps/glad")
	add_subdirectory("engine/deps/glfw")
elseif (ANDROID)
	include(cmake/android_config.cmake)
	if (${ANDROID_PLATFORM_LEVEL} LESS 12)
	  message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
						  (currently using ${ANDROID_PLATFORM_LEVEL}).")
	  return()
	else ()
	  set(OPENGL_LIB GLESv3)
	endif (${ANDROID_PLATFORM_LEVEL} LESS 12)
	# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE RENDERER_SRC "engine/src/*.cpp")
file(GLOB_RECURSE APP_SRC "sandbox/src/application/*.cpp")

if (WIN32)
	add_library(${PROJECT_NAME} STATIC ${RENDERER_SRC})

	target_link_libraries(${PROJECT_NAME} glad glfw)
	target_include_directories(${PROJECT_NAME} PUBLIC "engine/deps/glad/include")
	target_include_directories(${PROJECT_NAME} PUBLIC "engine/deps/glfw/include")
	target_include_directories(${PROJECT_NAME} PUBLIC "engine/deps/glm")
	target_include_directories(${PROJECT_NAME} PUBLIC "sandbox/src")

	set(SANDBOX_SRC sandbox/src)
	list(APPEND SANDBOX_SRC "sandbox/src/platform/windows/EntryPoint.cpp" ${APP_SRC})
	add_executable(sandbox ${SANDBOX_SRC})
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
	target_link_libraries(sandbox ${PROJECT_NAME})

elseif (ANDROID)
	list(APPEND RENDERER_SRC "sandbox/src/platform/android/EntryPoint.cpp" ${APP_SRC})
	add_library(${PROJECT_NAME} SHARED ${RENDERER_SRC})

	target_include_directories(${PROJECT_NAME} PUBLIC "engine/deps/glm")
	target_include_directories(${PROJECT_NAME} PUBLIC "sandbox/src")
	
	# Include libraries needed for glrenderer lib
	target_link_libraries(${PROJECT_NAME}
				${SANDBOX_SRCS}
				${OPENGL_LIB}
				android
				EGL
				log
				m)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "engine/src")
