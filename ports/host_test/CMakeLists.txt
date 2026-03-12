# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)

# ==============================================================
# HOST UNIT TEST BUILD
# ==============================================================
project(display_tests)
    
# Create a standard desktop executable
add_executable(unit_test_app 
    src/test_main.c 
    src/mock_os_abstraction.c
    src/mock_display_driver.c
    ../../src/state_manager.c 
    ../../src/display_manager.c
)
    
# Point it to your standard headers
target_include_directories(unit_test_app PRIVATE ../../include)

