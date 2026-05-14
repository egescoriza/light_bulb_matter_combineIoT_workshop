/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "app_task.h"

#include <zephyr/logging/log.h>

// EXTRA
#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <string.h>
// Settings
static const int32_t sleep_time_ms = 50;        // Target 20 FPS
static const struct gpio_dt_spec ret_led = GPIO_DT_SPEC_GET(DT_ALIAS(my_ret_led), gpios);
static const struct gpio_dt_spec vcc = GPIO_DT_SPEC_GET(DT_ALIAS(my_vcc), gpios);

LOG_MODULE_REGISTER(app, CONFIG_CHIP_APP_LOG_LEVEL);

int main()
{
	// CHIP_ERROR err = AppTask::Instance().StartApp();

	// LOG_ERR("Exited with code %" CHIP_ERROR_FORMAT, err.Format());
	// return err == CHIP_NO_ERROR ? EXIT_SUCCESS : EXIT_FAILURE;

	// uint32_t count = 0;
    // char buf[11] = {0};
    const struct device *display;
    // lv_obj_t *hello_label;
    // lv_obj_t *counter_label;
    // lv_obj_t *rect;
    // lv_obj_t *circle;
    // lv_style_t rect_style;
    // lv_style_t circle_style;
    // lv_point_precise_t rect_points[5] = { {0, 0}, {120, 0}, {120, 20}, {0, 20}, {0, 0} };
    // const uint32_t circle_radius = 15;

	// Make sure that the GPIO was initialized
	if (!gpio_is_ready_dt(&ret_led)) {
		LOG_DBG("ret led not ready");
		return 0;
	}
	if (!gpio_is_ready_dt(&vcc)) {
		LOG_DBG("vcc not ready");
		return 0;
	}

	// Set the GPIO as output
	int ret = gpio_pin_configure_dt(&ret_led, GPIO_OUTPUT);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&vcc, GPIO_OUTPUT);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_set_dt(&ret_led, 1);
	if (ret) {
		LOG_ERR("Enable backlight failed! (%d)", ret);
		return ret;
	}
	ret = gpio_pin_set_dt(&vcc, 1);
	if (ret) {
		LOG_ERR("Enable vcc failed! (%d)", ret);
		return ret;
	}

    // Initialize the display
    display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display)) {
        printk("Error: display not ready\r\n");
        return 0;
    }

    // // Create a static label widget
    // hello_label = lv_label_create(lv_scr_act());
    // lv_label_set_text(hello_label, "Hello, World!");
    // lv_obj_align(hello_label, LV_ALIGN_TOP_MID, 0, 5);

    // // Create a dynamic label widget
    // counter_label = lv_label_create(lv_scr_act());
    // lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, -2);

    // // Set line style
    // lv_style_init(&rect_style);
    // lv_style_set_line_color(&rect_style, lv_color_hex(0x0000FF));
    // lv_style_set_line_width(&rect_style, 3);

    // // Create a rectangle out of lines
    // rect = lv_line_create(lv_scr_act());
    // lv_obj_add_style(rect, &rect_style, 0);
    // lv_line_set_points(rect,
    //                    rect_points,
    //                    sizeof(rect_points) / sizeof(rect_points[0]));
    // lv_obj_align(rect, LV_ALIGN_TOP_MID, 0, 0);

    // // Set circle style
    // lv_style_init(&circle_style);
    // lv_style_set_radius(&circle_style, circle_radius);
    // lv_style_set_bg_opa(&circle_style, LV_OPA_100);
    // lv_style_set_bg_color(&circle_style, lv_color_hex(0xFF0000));

    // // Create an object with the new style
    // circle = lv_obj_create(lv_scr_act());
    // lv_obj_set_size(circle, circle_radius * 2, circle_radius * 2);
    // lv_obj_add_style(circle, &circle_style, 0);
    // lv_obj_align(circle, LV_ALIGN_CENTER, 0, 5);

    // Disable display blanking
    display_blanking_off(display);

	lv_obj_set_style_bg_color(lv_screen_active(),
                              lv_color_white(), 0);

    lv_obj_t * qr = lv_qrcode_create(lv_screen_active());

    lv_qrcode_set_size(qr, 110);

    lv_qrcode_set_dark_color(qr, lv_color_black());
    lv_qrcode_set_light_color(qr, lv_color_white());

    const char * data = "Quieres que te mantenga?";

    lv_qrcode_update(qr, data, strlen(data));

    lv_obj_center(qr);

	// Do forever
    while (1) {

        // // Update counter label every second
        // count++;
        // if ((count % (1000 / sleep_time_ms)) == 0) {
		// LOG_DBG("TEST");
        //     sprintf(buf, "%d", count / (1000 / sleep_time_ms));
        //     lv_label_set_text(counter_label, buf);
        // }

        // Must be called periodically
        lv_task_handler();

        // Sleep
        k_msleep(sleep_time_ms);
    }
}
