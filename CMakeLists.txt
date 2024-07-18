
cmake_minimum_required(VERSION 3.5)
project("capnp-echo" VERSION 0.2 LANGUAGES CXX)

find_package(CapnProto CONFIG REQUIRED)

capnp_generate_cpp(echoSources echoHeaders echo.capnp)
add_executable(echo-client client.cpp ${echoSources})
add_executable(echo-server server.cpp ${echoSources})
target_link_libraries(echo-client PRIVATE CapnProto::capnp-rpc)
target_link_libraries(echo-server PRIVATE CapnProto::capnp-rpc)
target_include_directories(echo-client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(echo-server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
