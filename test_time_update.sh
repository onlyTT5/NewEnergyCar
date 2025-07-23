#!/bin/bash

# 测试时间更新功能的脚本
# 这个脚本会显示当前系统时间，方便与UI显示的时间进行对比

echo "=== 实时时间更新测试 ==="
echo "当前系统时间："

while true; do
    # 显示当前时间（格式：HH:MM:SS）
    echo -ne "\r时间: $(date '+%H:%M:%S') | 日期: $(date '+%B %d, %a, %Y')"
    sleep 1
done
