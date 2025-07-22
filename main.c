#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// #include "./UI/ui.h"

static const char *getenv_default(const char *name, const char *dflt)
{
    return getenv(name) ?: dflt;
}

#if LV_USE_LINUX_FBDEV
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t *disp = lv_linux_fbdev_create();

    lv_linux_fbdev_set_file(disp, device);
}
#elif LV_USE_SDL
static void lv_linux_disp_init(void)
{
    const int width = atoi(getenv("LV_SDL_VIDEO_WIDTH") ?: "800");
    const int height = atoi(getenv("LV_SDL_VIDEO_HEIGHT") ?: "480");

    lv_sdl_window_create(width, height);
}
#else
#error Unsupported configuration
#endif

void my_chinese_font(char *text)
{
    // 1.创建中文字体
    lv_font_t *font = lv_freetype_font_create("/study/simhei.ttf", // 👍加载字库文件
                                              LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                              24,
                                              LV_FREETYPE_FONT_STYLE_NORMAL);

    // 2.创建中文字体样式
    static lv_style_t style;
    lv_style_init(&style);                                 // 初始化样式
    lv_style_set_text_font(&style, font);                  // 设置中文字体样式
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER); // 字体居中显示

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_obj_add_style(label, &style, 0); // 👍给标签添加中文字体
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

int main(void)
{
    lv_init();
    /*Linux display device init*/
    lv_linux_disp_init();

#if LV_USE_SDL
    // init input device
    lv_sdl_mouse_create();
    lv_sdl_keyboard_create();
    lv_sdl_mousewheel_create();
#endif

#if LV_USE_LINUX_FBDEV
    // 创建输入设备
    lv_indev_t *touch = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");
    // 校准输入设备屏幕坐标
    // lv_evdev_set_calibration(touch, 0, 0, 1024, 600); // 黑色边框的屏幕
    lv_evdev_set_calibration(touch, 0, 0, 800, 480); // 蓝色边框的屏幕
#endif
    ui_init();

    // lv_example_event_1(); // 测试按钮例子
    // lv_example_freetype_1();

    /*Handle LVGL tasks*/
    // my_chinese_font("你好999大王666\n");
    while (1)
    {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}