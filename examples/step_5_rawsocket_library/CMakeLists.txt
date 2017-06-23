file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_5_rawsocket_library ${CPPS})

target_include_directories(step_5_rawsocket_library PUBLIC "../../../tfvm/examples/step_5_rawsocket_library")
target_include_directories(step_5_rawsocket_library PUBLIC "../../../tfvm/include")

target_link_libraries(step_5_rawsocket_library nodes -lpthread)
