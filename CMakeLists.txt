cmake_minimum_required (VERSION 2.6)
project (SimpleProtocol)
# The version number.
set (SimplePacket_VERSION_MAJOR 1)
set (SimplePacket_VERSION_MINOR 0)

 
include_directories(
	include
	include/internal
)

set(Sources 
	${CMAKE_SOURCE_DIR}/include/SimpleProtocol.h 
	${CMAKE_SOURCE_DIR}/include/internal/PacketPool.h 
	${CMAKE_SOURCE_DIR}/src/PacketPool.cpp 
	${CMAKE_SOURCE_DIR}/include/internal/Decoder.h 
	${CMAKE_SOURCE_DIR}/src/Decoder.cpp 
	${CMAKE_SOURCE_DIR}/include/internal/Encoder.h 
	${CMAKE_SOURCE_DIR}/src/Encoder.cpp 
	${CMAKE_SOURCE_DIR}/include/internal/Packet.h
	${CMAKE_SOURCE_DIR}/src/Packet.cpp 
)
 
add_library(SimpleProtocol SHARED ${Sources})
target_compile_definitions(SimpleProtocol PRIVATE SIMPLEPROTOCOL_EXPORTS )
add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 -D_CRT_SECURE_NO_WARNINGS)

add_executable(SimpleFileStream samples/c/SimpleFileStream.c )
target_link_libraries(SimpleFileStream SimpleProtocol)

find_package(Boost COMPONENTS system)

if(Boost_FOUND)
	include_directories( ${Boost_INCLUDE_DIRS} )
	link_directories(${Boost_LIBRARY_DIRS})
	

	add_executable(udp_client samples/cpp/udp_client.cpp )
	target_link_libraries(udp_client SimpleProtocol ${Boost_LIBRARIES})

	add_executable(udp_server samples/cpp/udp_server.cpp )
	target_link_libraries(udp_server SimpleProtocol ${Boost_LIBRARIES})
endif()

enable_testing()

add_subdirectory(test ${PROJECT_SOURCE_DIR}/buildTest)
