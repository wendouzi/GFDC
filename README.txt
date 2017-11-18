windows 下编译方法一：
1. 打开visual studio 2013 的 prompt command 命令行， 切换到build目录，输入cmake .. -G"Visual Studio 12 Win64"

2. 输入 devenv DISEASECONTROL.sln   /build release ， 编译完成。

3. 运行


不带命令行输出窗口的编译：
1. 打开visual studio 2013 的 prompt command 命令行， 切换到build目录，输入cmake .. -G"Visual Studio 12 Win64"

2. 输入 devenv DISEASECONTROL.sln ， 选择  /build release， linker-> system:/subsystem:windows && ENTRY: mainCRTStartup。

3. 运行