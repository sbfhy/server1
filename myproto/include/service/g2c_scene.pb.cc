// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: service/g2c_scene.proto

#include "service/g2c_scene.pb.h"

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
namespace CMD {
class G2C_EnterSceneArgDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<G2C_EnterSceneArg> _instance;
} _G2C_EnterSceneArg_default_instance_;
class G2C_EnterSceneResDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<G2C_EnterSceneRes> _instance;
} _G2C_EnterSceneRes_default_instance_;
}  // namespace CMD
static void InitDefaultsscc_info_G2C_EnterSceneArg_service_2fg2c_5fscene_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::CMD::_G2C_EnterSceneArg_default_instance_;
    new (ptr) ::CMD::G2C_EnterSceneArg();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_G2C_EnterSceneArg_service_2fg2c_5fscene_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_G2C_EnterSceneArg_service_2fg2c_5fscene_2eproto}, {}};

static void InitDefaultsscc_info_G2C_EnterSceneRes_service_2fg2c_5fscene_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::CMD::_G2C_EnterSceneRes_default_instance_;
    new (ptr) ::CMD::G2C_EnterSceneRes();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_G2C_EnterSceneRes_service_2fg2c_5fscene_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_G2C_EnterSceneRes_service_2fg2c_5fscene_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_service_2fg2c_5fscene_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_service_2fg2c_5fscene_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_service_2fg2c_5fscene_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_service_2fg2c_5fscene_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::CMD::G2C_EnterSceneArg, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::CMD::G2C_EnterSceneRes, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::CMD::G2C_EnterSceneArg)},
  { 5, -1, sizeof(::CMD::G2C_EnterSceneRes)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::CMD::_G2C_EnterSceneArg_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::CMD::_G2C_EnterSceneRes_default_instance_),
};

const char descriptor_table_protodef_service_2fg2c_5fscene_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\027service/g2c_scene.proto\022\003CMD\"\023\n\021G2C_En"
  "terSceneArg\"\023\n\021G2C_EnterSceneRes2T\n\020G2C_"
  "SceneService\022@\n\016G2C_EnterScene\022\026.CMD.G2C"
  "_EnterSceneArg\032\026.CMD.G2C_EnterSceneResb\006"
  "proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_service_2fg2c_5fscene_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_service_2fg2c_5fscene_2eproto_sccs[2] = {
  &scc_info_G2C_EnterSceneArg_service_2fg2c_5fscene_2eproto.base,
  &scc_info_G2C_EnterSceneRes_service_2fg2c_5fscene_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_service_2fg2c_5fscene_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_service_2fg2c_5fscene_2eproto = {
  false, false, descriptor_table_protodef_service_2fg2c_5fscene_2eproto, "service/g2c_scene.proto", 166,
  &descriptor_table_service_2fg2c_5fscene_2eproto_once, descriptor_table_service_2fg2c_5fscene_2eproto_sccs, descriptor_table_service_2fg2c_5fscene_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_service_2fg2c_5fscene_2eproto::offsets,
  file_level_metadata_service_2fg2c_5fscene_2eproto, 2, file_level_enum_descriptors_service_2fg2c_5fscene_2eproto, file_level_service_descriptors_service_2fg2c_5fscene_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_service_2fg2c_5fscene_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_service_2fg2c_5fscene_2eproto)), true);
namespace CMD {

// ===================================================================

class G2C_EnterSceneArg::_Internal {
 public:
};

G2C_EnterSceneArg::G2C_EnterSceneArg(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:CMD.G2C_EnterSceneArg)
}
G2C_EnterSceneArg::G2C_EnterSceneArg(const G2C_EnterSceneArg& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:CMD.G2C_EnterSceneArg)
}

void G2C_EnterSceneArg::SharedCtor() {
}

G2C_EnterSceneArg::~G2C_EnterSceneArg() {
  // @@protoc_insertion_point(destructor:CMD.G2C_EnterSceneArg)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void G2C_EnterSceneArg::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void G2C_EnterSceneArg::ArenaDtor(void* object) {
  G2C_EnterSceneArg* _this = reinterpret_cast< G2C_EnterSceneArg* >(object);
  (void)_this;
}
void G2C_EnterSceneArg::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void G2C_EnterSceneArg::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const G2C_EnterSceneArg& G2C_EnterSceneArg::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_G2C_EnterSceneArg_service_2fg2c_5fscene_2eproto.base);
  return *internal_default_instance();
}


void G2C_EnterSceneArg::Clear() {
// @@protoc_insertion_point(message_clear_start:CMD.G2C_EnterSceneArg)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* G2C_EnterSceneArg::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* G2C_EnterSceneArg::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:CMD.G2C_EnterSceneArg)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:CMD.G2C_EnterSceneArg)
  return target;
}

size_t G2C_EnterSceneArg::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CMD.G2C_EnterSceneArg)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void G2C_EnterSceneArg::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CMD.G2C_EnterSceneArg)
  GOOGLE_DCHECK_NE(&from, this);
  const G2C_EnterSceneArg* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<G2C_EnterSceneArg>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CMD.G2C_EnterSceneArg)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CMD.G2C_EnterSceneArg)
    MergeFrom(*source);
  }
}

void G2C_EnterSceneArg::MergeFrom(const G2C_EnterSceneArg& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CMD.G2C_EnterSceneArg)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void G2C_EnterSceneArg::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CMD.G2C_EnterSceneArg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void G2C_EnterSceneArg::CopyFrom(const G2C_EnterSceneArg& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CMD.G2C_EnterSceneArg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool G2C_EnterSceneArg::IsInitialized() const {
  return true;
}

void G2C_EnterSceneArg::InternalSwap(G2C_EnterSceneArg* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
}

::PROTOBUF_NAMESPACE_ID::Metadata G2C_EnterSceneArg::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class G2C_EnterSceneRes::_Internal {
 public:
};

G2C_EnterSceneRes::G2C_EnterSceneRes(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:CMD.G2C_EnterSceneRes)
}
G2C_EnterSceneRes::G2C_EnterSceneRes(const G2C_EnterSceneRes& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:CMD.G2C_EnterSceneRes)
}

void G2C_EnterSceneRes::SharedCtor() {
}

G2C_EnterSceneRes::~G2C_EnterSceneRes() {
  // @@protoc_insertion_point(destructor:CMD.G2C_EnterSceneRes)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void G2C_EnterSceneRes::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void G2C_EnterSceneRes::ArenaDtor(void* object) {
  G2C_EnterSceneRes* _this = reinterpret_cast< G2C_EnterSceneRes* >(object);
  (void)_this;
}
void G2C_EnterSceneRes::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void G2C_EnterSceneRes::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const G2C_EnterSceneRes& G2C_EnterSceneRes::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_G2C_EnterSceneRes_service_2fg2c_5fscene_2eproto.base);
  return *internal_default_instance();
}


void G2C_EnterSceneRes::Clear() {
// @@protoc_insertion_point(message_clear_start:CMD.G2C_EnterSceneRes)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* G2C_EnterSceneRes::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* G2C_EnterSceneRes::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:CMD.G2C_EnterSceneRes)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:CMD.G2C_EnterSceneRes)
  return target;
}

size_t G2C_EnterSceneRes::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:CMD.G2C_EnterSceneRes)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void G2C_EnterSceneRes::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:CMD.G2C_EnterSceneRes)
  GOOGLE_DCHECK_NE(&from, this);
  const G2C_EnterSceneRes* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<G2C_EnterSceneRes>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:CMD.G2C_EnterSceneRes)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:CMD.G2C_EnterSceneRes)
    MergeFrom(*source);
  }
}

void G2C_EnterSceneRes::MergeFrom(const G2C_EnterSceneRes& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:CMD.G2C_EnterSceneRes)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void G2C_EnterSceneRes::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:CMD.G2C_EnterSceneRes)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void G2C_EnterSceneRes::CopyFrom(const G2C_EnterSceneRes& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:CMD.G2C_EnterSceneRes)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool G2C_EnterSceneRes::IsInitialized() const {
  return true;
}

void G2C_EnterSceneRes::InternalSwap(G2C_EnterSceneRes* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
}

::PROTOBUF_NAMESPACE_ID::Metadata G2C_EnterSceneRes::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

G2C_SceneService::~G2C_SceneService() {}

static const ::google::protobuf::ServiceDescriptor* G2C_SceneService_descriptor_ = NULL;

const ::google::protobuf::ServiceDescriptor* G2C_SceneService::descriptor() {
  // protobuf_AssignDescriptorsOnce();
  if (G2C_SceneService_descriptor_ == NULL)
    G2C_SceneService_descriptor_ = ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
        "service/g2c_scene.proto")->service(0);
  return G2C_SceneService_descriptor_;
}

const ::google::protobuf::ServiceDescriptor* G2C_SceneService::GetDescriptor() {
  return descriptor();
}

void G2C_SceneService::G2C_EnterScene(const ::CMD::G2C_EnterSceneArgPtr&,
                         const ::CMD::G2C_EnterSceneResPtr&) {
  assert(0);
}

void G2C_SceneService::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             const ::google::protobuf::MessagePtr& request,
                             const ::google::protobuf::MessagePtr& response) {
  GOOGLE_DCHECK_EQ(method->service(), G2C_SceneService_descriptor_);
  switch(method->index()) {
    case 0:
      G2C_EnterScene(::google::protobuf::down_pointer_cast<::CMD::G2C_EnterSceneArg>(request),
             ::google::protobuf::down_pointer_cast<::CMD::G2C_EnterSceneRes>(response));
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

void G2C_SceneService::DoneCallback(const ::google::protobuf::MethodDescriptor* method,
                               const ::google::protobuf::MessagePtr& request,
                               const ::google::protobuf::MessagePtr& response) {
  GOOGLE_DCHECK_EQ(method->service(), G2C_SceneService_descriptor_);
  switch(method->index()) {
    case 0:
      G2C_EnterScene_DoneCb(::google::protobuf::down_pointer_cast<::CMD::G2C_EnterSceneArg>(request),
                    ::google::protobuf::down_pointer_cast<::CMD::G2C_EnterSceneRes>(response));
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

void G2C_SceneService::TimeOut(const ::google::protobuf::MethodDescriptor* method,
                          const ::google::protobuf::MessagePtr& request) {
  GOOGLE_DCHECK_EQ(method->service(), G2C_SceneService_descriptor_);
  switch(method->index()) {
    case 0:
      G2C_EnterScene_TimeOut(::google::protobuf::down_pointer_cast<::CMD::G2C_EnterSceneArg>(request));
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::google::protobuf::Message& G2C_SceneService::GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::CMD::G2C_EnterSceneArg::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *reinterpret_cast< ::google::protobuf::Message*>(NULL);
  }
}

const ::google::protobuf::Message& G2C_SceneService::GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::CMD::G2C_EnterSceneRes::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *reinterpret_cast< ::google::protobuf::Message*>(NULL);
  }
}

G2C_SceneService_Stub::G2C_SceneService_Stub(::muduo::net::RpcChannelBase* channel__)
    : channel_(channel__), owns_channel_(false) 
{
}

G2C_SceneService_Stub::~G2C_SceneService_Stub() 
{
}

void G2C_SceneService_Stub::G2C_EnterScene(const ::CMD::G2C_EnterSceneArgPtr& request,
                              const ::CMD::G2C_EnterSceneResPtr& response) 
{
  channel_->CallMethod(descriptor()->method(0),
                       request, 
                       response);
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace CMD
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::CMD::G2C_EnterSceneArg* Arena::CreateMaybeMessage< ::CMD::G2C_EnterSceneArg >(Arena* arena) {
  return Arena::CreateMessageInternal< ::CMD::G2C_EnterSceneArg >(arena);
}
template<> PROTOBUF_NOINLINE ::CMD::G2C_EnterSceneRes* Arena::CreateMaybeMessage< ::CMD::G2C_EnterSceneRes >(Arena* arena) {
  return Arena::CreateMessageInternal< ::CMD::G2C_EnterSceneRes >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>