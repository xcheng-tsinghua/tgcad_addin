# 天工 CAD 插件代码

天工CAD下载链接：

https://www.newdimchina.com/product/94.html

## 加载插件

从 $(SolutionDir)bin/Debug/ 文件夹找到 regsvrUn.bat 和 regsvr.bat 文件

依次右键管理员运行 regsvrUn.bat 和 regsvr.bat 即可

## 代码配置

在 SampleAddin 的属性页 → 调试 → 命令 行输入 TianGong.exe 的路径，即配置完成。

## 代码功能

### AddinMenu.h：
enum MenuFunc：插件按钮响应函数的 id，每个 id 和一个响应函数对应

static const MY_COMMAND_INFO MyCommands3D[]：设置插件界面显示的按钮，每个大括号内的信息表示一个按钮

class AddinMenu：定义插件按钮的响应函数及将 id 与响应函数关联，static bool CallCommand(MenuFunc id); 函数用于将 id 与响应函数关联。

### CmdApp.h：
应用功能基类

### CmdDevice.h：
设备功能基类，比如鼠标事件、键盘事件的基类

### CmdUser.h：
用户的功能基类，它继承了 CmdDevice，因此可实现自定义的鼠标及键盘事件

### Document.h：
文件类

### DrawGL.h：
用于利用 openGL 实现在天工 CAD 界面显示内容

### MouseLocate.h：
设置鼠标停留在模型上的高亮事件

### Options.h：
选项子窗口

### SampleAddin.h：
定义插件

### SEAddin：
定义 SolidEdge 插件



