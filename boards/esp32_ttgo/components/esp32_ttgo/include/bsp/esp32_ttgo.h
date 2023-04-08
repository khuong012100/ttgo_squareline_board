/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

/**
 * @file
 * @brief ESP BSP: TTGO T-Display v1
 */

#pragma once

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "lvgl.h"

/**************************************************************************************************
 *  TTGO T-Display pinout
 **************************************************************************************************/
/* I2C */

/* Display */
#define BSP_LCD_DATA0         (GPIO_NUM_19)
#define BSP_LCD_PCLK          (GPIO_NUM_18)
#define BSP_LCD_CS            (GPIO_NUM_5)
#define BSP_LCD_DC            (GPIO_NUM_16)
#define BSP_LCD_RST           (GPIO_NUM_23)
#define BSP_LCD_BACKLIGHT     (GPIO_NUM_4)

/* USB */

/* Buttons */
typedef enum {
    BSP_BUTTON_LEFT  = GPIO_NUM_0,
    BSP_BUTTON_RIGHT = GPIO_NUM_35,
} bsp_button_t;

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * LCD interface
 *
 * TTGO T-Display is shipped with 2.4inch ST7789 display controller.
 * It features 16-bit colors, 135x240 resolution.
 *
 * LVGL is used as graphics library. LVGL is NOT thread safe, therefore the user must take LVGL mutex
 * by calling bsp_display_lock() before calling and LVGL API (lv_...) and then give the mutex with
 * bsp_display_unlock().
 *
 * Display's backlight must be enabled explicitly by calling bsp_display_backlight_on()
 **************************************************************************************************/
#define BSP_LCD_H_RES              (135)
#define BSP_LCD_V_RES              (240)
#define BSP_LCD_PIXEL_CLOCK_HZ     (40 * 1000 * 1000)
#define BSP_LCD_SPI_NUM            (SPI3_HOST)

#define BSP_LCD_DRAW_BUFF_SIZE     (BSP_LCD_H_RES * 10)
#define BSP_LCD_DRAW_BUFF_DOUBLE   (0)

/**
 * @brief Initialize display
 *
 * This function initializes SPI, display controller and starts LVGL handling task.
 * LCD backlight must be enabled separately by calling bsp_display_brightness_set()
 *
 * @return Pointer to LVGL display or NULL when error occured
 */
lv_disp_t *bsp_display_start(void);

/**
 * @brief Get pointer to input device (touch, buttons, ...)
 *
 * @note The LVGL input device is initialized in bsp_display_start() function.
 *
 * @return Pointer to LVGL input device or NULL when not initialized
 */
lv_indev_t *bsp_display_get_input_dev(void);

/**
 * @brief Take LVGL mutex
 *
 * @param timeout_ms Timeout in [ms]. 0 will block indefinitely.
 * @return true  Mutex was taken
 * @return false Mutex was NOT taken
 */
bool bsp_display_lock(uint32_t timeout_ms);

/**
 * @brief Give LVGL mutex
 *
 */
void bsp_display_unlock(void);

/**
 * @brief Set display's brightness
 *
 * Brightness is controlled with PWM signal to a pin controling backlight.
 *
 * @param[in] brightness_percent Brightness in [%]
 * @return
 *      - ESP_OK                On success
 *      - ESP_ERR_INVALID_ARG   Parameter error
 */
esp_err_t bsp_display_brightness_set(int brightness_percent);

/**
 * @brief Turn on display backlight
 *
 * Display must be already initialized by calling bsp_display_start()
 *
 * @return
 *      - ESP_OK                On success
 *      - ESP_ERR_INVALID_ARG   Parameter error
 */
esp_err_t bsp_display_backlight_on(void);

/**
 * @brief Turn off display backlight
 *
 * Display must be already initialized by calling bsp_display_start()
 *
 * @return
 *      - ESP_OK                On success
 *      - ESP_ERR_INVALID_ARG   Parameter error
 */
esp_err_t bsp_display_backlight_off(void);

/**
 * @brief Rotate screen
 *
 * Display must be already initialized by calling bsp_display_start()
 *
 * @param[in] disp Pointer to LVGL display
 * @param[in] rotation Angle of the display rotation
 */
void bsp_display_rotate(lv_disp_t *disp, lv_disp_rot_t rotation);
/**************************************************************************************************
 *
 * Button
 *
 * There are three buttons on ESP-BOX:
 *  - Reset:  Not programable
 *  - Config: Controls boot mode during reset. Can be programmed after application starts
 *  - Mute:   This button is wired to Logic Gates and its result is mapped to GPIO_NUM_1
 **************************************************************************************************/

/**
 * @brief Set button's GPIO as input
 *
 * @param[in] btn Button to be initialized
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t bsp_button_init(const bsp_button_t btn);

/**
 * @brief Get button's state
 *
 * Note: For LCD panel button which is defined as BSP_BUTTON_MAIN, bsp_display_start should
 *       be called before call this function.
 *
 * @param[in] btn Button to read
 * @return true  Button pressed
 * @return false Button released
 */
bool bsp_button_get(const bsp_button_t btn);

#ifdef __cplusplus
}
#endif
