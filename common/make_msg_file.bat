echo create cpp code

for %%a in (cs_msg\*.proto) do .\tools\protoc -I=.\cs_msg\ --cpp_out=.\cs_msg\cpp\ %%a

xcopy cs_msg\cpp\*.* ..\server\servers\pb\src  /Y /E

pause