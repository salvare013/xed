# 项目：xed 文本编辑器

## 作者:@xmx

qt 练手小玩意，熟悉常用组件功能，感兴趣可以下载构建体验

请使用 vcpkg 安装 qt5-base 包
安装命令：vcpkg install qt5-base

Windows 下最好在 vscode 下构建，并且使用 visual studio 安装 llvm 工具链并添加 bin 目录到环境变量，然后使用 clang++编译,因为测试发现 cl 无法编译，未找到原因

Linux 下测试 Ubuntu24 可以正常构建，其他会有各种报错，应该是 vcpkg 的依赖处理问题，无法解决
