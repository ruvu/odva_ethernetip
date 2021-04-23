/**
Software License Agreement (BSD)

\file      encap_packet.cpp
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

#include <vector>
#include <boost/make_shared.hpp>

#include "odva_ethernetip/encap_packet.h"
#include "odva_ethernetip/serialization/serializable_buffer.h"
#include "odva_ethernetip/serialization/buffer_reader.h"
#include "odva_ethernetip/serialization/buffer_writer.h"
#include "odva_ethernetip/serialization/copy_serializable.h"

using std::vector;
using boost::make_shared;

namespace eip {

using serialization::SerializableBuffer;
using serialization::BufferReader;
using serialization::BufferWriter;

void EncapPacket::getPayloadAs(Serializable& result) const
{
  serialization::copy_serializable(result, *payload_);
}

Writer& EncapPacket::serialize(Writer& writer) const
{
  // fixme: need to set length before this!
  // header_.length = (NULL == payload_) ? 0 : payload_->getLength();
  header_.serialize(writer);
  if (payload_)
  {
    payload_->serialize(writer);
  }
  return writer;
}

Reader& EncapPacket::deserialize(Reader& reader)
{
  header_.deserialize(reader);
  if (header_.length > 0)
  {
    shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer>();
    sb->deserialize(reader, header_.length);
    payload_ = sb;
  }
  else
  {
    payload_ = shared_ptr<Serializable>();
  }
  return reader;
}

Reader& EncapPacket::deserialize(Reader& reader, size_t length)
{
  if (length < header_.getLength())
  {
    throw std::length_error("Deserialization size less than header size");
  }
  header_.deserialize(reader);
  if (length != header_.getLength() + header_.length)
  {
    throw std::length_error("Packet length different than given to deserialize");
  }
  if (header_.length > 0)
  {
    payload_ = make_shared<SerializableBuffer>();
    payload_->deserialize(reader, header_.length);
  }
  else
  {
    payload_ = shared_ptr<Serializable>();
  }
  return reader;
}

} // namespace eip
