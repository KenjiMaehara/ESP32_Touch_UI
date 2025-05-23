#pragma once

#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_HOR_RES_MAX          (480)
#define LV_VER_RES_MAX          (320)

#define LV_USE_INDEV            1
#define LV_USE_BTN              1
#define LV_USE_LABEL            1

#define LV_USE_PERF_MONITOR     1
#define LV_USE_LOG              1
#define LV_LOG_LEVEL            LV_LOG_LEVEL_WARN

#define LV_COLOR_DEPTH          16
#define LV_MEM_SIZE             (32U * 1024U)  // サイズを32KBに調整

#endif // LV_CONF_H
