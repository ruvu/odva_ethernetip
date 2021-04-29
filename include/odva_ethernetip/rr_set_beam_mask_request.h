#ifndef ODVA_ETHERNETIP_RR_SET_BEAM_MASK_REQUEST_H
#define ODVA_ETHERNETIP_RR_SET_BEAM_MASK_REQUEST_H

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
using std::vector;

namespace eip {

using serialization::Serializable;
using serialization::SerializableBuffer;
using serialization::Reader;
using serialization::Writer;


class RRSetBeamMaskRequest : public Serializable
{
public:
  vector<EIP_UINT> beam_mask;

  /**
   * Get the length of serialized data that would be produced if serialized
   * @return Total length in bytes to be serialized
   */
  virtual size_t getLength() const
  {
    return beam_mask.size() * sizeof(EIP_UINT);
  }

  /**
   * Serialize data into the given buffer
   * @param writer Writer to use for serialization
   * @return the writer again
   * @throw std::length_error if the buffer is too small for the header data
   */
  virtual Writer& serialize(Writer& writer) const
  {
    throw std::logic_error("Not implemented");
  }

  /**
   * Don't need extra length info, as the message data defines its length
   */
  virtual Reader& deserialize(Reader& reader, size_t length)
  {
    return deserialize(reader);
  }

  /**
   * Deserialize the set bem mask request data. Length is implicit.
   */
  virtual Reader& deserialize(Reader& reader)
  {
    beam_mask.resize(44);
    reader.readBytes(&beam_mask[0], beam_mask.size() * sizeof(EIP_UINT));
    return reader;
  }


};

} // namespace eip

#endif  // ODVA_ETHERNETIP_RR_SET_BEAM_MASK_REQUEST_H
