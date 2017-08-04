1、确保lua库已编译，这里使用的是“\third\LuaJIT-2.1.0-beta2\src\”目录下的lua51.lib。


2、确保mysql库已安装（由于mysql的lib库太大，未上传至svn）。
下载链接：
http://dev.mysql.com/downloads/mysql/
选择“Windows (x86, 32-bit), ZIP Archive”包，下载后解压安装。
安装方法：
http://jingyan.baidu.com/article/597035521d5de28fc00740e6.html
http://blog.csdn.net/i_am_wangbo/article/details/49999407


3、以mysql数据库的连接为例，修改luasql目录下的makefile文件中的以下内容：
文件名：Makefile.win.mysql
修改内容：
（1）lua库相关路径：
LUA_INC=F:\Hades\third\LuaJIT-2.1.0-beta2\src
LUA_LIBDIR=F:\Hades\third\LuaJIT-2.1.0-beta2\src
LUA_LIB=F:\Hades\third\LuaJIT-2.1.0-beta2\src\lua51.lib

（2）mysql库相关路径：
DRIVER_INCLUDE= /I"F:\Hades\third\mysql_server\mysql-5.7.14-win32\include"
DRIVER_LIBS= "F:\Hades\third\mysql_server\mysql-5.7.14-win32\lib\libmySQL.lib" "F:\Hades\third\mysql_server\mysql-5.7.14-win32\lib\mysqlclient.lib" 

（3）Hades工具第三方库文件的输出目录：
OUTPUT_DIR="F:\Hades\build\function"


4、luasql的编译方法：
确保lua和mysql都使用32位库，windows平台下打开VS自带的32位命令行窗口，进入luasql所在目录，输入编译命令：
F:\Hades\function\luasql-2.3.2>nmake /f Makefile.win.mysql
如果编译成功，会在src目录下生成 mysql.dll 文件。
另，Makefile.win.mysql 文件中配置为编译完后拷贝一份 mysql.dll 到 OUTPUT_DIR 目录下。


5、用luasql连接mysql数据库：
（1）确保本机mysql数据库已启动。

（2）测试脚本：
myTest.lua：从数据库已有表格中取值；
myTest2.lua：向数据库中建立一个新表。