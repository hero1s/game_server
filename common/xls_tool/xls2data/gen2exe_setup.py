#-*- coding: UTF-8 -*-

from distutils.core import setup
import py2exe
import os
import glob
import shutil

setup(
    console=['gen.py',],
    options={
        'py2exe':{
            'excludes':['resconv.py', 'resconv.pyc']
        }
    }
)

# copy protobuf compile to dist directory
shutil.copy('protoc.exe', 'dist/protoc.exe')

# create dirs
dirs = ['dist/proto', 'dist/temp', 'dist/xls', 'dist/lua', 'dist/txt']
for dir in dirs:
    if not os.path.exists(dir): 
        os.makedirs(dir)

# copy xls files
xlsfiles = ('xls/*.xls', 'xls/*.xlsx')
for files in xlsfiles:
    for file in glob.glob(files):
        shutil.copy(file, 'dist/xls/')

# copy proto files
for file in glob.glob('proto/*.proto'):
    shutil.copy(file, 'dist/proto/')        