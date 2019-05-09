// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: contacts.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "contacts.pb.h"

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

namespace contact {

namespace {

const ::google::protobuf::Descriptor* contacts_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  contacts_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* contacts_Groups_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_contacts_2eproto() {
  protobuf_AddDesc_contacts_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "contacts.proto");
  GOOGLE_CHECK(file != NULL);
  contacts_descriptor_ = file->message_type(0);
  static const int contacts_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, number_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, groups_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, priority_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, email_),
  };
  contacts_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      contacts_descriptor_,
      contacts::default_instance_,
      contacts_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(contacts, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(contacts));
  contacts_Groups_descriptor_ = contacts_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_contacts_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    contacts_descriptor_, &contacts::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_contacts_2eproto() {
  delete contacts::default_instance_;
  delete contacts_reflection_;
  delete contacts::_default_name_;
  delete contacts::_default_number_;
}

void protobuf_AddDesc_contacts_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016contacts.proto\022\007contact\"\344\001\n\010contacts\022\026"
    "\n\004name\030\001 \002(\t:\010lishimin\022\033\n\006number\030\002 \001(\t:\013"
    "13030002000\022(\n\006groups\030\003 \003(\0162\030.contact.co"
    "ntacts.Groups\022\020\n\010priority\030\004 \003(\005\022\r\n\005email"
    "\030\005 \003(\t\"X\n\006Groups\022\n\n\006FAMILY\020\000\022\n\n\006FRIEND\020\001"
    "\022\r\n\tCLASSMATE\020\002\022\014\n\010WORKMATE\020\003\022\013\n\007COLLEGE"
    "\020\004\022\014\n\010STRANGER\020\005", 256);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "contacts.proto", &protobuf_RegisterTypes);
  contacts::_default_name_ =
      new ::std::string("lishimin", 8);
  contacts::_default_number_ =
      new ::std::string("13030002000", 11);
  contacts::default_instance_ = new contacts();
  contacts::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_contacts_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_contacts_2eproto {
  StaticDescriptorInitializer_contacts_2eproto() {
    protobuf_AddDesc_contacts_2eproto();
  }
} static_descriptor_initializer_contacts_2eproto_;

// ===================================================================

const ::google::protobuf::EnumDescriptor* contacts_Groups_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return contacts_Groups_descriptor_;
}
bool contacts_Groups_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const contacts_Groups contacts::FAMILY;
const contacts_Groups contacts::FRIEND;
const contacts_Groups contacts::CLASSMATE;
const contacts_Groups contacts::WORKMATE;
const contacts_Groups contacts::COLLEGE;
const contacts_Groups contacts::STRANGER;
const contacts_Groups contacts::Groups_MIN;
const contacts_Groups contacts::Groups_MAX;
const int contacts::Groups_ARRAYSIZE;
#endif  // _MSC_VER
::std::string* contacts::_default_name_ = NULL;
::std::string* contacts::_default_number_ = NULL;
#ifndef _MSC_VER
const int contacts::kNameFieldNumber;
const int contacts::kNumberFieldNumber;
const int contacts::kGroupsFieldNumber;
const int contacts::kPriorityFieldNumber;
const int contacts::kEmailFieldNumber;
#endif  // !_MSC_VER

contacts::contacts()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:contact.contacts)
}

void contacts::InitAsDefaultInstance() {
}

contacts::contacts(const contacts& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:contact.contacts)
}

void contacts::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  name_ = const_cast< ::std::string*>(_default_name_);
  number_ = const_cast< ::std::string*>(_default_number_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

contacts::~contacts() {
  // @@protoc_insertion_point(destructor:contact.contacts)
  SharedDtor();
}

void contacts::SharedDtor() {
  if (name_ != _default_name_) {
    delete name_;
  }
  if (number_ != _default_number_) {
    delete number_;
  }
  if (this != default_instance_) {
  }
}

void contacts::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* contacts::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return contacts_descriptor_;
}

const contacts& contacts::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_contacts_2eproto();
  return *default_instance_;
}

contacts* contacts::default_instance_ = NULL;

contacts* contacts::New() const {
  return new contacts;
}

void contacts::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    if (has_name()) {
      if (name_ != _default_name_) {
        name_->assign(*_default_name_);
      }
    }
    if (has_number()) {
      if (number_ != _default_number_) {
        number_->assign(*_default_number_);
      }
    }
  }
  groups_.Clear();
  priority_.Clear();
  email_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool contacts::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:contact.contacts)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string name = 1 [default = "lishimin"];
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "name");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_number;
        break;
      }

      // optional string number = 2 [default = "13030002000"];
      case 2: {
        if (tag == 18) {
         parse_number:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_number()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->number().data(), this->number().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "number");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_groups;
        break;
      }

      // repeated .contact.contacts.Groups groups = 3;
      case 3: {
        if (tag == 24) {
         parse_groups:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::contact::contacts_Groups_IsValid(value)) {
            add_groups(static_cast< ::contact::contacts_Groups >(value));
          } else {
            mutable_unknown_fields()->AddVarint(3, value);
          }
        } else if (tag == 26) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedEnumNoInline(
                 input,
                 &::contact::contacts_Groups_IsValid,
                 this->mutable_groups())));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_groups;
        if (input->ExpectTag(32)) goto parse_priority;
        break;
      }

      // repeated int32 priority = 4;
      case 4: {
        if (tag == 32) {
         parse_priority:
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 1, 32, input, this->mutable_priority())));
        } else if (tag == 34) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitiveNoInline<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, this->mutable_priority())));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_priority;
        if (input->ExpectTag(42)) goto parse_email;
        break;
      }

      // repeated string email = 5;
      case 5: {
        if (tag == 42) {
         parse_email:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_email()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->email(this->email_size() - 1).data(),
            this->email(this->email_size() - 1).length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "email");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(42)) goto parse_email;
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:contact.contacts)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:contact.contacts)
  return false;
#undef DO_
}

void contacts::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:contact.contacts)
  // required string name = 1 [default = "lishimin"];
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  // optional string number = 2 [default = "13030002000"];
  if (has_number()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->number().data(), this->number().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "number");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->number(), output);
  }

  // repeated .contact.contacts.Groups groups = 3;
  for (int i = 0; i < this->groups_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->groups(i), output);
  }

  // repeated int32 priority = 4;
  for (int i = 0; i < this->priority_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(
      4, this->priority(i), output);
  }

  // repeated string email = 5;
  for (int i = 0; i < this->email_size(); i++) {
  ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
    this->email(i).data(), this->email(i).length(),
    ::google::protobuf::internal::WireFormat::SERIALIZE,
    "email");
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->email(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:contact.contacts)
}

::google::protobuf::uint8* contacts::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:contact.contacts)
  // required string name = 1 [default = "lishimin"];
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  // optional string number = 2 [default = "13030002000"];
  if (has_number()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->number().data(), this->number().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "number");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->number(), target);
  }

  // repeated .contact.contacts.Groups groups = 3;
  for (int i = 0; i < this->groups_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      3, this->groups(i), target);
  }

  // repeated int32 priority = 4;
  for (int i = 0; i < this->priority_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteInt32ToArray(4, this->priority(i), target);
  }

  // repeated string email = 5;
  for (int i = 0; i < this->email_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->email(i).data(), this->email(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "email");
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(5, this->email(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:contact.contacts)
  return target;
}

int contacts::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string name = 1 [default = "lishimin"];
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // optional string number = 2 [default = "13030002000"];
    if (has_number()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->number());
    }

  }
  // repeated .contact.contacts.Groups groups = 3;
  {
    int data_size = 0;
    for (int i = 0; i < this->groups_size(); i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::EnumSize(
        this->groups(i));
    }
    total_size += 1 * this->groups_size() + data_size;
  }

  // repeated int32 priority = 4;
  {
    int data_size = 0;
    for (int i = 0; i < this->priority_size(); i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::
        Int32Size(this->priority(i));
    }
    total_size += 1 * this->priority_size() + data_size;
  }

  // repeated string email = 5;
  total_size += 1 * this->email_size();
  for (int i = 0; i < this->email_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->email(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void contacts::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const contacts* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const contacts*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void contacts::MergeFrom(const contacts& from) {
  GOOGLE_CHECK_NE(&from, this);
  groups_.MergeFrom(from.groups_);
  priority_.MergeFrom(from.priority_);
  email_.MergeFrom(from.email_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_name()) {
      set_name(from.name());
    }
    if (from.has_number()) {
      set_number(from.number());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void contacts::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void contacts::CopyFrom(const contacts& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool contacts::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void contacts::Swap(contacts* other) {
  if (other != this) {
    std::swap(name_, other->name_);
    std::swap(number_, other->number_);
    groups_.Swap(&other->groups_);
    priority_.Swap(&other->priority_);
    email_.Swap(&other->email_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata contacts::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = contacts_descriptor_;
  metadata.reflection = contacts_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace contact

// @@protoc_insertion_point(global_scope)
