# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)

# ==============================================================
# ZEPHYR FIRMWARE BUILD
# ==============================================================
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hatch_take_home)

zephyr_include_directories(../../include)

target_sources(app PRIVATE 
    ../../src/state_manager.c 
    ../../src/display_manager.c
    ../../src/button_listener.c
    src/main.c
    src/os_abstraction_zephyr.c
    src/zephyr_display_driver.c 
    src/zephyr_button_driver.c    
)
