set(COMPONENT_SRCDIRS "lvgl/src/lv_core" 
                      "lvgl/src/lv_draw" 
                      "lvgl/src/lv_font" 
                      "lvgl/src/lv_gpu" 
                      "lvgl/src/lv_hal" 
                      "lvgl/src/lv_misc" 
                      "lvgl/src/lv_themes"
                      "lvgl/src/lv_widgets")
set(COMPONENT_ADD_INCLUDEDIRS "." 
                              "lvgl"
                              "lvgl/src/lv_core"
                              "lvgl/src/lv_draw"
                              "lvgl/src/lv_font"
                              "lvgl/src/lv_gpu"
                              "lvgl/src/lv_hal"
                              "lvgl/src/lv_misc"
                              "lvgl/src/lv_themes"
                              "lvgl/src/lv_widgets")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

register_component()
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)
