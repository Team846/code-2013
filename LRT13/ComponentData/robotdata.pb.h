// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: robotdata.proto

#ifndef PROTOBUF_robotdata_2eproto__INCLUDED
#define PROTOBUF_robotdata_2eproto__INCLUDED

#include <string>

#include "../Network/protobuf/stubs/common.h"

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include "../Network/protobuf/generated_message_util.h"
#include "../Network/protobuf/repeated_field.h"
#include "../Network/protobuf/extension_set.h"

// @@protoc_insertion_point(includes)

namespace data {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_robotdata_2eproto();
void protobuf_AssignDesc_robotdata_2eproto();
void protobuf_ShutdownFile_robotdata_2eproto();

class DataMessage;
class DataMessage_IndexToTypeMapMessage;
class DataMessage_IndexToValueMapMessage;
class CompleteMessage;

// ===================================================================

class DataMessage_IndexToTypeMapMessage : public ::google::protobuf::MessageLite {
 public:
  DataMessage_IndexToTypeMapMessage();
  virtual ~DataMessage_IndexToTypeMapMessage();
  
  DataMessage_IndexToTypeMapMessage(const DataMessage_IndexToTypeMapMessage& from);
  
  inline DataMessage_IndexToTypeMapMessage& operator=(const DataMessage_IndexToTypeMapMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const DataMessage_IndexToTypeMapMessage& default_instance();
  
  void Swap(DataMessage_IndexToTypeMapMessage* other);
  
  // implements Message ----------------------------------------------
  
  DataMessage_IndexToTypeMapMessage* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const DataMessage_IndexToTypeMapMessage& from);
  void MergeFrom(const DataMessage_IndexToTypeMapMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated string type = 1;
  inline int type_size() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline const ::std::string& type(int index) const;
  inline ::std::string* mutable_type(int index);
  inline void set_type(int index, const ::std::string& value);
  inline void set_type(int index, const char* value);
  inline void set_type(int index, const char* value, size_t size);
  inline ::std::string* add_type();
  inline void add_type(const ::std::string& value);
  inline void add_type(const char* value);
  inline void add_type(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& type() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_type();
  
  // @@protoc_insertion_point(class_scope:data.DataMessage.IndexToTypeMapMessage)
 private:
  
  ::google::protobuf::RepeatedPtrField< ::std::string> type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();
  
  void InitAsDefaultInstance();
  static DataMessage_IndexToTypeMapMessage* default_instance_;
};
// -------------------------------------------------------------------

class DataMessage_IndexToValueMapMessage : public ::google::protobuf::MessageLite {
 public:
  DataMessage_IndexToValueMapMessage();
  virtual ~DataMessage_IndexToValueMapMessage();
  
  DataMessage_IndexToValueMapMessage(const DataMessage_IndexToValueMapMessage& from);
  
  inline DataMessage_IndexToValueMapMessage& operator=(const DataMessage_IndexToValueMapMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const DataMessage_IndexToValueMapMessage& default_instance();
  
  void Swap(DataMessage_IndexToValueMapMessage* other);
  
  // implements Message ----------------------------------------------
  
  DataMessage_IndexToValueMapMessage* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const DataMessage_IndexToValueMapMessage& from);
  void MergeFrom(const DataMessage_IndexToValueMapMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated string value = 1;
  inline int value_size() const;
  inline void clear_value();
  static const int kValueFieldNumber = 1;
  inline const ::std::string& value(int index) const;
  inline ::std::string* mutable_value(int index);
  inline void set_value(int index, const ::std::string& value);
  inline void set_value(int index, const char* value);
  inline void set_value(int index, const char* value, size_t size);
  inline ::std::string* add_value();
  inline void add_value(const ::std::string& value);
  inline void add_value(const char* value);
  inline void add_value(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& value() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_value();
  
  // @@protoc_insertion_point(class_scope:data.DataMessage.IndexToValueMapMessage)
 private:
  
  ::google::protobuf::RepeatedPtrField< ::std::string> value_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();
  
  void InitAsDefaultInstance();
  static DataMessage_IndexToValueMapMessage* default_instance_;
};
// -------------------------------------------------------------------

class DataMessage : public ::google::protobuf::MessageLite {
 public:
  DataMessage();
  virtual ~DataMessage();
  
  DataMessage(const DataMessage& from);
  
  inline DataMessage& operator=(const DataMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const DataMessage& default_instance();
  
  void Swap(DataMessage* other);
  
  // implements Message ----------------------------------------------
  
  DataMessage* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const DataMessage& from);
  void MergeFrom(const DataMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  typedef DataMessage_IndexToTypeMapMessage IndexToTypeMapMessage;
  typedef DataMessage_IndexToValueMapMessage IndexToValueMapMessage;
  
  // accessors -------------------------------------------------------
  
  // required string ClassName = 1;
  inline bool has_classname() const;
  inline void clear_classname();
  static const int kClassNameFieldNumber = 1;
  inline const ::std::string& classname() const;
  inline void set_classname(const ::std::string& value);
  inline void set_classname(const char* value);
  inline void set_classname(const char* value, size_t size);
  inline ::std::string* mutable_classname();
  inline ::std::string* release_classname();
  
  // required .data.DataMessage.IndexToTypeMapMessage IndexToTypeMap = 2;
  inline bool has_indextotypemap() const;
  inline void clear_indextotypemap();
  static const int kIndexToTypeMapFieldNumber = 2;
  inline const ::data::DataMessage_IndexToTypeMapMessage& indextotypemap() const;
  inline ::data::DataMessage_IndexToTypeMapMessage* mutable_indextotypemap();
  inline ::data::DataMessage_IndexToTypeMapMessage* release_indextotypemap();
  
  // required .data.DataMessage.IndexToValueMapMessage IndexToValueMap = 3;
  inline bool has_indextovaluemap() const;
  inline void clear_indextovaluemap();
  static const int kIndexToValueMapFieldNumber = 3;
  inline const ::data::DataMessage_IndexToValueMapMessage& indextovaluemap() const;
  inline ::data::DataMessage_IndexToValueMapMessage* mutable_indextovaluemap();
  inline ::data::DataMessage_IndexToValueMapMessage* release_indextovaluemap();
  
  // @@protoc_insertion_point(class_scope:data.DataMessage)
 private:
  inline void set_has_classname();
  inline void clear_has_classname();
  inline void set_has_indextotypemap();
  inline void clear_has_indextotypemap();
  inline void set_has_indextovaluemap();
  inline void clear_has_indextovaluemap();
  
  ::std::string* classname_;
  ::data::DataMessage_IndexToTypeMapMessage* indextotypemap_;
  ::data::DataMessage_IndexToValueMapMessage* indextovaluemap_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();
  
  void InitAsDefaultInstance();
  static DataMessage* default_instance_;
};
// -------------------------------------------------------------------

class CompleteMessage : public ::google::protobuf::MessageLite {
 public:
  CompleteMessage();
  virtual ~CompleteMessage();
  
  CompleteMessage(const CompleteMessage& from);
  
  inline CompleteMessage& operator=(const CompleteMessage& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const CompleteMessage& default_instance();
  
  void Swap(CompleteMessage* other);
  
  // implements Message ----------------------------------------------
  
  CompleteMessage* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const CompleteMessage& from);
  void MergeFrom(const CompleteMessage& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated .data.DataMessage Data = 1;
  inline int data_size() const;
  inline void clear_data();
  static const int kDataFieldNumber = 1;
  inline const ::data::DataMessage& data(int index) const;
  inline ::data::DataMessage* mutable_data(int index);
  inline ::data::DataMessage* add_data();
  inline const ::google::protobuf::RepeatedPtrField< ::data::DataMessage >&
      data() const;
  inline ::google::protobuf::RepeatedPtrField< ::data::DataMessage >*
      mutable_data();
  
  // @@protoc_insertion_point(class_scope:data.CompleteMessage)
 private:
  
  ::google::protobuf::RepeatedPtrField< ::data::DataMessage > data_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();
  
  void InitAsDefaultInstance();
  static CompleteMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// DataMessage_IndexToTypeMapMessage

// repeated string type = 1;
inline int DataMessage_IndexToTypeMapMessage::type_size() const {
  return type_.size();
}
inline void DataMessage_IndexToTypeMapMessage::clear_type() {
  type_.Clear();
}
inline const ::std::string& DataMessage_IndexToTypeMapMessage::type(int index) const {
  return type_.Get(index);
}
inline ::std::string* DataMessage_IndexToTypeMapMessage::mutable_type(int index) {
  return type_.Mutable(index);
}
inline void DataMessage_IndexToTypeMapMessage::set_type(int index, const ::std::string& value) {
  type_.Mutable(index)->assign(value);
}
inline void DataMessage_IndexToTypeMapMessage::set_type(int index, const char* value) {
  type_.Mutable(index)->assign(value);
}
inline void DataMessage_IndexToTypeMapMessage::set_type(int index, const char* value, size_t size) {
  type_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DataMessage_IndexToTypeMapMessage::add_type() {
  return type_.Add();
}
inline void DataMessage_IndexToTypeMapMessage::add_type(const ::std::string& value) {
  type_.Add()->assign(value);
}
inline void DataMessage_IndexToTypeMapMessage::add_type(const char* value) {
  type_.Add()->assign(value);
}
inline void DataMessage_IndexToTypeMapMessage::add_type(const char* value, size_t size) {
  type_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
DataMessage_IndexToTypeMapMessage::type() const {
  return type_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
DataMessage_IndexToTypeMapMessage::mutable_type() {
  return &type_;
}

// -------------------------------------------------------------------

// DataMessage_IndexToValueMapMessage

// repeated string value = 1;
inline int DataMessage_IndexToValueMapMessage::value_size() const {
  return value_.size();
}
inline void DataMessage_IndexToValueMapMessage::clear_value() {
  value_.Clear();
}
inline const ::std::string& DataMessage_IndexToValueMapMessage::value(int index) const {
  return value_.Get(index);
}
inline ::std::string* DataMessage_IndexToValueMapMessage::mutable_value(int index) {
  return value_.Mutable(index);
}
inline void DataMessage_IndexToValueMapMessage::set_value(int index, const ::std::string& value) {
  value_.Mutable(index)->assign(value);
}
inline void DataMessage_IndexToValueMapMessage::set_value(int index, const char* value) {
  value_.Mutable(index)->assign(value);
}
inline void DataMessage_IndexToValueMapMessage::set_value(int index, const char* value, size_t size) {
  value_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DataMessage_IndexToValueMapMessage::add_value() {
  return value_.Add();
}
inline void DataMessage_IndexToValueMapMessage::add_value(const ::std::string& value) {
  value_.Add()->assign(value);
}
inline void DataMessage_IndexToValueMapMessage::add_value(const char* value) {
  value_.Add()->assign(value);
}
inline void DataMessage_IndexToValueMapMessage::add_value(const char* value, size_t size) {
  value_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
DataMessage_IndexToValueMapMessage::value() const {
  return value_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
DataMessage_IndexToValueMapMessage::mutable_value() {
  return &value_;
}

// -------------------------------------------------------------------

// DataMessage

// required string ClassName = 1;
inline bool DataMessage::has_classname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DataMessage::set_has_classname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DataMessage::clear_has_classname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DataMessage::clear_classname() {
  if (classname_ != &::google::protobuf::internal::kEmptyString) {
    classname_->clear();
  }
  clear_has_classname();
}
inline const ::std::string& DataMessage::classname() const {
  return *classname_;
}
inline void DataMessage::set_classname(const ::std::string& value) {
  set_has_classname();
  if (classname_ == &::google::protobuf::internal::kEmptyString) {
    classname_ = new ::std::string;
  }
  classname_->assign(value);
}
inline void DataMessage::set_classname(const char* value) {
  set_has_classname();
  if (classname_ == &::google::protobuf::internal::kEmptyString) {
    classname_ = new ::std::string;
  }
  classname_->assign(value);
}
inline void DataMessage::set_classname(const char* value, size_t size) {
  set_has_classname();
  if (classname_ == &::google::protobuf::internal::kEmptyString) {
    classname_ = new ::std::string;
  }
  classname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DataMessage::mutable_classname() {
  set_has_classname();
  if (classname_ == &::google::protobuf::internal::kEmptyString) {
    classname_ = new ::std::string;
  }
  return classname_;
}
inline ::std::string* DataMessage::release_classname() {
  clear_has_classname();
  if (classname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = classname_;
    classname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required .data.DataMessage.IndexToTypeMapMessage IndexToTypeMap = 2;
inline bool DataMessage::has_indextotypemap() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DataMessage::set_has_indextotypemap() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DataMessage::clear_has_indextotypemap() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DataMessage::clear_indextotypemap() {
  if (indextotypemap_ != NULL) indextotypemap_->::data::DataMessage_IndexToTypeMapMessage::Clear();
  clear_has_indextotypemap();
}
inline const ::data::DataMessage_IndexToTypeMapMessage& DataMessage::indextotypemap() const {
  return indextotypemap_ != NULL ? *indextotypemap_ : *default_instance_->indextotypemap_;
}
inline ::data::DataMessage_IndexToTypeMapMessage* DataMessage::mutable_indextotypemap() {
  set_has_indextotypemap();
  if (indextotypemap_ == NULL) indextotypemap_ = new ::data::DataMessage_IndexToTypeMapMessage;
  return indextotypemap_;
}
inline ::data::DataMessage_IndexToTypeMapMessage* DataMessage::release_indextotypemap() {
  clear_has_indextotypemap();
  ::data::DataMessage_IndexToTypeMapMessage* temp = indextotypemap_;
  indextotypemap_ = NULL;
  return temp;
}

// required .data.DataMessage.IndexToValueMapMessage IndexToValueMap = 3;
inline bool DataMessage::has_indextovaluemap() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void DataMessage::set_has_indextovaluemap() {
  _has_bits_[0] |= 0x00000004u;
}
inline void DataMessage::clear_has_indextovaluemap() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void DataMessage::clear_indextovaluemap() {
  if (indextovaluemap_ != NULL) indextovaluemap_->::data::DataMessage_IndexToValueMapMessage::Clear();
  clear_has_indextovaluemap();
}
inline const ::data::DataMessage_IndexToValueMapMessage& DataMessage::indextovaluemap() const {
  return indextovaluemap_ != NULL ? *indextovaluemap_ : *default_instance_->indextovaluemap_;
}
inline ::data::DataMessage_IndexToValueMapMessage* DataMessage::mutable_indextovaluemap() {
  set_has_indextovaluemap();
  if (indextovaluemap_ == NULL) indextovaluemap_ = new ::data::DataMessage_IndexToValueMapMessage;
  return indextovaluemap_;
}
inline ::data::DataMessage_IndexToValueMapMessage* DataMessage::release_indextovaluemap() {
  clear_has_indextovaluemap();
  ::data::DataMessage_IndexToValueMapMessage* temp = indextovaluemap_;
  indextovaluemap_ = NULL;
  return temp;
}

// -------------------------------------------------------------------

// CompleteMessage

// repeated .data.DataMessage Data = 1;
inline int CompleteMessage::data_size() const {
  return data_.size();
}
inline void CompleteMessage::clear_data() {
  data_.Clear();
}
inline const ::data::DataMessage& CompleteMessage::data(int index) const {
  return data_.Get(index);
}
inline ::data::DataMessage* CompleteMessage::mutable_data(int index) {
  return data_.Mutable(index);
}
inline ::data::DataMessage* CompleteMessage::add_data() {
  return data_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::data::DataMessage >&
CompleteMessage::data() const {
  return data_;
}
inline ::google::protobuf::RepeatedPtrField< ::data::DataMessage >*
CompleteMessage::mutable_data() {
  return &data_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace data

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_robotdata_2eproto__INCLUDED