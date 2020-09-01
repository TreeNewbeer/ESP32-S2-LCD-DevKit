#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lcd.h"
#include "ft5x06.h"
#include "lv_demo_printer_theme.h"

#define TAG "main"
#define LV_USE_DEMO_PRINTER 1
#define LVGL_BUFFER_SIZE  (sizeof(lv_color_t) * (LV_HOR_RES_MAX * LV_VER_RES_MAX))
#define LVGL_INIT_PIXCNT  (LV_HOR_RES_MAX * LV_VER_RES_MAX)

void IRAM_ATTR disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t len = (sizeof(uint16_t) * ((area->y2 - area->y1 + 1)*(area->x2 - area->x1 + 1)));

    lcd_set_index(area->x1, area->y1, area->x2, area->y2);
    lcd_write_data((uint16_t *)color_p, len);
    lv_disp_flush_ready(disp_drv);
}

bool IRAM_ATTR disp_input(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint16_t x = 0, y = 0;
    if (ft5x06_pos_read(&x, &y)) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    data->point.x = x;
    data->point.y = y;
    return false; /*No buffering so no more data read*/
}

static void lv_memory_monitor(lv_task_t * param)
{
    (void) param; /*Unused*/
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d, system free: %d/%d/%d\n", (int)mon.total_size - mon.free_size,
           mon.used_pct,
           mon.frag_pct,
           (int)mon.free_biggest_size,
           heap_caps_get_free_size(MALLOC_CAP_INTERNAL), heap_caps_get_free_size(MALLOC_CAP_SPIRAM), esp_get_free_heap_size());
}

static void gui_tick_task(void * arg)
{
    while(1) {
        lv_tick_inc(10);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void gui_task(void *arg)
{
    lv_init();
    xTaskCreate(gui_tick_task, "gui_tick_task", 1024, NULL, 10, NULL);

    /*Create a display buffer*/
    static lv_disp_buf_t disp_buf;
    static lv_color_t *lv_buf1 = NULL;

    lv_buf1 = (lv_color_t *)heap_caps_malloc(LVGL_BUFFER_SIZE, MALLOC_CAP_SPIRAM);

    lv_disp_buf_init(&disp_buf, lv_buf1, NULL, LVGL_INIT_PIXCNT);

    /*Create a display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = disp_flush;
    lv_disp_drv_register(&disp_drv);

    LV_IMG_DECLARE(mouse_cursor_icon);                          /*Declare the image file.*/
    lv_obj_t * cursor_obj =  lv_img_create(lv_scr_act(), NULL); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);             /*Set the image source*/

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = disp_input;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv);
    lv_indev_set_cursor(mouse_indev, cursor_obj);               /*Connect the image  object to the driver*/

    lv_task_create(lv_memory_monitor, 3000, LV_TASK_PRIO_LOWEST, NULL);

    lv_demo_printer();
    while(1) {
        lv_task_handler();
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void app_main(void)
{
    lcd_init();
    ft5x06_init();
    xTaskCreate(gui_task, "gui_task", 20 * 1024, NULL, 10, NULL);
}
