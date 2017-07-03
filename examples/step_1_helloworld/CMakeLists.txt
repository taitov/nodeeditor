file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_1_helloworld ${CPPS})

target_include_directories(step_1_helloworld PUBLIC "../../../tvm/examples/step_1_helloworld")
target_include_directories(step_1_helloworld PUBLIC "../../../tvm/include")

target_link_libraries(step_1_helloworld nodes -lpthread)
