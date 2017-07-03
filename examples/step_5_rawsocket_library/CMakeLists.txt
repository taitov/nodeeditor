file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_5_rawsocket_library ${CPPS})

target_include_directories(step_5_rawsocket_library PUBLIC "../../../tvm/examples/step_5_rawsocket_library")
target_include_directories(step_5_rawsocket_library PUBLIC "../../../tvm/include")

target_link_libraries(step_5_rawsocket_library nodes -lpthread)
