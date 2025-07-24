# Screen4 快充功能实现说明

## 功能概述
在Screen4（充电界面）中实现了一个智能快充系统，用户可以通过点击Button1按钮来切换充电模式。

## 功能特性

### 1. 双模式充电
- **慢充模式（默认）**：
  - 功率：60kW
  - 价格：$1.5/kWh
  - 充电速度：每秒1%电量增长
  - 标签显示：DC slow charging

- **快充模式**：
  - 功率：120kW
  - 价格：$1.8/kWh
  - 充电速度：每秒2%电量增长（双倍速度）
  - 标签显示：DC fast charging

### 2. 模式切换操作
- 点击Button1按钮可以在慢充和快充模式之间切换
- 切换时会实时更新所有相关UI显示

### 3. UI元素更新
切换快充模式时，以下UI元素会自动更新：

- **ui_Power**：显示当前功率（60kW ↔ 120kW）
- **ui_Price**：显示当前价格（1.5 ↔ 1.8）
- **ui_Label25**：显示充电模式（DC slow charging ↔ DC fast charging）
- **ui_Label29**：显示当前功率值（60 ↔ 120）

### 4. 充电过程差异
在充电进行时，快充模式具有以下特点：

- **充电速度**：每秒电量增长2%（慢充为1%）
- **费用扣除**：每秒扣费$1.8（慢充为$1.5）
- **Label37进度显示**：加倍速度更新充电进度百分比
- **进度条ui_CurrentCharging**：加倍速度填充
- **余额Label33**：按快充价格更快扣费

### 5. 实时反馈
- 控制台会显示当前充电模式（快充/慢充）
- 充电过程中实时显示充电进度、时间、电量、费用和模式信息

## 技术实现

### 代码修改位置
1. **UI/screens/ui_Screen4.c**：
   - 添加快充状态变量
   - 实现Button1事件处理函数
   - 修改充电定时器回调函数
   - 更新StopChargeBtn事件处理

2. **UI/screens/ui_Screen4.h**：
   - 添加Button1、StopChargeBtn、Button2事件函数声明

### 核心变量
```c
static bool is_fast_charging = false;  // 快充模式标记
static float normal_price = 1.5f;      // 正常充电价格
static float fast_price = 1.8f;        // 快充价格
static int normal_power = 60;          // 正常功率
static int fast_power = 120;           // 快充功率
```

### 关键函数
- `ui_event_Button1()`: 快充模式切换事件处理
- `charging_timer_cb()`: 充电定时器回调（支持双速充电）
- `ui_event_StopChargeBtn()`: 开始/停止充电按钮事件

## 使用方法

1. 进入Screen4充电界面
2. 点击Button1按钮切换充电模式
3. 观察Power、Price、Label25、Label29的数值变化
4. 点击Start Charging开始充电
5. 在充电过程中观察快充模式下的双倍速度效果

## 安全特性

- 余额检查：根据当前充电模式检查用户余额是否足够
- 自动停充：余额不足时自动停止充电
- 模式锁定：充电过程中模式切换不会影响当前充电会话

## 注意事项

- 快充模式下费用更高，请确保账户余额充足
- 充电速度和费用都会相应调整
- 所有UI显示都会实时同步更新
