/*
 * drivers/net/wireless/esp_prealloc/esp_log.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _ESP_LOG_H_
#define _ESP_LOG_H_

#ifdef ESP_DEBUG

#define loge(fmt, args...) do { \
            printk(fmt, ##args); \
        } while (0)

#define logi(fmt, args...) do { \
            printk(fmt, ##args); \
        } while (0)

#define logd(fmt, args...) do { \
            printk(fmt, ##args); \
        } while (0)
#else

#define loge(fmt, args...) do { \
            printk(fmt, ##args); \
        } while (0)

#define logi(fmt, args...) do { \
            printk(fmt, ##args); \
        } while (0)

#define logd(fmt, args...) do { \
        } while (0)

#endif /* ESP_DEBUG */
   
#endif /* _ESP_LOG_H_*/
