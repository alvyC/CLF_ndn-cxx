/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_CXX_LOCATION_HPP
#define NDN_CXX_LOCATION_HPP

#include "ndn-cxx/security/v2/key-chain.hpp"
namespace ndn {

/** \brief A prefix announcement object that represents an application's intent of registering a
 *         prefix toward itself.
 *  \sa https://redmine.named-data.net/projects/nfd/wiki/PrefixAnnouncement
 */
namespace tlv {
namespace location {
  enum {
    Double = 253,
    Longitude = 254,
    Latitude = 255,
    MyLocation = 256,
    PrevLocation = 257,
    DestLocation = 258
  };

  inline double
  readDouble(const ndn::Block& block)
  {
    block.parse();
    auto it = block.elements_begin();

    double doubleFromBlock = 0.0;
    if (it == it->elements_end() || it->type() != Double ||
        it->value_size() != sizeof(doubleFromBlock)) {
      BOOST_THROW_EXCEPTION(ndn::tlv::Error("Block does not contain a double"));
    }
    memcpy(&doubleFromBlock, it->value(), sizeof(doubleFromBlock));
    return doubleFromBlock;
  }

  template<ndn::encoding::Tag TAG>
  inline size_t
  prependDouble(ndn::EncodingImpl<TAG>& encoder, uint32_t type, double value)
  {
    size_t totalLength = 0;

    const uint8_t* doubleBytes = reinterpret_cast<const uint8_t*>(&value);
    totalLength = encoder.prependByteArrayBlock(Double, doubleBytes, 8);
    totalLength += encoder.prependVarNumber(totalLength);
    totalLength += encoder.prependVarNumber(type);

    return totalLength;
  }
}
}
class Location 
{
public:
  class Error : public tlv::Error
  {
  public:
    using tlv::Error::Error;
  };

  Location();

  Location(double, double);
  
  double
  getLongitude() const {
    return m_longitude;
  }

  void
  setLongitude(const double lg) {
    m_longitude = lg;
  }

  double
  getLatitude() const {
    return m_latitude;
  }

  void
  setLaitude(const double lt) {
    m_latitude = lt;
  }

//private:
  double m_longitude;
  double m_latitude;
};

} // namespace ndn

#endif // NDN_CXX_LOCATION_HPP
