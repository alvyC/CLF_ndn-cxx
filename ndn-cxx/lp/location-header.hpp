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

#ifndef NDN_CXX_LOCATION_HEADER_HPP
#define NDN_CXX_LOCATION_HEADER_HPP

#include "ndn-cxx/location.hpp"

namespace ndn {
namespace lp {

/** \brief represents a PrefixAnnouncement header field in NDNLP
 */
class LocationHeader
{
public:
  class Error : public ndn::tlv::Error
  {
  public:
    using ndn::tlv::Error::Error;
  };

  LocationHeader();
  
  LocationHeader(Location ml, Location pl, Location dl);
  
  explicit
  LocationHeader(const Block& block);

  Location
  getMyLocation() const {
    return m_myLocation;
  }

  void
  setMyLocation(Location ml) {
    m_myLocation = ml;
  }
  
  Location
  getPrevLocation() const {
    return m_prevLocation;
  }

  void
  setPrevLocation(Location pl) {
    m_prevLocation = pl;
  }
  
  Location
  getDestLocation() const {
    return m_destLocation;
  }

  void
  setDestLocation(Location dl) {
    m_destLocation = dl;
  }

  template<encoding::Tag TAG>
  size_t
  wireEncode(EncodingImpl<TAG>& encoder) const;

  void
  wireDecode(const Block& wire);

private:
  //todo: make  it optional
  //mutable optional<Location> m_myLocation;
  Location m_myLocation;
  Location m_prevLocation;
  Location m_destLocation;
};

NDN_CXX_DECLARE_WIRE_ENCODE_INSTANTIATIONS(LocationHeader);

} // namespace lp
} // namespace ndn

#endif // NDN_CXX_LOCATION_HEADER_HPP
