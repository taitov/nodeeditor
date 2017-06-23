file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_2_two_actions_and_memory ${CPPS})

target_include_directories(step_2_two_actions_and_memory PUBLIC "../../../tfvm/examples/step_2_two_actions_and_memory")
target_include_directories(step_2_two_actions_and_memory PUBLIC "../../../tfvm/include")

target_link_libraries(step_2_two_actions_and_memory nodes -lpthread)
