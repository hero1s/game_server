// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: error_code.proto

#include "error_code.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace net {
}  // namespace net
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_error_5fcode_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_error_5fcode_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_error_5fcode_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_error_5fcode_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_error_5fcode_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020error_code.proto\022\003net*\321\004\n\013RESULT_CODE\022"
  "\024\n\020RESULT_CODE_FAIL\020\000\022\027\n\023RESULT_CODE_SUC"
  "CESS\020\001\022\032\n\026RESULT_CODE_CION_ERROR\020\003\022\034\n\030RE"
  "SULT_CODE_PASSWD_ERROR\020\004\022\034\n\030RESULT_CODE_"
  "NEED_INLOBBY\020\005\022\032\n\026RESULT_CODE_REPEAT_GET"
  "\020\006\022\030\n\024RESULT_CODE_NOT_COND\020\007\022\033\n\027RESULT_C"
  "ODE_ERROR_PARAM\020\010\022\031\n\025RESULT_CODE_NOT_TAB"
  "LE\020\t\022\030\n\024RESULT_CODE_NOT_OWER\020\n\022\031\n\025RESULT"
  "_CODE_BLACKLIST\020\013\022\033\n\027RESULT_CODE_NOT_DIA"
  "MOND\020\014\022\036\n\032RESULT_CODE_ERROR_PLAYERID\020\r\022\032"
  "\n\026RESULT_CODE_TABLE_FULL\020\016\022\027\n\023RESULT_COD"
  "E_GAMEING\020\017\022\033\n\027RESULT_CODE_ERROR_STATE\020\020"
  "\022\033\n\027RESULT_CODE_LOGIN_OTHER\020\021\022\032\n\026RESULT_"
  "CODE_SVR_REPAIR\020\022\022\025\n\021RESULT_CODE_CDING\020\023"
  "\022\031\n\025RESULT_CODE_EXIST_OBJ\020\024\022\036\n\032RESULT_CO"
  "DE_ENTER_SVR_FAIL\020\025"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_error_5fcode_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_error_5fcode_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_error_5fcode_2eproto_once;
static bool descriptor_table_error_5fcode_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_error_5fcode_2eproto = {
  &descriptor_table_error_5fcode_2eproto_initialized, descriptor_table_protodef_error_5fcode_2eproto, "error_code.proto", 619,
  &descriptor_table_error_5fcode_2eproto_once, descriptor_table_error_5fcode_2eproto_sccs, descriptor_table_error_5fcode_2eproto_deps, 0, 0,
  schemas, file_default_instances, TableStruct_error_5fcode_2eproto::offsets,
  file_level_metadata_error_5fcode_2eproto, 0, file_level_enum_descriptors_error_5fcode_2eproto, file_level_service_descriptors_error_5fcode_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_error_5fcode_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_error_5fcode_2eproto), true);
namespace net {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* RESULT_CODE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_error_5fcode_2eproto);
  return file_level_enum_descriptors_error_5fcode_2eproto[0];
}
bool RESULT_CODE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace net
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
