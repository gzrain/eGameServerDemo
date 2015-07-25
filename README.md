# eGameServerDemo
A simple game server demo written in C++

1.开发环境及平台
	VS2013 + Windows

2.开发语言
	C++

3.数据库

	1）数据库使用MySql引擎
	
	2）在运行项目前，需先将mysql表结构导入，具体导入方法在 database 目录下
	
	3）程序运行时，运行目录下会自动生成Setting.ini文件，Mysql的连接参数，如用户名、密码，在ini文件中配置
	
	4）导入、配置 结束后，即可正常运行项目
	

4.测试
	测试程序在test目录下

	
##项目源码结构
	Common ：基础模块，提供数据库访问等一些项目无关服务
	GameServerCore ：服务器核心模块，提供服务器TCP通讯、服务器业务逻辑等功能
	MFCClient ： 客户端测试程序，使用MFC编写
	ServerConsoleDemo ：服务端测试Win32程序
