# LVGL 中文字体使用指南

## 概述

本项目提供了一套完整的中文字体解决方案，支持动态字体大小调整和智能内存管理。所有相关代码位于 `UI/ui_chinese_font.c` 文件中。

## 核心特性

- ✅ **一劳永逸的解决方案**：无需为每个界面重复配置中文字体
- ✅ **动态字体大小**：支持 1-100 像素的任意字体大小
- ✅ **智能缓存机制**：自动缓存常用字体大小，避免内存浪费
- ✅ **多种使用方式**：支持创建新标签或为现有标签应用字体
- ✅ **内存管理**：最多缓存 10 种不同大小的字体，自动内存管理

## API 函数说明

### 1. 核心字体获取函数

```c
lv_font_t *get_chinese_font(int fontsize);
```

- **功能**：获取指定大小的中文字体（使用 simhei.ttf）
- **参数**：fontsize - 字体大小（1-100像素）
- **返回值**：字体对象指针，如果创建失败返回 NULL
- **特点**：使用缓存机制，相同大小的字体只创建一次

### 2. 标签创建函数

#### 创建默认大小(24px)的中文标签
```c
lv_obj_t *create_chinese_label(lv_obj_t *parent, const char *text);
```

#### 创建指定大小的中文标签
```c
lv_obj_t *create_chinese_label_sized(lv_obj_t *parent, const char *text, int fontsize);
```

**示例**：
```c
// 创建默认 24px 的中文标签
lv_obj_t *label1 = create_chinese_label(parent, "新能源共享充电桩系统");

// 创建 32px 的中文标签
lv_obj_t *label2 = create_chinese_label_sized(parent, "智能充电", 32);
```

### 3. 字体应用函数

#### 为现有标签应用默认大小(24px)中文字体
```c
void apply_chinese_font_to_label(lv_obj_t *label);
```

#### 为现有标签应用指定大小中文字体
```c
void apply_chinese_font_to_label_sized(lv_obj_t *label, int fontsize);
```

**示例**：
```c
// 先创建标签
lv_obj_t *label = lv_label_create(parent);
lv_label_set_text(label, "绿色出行");

// 再应用中文字体
apply_chinese_font_to_label_sized(label, 28);
```

### 4. 样式化标签创建函数

#### 创建带样式的默认大小中文标签
```c
lv_obj_t *create_styled_chinese_label(lv_obj_t *parent, const char *text, 
                                     lv_color_t color, lv_align_t align);
```

#### 创建带样式的指定大小中文标签
```c
lv_obj_t *create_styled_chinese_label_sized(lv_obj_t *parent, const char *text, 
                                           lv_color_t color, lv_align_t align, int fontsize);
```

**示例**：
```c
// 创建红色、居中对齐、32px 的中文标签
lv_obj_t *label = create_styled_chinese_label_sized(parent, 
                                                   "便民服务", 
                                                   lv_color_hex(0xFF0000), 
                                                   LV_ALIGN_CENTER, 
                                                   32);
```

## 使用示例

### 基础用法
```c
void ui_Screen1_screen_init(void)
{
    // 方式1：直接创建中文标签
    lv_obj_t *title = create_chinese_label_sized(ui_Screen1, "新能源共享充电桩系统", 28);
    lv_obj_set_align(title, LV_ALIGN_TOP_MID);
    
    // 方式2：先创建标签，再应用字体
    lv_obj_t *subtitle = lv_label_create(ui_Screen1);
    lv_label_set_text(subtitle, "智能充电 绿色出行 便民服务");
    apply_chinese_font_to_label_sized(subtitle, 20);
    lv_obj_set_align(subtitle, LV_ALIGN_CENTER);
    
    // 方式3：创建带样式的中文标签
    lv_obj_t *status = create_styled_chinese_label_sized(ui_Screen1, 
                                                        "设备状态：正常", 
                                                        lv_color_hex(0x00FF00), 
                                                        LV_ALIGN_BOTTOM_MID, 
                                                        16);
}
```

### 常用字体大小建议
- **标题文字**：28-36px
- **正文内容**：20-24px
- **状态信息**：16-18px
- **小标签**：12-14px

## 技术实现细节

### 字体缓存机制
系统维护一个最多包含 10 种不同字体大小的缓存数组：

```c
typedef struct {
    lv_font_t *font;
    int size;
    bool is_used;
} font_cache_t;

static font_cache_t font_cache[MAX_FONT_SIZES] = {0};
```

### 内存管理策略
1. **首次请求**：创建新字体并加入缓存
2. **重复请求**：直接返回缓存中的字体
3. **缓存满时**：替换最少使用的字体（简单 LRU 策略）
4. **程序退出**：自动释放所有缓存的字体

### 字体文件路径
当前使用的中文字体文件：`simhei.ttf`（黑体）
如需更换字体，请修改 `ui_chinese_font.c` 中的 `CHINESE_FONT_PATH` 宏定义。

## 故障排除

### 1. 中文显示为方框或乱码
- 检查 `simhei.ttf` 文件是否存在
- 确认 FreeType 库正确链接
- 验证 `lv_conf.h` 中 `LV_USE_FREETYPE` 设置为 1

### 2. 编译错误
- 确保 `ui_chinese_font.c` 已添加到 `CMakeLists.txt`
- 检查头文件 `ui.h` 中的函数声明是否与实现匹配

### 3. 内存不足
- 减少使用的字体大小种类
- 考虑调整 `MAX_FONT_SIZES` 的值（当前为 10）

## 项目构建

### Makefile 构建
```bash
make clean && make
```

### CMake 构建
```bash
mkdir build && cd build
cmake ..
make
```

## 版本信息

- **LVGL 版本**：9.2.2
- **FreeType 版本**：2.x
- **最后更新**：2024年
- **维护状态**：活跃维护

## 联系与支持

如需技术支持或发现问题，请检查项目文档或提交问题报告。

---

**注意**：本解决方案已在 GEC6818 硬件平台上测试通过，理论上兼容所有支持 LVGL 和 FreeType 的平台。
