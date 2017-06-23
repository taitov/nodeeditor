file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(step_1_helloworld ${CPPS})

#qt5_wrap_cpp(gui_SRC gui.h gui.cpp)

target_include_directories(step_1_helloworld PUBLIC "../../../tfvm/examples/step_1_helloworld")
target_include_directories(step_1_helloworld PUBLIC "../../../tfvm/include")

target_link_libraries(step_1_helloworld nodes -lpthread)
