#pragma once

#include <stdio.h>
#include <stdint.h>
#include "driver/i2s.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t bit_width;
    uint8_t pin_clk;
    uint8_t *pin_num;
    uint8_t ws_clk_div;
} i2s_parallel_config_t;


void i2s_parallel_write_data(uint8_t *data, uint32_t len);
esp_err_t i2s_parallel_init(i2s_parallel_config_t *config);

#ifdef __cplusplus
}
#endif

