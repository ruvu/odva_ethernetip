/**
Software License Agreement (BSD)

\file      eip_path.cpp
\authors   Kareem Shehata <kareem@shehata.ca>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "odva_ethernetip/path.h"

#include <boost/asio.hpp>

namespace eip {

Path::Path(bool pad_after_length) : pad_after_length_(pad_after_length)
{
  path_buf_.reserve(6);
}

Path::Path(EIP_USINT class_id, EIP_USINT instance_id, EIP_USINT attribute_id,
  bool pad_after_length) : pad_after_length_(pad_after_length)
{
  path_buf_.reserve(6);
  addLogicalClass(class_id);
  addLogicalInstance(instance_id);
  addLogicalAttribute(attribute_id);
}

Path::Path(EIP_USINT class_id, EIP_USINT instance_id) : pad_after_length_(false)
{
  path_buf_.reserve(4);
  addLogicalClass(class_id);
  addLogicalInstance(instance_id);
}

void Path::addSegment(EIP_USINT type, EIP_USINT data)
{
  path_buf_.push_back(type);
  path_buf_.push_back(data);
}

EIP_USINT Path::getSegment(EIP_USINT type)
{
  for (std::size_t i = 0; i < path_buf_.size(); i += 2)
    if (path_buf_[i] == type)
      return path_buf_[i + 1];
  return 0;
}

void Path::addLogicalClass(EIP_USINT class_id)
{
  addSegment(0x20, class_id);
}

void Path::addLogicalInstance(EIP_USINT instance_id)
{
  addSegment(0x24, instance_id);
}

void Path::addLogicalAttribute(EIP_USINT attribute_id)
{
  addSegment(0x30, attribute_id);
}

void Path::addLogicalConnectionPoint(EIP_USINT connection_id)
{
  addSegment(0x2C, connection_id);
}

EIP_USINT Path::getLogicalClass()
{
  return getSegment(0x20);
}

EIP_USINT Path::getLogicalInstance()
{
  return getSegment(0x24);
}

EIP_USINT Path::getLogicalAttribute()
{
  return getSegment(0x30);
}

EIP_USINT Path::getLogicalConnectionPoint()
{
  return getSegment(0x2C);
}

size_t Path::getLength() const
{
  return sizeof(EIP_USINT) + path_buf_.size() * sizeof(EIP_USINT) + (pad_after_length_ ? 1 : 0);
}

Writer& Path::serialize(Writer& writer, bool pad_after_length) const
{
  EIP_USINT length = path_buf_.size() / 2;
  writer.write(length);
  if (pad_after_length)
  {
    EIP_BYTE reserved = 0;
    writer.write(reserved);
  }
  writer.writeBuffer(boost::asio::buffer(path_buf_));
  return writer;
}

Reader& Path::deserialize(Reader &reader)
{
  EIP_USINT length;
  reader.read(length);
  path_buf_.reserve(length * 2);
  reader.readBuffer(boost::asio::buffer(path_buf_));
  return reader;
}

} // namespace eip
