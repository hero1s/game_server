// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg_define.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "msg_define.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace net {

namespace {

const ::google::protobuf::EnumDescriptor* SERVER_MSG_ID_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* CLIENT_MSG_ID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_msg_5fdefine_2eproto() {
  protobuf_AddDesc_msg_5fdefine_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "msg_define.proto");
  GOOGLE_CHECK(file != NULL);
  SERVER_MSG_ID_descriptor_ = file->enum_type(0);
  CLIENT_MSG_ID_descriptor_ = file->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_msg_5fdefine_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_msg_5fdefine_2eproto() {
}

void protobuf_AddDesc_msg_5fdefine_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::net::protobuf_AddDesc_error_5fcode_2eproto();
  ::net::protobuf_AddDesc_base_5fmsg_5fstruct_2eproto();
  ::net::cli::protobuf_AddDesc_client_5flogic_5fmsg_2eproto();
  ::net::svr::protobuf_AddDesc_servers_5fmsg_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\020msg_define.proto\022\003net\032\020error_code.prot"
    "o\032\025base_msg_struct.proto\032\026client_logic_m"
    "sg.proto\032\021servers_msg.proto*p\n\rSERVER_MS"
    "G_ID\022\035\n\030S2CS_MSG_REGISTER_CENTER\020\311\001\022!\n\034C"
    "S2S_MSG_REGISTER_CENTER_REP\020\312\001\022\035\n\030CS2S_M"
    "SG_SERVER_LIST_REP\020\313\001*\274\001\n\rCLIENT_MSG_ID\022"
    "\022\n\rC2S_MSG_HEART\020\350\007\022\022\n\rC2S_MSG_LOGIN\020\351\007\022"
    "\026\n\021S2C_MSG_LOGIN_REP\020\352\007\022\031\n\024S2C_MSG_LOGIN"
    "OUT_REP\020\353\007\022\035\n\030S2C_MSG_VERSION_INFO_REP\020\354"
    "\007\022\027\n\022S2C_MSG_ENTER_GAME\020\362\007\022\030\n\023S2C_MSG_PL"
    "AYER_INFO\020\363\007", 412);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "msg_define.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_msg_5fdefine_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_msg_5fdefine_2eproto {
  StaticDescriptorInitializer_msg_5fdefine_2eproto() {
    protobuf_AddDesc_msg_5fdefine_2eproto();
  }
} static_descriptor_initializer_msg_5fdefine_2eproto_;
const ::google::protobuf::EnumDescriptor* SERVER_MSG_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SERVER_MSG_ID_descriptor_;
}
bool SERVER_MSG_ID_IsValid(int value) {
  switch(value) {
    case 201:
    case 202:
    case 203:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* CLIENT_MSG_ID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CLIENT_MSG_ID_descriptor_;
}
bool CLIENT_MSG_ID_IsValid(int value) {
  switch(value) {
    case 1000:
    case 1001:
    case 1002:
    case 1003:
    case 1004:
    case 1010:
    case 1011:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace net

// @@protoc_insertion_point(global_scope)
