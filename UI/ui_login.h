#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include "lvgl.h"
#include "user_management.h"
#include "ui.h" // 包含UI对象声明

// 登录界面函数
void show_login_screen(void);
void handle_login_attempt(void);
void handle_register_attempt(void);
void clear_login_inputs(void);

// 回调函数
void login_btn_callback(lv_event_t *e);
void register_btn_callback(lv_event_t *e);

#endif // UI_LOGIN_H
