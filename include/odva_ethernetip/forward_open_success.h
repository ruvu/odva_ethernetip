/**
Software License Agreement (BSD)

\file      forward_open_success.h
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

#ifndef ODVA_ETHERNETIP_FORWARD_OPEN_SUCCESS_H
#define ODVA_ETHERNETIP_FORWARD_OPEN_SUCCESS_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "odva_ethernetip/eip_types.h"
#include "odva_ethernetip/serialization/serializable.h"
#include "odva_ethernetip/serialization/serializable_buffer.h"
#include "odva_ethernetip/serialization/reader.h"
#include "odva_ethernetip/serialization/writer.h"

using boost::shared_ptr;
using boost::make_shared;

namespace eip {

using serialization::Serializable;
using serialization::SerializableBuffer;
using serialization::Reader;
using serialization::Writer;

/**
 * Class to encapsulate the response to a Forward Open request when successful
 * Note that the response is different if the request fails.
 */
class ForwardOpenSuccess : public Serializable
{
public:

  EIP_UDINT o_to_t_connection_id;
  EIP_UDINT t_to_o_connection_id;
  EIP_UINT connection_sn;
  EIP_UINT originator_vendor_id;
  EIP_UDINT originator_sn;
  EIP_UDINT o_to_t_api;
  EIP_UDINT t_to_o_api;

  /**
   * Get the response data object in this message
   * @return Serializable data to be used for this response
   */
  shared_ptr<Serializable> getResponseData() const
  {
    return response_data_;
  }

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    size_t result = sizeof(o_to_t_connection_id)
      + sizeof(t_to_o_connection_id)
      + sizeof(connection_sn)
      + sizeof(originator_vendor_id)
      + sizeof(originator_sn)
      + sizeof(o_to_t_api)
      + sizeof(t_to_o_api)
      + 2; // reserved byte and data length
    if (response_data_)
    {
      result += response_data_->getLength();
    }
    return result;
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    EIP_BYTE reserved = 0;
    writer.write(o_to_t_connection_id);
    writer.write(t_to_o_connection_id);
    writer.write(connection_sn);
    writer.write(originator_vendor_id);
    writer.write(originator_sn);
    writer.write(o_to_t_api);
    writer.write(t_to_o_api);
    if (response_data_)
    {
        writer.write(response_data_->getLength());
        writer.write(reserved);
        response_data_->serialize(writer);
    } else
    {
        writer.write(reserved);
        writer.write(reserved);
    }
    return writer;
  }

  /**
   * Don't need extra length info, as the message data defines its length
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    return deserialize(reader);
  }

  /**
   * Deserialize the forward open response data. Length is implicit.
   */
  virtual Reader& deserialize(Reader& reader)
  {
    EIP_USINT data_length;
    reader.read(o_to_t_connection_id);
    reader.read(t_to_o_connection_id);
    reader.read(connection_sn);
    reader.read(originator_vendor_id);
    reader.read(originator_sn);
    reader.read(o_to_t_api);
    reader.read(t_to_o_api);
    reader.read(data_length);
    reader.skip(1);
    if (data_length > 0)
    {
      shared_ptr<SerializableBuffer> sb = make_shared<SerializableBuffer> ();
      sb->deserialize(reader, data_length*2);
      response_data_ = sb;
    }
    else
    {
      response_data_ = shared_ptr<Serializable>();
    }
    return reader;
  }

private:
  shared_ptr<Serializable> response_data_;

};

} // namespace eip

#endif  // ODVA_ETHERNETIP_FORWARD_OPEN_SUCCESS_H
