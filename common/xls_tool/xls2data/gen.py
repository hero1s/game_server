#-*- coding: UTF-8 -*-

import os

# 生成protobuf文件
cmd = 'protoc.exe --python_out=temp --proto_path=proto  proto/res.proto'
os.system(cmd)


import cStringIO
import sys
import codecs
import xlrd
import resconv
import temp.res_pb2 as res_pb2
from google.protobuf.descriptor import FieldDescriptor as _FieldDescriptor
from google.protobuf import text_format_to_lua as _TextFormatToLua
from google.protobuf import text_format as _TextFormat
import collections
import temp.res_pb2 as res_pb2   


def output_as_txt(pbdata, filename):
    f = file(filename, 'wb')
    txt = _TextFormat.MessageToString(pbdata)
    f.write(txt)
    f.close()


def output_as_lua(pbdata, filename):
    # write to buffer
    out = cStringIO.StringIO()
    out.write(codecs.BOM_UTF8)
    out.write(_TextFormatToLua.MessageToString(pbdata))
    txt = out.getvalue()
    out.close()

    # write to lua file
    f = file(filename, 'wb')
    f.write(txt[3:]) # remove utf-8 bom
    f.close()


def meta_of(parent_type, field_name):
    if parent_type == None:
        return res_pb2.DESCRIPTOR.message_types_by_name[field_name], None
    else:
        msg_type = parent_type.fields_by_name[field_name].message_type
        for field_type in parent_type.fields:
            if field_type.name == field_name:
                return msg_type, field_type
        return msg_type, None
        

def value_of(conf, type, raw_val):
    if (type == _FieldDescriptor.TYPE_DOUBLE or
        type == _FieldDescriptor.TYPE_FLOAT):
        if raw_val == '':
            return 0.0
        return float(field_value)
    elif (type == _FieldDescriptor.TYPE_INT32 or
          type == _FieldDescriptor.TYPE_FIXED32 or
          type == _FieldDescriptor.TYPE_UINT32 or
          type == _FieldDescriptor.TYPE_SFIXED32 or
          type == _FieldDescriptor.TYPE_SINT32):
        if raw_val == '':
            return 0
        try:
            return int(raw_val)
        except:
            try:
                return conf["Keywords"][raw_val]
            except:
                try:
                    print u'字段值"%s"没有找到映射项' % raw_val
                    input()
                except:
                    pass
                sys.exit(-1)
    elif (type == _FieldDescriptor.TYPE_INT64 or
          type == _FieldDescriptor.TYPE_UINT64 or
          type == _FieldDescriptor.TYPE_FIXED64 or
          type == _FieldDescriptor.TYPE_SFIXED64 or
          type == _FieldDescriptor.TYPE_SINT64):
        if raw_val == '':
            return 0
        try:
            return long(raw_val)
        except:
            try:
                return conf["Keywords"][raw_val]
            except:
                try:
                    print u'字段值"%s"没有找到映射项' % raw_val
                    input()
                except:
                    pass
                sys.exit(-1)

    else:
        return raw_val


# 字段处理
class FieldHandler:

    # 初始化
    def __init__(self, conf, parent, name, cur_level, max_level):

        self.conf = conf
        self.parent = parent
        self.name = name
        self.creator_name = ""
        
        self.msg_type = None
        self.field_type = None
        
        self.is_list = False
        self.is_msg = False
        self.add_new = False

        if parent == None:
            self.msg_type, self.field_type = meta_of(None, name)
        else:
            self.msg_type, self.field_type = meta_of(parent.msg_type, name)

        if self.field_type != None and self.field_type.label == _FieldDescriptor.LABEL_REPEATED:
            self.is_list = True

        if self.field_type != None and self.field_type.type == _FieldDescriptor.TYPE_MESSAGE:
            self.is_msg = True

        if cur_level >= max_level and parent != None and parent.is_list and parent.is_msg and parent.creator_name == "":
            parent.creator_name = name
            self.add_new = True
        
        
    # 解析字段
    def set_val(self, parent_val, col_val):

        if self.parent == None:
            return parent_val

        # 列表
        if self.parent.is_list:
            # 消息列表
            if self.parent.is_msg:
                # 消息列表第一个字段
                if self.add_new:
                    # 新增一个列表值
                    cur = parent_val.add()
                    if self.is_list:
                        cur = getattr(cur, self.name)
                        if self.is_msg:
                            return cur
                        else:
                            cur.append(value_of(self.conf, self.field_type.type, col_val))
                            return cur
                    # 设置字段值
                    setattr(cur, self.name, value_of(self.conf, self.field_type.type, col_val))
                    return getattr(cur, self.name)
                # 不是消息列表第一个字段
                else:
                    # 获取列表最后一个值
                    num = len(parent_val)
                    cur = parent_val[num-1]
                    if self.is_list:
                        cur = getattr(cur, self.name)
                        if self.is_msg:
                            return cur
                        else:
                            cur.append(value_of(self.conf, self.field_type.type, col_val))
                            return cur
                    # 设置字段值
                    setattr(cur, self.name, value_of(self.conf, self.field_type.type, col_val))
                    return getattr(cur, self.name)
            # 简单列表
            else:
                cur = parent_val
                # 设置字段值
                cur.append(valOf(col_val))
        # 不是列表
        else:
            cur = parent_val
            if self.is_list:
                cur = getattr(cur, self.name)
                if self.is_msg:
                    return cur
                else:
                    cur.append(value_of(self.conf, self.field_type.type, col_val))
                    return cur
            # 设置字段值
            setattr(cur, self.name, value_of(self.conf, self.field_type.type, col_val))
            return getattr(cur, self.name)
            
    
def conv_sheet(conf, pbdata, sheet, titles):

    # 生成字段处理器
    handlers = {}
    for title in titles:
        field = conf["Fields" ][title]
        parent = None
        parts = field.split(".")
        full_name = ""
        name_array = []
        num = len(parts)
        for i, part in enumerate(parts):
            name_array.append(part)
            full_name = ".".join(name_array)
            if full_name in handlers.keys():
                parent = handlers[full_name]
                continue
            handler = FieldHandler(conf, parent, part, i, num-1)
            handlers[full_name] = handler
            parent = handler

    # 把exceltitle资源表格字段对应起来
    col2titles = {}
    col2ignor = []
    for col in range(0, sheet.ncols):
        title = sheet.cell_value(0, col)
        if title not in titles:
            col2ignor.append(col)
            continue
        col2titles[col] = title

    # 处理表格资源
    for row in range(1, sheet.nrows):
        for col in range(0, sheet.ncols):
            val = sheet.cell_value(row, col)
            if col in col2ignor:
                continue
            title = col2titles[col]
            field = conf["Fields" ][title]
            parts = field.split(".")
            full_name = ""
            name_array = []
            parent_val = pbdata
            for part in parts:
                name_array.append(part)
                full_name = ".".join(name_array)
                has_handler = full_name in handlers.keys()
                if not has_handler:
                    print full_name, "not found"
                    sys.exit(-1)
                handler = handlers[full_name]
                parent_val = handler.set_val(parent_val, val)


def conv_res(conf):

    # 原始的数据
    _pbdata = getattr(res_pb2, conf["DataType"])()

    # excel表格字段名
    titles = []

    # 处理excel文件
    xls = xlrd.open_workbook(resconv.xls_path+conf["Filename"]["Input"])
    for sheet_name in conf["SheetNames"]:
        sheet = xls.sheet_by_name(sheet_name)
        # 收集表格字段名
        for col in range(0, sheet.ncols):
            title = sheet.cell_value(0, col)
            if title not in conf["Fields" ].keys():
                continue
            if title not in titles:
                titles.append(title)

    # 正式处理表格
    for sheet_name in conf["SheetNames"]:
        sheet = xls.sheet_by_name(sheet_name)
        conv_sheet(conf, _pbdata, sheet, titles)

    # 打印数据
    #print _pbdata

    # 客户端和后台输出同样的资源
    res_type = conf["ResType"]
    res_file = conf["Filename"]["Output"]

    if resconv.RES_FOR_CLT_AND_SVR == res_type:
        output_as_lua(_pbdata, resconv.lua_path + res_file + '.lua')
        output_as_txt(_pbdata, resconv.txt_path + res_file + '.txt')
    # 仅输出为客户端资源
    elif resconv.RES_FOR_CLT == res_type:
        output_as_lua(_pbdata, resconv.lua_path + res_file + '.lua')
    # 仅输出为后台资源
    elif resconv.RES_FOR_SVR == res_type:
        output_as_txt(_pbdata, resconv.txt_path + res_file + '.txt')
    else:
        print u'-[错误:resconv.py]表格"%s"没有正确配置资源输出类型' % (res_file, )
        try:
            input()
        except:
            pass
        sys.exit(-1)


    
for conf in resconv.confs:
    res_file = conf["Filename"]["Input"]
    print res_file,"..."
    conv_res(conf)


try:
  print u'转表完成，请按任意键退出'
  input()
except:
  pass
