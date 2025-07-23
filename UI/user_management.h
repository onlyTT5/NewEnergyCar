#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PHONE_LEN 20
#define MAX_PASSWORD_LEN 50
#define MAX_USERS 100
#define USER_DATA_FILE "users.txt"

// 用户信息结构体
typedef struct
{
    int id;
    char phone[MAX_PHONE_LEN];
    char password[MAX_PASSWORD_LEN];
    int login_count;
    float balance;
    float consumption;
    bool is_admin;
} user_info_t;

// 用户管理器结构体
typedef struct
{
    user_info_t users[MAX_USERS];
    int user_count;
    user_info_t *current_user;
} user_manager_t;

// 全局用户管理器
extern user_manager_t g_user_manager;

// 函数声明
void user_manager_init(void);
bool register_user(const char *phone, const char *password);
bool login_user(const char *phone, const char *password);
void logout_user(void);
bool save_users_to_file(void);
bool load_users_from_file(void);
user_info_t *get_current_user(void);
user_info_t *get_user_by_index(int index);
int get_user_count(void);
bool is_phone_exists(const char *phone);
void init_admin_user(void);

#endif // USER_MANAGEMENT_H
