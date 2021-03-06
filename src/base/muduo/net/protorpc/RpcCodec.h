// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#pragma once

#include "muduo/base/common/time_stamp.h"
#include "muduo/net/protobuf/ProtobufCodecLite.h"

namespace muduo
{
namespace net
{

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

extern const char rpctag[5];// = "RPC0";

// wire format
//
// Field     Length  Content
//
// size      4-byte  N+8
// "RPC0"    4-byte
// payload   N-byte
// checksum  4-byte  adler32 of "RPC0"+payload
//


}  // namespace net
}  // namespace muduo


namespace CMD {

class RpcMessage;
typedef std::shared_ptr<RpcMessage> RpcMessagePtr;

}   // namespace CMD


typedef muduo::net::ProtobufCodecLiteT<CMD::RpcMessage, muduo::net::rpctag> RpcCodec;
