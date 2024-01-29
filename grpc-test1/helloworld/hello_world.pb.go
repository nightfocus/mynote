//声明proto的版本 只有 proto3 才支持 gRPC

// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.27.1
// 	protoc        v3.19.3
// source: hello_world.proto

// 指定当前proto文件属于helloworld包

package helloworld

import (
	_struct "github.com/golang/protobuf/ptypes/struct"
	_ "github.com/grpc-ecosystem/grpc-gateway/v2/protoc-gen-openapiv2/options"
	_ "google.golang.org/genproto/googleapis/api/annotations"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

// HelloRequest 入参
//
// | Syntax | Description |
// | ----------- | ----------- |
// | Header | Title |
// | Paragraph | Text |
type HelloRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// 用户上传的name参数
	//
	// 这个name可以是a,b,c
	Name   string             `protobuf:"bytes,1,opt,name=name,proto3" json:"name,omitempty"`
	Z      int32              `protobuf:"varint,2,opt,name=z,proto3" json:"z,omitempty"`
	Arrays *_struct.ListValue `protobuf:"bytes,4,opt,name=arrays,proto3,oneof" json:"arrays,omitempty"`
}

func (x *HelloRequest) Reset() {
	*x = HelloRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_hello_world_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *HelloRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*HelloRequest) ProtoMessage() {}

func (x *HelloRequest) ProtoReflect() protoreflect.Message {
	mi := &file_hello_world_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use HelloRequest.ProtoReflect.Descriptor instead.
func (*HelloRequest) Descriptor() ([]byte, []int) {
	return file_hello_world_proto_rawDescGZIP(), []int{0}
}

func (x *HelloRequest) GetName() string {
	if x != nil {
		return x.Name
	}
	return ""
}

func (x *HelloRequest) GetZ() int32 {
	if x != nil {
		return x.Z
	}
	return 0
}

func (x *HelloRequest) GetArrays() *_struct.ListValue {
	if x != nil {
		return x.Arrays
	}
	return nil
}

// HelloReply
//
// - [x] Write the press release
// - [ ] Update the website
// - [ ] Contact the media
type HelloReply struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// 这是返回描述
	//
	// 这是返回的应答参数dddddrfwefe
	Message   string `protobuf:"bytes,1,opt,name=message,proto3" json:"message,omitempty"`
	VoPeeeMdd string `protobuf:"bytes,2,opt,name=voPeeeMdd,proto3" json:"voPeeeMdd,omitempty"`
	// 这是嵌套的struct，它无法在swagger显示。
	Core   *HelloReplyCore    `protobuf:"bytes,3,opt,name=core,proto3" json:"core,omitempty"`
	Arrays *_struct.ListValue `protobuf:"bytes,4,opt,name=arrays,proto3,oneof" json:"arrays,omitempty"`
}

func (x *HelloReply) Reset() {
	*x = HelloReply{}
	if protoimpl.UnsafeEnabled {
		mi := &file_hello_world_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *HelloReply) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*HelloReply) ProtoMessage() {}

func (x *HelloReply) ProtoReflect() protoreflect.Message {
	mi := &file_hello_world_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use HelloReply.ProtoReflect.Descriptor instead.
func (*HelloReply) Descriptor() ([]byte, []int) {
	return file_hello_world_proto_rawDescGZIP(), []int{1}
}

func (x *HelloReply) GetMessage() string {
	if x != nil {
		return x.Message
	}
	return ""
}

func (x *HelloReply) GetVoPeeeMdd() string {
	if x != nil {
		return x.VoPeeeMdd
	}
	return ""
}

func (x *HelloReply) GetCore() *HelloReplyCore {
	if x != nil {
		return x.Core
	}
	return nil
}

func (x *HelloReply) GetArrays() *_struct.ListValue {
	if x != nil {
		return x.Arrays
	}
	return nil
}

// HelloReplyCore
// 描述它
type HelloReplyCore struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// 这是core内部的a
	A string `protobuf:"bytes,1,opt,name=a,proto3" json:"a,omitempty"`
}

func (x *HelloReplyCore) Reset() {
	*x = HelloReplyCore{}
	if protoimpl.UnsafeEnabled {
		mi := &file_hello_world_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *HelloReplyCore) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*HelloReplyCore) ProtoMessage() {}

func (x *HelloReplyCore) ProtoReflect() protoreflect.Message {
	mi := &file_hello_world_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use HelloReplyCore.ProtoReflect.Descriptor instead.
func (*HelloReplyCore) Descriptor() ([]byte, []int) {
	return file_hello_world_proto_rawDescGZIP(), []int{2}
}

func (x *HelloReplyCore) GetA() string {
	if x != nil {
		return x.A
	}
	return ""
}

var File_hello_world_proto protoreflect.FileDescriptor

var file_hello_world_proto_rawDesc = []byte{
	0x0a, 0x11, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x5f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x2e, 0x70, 0x72,
	0x6f, 0x74, 0x6f, 0x12, 0x0a, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x1a,
	0x1c, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2f, 0x61, 0x70, 0x69, 0x2f, 0x61, 0x6e, 0x6e, 0x6f,
	0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x1c, 0x67,
	0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2f, 0x73,
	0x74, 0x72, 0x75, 0x63, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x2e, 0x70, 0x72, 0x6f,
	0x74, 0x6f, 0x63, 0x2d, 0x67, 0x65, 0x6e, 0x2d, 0x6f, 0x70, 0x65, 0x6e, 0x61, 0x70, 0x69, 0x76,
	0x32, 0x2f, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2f, 0x61, 0x6e, 0x6e, 0x6f, 0x74, 0x61,
	0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x22, 0x74, 0x0a, 0x0c, 0x48,
	0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12, 0x12, 0x0a, 0x04, 0x6e,
	0x61, 0x6d, 0x65, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x04, 0x6e, 0x61, 0x6d, 0x65, 0x12,
	0x0c, 0x0a, 0x01, 0x7a, 0x18, 0x02, 0x20, 0x01, 0x28, 0x05, 0x52, 0x01, 0x7a, 0x12, 0x37, 0x0a,
	0x06, 0x61, 0x72, 0x72, 0x61, 0x79, 0x73, 0x18, 0x04, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x1a, 0x2e,
	0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2e,
	0x4c, 0x69, 0x73, 0x74, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x48, 0x00, 0x52, 0x06, 0x61, 0x72, 0x72,
	0x61, 0x79, 0x73, 0x88, 0x01, 0x01, 0x42, 0x09, 0x0a, 0x07, 0x5f, 0x61, 0x72, 0x72, 0x61, 0x79,
	0x73, 0x22, 0xb8, 0x01, 0x0a, 0x0a, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x70, 0x6c, 0x79,
	0x12, 0x18, 0x0a, 0x07, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x18, 0x01, 0x20, 0x01, 0x28,
	0x09, 0x52, 0x07, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x12, 0x1c, 0x0a, 0x09, 0x76, 0x6f,
	0x50, 0x65, 0x65, 0x65, 0x4d, 0x64, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52, 0x09, 0x76,
	0x6f, 0x50, 0x65, 0x65, 0x65, 0x4d, 0x64, 0x64, 0x12, 0x2e, 0x0a, 0x04, 0x63, 0x6f, 0x72, 0x65,
	0x18, 0x03, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x1a, 0x2e, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77, 0x6f,
	0x72, 0x6c, 0x64, 0x2e, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x43, 0x6f,
	0x72, 0x65, 0x52, 0x04, 0x63, 0x6f, 0x72, 0x65, 0x12, 0x37, 0x0a, 0x06, 0x61, 0x72, 0x72, 0x61,
	0x79, 0x73, 0x18, 0x04, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x1a, 0x2e, 0x67, 0x6f, 0x6f, 0x67, 0x6c,
	0x65, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x62, 0x75, 0x66, 0x2e, 0x4c, 0x69, 0x73, 0x74, 0x56,
	0x61, 0x6c, 0x75, 0x65, 0x48, 0x00, 0x52, 0x06, 0x61, 0x72, 0x72, 0x61, 0x79, 0x73, 0x88, 0x01,
	0x01, 0x42, 0x09, 0x0a, 0x07, 0x5f, 0x61, 0x72, 0x72, 0x61, 0x79, 0x73, 0x22, 0x1e, 0x0a, 0x0e,
	0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x43, 0x6f, 0x72, 0x65, 0x12, 0x0c,
	0x0a, 0x01, 0x61, 0x18, 0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x01, 0x61, 0x32, 0xcc, 0x01, 0x0a,
	0x0a, 0x47, 0x72, 0x65, 0x65, 0x74, 0x65, 0x72, 0x61, 0x61, 0x61, 0x12, 0x5d, 0x0a, 0x08, 0x53,
	0x61, 0x79, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x12, 0x18, 0x2e, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77,
	0x6f, 0x72, 0x6c, 0x64, 0x2e, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73,
	0x74, 0x1a, 0x16, 0x2e, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x2e, 0x48,
	0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x22, 0x1f, 0x82, 0xd3, 0xe4, 0x93, 0x02,
	0x19, 0x22, 0x14, 0x2f, 0x76, 0x31, 0x2f, 0x67, 0x72, 0x65, 0x65, 0x74, 0x65, 0x72, 0x2f, 0x73,
	0x61, 0x79, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x3a, 0x01, 0x2a, 0x12, 0x5f, 0x0a, 0x09, 0x53, 0x61,
	0x79, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x32, 0x12, 0x18, 0x2e, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77,
	0x6f, 0x72, 0x6c, 0x64, 0x2e, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73,
	0x74, 0x1a, 0x16, 0x2e, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x2e, 0x48,
	0x65, 0x6c, 0x6c, 0x6f, 0x52, 0x65, 0x70, 0x6c, 0x79, 0x22, 0x20, 0x82, 0xd3, 0xe4, 0x93, 0x02,
	0x1a, 0x22, 0x15, 0x2f, 0x76, 0x31, 0x2f, 0x67, 0x72, 0x65, 0x65, 0x74, 0x65, 0x72, 0x2f, 0x73,
	0x61, 0x79, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x32, 0x3a, 0x01, 0x2a, 0x42, 0x91, 0x01, 0x5a, 0x18,
	0x70, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x31, 0x2f, 0x61, 0x62, 0x63, 0x64, 0x2f, 0x68, 0x65,
	0x6c, 0x6c, 0x6f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x92, 0x41, 0x74, 0x12, 0x4b, 0x0a, 0x15, 0x67,
	0x6f, 0x2d, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x2d, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x77,
	0x6f, 0x72, 0x6c, 0x64, 0x12, 0x2b, 0x67, 0x52, 0x50, 0x43, 0xe6, 0x9c, 0x8d, 0xe5, 0x8a, 0xa1,
	0xe7, 0x94, 0xa8, 0xe7, 0x9a, 0x84, 0x0a, 0x2a, 0x20, 0xe6, 0x8f, 0x8f, 0xe8, 0xbf, 0xb0, 0x31,
	0xe8, 0xa1, 0x8c, 0x0a, 0x2a, 0x20, 0xe6, 0x8f, 0x8f, 0xe8, 0xbf, 0xb0, 0x32, 0xe8, 0xa1, 0x8c,
	0x0a, 0x32, 0x05, 0x31, 0x2e, 0x30, 0x2e, 0x30, 0x2a, 0x01, 0x01, 0x32, 0x10, 0x61, 0x70, 0x70,
	0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x6a, 0x73, 0x6f, 0x6e, 0x3a, 0x10, 0x61,
	0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x6a, 0x73, 0x6f, 0x6e, 0x62,
	0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_hello_world_proto_rawDescOnce sync.Once
	file_hello_world_proto_rawDescData = file_hello_world_proto_rawDesc
)

func file_hello_world_proto_rawDescGZIP() []byte {
	file_hello_world_proto_rawDescOnce.Do(func() {
		file_hello_world_proto_rawDescData = protoimpl.X.CompressGZIP(file_hello_world_proto_rawDescData)
	})
	return file_hello_world_proto_rawDescData
}

var file_hello_world_proto_msgTypes = make([]protoimpl.MessageInfo, 3)
var file_hello_world_proto_goTypes = []interface{}{
	(*HelloRequest)(nil),      // 0: helloworld.HelloRequest
	(*HelloReply)(nil),        // 1: helloworld.HelloReply
	(*HelloReplyCore)(nil),    // 2: helloworld.HelloReplyCore
	(*_struct.ListValue)(nil), // 3: google.protobuf.ListValue
}
var file_hello_world_proto_depIdxs = []int32{
	3, // 0: helloworld.HelloRequest.arrays:type_name -> google.protobuf.ListValue
	2, // 1: helloworld.HelloReply.core:type_name -> helloworld.HelloReplyCore
	3, // 2: helloworld.HelloReply.arrays:type_name -> google.protobuf.ListValue
	0, // 3: helloworld.Greeteraaa.SayHello:input_type -> helloworld.HelloRequest
	0, // 4: helloworld.Greeteraaa.SayHello2:input_type -> helloworld.HelloRequest
	1, // 5: helloworld.Greeteraaa.SayHello:output_type -> helloworld.HelloReply
	1, // 6: helloworld.Greeteraaa.SayHello2:output_type -> helloworld.HelloReply
	5, // [5:7] is the sub-list for method output_type
	3, // [3:5] is the sub-list for method input_type
	3, // [3:3] is the sub-list for extension type_name
	3, // [3:3] is the sub-list for extension extendee
	0, // [0:3] is the sub-list for field type_name
}

func init() { file_hello_world_proto_init() }
func file_hello_world_proto_init() {
	if File_hello_world_proto != nil {
		return
	}
	if !protoimpl.UnsafeEnabled {
		file_hello_world_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*HelloRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_hello_world_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*HelloReply); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_hello_world_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*HelloReplyCore); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	file_hello_world_proto_msgTypes[0].OneofWrappers = []interface{}{}
	file_hello_world_proto_msgTypes[1].OneofWrappers = []interface{}{}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_hello_world_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   3,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_hello_world_proto_goTypes,
		DependencyIndexes: file_hello_world_proto_depIdxs,
		MessageInfos:      file_hello_world_proto_msgTypes,
	}.Build()
	File_hello_world_proto = out.File
	file_hello_world_proto_rawDesc = nil
	file_hello_world_proto_goTypes = nil
	file_hello_world_proto_depIdxs = nil
}