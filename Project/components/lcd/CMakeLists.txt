set(COMPONENT_SRCS "lcd.c" "ft5x06.c")
set(COMPONENT_ADD_INCLUDEDIRS "include")
set(COMPONENT_REQUIRES i2s_parallel)

register_component()
