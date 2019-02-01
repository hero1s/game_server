rem  复制配置文件到server目录下

rem  清除旧有文件
del /Q ..\server\servers\pb\src\*.h
del /Q ..\server\servers\pb\src\*.cpp


rem 复制新数据

xcopy cs_msg\cpp\*.* ..\server\servers\pb\src  /Y /E

pause