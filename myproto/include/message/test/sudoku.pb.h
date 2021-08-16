// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message/test/sudoku.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_message_2ftest_2fsudoku_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_message_2ftest_2fsudoku_2eproto

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
#include <google/protobuf/unknown_field_set.h>
#include "service.h"
#include <memory>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_message_2ftest_2fsudoku_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_message_2ftest_2fsudoku_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_message_2ftest_2fsudoku_2eproto;
namespace sudoku {
class SudokuRequest;
class SudokuRequestDefaultTypeInternal;
extern SudokuRequestDefaultTypeInternal _SudokuRequest_default_instance_;
class SudokuResponse;
class SudokuResponseDefaultTypeInternal;
extern SudokuResponseDefaultTypeInternal _SudokuResponse_default_instance_;
}  // namespace sudoku
PROTOBUF_NAMESPACE_OPEN
template<> ::sudoku::SudokuRequest* Arena::CreateMaybeMessage<::sudoku::SudokuRequest>(Arena*);
template<> ::sudoku::SudokuResponse* Arena::CreateMaybeMessage<::sudoku::SudokuResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace sudoku {

// ===================================================================

class SudokuRequest PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:sudoku.SudokuRequest) */ {
 public:
  inline SudokuRequest() : SudokuRequest(nullptr) {}
  virtual ~SudokuRequest();

  SudokuRequest(const SudokuRequest& from);
  SudokuRequest(SudokuRequest&& from) noexcept
    : SudokuRequest() {
    *this = ::std::move(from);
  }

  inline SudokuRequest& operator=(const SudokuRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline SudokuRequest& operator=(SudokuRequest&& from) noexcept {
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
  static const SudokuRequest& default_instance();

  static inline const SudokuRequest* internal_default_instance() {
    return reinterpret_cast<const SudokuRequest*>(
               &_SudokuRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SudokuRequest& a, SudokuRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(SudokuRequest* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SudokuRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SudokuRequest* New() const final {
    return CreateMaybeMessage<SudokuRequest>(nullptr);
  }

  SudokuRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SudokuRequest>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SudokuRequest& from);
  void MergeFrom(const SudokuRequest& from);
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
  void InternalSwap(SudokuRequest* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "sudoku.SudokuRequest";
  }
  protected:
  explicit SudokuRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_message_2ftest_2fsudoku_2eproto);
    return ::descriptor_table_message_2ftest_2fsudoku_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCheckerboardFieldNumber = 1,
  };
  // string checkerboard = 1;
  void clear_checkerboard();
  const std::string& checkerboard() const;
  void set_checkerboard(const std::string& value);
  void set_checkerboard(std::string&& value);
  void set_checkerboard(const char* value);
  void set_checkerboard(const char* value, size_t size);
  std::string* mutable_checkerboard();
  std::string* release_checkerboard();
  void set_allocated_checkerboard(std::string* checkerboard);
  private:
  const std::string& _internal_checkerboard() const;
  void _internal_set_checkerboard(const std::string& value);
  std::string* _internal_mutable_checkerboard();
  public:

  // @@protoc_insertion_point(class_scope:sudoku.SudokuRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr checkerboard_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_message_2ftest_2fsudoku_2eproto;
};
// -------------------------------------------------------------------

class SudokuResponse PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:sudoku.SudokuResponse) */ {
 public:
  inline SudokuResponse() : SudokuResponse(nullptr) {}
  virtual ~SudokuResponse();

  SudokuResponse(const SudokuResponse& from);
  SudokuResponse(SudokuResponse&& from) noexcept
    : SudokuResponse() {
    *this = ::std::move(from);
  }

  inline SudokuResponse& operator=(const SudokuResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline SudokuResponse& operator=(SudokuResponse&& from) noexcept {
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
  static const SudokuResponse& default_instance();

  static inline const SudokuResponse* internal_default_instance() {
    return reinterpret_cast<const SudokuResponse*>(
               &_SudokuResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SudokuResponse& a, SudokuResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(SudokuResponse* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SudokuResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SudokuResponse* New() const final {
    return CreateMaybeMessage<SudokuResponse>(nullptr);
  }

  SudokuResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SudokuResponse>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SudokuResponse& from);
  void MergeFrom(const SudokuResponse& from);
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
  void InternalSwap(SudokuResponse* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "sudoku.SudokuResponse";
  }
  protected:
  explicit SudokuResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_message_2ftest_2fsudoku_2eproto);
    return ::descriptor_table_message_2ftest_2fsudoku_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kCheckerboardFieldNumber = 2,
    kSolvedFieldNumber = 1,
  };
  // string checkerboard = 2;
  void clear_checkerboard();
  const std::string& checkerboard() const;
  void set_checkerboard(const std::string& value);
  void set_checkerboard(std::string&& value);
  void set_checkerboard(const char* value);
  void set_checkerboard(const char* value, size_t size);
  std::string* mutable_checkerboard();
  std::string* release_checkerboard();
  void set_allocated_checkerboard(std::string* checkerboard);
  private:
  const std::string& _internal_checkerboard() const;
  void _internal_set_checkerboard(const std::string& value);
  std::string* _internal_mutable_checkerboard();
  public:

  // bool solved = 1;
  void clear_solved();
  bool solved() const;
  void set_solved(bool value);
  private:
  bool _internal_solved() const;
  void _internal_set_solved(bool value);
  public:

  // @@protoc_insertion_point(class_scope:sudoku.SudokuResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr checkerboard_;
  bool solved_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_message_2ftest_2fsudoku_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SudokuRequest

// string checkerboard = 1;
inline void SudokuRequest::clear_checkerboard() {
  checkerboard_.ClearToEmpty();
}
inline const std::string& SudokuRequest::checkerboard() const {
  // @@protoc_insertion_point(field_get:sudoku.SudokuRequest.checkerboard)
  return _internal_checkerboard();
}
inline void SudokuRequest::set_checkerboard(const std::string& value) {
  _internal_set_checkerboard(value);
  // @@protoc_insertion_point(field_set:sudoku.SudokuRequest.checkerboard)
}
inline std::string* SudokuRequest::mutable_checkerboard() {
  // @@protoc_insertion_point(field_mutable:sudoku.SudokuRequest.checkerboard)
  return _internal_mutable_checkerboard();
}
inline const std::string& SudokuRequest::_internal_checkerboard() const {
  return checkerboard_.Get();
}
inline void SudokuRequest::_internal_set_checkerboard(const std::string& value) {
  
  checkerboard_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void SudokuRequest::set_checkerboard(std::string&& value) {
  
  checkerboard_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:sudoku.SudokuRequest.checkerboard)
}
inline void SudokuRequest::set_checkerboard(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  checkerboard_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:sudoku.SudokuRequest.checkerboard)
}
inline void SudokuRequest::set_checkerboard(const char* value,
    size_t size) {
  
  checkerboard_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:sudoku.SudokuRequest.checkerboard)
}
inline std::string* SudokuRequest::_internal_mutable_checkerboard() {
  
  return checkerboard_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* SudokuRequest::release_checkerboard() {
  // @@protoc_insertion_point(field_release:sudoku.SudokuRequest.checkerboard)
  return checkerboard_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SudokuRequest::set_allocated_checkerboard(std::string* checkerboard) {
  if (checkerboard != nullptr) {
    
  } else {
    
  }
  checkerboard_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), checkerboard,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:sudoku.SudokuRequest.checkerboard)
}

// -------------------------------------------------------------------

// SudokuResponse

// bool solved = 1;
inline void SudokuResponse::clear_solved() {
  solved_ = false;
}
inline bool SudokuResponse::_internal_solved() const {
  return solved_;
}
inline bool SudokuResponse::solved() const {
  // @@protoc_insertion_point(field_get:sudoku.SudokuResponse.solved)
  return _internal_solved();
}
inline void SudokuResponse::_internal_set_solved(bool value) {
  
  solved_ = value;
}
inline void SudokuResponse::set_solved(bool value) {
  _internal_set_solved(value);
  // @@protoc_insertion_point(field_set:sudoku.SudokuResponse.solved)
}

// string checkerboard = 2;
inline void SudokuResponse::clear_checkerboard() {
  checkerboard_.ClearToEmpty();
}
inline const std::string& SudokuResponse::checkerboard() const {
  // @@protoc_insertion_point(field_get:sudoku.SudokuResponse.checkerboard)
  return _internal_checkerboard();
}
inline void SudokuResponse::set_checkerboard(const std::string& value) {
  _internal_set_checkerboard(value);
  // @@protoc_insertion_point(field_set:sudoku.SudokuResponse.checkerboard)
}
inline std::string* SudokuResponse::mutable_checkerboard() {
  // @@protoc_insertion_point(field_mutable:sudoku.SudokuResponse.checkerboard)
  return _internal_mutable_checkerboard();
}
inline const std::string& SudokuResponse::_internal_checkerboard() const {
  return checkerboard_.Get();
}
inline void SudokuResponse::_internal_set_checkerboard(const std::string& value) {
  
  checkerboard_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void SudokuResponse::set_checkerboard(std::string&& value) {
  
  checkerboard_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:sudoku.SudokuResponse.checkerboard)
}
inline void SudokuResponse::set_checkerboard(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  checkerboard_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:sudoku.SudokuResponse.checkerboard)
}
inline void SudokuResponse::set_checkerboard(const char* value,
    size_t size) {
  
  checkerboard_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:sudoku.SudokuResponse.checkerboard)
}
inline std::string* SudokuResponse::_internal_mutable_checkerboard() {
  
  return checkerboard_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* SudokuResponse::release_checkerboard() {
  // @@protoc_insertion_point(field_release:sudoku.SudokuResponse.checkerboard)
  return checkerboard_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SudokuResponse::set_allocated_checkerboard(std::string* checkerboard) {
  if (checkerboard != nullptr) {
    
  } else {
    
  }
  checkerboard_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), checkerboard,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:sudoku.SudokuResponse.checkerboard)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------



// ===================================================================

typedef     ::std::shared_ptr<SudokuRequest>     SudokuRequestPtr;
typedef     ::std::shared_ptr<SudokuResponse>     SudokuResponsePtr;

// -------------------------------------------------------------------

class SudokuService_Stub;

class SudokuService : public ::muduo::net::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline SudokuService() {};
 public:
  virtual ~SudokuService();

  typedef SudokuService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void Solve(const ::sudoku::SudokuRequestPtr& request,
                       const ::sudoku::SudokuResponsePtr& response);
  virtual void Solve_DoneCb(const ::sudoku::SudokuRequestPtr& request,
                              const ::sudoku::SudokuResponsePtr& response) {} 
  virtual void Solve_TimeOut(const ::sudoku::SudokuRequestPtr& request) {} 


  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  const ::google::protobuf::Message& GetRequestPrototype(
                             const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
                             const ::google::protobuf::MethodDescriptor* method) const;

  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  const ::google::protobuf::MessagePtr& request,
                  const ::google::protobuf::MessagePtr& response);

  void DoneCallback(const ::google::protobuf::MethodDescriptor* method,
                    const ::google::protobuf::MessagePtr& request,
                    const ::google::protobuf::MessagePtr& response);

  void TimeOut(const ::google::protobuf::MethodDescriptor* method,
               const ::google::protobuf::MessagePtr& request);

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SudokuService);
};

class SudokuService_Stub : public SudokuService {
 public:
  SudokuService_Stub() {} 
  SudokuService_Stub(::muduo::net::RpcChannelBase* channel);
  ~SudokuService_Stub();

  inline ::muduo::net::RpcChannelBase* channel() { return channel_; }

  // implements SudokuService ------------------------------------------

  virtual void Solve(const ::sudoku::SudokuRequestPtr& request,
                       const ::sudoku::SudokuResponsePtr& response);

 private:
  muduo::net::RpcChannelBase* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SudokuService_Stub);
};

// @@protoc_insertion_point(namespace_scope)

}  // namespace sudoku

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_message_2ftest_2fsudoku_2eproto
