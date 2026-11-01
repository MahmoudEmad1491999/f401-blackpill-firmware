# startup: full vector table + Reset_Handler (.data/.bss init) + weak handlers.
add_library(startup OBJECT
    startup.c
)
target_link_libraries(startup PRIVATE firmware_config)
