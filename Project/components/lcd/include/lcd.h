#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lcd_init(void);
void lcd_set_index(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void IRAM_ATTR lcd_write_data(uint16_t *data, size_t len);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif

