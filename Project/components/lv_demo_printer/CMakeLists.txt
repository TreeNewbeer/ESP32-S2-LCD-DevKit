set(COMPONENT_SRCDIRS "." "images" )
set(COMPONENT_ADD_INCLUDEDIRS "." "images")
set(COMPONENT_REQUIRES lvgl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

register_component()
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)
