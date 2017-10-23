 # Copyright (c) 2017 Arthur Gay
 # 
 # Permission is hereby granted, free of charge, to any person obtaining
 # a copy of this software and associated documentation files (the
 # "Software"), to deal in the Software without restriction, including
 # without limitation the rights to use, copy, modify, merge, publish,
 # distribute, sublicense, and/or sell copies of the Software, and to
 # permit persons to whom the Software is furnished to do so, subject to
 # the following conditions:
 # 
 # The above copyright notice and this permission notice shall be
 # included in all copies or substantial portions of the Software.
 # 
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 # EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 # MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 # NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 # LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 # OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 # WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 # 

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

FIND_PACKAGE( Boost 1.40 COMPONENTS system REQUIRED )

# Protobuf
find_package(Protobuf REQUIRED)
set(PROTOBUF_IMPORT_DIRS)
foreach(ITR ${GAZEBO_INCLUDE_DIRS})
  if(ITR MATCHES ".*gazebo-[0-9.]+$")
    set(PROTOBUF_IMPORT_DIRS "${ITR}/gazebo/msgs/proto")
  endif()
endforeach()
set (msgs
  platform-proto/platform.proto
)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})
add_library(platform_msgs SHARED ${PROTO_SRCS})
target_link_libraries(platform_msgs ${PROTOBUF_LIBRARY})

# Gazebo
find_package(gazebo REQUIRED)

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
)
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

# Platform
set(PROGRAM_NAME "Platform Tool")
execute_process(COMMAND git describe --long --dirty --abbrev=10 --tags
				COMMAND tr -d '\n'
				OUTPUT_VARIABLE PROGRAM_VERSION)
execute_process(COMMAND git remote get-url origin
				COMMAND tr -d '\n'
				OUTPUT_VARIABLE PROGRAM_SOURCE)
set(files platformcmd.cc platformcmd.hh)
configure_file(platformcmd.hh.in platformcmd.hh)
add_executable(platform ${files})
target_link_libraries(platform platform_msgs ${GAZEBO_LIBRARIES} pthread)
add_dependencies(platform platform_msgs)
