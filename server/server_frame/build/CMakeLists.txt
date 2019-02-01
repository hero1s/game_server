# cmake build list
# Copyright (C) 2013 toney
# @author: toney
# 2013-04-18
# serverframe cmake

cmake_minimum_required(VERSION 2.8.3)

include(./cmake_config/config.cmake)
include(./cmake_config/macros.cmake)
include(./cmake_config/common_path.cmake)


clear_all_list()

add_include_path(../include)
add_include_path(${COMMON_INCLUDE})
add_include_path(${MYSQL_INCLUDE})
add_include_path(../include/lua)
add_include_path(../include/zlib)
add_include_path(../include/redis)
add_include_path(../include/modern)

add_source_files(../src/*.*)

build_static_lib(svrlib ../lib)
