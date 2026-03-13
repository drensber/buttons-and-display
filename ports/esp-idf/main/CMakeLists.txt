# SPDX-License-Identifier: Apache-2.0

idf_component_register(
    SRCS 
        "main.c"
        "os_abstraction_freertos.c"
        "freertos_display_driver.c"
        "freertos_button_driver.c"
        "../../../src/state_manager.c"
        "../../../src/display_manager.c"
        "../../../src/button_listener.c"
    INCLUDE_DIRS 
        "../../../include"
)
