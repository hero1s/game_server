rem  ���������ļ���serverĿ¼��

rem  ��������ļ�
del /Q ..\server\servers\pb\src\*.h
del /Q ..\server\servers\pb\src\*.cpp


rem ����������

xcopy cs_msg\cpp\*.* ..\server\servers\pb\src  /Y /E

pause