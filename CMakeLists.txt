# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)

# Define a flag we can pass from the command line (defaults to OFF)
option(BUILD_UNIT_TESTS "Build the host-based unit tests instead of firmware" OFF)

if(BUILD_UNIT_TESTS)
    # ==============================================================
    # HOST UNIT TEST BUILD
    # ==============================================================
    project(display_tests)
    
    # Create a standard desktop executable
    add_executable(unit_test_app 
        test/test_main.c 
        test/mock_os_abstraction.c
        test/mock_display_driver.c
        src/state_manager.c 
        src/display_manager.c
    )
    
    # Point it to your standard headers
    target_include_directories(unit_test_app PRIVATE include)

else()
    # ==============================================================
    # ZEPHYR FIRMWARE BUILD
    # ==============================================================
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
    project(minimal)

    zephyr_include_directories(include)

    target_sources(app PRIVATE 
        src/main.c 
        src/state_manager.c 
        src/display_manager.c 
        src/button_listener.c
        src/os_abstraction_zephyr.c
        src/zephyr_display_driver.c 
        src/zephyr_button_driver.c    
    )
endif()
