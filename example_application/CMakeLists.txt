add_executable(fb_application_example main.cpp)

target_link_libraries(fb_application_example PRIVATE daq::opendaq)

add_dependencies(
	fb_application_example
	example_module
    daq::ref_device_module
    daq::ref_fb_module
)