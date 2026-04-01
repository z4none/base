# base

自用的 C++ 基础库，主要是一些平时项目里会反复用到的小工具和 Win32 相关封装。

当前仓库更偏向 Windows / Visual Studio 环境，适合直接作为静态库引入自己的工程。

## 包含内容

- 字符串工具：大小写转换、裁剪、替换、查找、分割、拼接、格式化
- 时间工具：毫秒/微秒时间戳、时间格式化、简单耗时统计
- 路径与文件系统：基于 `ghc::filesystem`
- 编码转换：`UTF-8 / UTF-16 / UTF-32 / GBK`
- 任务队列：简单延迟执行、定时任务
- 事件总线：轻量消息派发
- `defer`：作用域退出时自动执行
- 随机工具：随机整数、随机字符串、十六进制串
- Win32 工具：进程、线程命名、文件对话框、资源管理器定位等

## 目录结构

`base/`
核心代码

`test/`
简单测试工程

`plog/`
附带的日志库头文件

## 依赖

- Visual Studio
- Windows SDK
- 仓库内自带的 `ghc::filesystem`
- 仓库内自带的 `iconv`
- `plog`

## 使用方式

这个仓库当前是一个 Visual Studio 工程，直接打开 [base.sln](/d:/Workspace/base/base.sln) 即可。

如果作为静态库使用，通常做法是：

1. 编译 `base` 生成 `base.lib`
2. 在自己的项目里添加头文件目录
3. 链接 `base.lib`

示例：

```cpp
#include <base/task.h>
#include <base/datetime.h>

using namespace std::chrono_literals;

int main()
{
    base::TaskQueue queue;

    queue.Queue([]() {
        printf("run now\n");
    });

    queue.SetTimeout(1000ms, []() {
        printf("run after 1s\n");
    });

    while (true)
    {
        queue.Run();
        Sleep(100);
    }
}
```

## 备注

这是自用基础库，接口设计以顺手和够用为主，不追求完整抽象和跨平台一致性。
