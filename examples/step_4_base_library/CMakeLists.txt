file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_4_base_library ${CPPS})

target_include_directories(step_4_base_library PUBLIC "../../../tvm/examples/step_4_base_library")
target_include_directories(step_4_base_library PUBLIC "../../../tvm/include")

target_link_libraries(step_4_base_library nodes -lpthread)
