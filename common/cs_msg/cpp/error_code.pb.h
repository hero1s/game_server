// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: error_code.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_error_5fcode_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_error_5fcode_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_error_5fcode_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_error_5fcode_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_error_5fcode_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace net {

enum RESULT_CODE : int {
  RESULT_CODE_FAIL = 0,
  RESULT_CODE_SUCCESS = 1,
  RESULT_CODE_CION_ERROR = 3,
  RESULT_CODE_PASSWD_ERROR = 4,
  RESULT_CODE_NEED_INLOBBY = 5,
  RESULT_CODE_REPEAT_GET = 6,
  RESULT_CODE_NOT_COND = 7,
  RESULT_CODE_ERROR_PARAM = 8,
  RESULT_CODE_NOT_TABLE = 9,
  RESULT_CODE_NOT_OWER = 10,
  RESULT_CODE_BLACKLIST = 11,
  RESULT_CODE_NOT_DIAMOND = 12,
  RESULT_CODE_ERROR_PLAYERID = 13,
  RESULT_CODE_TABLE_FULL = 14,
  RESULT_CODE_GAMEING = 15,
  RESULT_CODE_ERROR_STATE = 16,
  RESULT_CODE_LOGIN_OTHER = 17,
  RESULT_CODE_SVR_REPAIR = 18,
  RESULT_CODE_CDING = 19,
  RESULT_CODE_EXIST_OBJ = 20,
  RESULT_CODE_ENTER_SVR_FAIL = 21
};
bool RESULT_CODE_IsValid(int value);
constexpr RESULT_CODE RESULT_CODE_MIN = RESULT_CODE_FAIL;
constexpr RESULT_CODE RESULT_CODE_MAX = RESULT_CODE_ENTER_SVR_FAIL;
constexpr int RESULT_CODE_ARRAYSIZE = RESULT_CODE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* RESULT_CODE_descriptor();
template<typename T>
inline const std::string& RESULT_CODE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, RESULT_CODE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function RESULT_CODE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    RESULT_CODE_descriptor(), enum_t_value);
}
inline bool RESULT_CODE_Parse(
    const std::string& name, RESULT_CODE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<RESULT_CODE>(
    RESULT_CODE_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace net

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::net::RESULT_CODE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::net::RESULT_CODE>() {
  return ::net::RESULT_CODE_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_error_5fcode_2eproto
