// 中文字体支持工具 - 一劳永逸的解决方案
// Chinese Font Support Utilities - One-time solution

#include "ui.h"

// 支持多种字体大小的缓存
#define MAX_FONT_SIZES 10
typedef struct
{
    int size;
    lv_font_t *font;
} font_cache_t;

static font_cache_t font_cache[MAX_FONT_SIZES];
static int font_cache_count = 0;

/**
 * 获取指定大小的中文字体
 * @param fontsize 字体大小
 * @return 中文字体指针，失败返回NULL
 */
lv_font_t *get_chinese_font(int fontsize)
{
    // 查找缓存中是否已有该大小的字体
    for (int i = 0; i < font_cache_count; i++)
    {
        if (font_cache[i].size == fontsize)
        {
            return font_cache[i].font;
        }
    }

    // 如果缓存已满，返回默认字体
    if (font_cache_count >= MAX_FONT_SIZES)
    {
        return font_cache[0].font; // 返回第一个缓存的字体
    }

    // 创建新的字体
    lv_font_t *new_font = NULL;

    // 创建中文字体，使用绝对路径
    new_font = lv_freetype_font_create("simhei.ttf",
                                       LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                       fontsize,
                                       LV_FREETYPE_FONT_STYLE_NORMAL);
    if (new_font == NULL)
    {
        // 如果绝对路径失败，尝试相对路径
        new_font = lv_freetype_font_create("simhei.ttf",
                                           LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                           fontsize,
                                           LV_FREETYPE_FONT_STYLE_NORMAL);
    }

    // 如果创建成功，添加到缓存
    if (new_font != NULL)
    {
        font_cache[font_cache_count].size = fontsize;
        font_cache[font_cache_count].font = new_font;
        font_cache_count++;
    }

    return new_font;
}

/**
 * 获取默认大小(24)的中文字体 - 兼容旧版本API
 * @return 中文字体指针，失败返回NULL
 */
lv_font_t *get_chinese_font_default(void)
{
    return get_chinese_font(24);
}

/**
 * 创建一个支持中文的标签
 * @param parent 父对象
 * @param text 要显示的中文文本
 * @param fontsize 字体大小 (默认24)
 * @return 创建的标签对象
 */
lv_obj_t *create_chinese_label_sized(lv_obj_t *parent, const char *text, int fontsize)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);

    // 自动应用中文字体
    lv_font_t *font = get_chinese_font(fontsize);
    if (font != NULL)
    {
        lv_obj_set_style_text_font(label, font, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    return label;
}

/**
 * 创建一个支持中文的标签(默认字体大小)
 * @param parent 父对象
 * @param text 要显示的中文文本
 * @return 创建的标签对象
 */
lv_obj_t *create_chinese_label(lv_obj_t *parent, const char *text)
{
    return create_chinese_label_sized(parent, text, 24);
}

/**
 * 为现有标签应用指定大小的中文字体
 * @param label 要应用字体的标签对象
 * @param fontsize 字体大小
 */
void apply_chinese_font_to_label_sized(lv_obj_t *label, int fontsize)
{
    if (label == NULL)
        return;

    lv_font_t *font = get_chinese_font(fontsize);
    if (font != NULL)
    {
        lv_obj_set_style_text_font(label, font, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

/**
 * 为现有标签应用中文字体(默认大小)
 * @param label 要应用字体的标签对象
 */
void apply_chinese_font_to_label(lv_obj_t *label)
{
    apply_chinese_font_to_label_sized(label, 24);
}

/**
 * 创建带中文字体的标签，并设置基本样式(指定字体大小)
 * @param parent 父对象
 * @param text 要显示的文本
 * @param color 文字颜色
 * @param align 对齐方式
 * @param fontsize 字体大小
 * @return 创建的标签对象
 */
lv_obj_t *create_styled_chinese_label_sized(lv_obj_t *parent, const char *text, lv_color_t color, lv_align_t align, int fontsize)
{
    lv_obj_t *label = create_chinese_label_sized(parent, text, fontsize);
    lv_obj_set_style_text_color(label, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align(label, align);
    return label;
}

/**
 * 创建带中文字体的标签，并设置基本样式(默认字体大小)
 * @param parent 父对象
 * @param text 要显示的文本
 * @param color 文字颜色
 * @param align 对齐方式
 * @return 创建的标签对象
 */
lv_obj_t *create_styled_chinese_label(lv_obj_t *parent, const char *text, lv_color_t color, lv_align_t align)
{
    return create_styled_chinese_label_sized(parent, text, color, align, 24);
}
