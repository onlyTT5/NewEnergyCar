#include "ui_login.h"
#include "ui.h"
#include <stdio.h>

// 显示登录界面
void show_login_screen(void)
{
    // 清空输入框
    clear_login_inputs();
    printf("Login screen displayed\n");
}

// 处理登录尝试
void handle_login_attempt(void)
{
    // 获取输入的手机号和密码
    const char *phone = lv_textarea_get_text(ui_PhoneTx1);
    const char *password = lv_textarea_get_text(ui_PasswordTx);

    if (phone && password && strlen(phone) > 0 && strlen(password) > 0)
    {
        if (login_user(phone, password))
        {
            // 登录成功，更新用户管理显示
            update_user_management_display();
            printf("Login successful, redirecting to main screen\n");
            // 清空登录输入框
            lv_textarea_set_text(ui_PhoneTx1, "");
            lv_textarea_set_text(ui_PasswordTx, "");
            // 跳转到主界面 Screen4
            _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
        }
        else
        {
            printf("Login failed: invalid credentials\n");
            // 这里可以添加显示错误消息的代码
        }
    }
    else
    {
        printf("Login failed: empty phone or password\n");
    }
}

// 处理注册尝试
void handle_register_attempt(void)
{
    // 获取注册输入的信息
    const char *phone = lv_textarea_get_text(ui_PhoneTx2);
    const char *password = lv_textarea_get_text(ui_PasswordTx1);
    const char *confirm_password = lv_textarea_get_text(ui_ConfirmTx);

    // 添加调试信息
    printf("=== Registration Debug Info ===\n");
    printf("ui_PhoneTx2 pointer: %p\n", (void *)ui_PhoneTx2);
    printf("ui_PasswordTx1 pointer: %p\n", (void *)ui_PasswordTx1);
    printf("ui_ConfirmTx pointer: %p\n", (void *)ui_ConfirmTx);

    // 检查注册界面是否可见
    if (ui_RegisterOpera)
    {
        bool is_hidden = lv_obj_has_flag(ui_RegisterOpera, LV_OBJ_FLAG_HIDDEN);
        printf("Register Opera is hidden: %s\n", is_hidden ? "YES" : "NO");
    }

    if (ui_PhoneTx2)
    {
        printf("Phone input text: '%s' (length: %zu)\n", phone ? phone : "NULL", phone ? strlen(phone) : 0);
    }
    else
    {
        printf("ui_PhoneTx2 is NULL!\n");
    }

    if (ui_PasswordTx1)
    {
        printf("Password input text: '%s' (length: %zu)\n", password ? password : "NULL", password ? strlen(password) : 0);
    }
    else
    {
        printf("ui_PasswordTx1 is NULL!\n");
    }

    if (ui_ConfirmTx)
    {
        printf("Confirm password text: '%s' (length: %zu)\n", confirm_password ? confirm_password : "NULL", confirm_password ? strlen(confirm_password) : 0);
    }
    else
    {
        printf("ui_ConfirmTx is NULL!\n");
    }
    printf("==============================\n");

    if (!phone || !password || !confirm_password ||
        strlen(phone) == 0 || strlen(password) == 0 || strlen(confirm_password) == 0)
    {
        printf("Registration failed: empty fields\n");
        return;
    }

    if (strcmp(password, confirm_password) != 0)
    {
        printf("Registration failed: passwords do not match\n");
        return;
    }

    if (register_user(phone, password))
    {
        printf("Registration successful for phone: %s\n", phone);
        // 清空注册表单
        lv_textarea_set_text(ui_PhoneTx2, "");
        lv_textarea_set_text(ui_PasswordTx1, "");
        lv_textarea_set_text(ui_ConfirmTx, "");
        // 更新用户管理显示
        update_user_management_display();

        // 注册成功后切换到登录界面
        _ui_flag_modify(ui_RegisterOpera, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_LoginOpera, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        printf("Switched to login interface after successful registration\n");
    }
    else
    {
        printf("Registration failed\n");
    }
}

// 清空登录输入框
void clear_login_inputs(void)
{
    if (ui_PhoneTx1)
    {
        lv_textarea_set_text(ui_PhoneTx1, "");
    }
    if (ui_PasswordTx)
    {
        lv_textarea_set_text(ui_PasswordTx, "");
    }
    if (ui_PhoneTx2)
    {
        lv_textarea_set_text(ui_PhoneTx2, "");
    }
    if (ui_PasswordTx1)
    {
        lv_textarea_set_text(ui_PasswordTx1, "");
    }
    if (ui_ConfirmTx)
    {
        lv_textarea_set_text(ui_ConfirmTx, "");
    }
}

// 登录按钮回调
void login_btn_callback(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        handle_login_attempt();
    }
}

// 注册按钮回调
void register_btn_callback(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        handle_register_attempt();
    }
}
