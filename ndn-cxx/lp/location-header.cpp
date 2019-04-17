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

#include "ndn-cxx/lp/location-header.hpp"
#include "ndn-cxx/lp/tlv.hpp"
#include "ndn-cxx/encoding/block-helpers.hpp"

#include <iostream>

namespace ndn {
namespace lp {

LocationHeader::LocationHeader() = default;

LocationHeader::LocationHeader(const Block& block)
{
  wireDecode(block);  
}

template<encoding::Tag TAG>
size_t
LocationHeader::wireEncode(EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;
  
  totalLength += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Longitude, m_myLocation.m_longitude); 
  /*const uint8_t* longitudeBytes = reinterpret_cast<const uint8_t*>(&m_myLocation.m_longitude);
  totalLength += encoder.prependByteArrayBlock(ndn::tlv::location::Double, longitudeBytes, 8);
  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(ndn::tlv::location::Longitude);

  const uint8_t* latitudeBytes = reinterpret_cast<const uint8_t*>(&m_myLocation.m_latitude);
  totalLength += encoder.prependByteArrayBlock(ndn::tlv::location::Double, latitudeBytes, 8);
  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(ndn::tlv::location::Latitude);*/

  totalLength += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Latitude, m_myLocation.m_latitude); 

  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(ndn::tlv::location::MyLocation);

  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::Location);
  return totalLength;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(LocationHeader);

void
LocationHeader::wireDecode(const Block& wire)
{
  if (wire.type() != tlv::Location) {
    std::stringstream error;
    error << "Expected MyLocaton Block, but Block is of a different type: #"
          << wire.type();
    BOOST_THROW_EXCEPTION(Error(error.str()));
  }
  std::cout << "tlv::Location type: " << wire.type() << std::endl; 
  wire.parse();
  
  ndn::Block::element_const_iterator val = wire.elements_begin();
  
  // Location
  if (val != wire.elements_end() && val->type() == ndn::tlv::location::MyLocation) {
    std::cout << "tlv::MyLocation type: " << val->type() << std::endl; 
    val->parse();
    ndn::Block::element_const_iterator ival = val->elements_begin(); 
    
    // Latitude 
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Latitude) {
      //m_myLocation.m_latitude = ndn::encoding::readDouble(*val);
      //m_myLocation.m_latitude = ndn::tlv::location::readDouble(*val);
      std::cout << "tlv::Latitude type: " << ival->type() << std::endl;
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_myLocation.m_latitude = *reinterpret_cast<const double*>(it->value());

        ++ival;
      }
      else {
        std::cout << "Missing required location::Latitude::Double field." << std::endl;
        BOOST_THROW_EXCEPTION(Error("Missing required MyLocation::Latitude field"));
        //++ival;
      }
    }
    else {
      std::cout << "Missing required Mylocation::Latitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required MyLocation::Latitude field"));
    }
    
    // Longitude
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Longitude) {
      std::cout << "tlv::Longitude type: " << ival->type() << std::endl;
      //m_myLocation.m_longitude = ndn::encoding::readDouble(*val);
      //m_myLocation.m_longitude = ndn::tlv::location::readDouble(*val);
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_myLocation.m_longitude = *reinterpret_cast<const double*>(it->value());

        ++ival;
      }
      else {
        BOOST_THROW_EXCEPTION(Error("Missing required MyLocation::Longitude::Double field"));
      }
    }
    else {
      std::cout << "Missing required Mylocation::Longitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required MyLocation::Longitude field"));
    }

    ++val; // next field: prev location
  }
  else {
    std::cout << "Missing required location::MyLocation field" << std::endl;
    BOOST_THROW_EXCEPTION(Error("Missing required location::MyLocation field"));
  }
 
}
} // namespace lp
} // namespace ndn
