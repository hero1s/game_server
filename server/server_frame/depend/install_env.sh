#!/usr/bin/env bash

echo "install gcc 8.3 begin"
yum install cmake -y
yum install dos2unix -y
yum install readline-devel -y
yum install mysql-devel -y

yum install centos-release-scl -y
yum install devtoolset-8-toolchain -y
scl enable devtoolset-8 bash
echo "source /opt/rh/devtoolset-8/enable" >> ~/.bash_profile
source ~/.bash_profile

echo "install gcc 8.3 ok"

tar -vxf jemalloc-5.2.0.tar.bz2
cd jemalloc-5.2.0/
./configure
make && make install
cd ..
rm protobuf-2.5.0 -rf

tar -vxf protobuf-2.5.0.tar.bz2
cd protobuf-2.5.0/
./configure
make && make install
cd ..
rm jemalloc-5.2.0 -rf

echo "add lddconfig"

echo "/usr/lib/" >> /etc/ld.so.conf
echo "/usr/lib64/" >> /etc/ld.so.conf
echo "/usr/local/lib/" >> /etc/ld.so.conf
echo "/usr/local/lib64/" >> /etc/ld.so.conf
ldconfig

echo "ldconfig over"
