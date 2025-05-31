#pragma once

#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_HOR_RES_MAX          (480)
#define LV_VER_RES_MAX          (320)

#define LV_USE_INDEV            1
#define LV_USE_BTN              1
#define LV_USE_LABEL            1
#define LV_USE_USER_DATA        1  // ★この行を追加

#define LV_USE_FONT_CUSTOM      1  // ★これを追加

#define LV_FONT_MONTSERRAT_32   1
#define LV_FONT_MONTSERRAT_28   1

#define LV_FONT_FMT_TXT_LARGE 1

// カスタムフォントの extern 宣言（★複数定義も可能）
#define LV_FONT_CUSTOM_DECLARE         \
    extern const lv_font_t NotoSansBold64; \
    extern const lv_font_t lv_font_montserrat_32; \
    extern const lv_font_t lv_font_montserrat_28;

//#define LV_FONT_COMPRESSED 1  // ← これが必要

#define LV_USE_PERF_MONITOR     1
#define LV_USE_LOG              1
#define LV_LOG_LEVEL            LV_LOG_LEVEL_WARN

#define LV_COLOR_DEPTH          16
#define LV_MEM_SIZE             (32U * 1024U)  // サイズを32KBに調整

#endif // LV_CONF_H
