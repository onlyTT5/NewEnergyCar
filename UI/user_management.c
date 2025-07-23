#include "user_management.h"

// 全局用户管理器实例
user_manager_t g_user_manager = {0};

// 初始化用户管理器
void user_manager_init(void)
{
    memset(&g_user_manager, 0, sizeof(user_manager_t));
    g_user_manager.user_count = 0;
    g_user_manager.current_user = NULL;

    // 从文件加载用户数据
    load_users_from_file();

    // 初始化管理员用户
    init_admin_user();

    printf("User management system initialized\n");
}

// 初始化管理员用户
void init_admin_user(void)
{
    // 检查是否已存在管理员用户
    if (!is_phone_exists("admin"))
    {
        user_info_t admin = {0};
        admin.id = 1;
        strcpy(admin.phone, "admin");
        strcpy(admin.password, "admin123");
        admin.login_count = 0;
        admin.balance = 10000.0f;
        admin.consumption = 0.0f;
        admin.is_admin = true;

        g_user_manager.users[g_user_manager.user_count] = admin;
        g_user_manager.user_count++;

        save_users_to_file();
        printf("Admin user initialized: phone=admin, password=admin123\n");
    }
}

// 用户注册
bool register_user(const char *phone, const char *password)
{
    if (!phone || !password || strlen(phone) == 0 || strlen(password) == 0)
    {
        printf("Invalid phone or password\n");
        return false;
    }

    if (g_user_manager.user_count >= MAX_USERS)
    {
        printf("Maximum user limit reached\n");
        return false;
    }

    if (is_phone_exists(phone))
    {
        printf("Phone number already exists\n");
        return false;
    }

    // 创建新用户
    user_info_t new_user = {0};
    new_user.id = g_user_manager.user_count + 1;
    strncpy(new_user.phone, phone, MAX_PHONE_LEN - 1);
    strncpy(new_user.password, password, MAX_PASSWORD_LEN - 1);
    new_user.login_count = 0;
    new_user.balance = 1000.0f; // 新用户默认余额
    new_user.consumption = 0.0f;
    new_user.is_admin = false;

    g_user_manager.users[g_user_manager.user_count] = new_user;
    g_user_manager.user_count++;

    // 保存到文件
    save_users_to_file();

    printf("User registered successfully: %s\n", phone);
    return true;
}

// 用户登录
bool login_user(const char *phone, const char *password)
{
    if (!phone || !password)
    {
        return false;
    }

    for (int i = 0; i < g_user_manager.user_count; i++)
    {
        user_info_t *user = &g_user_manager.users[i];
        if (strcmp(user->phone, phone) == 0 && strcmp(user->password, password) == 0)
        {
            g_user_manager.current_user = user;
            user->login_count++;
            save_users_to_file();
            printf("User logged in: %s (login count: %d)\n", phone, user->login_count);
            return true;
        }
    }

    printf("Invalid phone or password\n");
    return false;
}

// 用户登出
void logout_user(void)
{
    if (g_user_manager.current_user)
    {
        printf("User logged out: %s\n", g_user_manager.current_user->phone);
        g_user_manager.current_user = NULL;
    }
}

// 检查手机号是否已存在
bool is_phone_exists(const char *phone)
{
    if (!phone)
        return false;

    for (int i = 0; i < g_user_manager.user_count; i++)
    {
        if (strcmp(g_user_manager.users[i].phone, phone) == 0)
        {
            return true;
        }
    }
    return false;
}

// 保存用户数据到文件
bool save_users_to_file(void)
{
    FILE *file = fopen(USER_DATA_FILE, "w");
    if (!file)
    {
        printf("Failed to open file for writing: %s\n", USER_DATA_FILE);
        return false;
    }

    // 写入用户数量
    fprintf(file, "%d\n", g_user_manager.user_count);

    // 写入每个用户的信息
    for (int i = 0; i < g_user_manager.user_count; i++)
    {
        user_info_t *user = &g_user_manager.users[i];
        fprintf(file, "%d %s %s %d %.2f %.2f %d\n",
                user->id, user->phone, user->password,
                user->login_count, user->balance, user->consumption,
                user->is_admin ? 1 : 0);
    }

    fclose(file);
    printf("User data saved to %s\n", USER_DATA_FILE);
    return true;
}

// 从文件加载用户数据
bool load_users_from_file(void)
{
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (!file)
    {
        printf("No existing user data file found, starting fresh\n");
        return false;
    }

    int user_count;
    if (fscanf(file, "%d", &user_count) != 1 || user_count < 0 || user_count > MAX_USERS)
    {
        printf("Invalid user count in file\n");
        fclose(file);
        return false;
    }

    g_user_manager.user_count = 0;

    for (int i = 0; i < user_count; i++)
    {
        user_info_t user = {0};
        int is_admin;

        if (fscanf(file, "%d %s %s %d %f %f %d",
                   &user.id, user.phone, user.password,
                   &user.login_count, &user.balance, &user.consumption,
                   &is_admin) == 7)
        {
            user.is_admin = (is_admin != 0);
            g_user_manager.users[g_user_manager.user_count] = user;
            g_user_manager.user_count++;
        }
        else
        {
            printf("Error reading user data at line %d\n", i + 2);
            break;
        }
    }

    fclose(file);
    printf("Loaded %d users from %s\n", g_user_manager.user_count, USER_DATA_FILE);
    return true;
}

// 获取当前用户
user_info_t *get_current_user(void)
{
    return g_user_manager.current_user;
}

// 根据索引获取用户
user_info_t *get_user_by_index(int index)
{
    if (index >= 0 && index < g_user_manager.user_count)
    {
        return &g_user_manager.users[index];
    }
    return NULL;
}

// 获取用户总数
int get_user_count(void)
{
    return g_user_manager.user_count;
}

// 用户充值功能
bool recharge_user_balance(float amount)
{
    if (g_user_manager.current_user == NULL)
    {
        printf("充值失败：没有当前登录用户\n");
        return false;
    }

    if (amount <= 0)
    {
        printf("充值失败：充值金额必须大于0\n");
        return false;
    }

    // 增加用户余额
    g_user_manager.current_user->balance += amount;

    printf("充值成功：用户 %s 充值 $%.2f，当前余额 $%.2f\n",
           g_user_manager.current_user->phone, amount,
           g_user_manager.current_user->balance);

    // 保存到文件
    if (save_users_to_file())
    {
        printf("用户数据已保存到文件\n");
        return true;
    }
    else
    {
        printf("警告：充值成功但保存文件失败\n");
        return false;
    }
}

// 用户扣费功能
bool deduct_user_balance(float amount)
{
    if (g_user_manager.current_user == NULL)
    {
        printf("扣费失败：没有当前登录用户\n");
        return false;
    }

    if (amount <= 0)
    {
        printf("扣费失败：扣费金额必须大于0\n");
        return false;
    }

    if (g_user_manager.current_user->balance < amount)
    {
        printf("扣费失败：余额不足，当前余额 $%.2f，需要扣费 $%.2f\n",
               g_user_manager.current_user->balance, amount);
        return false;
    }

    // 扣除用户余额
    g_user_manager.current_user->balance -= amount;

    printf("扣费成功：用户 %s 扣费 $%.2f，当前余额 $%.2f\n",
           g_user_manager.current_user->phone, amount,
           g_user_manager.current_user->balance);

    // 保存到文件
    if (save_users_to_file())
    {
        printf("用户数据已保存到文件\n");
        return true;
    }
    else
    {
        printf("警告：扣费成功但保存文件失败\n");
        return false;
    }
}
