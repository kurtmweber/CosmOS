/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _VIDEO_H
#define _VIDEO_H

#include <types.h>

typedef enum video_driver { VIDEO_DRIVER_VGA = 0, VIDEO_DRIVER_LAST } video_driver;

typedef enum video_mode { VIDEO_MODE_TEXT = 1, VIDEO_MODE_MAX } video_mode;

typedef enum video_text_color { VIDEO_TEXT_BLACK = 0, VIDEO_TEXT_BLUE, VIDEO_TEXT_GREEN, VIDEO_TEXT_CYAN, VIDEO_TEXT_RED, VIDEO_TEXT_PURPLE, VIDEO_TEXT_BROWN, VIDEO_TEXT_GRAY, VIDEO_TEXT_DARKGRAY, VIDEO_TEXT_LIGHTBLUE, VIDEO_TEXT_LIGHTGREEN, VIDEO_TEXT_LIGHTCYAN, VIDEO_TEXT_LIGHTRED, VIDEO_TEXT_LIGHTPURPLE, VIDEO_TEXT_YELLOW, VIDEO_TEXT_WHITE, VIDEO_TEXT_COLOR_MAX } video_text_color;

typedef struct video_driver_interface_t {
    void (*cursor_loc)(uint8_t row, uint8_t col);
    void (*init)();
    uint8_t (*query_resolution)(uint16_t *x, uint16_t *y);
    void (*scroll_text)(void);
    uint8_t (*set_mode)(video_mode);
    uint8_t (*write_text)(const char *text, uint8_t start_row, uint8_t start_col, uint8_t attrib, video_text_color fg_color, video_text_color bg_color);
} video_driver_interface_t;

void video_init();
uint8_t video_query_resolution(uint16_t *x, uint16_t *y);
uint8_t video_select_driver(video_driver driver);
uint8_t video_select_mode(video_mode mode);
void video_scroll_text(void);
uint8_t video_write_text(const char *txt, uint8_t start_row, uint8_t start_col, uint8_t attrib, video_text_color fg_color, video_text_color bg_color);

extern video_driver_interface_t video_interfaces[VIDEO_DRIVER_LAST];
extern video_driver video_active_driver;
extern video_mode video_active_mode;

#endif