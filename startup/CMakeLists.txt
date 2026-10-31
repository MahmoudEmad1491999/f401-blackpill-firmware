# startup_stub: minimal vector table + Reset_Handler.
# Replaced by the full startup in the next Phase-0 task.
add_library(startup_stub OBJECT
    startup_stub.c
)
target_link_libraries(startup_stub PRIVATE firmware_config)
