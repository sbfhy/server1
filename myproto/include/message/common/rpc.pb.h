// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message/common/rpc.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_message_2fcommon_2frpc_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_message_2fcommon_2frpc_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3014000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3014000 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/descriptor.pb.h>
#include "service/service_enum.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_message_2fcommon_2frpc_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_message_2fcommon_2frpc_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_message_2fcommon_2frpc_2eproto;
namespace muduo {
namespace net {
class RpcMessage;
class RpcMessageDefaultTypeInternal;
extern RpcMessageDefaultTypeInternal _RpcMessage_default_instance_;
}  // namespace net
}  // namespace muduo
PROTOBUF_NAMESPACE_OPEN
template<> ::muduo::net::RpcMessage* Arena::CreateMaybeMessage<::muduo::net::RpcMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace muduo {
namespace net {

enum MessageType : int {
  MSGTYPE_DEFAULT = 0,
  MSGTYPE_REQUEST = 1,
  MSGTYPE_RESPONSE = 2,
  MSGTYPE_ERROR = 3,
  MessageType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  MessageType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool MessageType_IsValid(int value);
constexpr MessageType MessageType_MIN = MSGTYPE_DEFAULT;
constexpr MessageType MessageType_MAX = MSGTYPE_ERROR;
constexpr int MessageType_ARRAYSIZE = MessageType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MessageType_descriptor();
template<typename T>
inline const std::string& MessageType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MessageType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MessageType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MessageType_descriptor(), enum_t_value);
}
inline bool MessageType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MessageType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MessageType>(
    MessageType_descriptor(), name, value);
}
enum ErrorCode : int {
  ERR_NO_ERROR = 0,
  ERR_WRONG_PROTO = 1,
  ERR_NO_SERVICE = 2,
  ERR_NO_METHOD = 3,
  ERR_INVALID_REQUEST = 4,
  ERR_INVALID_RESPONSE = 5,
  ERR_TIMEOUT = 6,
  ErrorCode_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ErrorCode_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ErrorCode_IsValid(int value);
constexpr ErrorCode ErrorCode_MIN = ERR_NO_ERROR;
constexpr ErrorCode ErrorCode_MAX = ERR_TIMEOUT;
constexpr int ErrorCode_ARRAYSIZE = ErrorCode_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ErrorCode_descriptor();
template<typename T>
inline const std::string& ErrorCode_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ErrorCode>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ErrorCode_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ErrorCode_descriptor(), enum_t_value);
}
inline bool ErrorCode_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ErrorCode* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ErrorCode>(
    ErrorCode_descriptor(), name, value);
}
// ===================================================================

class RpcMessage PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:muduo.net.RpcMessage) */ {
 public:
  inline RpcMessage() : RpcMessage(nullptr) {}
  virtual ~RpcMessage();

  RpcMessage(const RpcMessage& from);
  RpcMessage(RpcMessage&& from) noexcept
    : RpcMessage() {
    *this = ::std::move(from);
  }

  inline RpcMessage& operator=(const RpcMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline RpcMessage& operator=(RpcMessage&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const RpcMessage& default_instance();

  static inline const RpcMessage* internal_default_instance() {
    return reinterpret_cast<const RpcMessage*>(
               &_RpcMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RpcMessage& a, RpcMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(RpcMessage* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(RpcMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline RpcMessage* New() const final {
    return CreateMaybeMessage<RpcMessage>(nullptr);
  }

  RpcMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<RpcMessage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const RpcMessage& from);
  void MergeFrom(const RpcMessage& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "muduo.net.RpcMessage";
  }
  protected:
  explicit RpcMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_message_2fcommon_2frpc_2eproto);
    return ::descriptor_table_message_2fcommon_2frpc_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRequestFieldNumber = 5,
    kResponseFieldNumber = 6,
    kIdFieldNumber = 2,
    kTypeFieldNumber = 1,
    kServiceFieldNumber = 3,
    kMethodFieldNumber = 4,
    kErrorFieldNumber = 7,
  };
  // bytes request = 5;
  void clear_request();
  const std::string& request() const;
  void set_request(const std::string& value);
  void set_request(std::string&& value);
  void set_request(const char* value);
  void set_request(const void* value, size_t size);
  std::string* mutable_request();
  std::string* release_request();
  void set_allocated_request(std::string* request);
  private:
  const std::string& _internal_request() const;
  void _internal_set_request(const std::string& value);
  std::string* _internal_mutable_request();
  public:

  // bytes response = 6;
  void clear_response();
  const std::string& response() const;
  void set_response(const std::string& value);
  void set_response(std::string&& value);
  void set_response(const char* value);
  void set_response(const void* value, size_t size);
  std::string* mutable_response();
  std::string* release_response();
  void set_allocated_response(std::string* response);
  private:
  const std::string& _internal_response() const;
  void _internal_set_response(const std::string& value);
  std::string* _internal_mutable_response();
  public:

  // fixed64 id = 2;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // .muduo.net.MessageType type = 1;
  void clear_type();
  ::muduo::net::MessageType type() const;
  void set_type(::muduo::net::MessageType value);
  private:
  ::muduo::net::MessageType _internal_type() const;
  void _internal_set_type(::muduo::net::MessageType value);
  public:

  // .ENUM.EServiceType service = 3;
  void clear_service();
  ::ENUM::EServiceType service() const;
  void set_service(::ENUM::EServiceType value);
  private:
  ::ENUM::EServiceType _internal_service() const;
  void _internal_set_service(::ENUM::EServiceType value);
  public:

  // int32 method = 4;
  void clear_method();
  ::PROTOBUF_NAMESPACE_ID::int32 method() const;
  void set_method(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_method() const;
  void _internal_set_method(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // .muduo.net.ErrorCode error = 7;
  void clear_error();
  ::muduo::net::ErrorCode error() const;
  void set_error(::muduo::net::ErrorCode value);
  private:
  ::muduo::net::ErrorCode _internal_error() const;
  void _internal_set_error(::muduo::net::ErrorCode value);
  public:

  // @@protoc_insertion_point(class_scope:muduo.net.RpcMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr request_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr response_;
  ::PROTOBUF_NAMESPACE_ID::uint64 id_;
  int type_;
  int service_;
  ::PROTOBUF_NAMESPACE_ID::int32 method_;
  int error_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_message_2fcommon_2frpc_2eproto;
};
// ===================================================================

static const int kIdempotentFieldNumber = 1111;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::PrimitiveTypeTraits< bool >, 8, false >
  idempotent;
static const int kNoreturnFieldNumber = 1112;
extern ::PROTOBUF_NAMESPACE_ID::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::PROTOBUF_NAMESPACE_ID::internal::PrimitiveTypeTraits< bool >, 8, false >
  noreturn;

// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcMessage

// .muduo.net.MessageType type = 1;
inline void RpcMessage::clear_type() {
  type_ = 0;
}
inline ::muduo::net::MessageType RpcMessage::_internal_type() const {
  return static_cast< ::muduo::net::MessageType >(type_);
}
inline ::muduo::net::MessageType RpcMessage::type() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.type)
  return _internal_type();
}
inline void RpcMessage::_internal_set_type(::muduo::net::MessageType value) {
  
  type_ = value;
}
inline void RpcMessage::set_type(::muduo::net::MessageType value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.type)
}

// fixed64 id = 2;
inline void RpcMessage::clear_id() {
  id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 RpcMessage::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 RpcMessage::id() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.id)
  return _internal_id();
}
inline void RpcMessage::_internal_set_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  id_ = value;
}
inline void RpcMessage::set_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.id)
}

// .ENUM.EServiceType service = 3;
inline void RpcMessage::clear_service() {
  service_ = 0;
}
inline ::ENUM::EServiceType RpcMessage::_internal_service() const {
  return static_cast< ::ENUM::EServiceType >(service_);
}
inline ::ENUM::EServiceType RpcMessage::service() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.service)
  return _internal_service();
}
inline void RpcMessage::_internal_set_service(::ENUM::EServiceType value) {
  
  service_ = value;
}
inline void RpcMessage::set_service(::ENUM::EServiceType value) {
  _internal_set_service(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.service)
}

// int32 method = 4;
inline void RpcMessage::clear_method() {
  method_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 RpcMessage::_internal_method() const {
  return method_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 RpcMessage::method() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.method)
  return _internal_method();
}
inline void RpcMessage::_internal_set_method(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  method_ = value;
}
inline void RpcMessage::set_method(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_method(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.method)
}

// bytes request = 5;
inline void RpcMessage::clear_request() {
  request_.ClearToEmpty();
}
inline const std::string& RpcMessage::request() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.request)
  return _internal_request();
}
inline void RpcMessage::set_request(const std::string& value) {
  _internal_set_request(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.request)
}
inline std::string* RpcMessage::mutable_request() {
  // @@protoc_insertion_point(field_mutable:muduo.net.RpcMessage.request)
  return _internal_mutable_request();
}
inline const std::string& RpcMessage::_internal_request() const {
  return request_.Get();
}
inline void RpcMessage::_internal_set_request(const std::string& value) {
  
  request_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void RpcMessage::set_request(std::string&& value) {
  
  request_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:muduo.net.RpcMessage.request)
}
inline void RpcMessage::set_request(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  request_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:muduo.net.RpcMessage.request)
}
inline void RpcMessage::set_request(const void* value,
    size_t size) {
  
  request_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:muduo.net.RpcMessage.request)
}
inline std::string* RpcMessage::_internal_mutable_request() {
  
  return request_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* RpcMessage::release_request() {
  // @@protoc_insertion_point(field_release:muduo.net.RpcMessage.request)
  return request_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void RpcMessage::set_allocated_request(std::string* request) {
  if (request != nullptr) {
    
  } else {
    
  }
  request_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), request,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:muduo.net.RpcMessage.request)
}

// bytes response = 6;
inline void RpcMessage::clear_response() {
  response_.ClearToEmpty();
}
inline const std::string& RpcMessage::response() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.response)
  return _internal_response();
}
inline void RpcMessage::set_response(const std::string& value) {
  _internal_set_response(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.response)
}
inline std::string* RpcMessage::mutable_response() {
  // @@protoc_insertion_point(field_mutable:muduo.net.RpcMessage.response)
  return _internal_mutable_response();
}
inline const std::string& RpcMessage::_internal_response() const {
  return response_.Get();
}
inline void RpcMessage::_internal_set_response(const std::string& value) {
  
  response_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void RpcMessage::set_response(std::string&& value) {
  
  response_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:muduo.net.RpcMessage.response)
}
inline void RpcMessage::set_response(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  response_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:muduo.net.RpcMessage.response)
}
inline void RpcMessage::set_response(const void* value,
    size_t size) {
  
  response_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:muduo.net.RpcMessage.response)
}
inline std::string* RpcMessage::_internal_mutable_response() {
  
  return response_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* RpcMessage::release_response() {
  // @@protoc_insertion_point(field_release:muduo.net.RpcMessage.response)
  return response_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void RpcMessage::set_allocated_response(std::string* response) {
  if (response != nullptr) {
    
  } else {
    
  }
  response_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), response,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:muduo.net.RpcMessage.response)
}

// .muduo.net.ErrorCode error = 7;
inline void RpcMessage::clear_error() {
  error_ = 0;
}
inline ::muduo::net::ErrorCode RpcMessage::_internal_error() const {
  return static_cast< ::muduo::net::ErrorCode >(error_);
}
inline ::muduo::net::ErrorCode RpcMessage::error() const {
  // @@protoc_insertion_point(field_get:muduo.net.RpcMessage.error)
  return _internal_error();
}
inline void RpcMessage::_internal_set_error(::muduo::net::ErrorCode value) {
  
  error_ = value;
}
inline void RpcMessage::set_error(::muduo::net::ErrorCode value) {
  _internal_set_error(value);
  // @@protoc_insertion_point(field_set:muduo.net.RpcMessage.error)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace net
}  // namespace muduo

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::muduo::net::MessageType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::muduo::net::MessageType>() {
  return ::muduo::net::MessageType_descriptor();
}
template <> struct is_proto_enum< ::muduo::net::ErrorCode> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::muduo::net::ErrorCode>() {
  return ::muduo::net::ErrorCode_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_message_2fcommon_2frpc_2eproto
