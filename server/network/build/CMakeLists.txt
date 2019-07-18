# cmake build list
# Copyright (C) 2013 toney
# @author: toney
# 2013-04-18
# network cmake

cmake_minimum_required(VERSION 2.8.3)

include(../../cmake_config/config_lib.cmake)
include(../../cmake_config/macros.cmake)
include(../../cmake_config/common_path.cmake)

clear_all_list()

add_include_path(../../server_frame/include)
add_include_path(${COMMON_INCLUDE})
add_include_path(../../server_frame/include/third/lua)
add_include_path(../../server_frame/include/third/zlib)
add_include_path(../../server_frame/include/modern)
add_include_path(../../server_frame/include/network_asio)
add_include_path(../../server_frame/include/third)

add_source_files(../src/*.*)

build_static_lib(network ../../server_frame/lib)
