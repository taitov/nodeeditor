file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_3_http_server ${CPPS})

target_include_directories(step_3_http_server PUBLIC "../../../tvm/examples/step_3_http_server")
target_include_directories(step_3_http_server PUBLIC "../../../tvm/include")

target_link_libraries(step_3_http_server nodes -lpthread)
