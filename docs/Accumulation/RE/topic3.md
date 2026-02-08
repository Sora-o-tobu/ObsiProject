
# DLL 注入

## Hook

**Hook** 技术广泛应用于计算机领域，其中最常用的是 *Message Hook*。

我们以键盘消息为例：

- <1> 发生键盘操作事件时，`WM_KEYDOWN` 消息被添加到 OS Message Queue。
- <2> OS 判断哪个应用程序发生了事件，从 OS Message Queue 中取出消息，添加到相应程序的 Application Message Queue
- <3> 应用程序监视自身的 Application Message Queue，发现新添加的消息后，调用相应的 Handler 进行处理

因此，OS Message Queue 和 Application Message Queue 中存在一条 **Hook Chain**，我们可以在这条链中设置一个 Hook，它会比应用程序先收到这个消息，并且可以对消息进行修改、拦截等操作。

!!! note "中间可以设置多个依次执行的 Hook，因此被称为 Hook Chain"

API `SetWindowsHookEx()` 专门用来实现 Message Hook，它的定义如下：

```c
HHOOK SetWindowsHookEx() (
	int idHook,        // hook type
	HOOKPROC lpfn,     // hook procedure
	HINSTANCE hMod,    // DLL handle
	DWORD dwThreadId   // 想要挂钩的线程 ID
);
```

Hook Procedure 需要在一个 DLL 内部，`hMod` 参数即为该 DLL 的句柄；若参数 `dwThreadId` 置 0，则认为该 Hook 影响所有进程。

设置好 Hook 后，当某个进程生成指定消息后，OS 会将该 *DLL 强制注入*该进程，然后调用注册的 Hook Procedure 进行处理。

在书写一个 Hook 实例前，我们需要稍微了解以下 DLL 的语法。

DLL 被加载到进程时，会自动运行 `DllMain()` 函数，它是 DLL 的入口点：

```c
BOOL APIENTRY DllMain( HINSTANCE hinstDLL,
                       DWORD  dwReason,
                       LPVOID lpvReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // DLL 被加载到当前进程空间时触发（只触发一次）
        // 可以在这里初始化全局变量
        break;
    case DLL_THREAD_ATTACH:
        // 当前进程创建新线程时触发
        break;
    case DLL_THREAD_DETACH:
        // 线程结束时触发
        break;
    case DLL_PROCESS_DETACH:
        // DLL 被卸载前触发（清理资源）
        break;
    }
    return TRUE;
}
```

!!! danger "`DllMain()` 执行时有一个全局锁，因此不能在该函数中做以下事情，否则会死锁"
	- 调用 `LoadLibrary` 或 `FreeLibrary`
	- 调用 `CreateThread` 或等待线程结束
	- 调用涉及 Windows 消息泵的函数（如 COM 初始化）

DLL 中的函数默认是私有函数，为了让外界调用，我们通常在函数声明时添加关键字进行导出，例如：

```c
// MyDLL.h
extern "C" __declspec(dllexport) void MyFunction();
```

!!! info "`extern "c"` 保证了函数名不变，C++ 编译器有可能会修改函数名，如 `?MyFunction@@YAXXZ`"

另外一种更工程化的方式是在项目中创建一个 `.def` 文件，该方法更加精确，并且可以指定 Ordinal，也可以不导出函数名：

```c
LIBRARY MyDll
EXPORTS
   MyFunction   @1
   HookProcess  @2 NONAME
```

