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

LocationHeader::LocationHeader(Location ml, Location pl, Location dl)
  : m_myLocation(ml)
  , m_prevLocation(pl)
  , m_destLocation(dl)
  {
  } 

template<encoding::Tag TAG>
size_t
LocationHeader::wireEncode(EncodingImpl<TAG>& encoder) const
{
  //std::cout << "LocationHeader::wireEncode(EncodingImpl<TAG>& encoder)" << std::endl;
  size_t totalLength = 0;
  size_t length;   
  
  // DestLocation
  length = 0;
  length += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Longitude, m_destLocation.m_longitude); 
  
  length += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Latitude, m_destLocation.m_latitude); 

  length += encoder.prependVarNumber(length);
  length += encoder.prependVarNumber(ndn::tlv::location::DestLocation);
  
  // final length for Location type
  totalLength += length;
 
  // PrevLocation
  length = 0;
  length += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Longitude, m_prevLocation.m_longitude); 
  
  length += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Latitude, m_prevLocation.m_latitude); 

  length += encoder.prependVarNumber(length);
  length += encoder.prependVarNumber(ndn::tlv::location::PrevLocation);
  
  // final length for Location type
  totalLength += length;
    
  // MyLocation
  length = 0; 
  length += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Longitude, m_myLocation.m_longitude); 
  
  length += ndn::tlv::location::prependDouble(encoder, ndn::tlv::location::Latitude, m_myLocation.m_latitude); 

  length += encoder.prependVarNumber(length);
  length += encoder.prependVarNumber(ndn::tlv::location::MyLocation);

  // final length for Location type
  totalLength += length;

  // Location
  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::Location);
  return totalLength;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(LocationHeader);

void
LocationHeader::wireDecode(const Block& wire)
{
  //std::cout << "LocationHeader::wireDecode()" << std::endl;
  if (wire.type() != tlv::Location) {
    std::stringstream error;
    error << "Expected MyLocaton Block, but Block is of a different type: #"
          << wire.type();
    BOOST_THROW_EXCEPTION(Error(error.str()));
  }
  //std::cout << "tlv::Location type: " << wire.type() << std::endl; 
  wire.parse();
  
  ndn::Block::element_const_iterator val = wire.elements_begin();
  
  // My Location
  if (val != wire.elements_end() && val->type() == ndn::tlv::location::MyLocation) {
    //std::cout << "tlv::MyLocation type: " << val->type() << std::endl; 
    val->parse();
    ndn::Block::element_const_iterator ival = val->elements_begin(); 
    
    // Latitude 
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Latitude) {
      //m_myLocation.m_latitude = ndn::encoding::readDouble(*val);
      //m_myLocation.m_latitude = ndn::tlv::location::readDouble(*val);
      //std::cout << "tlv::Latitude type: " << ival->type() << std::endl;
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_myLocation.m_latitude = *reinterpret_cast<const double*>(it->value());
        ++ival;
      }
      else {
        //std::cout << "Missing required location::Latitude::Double field." << std::endl;
        BOOST_THROW_EXCEPTION(Error("Missing required MyLocation::Latitude field"));
        //++ival;
      }
    }
    else {
      std::cout << "Missing required MyLocation::Latitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required MyLocation::Latitude field"));
    }
    
    // Longitude
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Longitude) {
      //std::cout << "tlv::Longitude type: " << ival->type() << std::endl;
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
    
    //std::cout << "m_myLocation: " << m_myLocation.m_longitude << ", " <<  m_myLocation.m_latitude << std::endl;
    ++val; // next field: prev location
  }
  else {
    std::cout << "Missing required location::MyLocation field" << std::endl;
    BOOST_THROW_EXCEPTION(Error("Missing required location::MyLocation field"));
  }

  // Previous Location
  if (val != wire.elements_end() && val->type() == ndn::tlv::location::PrevLocation) {
    //std::cout << "tlv::PrevLocation type: " << val->type() << std::endl; 
    val->parse();
    ndn::Block::element_const_iterator ival = val->elements_begin(); 
    
    // Latitude 
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Latitude) {
      //m_myLocation.m_latitude = ndn::encoding::readDouble(*ival);
      //m_myLocation.m_latitude = ndn::tlv::location::readDouble(*ival);
      //std::cout << "tlv::Latitude type: " << ival->type() << std::endl;
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_prevLocation.m_latitude = *reinterpret_cast<const double*>(it->value());
        ++ival;
      }
      else {
        //std::cout << "Missing required location::Latitude::Double field." << std::endl;
        BOOST_THROW_EXCEPTION(Error("Missing required PrevLocation::Latitude field"));
        //++ival;
      }
    }
    else {
      //std::cout << "Missing required PrevLocation::Latitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required PrevLocation::Latitude field"));
    }
    
    // Longitude
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Longitude) {
      //std::cout << "tlv::Longitude type: " << ival->type() << std::endl;
      //m_myLocation.m_longitude = ndn::encoding::readDouble(*ival);
      //m_myLocation.m_longitude = ndn::tlv::location::readDouble(*ival);
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_prevLocation.m_longitude = *reinterpret_cast<const double*>(it->value());
        ++ival;
      }
      else {
        BOOST_THROW_EXCEPTION(Error("Missing required PrevLocation::Longitude::Double field"));
      }
    }
    else {
      std::cout << "Missing required PrevLocation::Longitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required PrevLocation::Longitude field"));
    }
    
    //std::cout << "m_prevLocation: " << m_prevLocation.m_longitude << ", " <<  m_prevLocation.m_latitude << std::endl;
    ++val; // next field: dest location
  }
  else {
    std::cout << "Missing required location::PrevLocation field. Got: " << val->type() << std::endl;
    BOOST_THROW_EXCEPTION(Error("Missing required location::PrevLocation field"));
  }
  
  // Destination Location
  if (val != wire.elements_end() && val->type() == ndn::tlv::location::DestLocation) {
    //std::cout << "tlv::DestLocation type: " << val->type() << std::endl; 
    val->parse();
    ndn::Block::element_const_iterator ival = val->elements_begin(); 
    
    // Latitude 
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Latitude) {
      //m_myLocation.m_latitude = ndn::encoding::readDouble(*ival);
      //m_myLocation.m_latitude = ndn::tlv::location::readDouble(*ival);
      //std::cout << "tlv::Latitude type: " << ival->type() << std::endl;
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_destLocation.m_latitude = *reinterpret_cast<const double*>(it->value());
        ++ival;
      }
      else {
        //std::cout << "Missing required location::Latitude::Double field." << std::endl;
        BOOST_THROW_EXCEPTION(Error("Missing required DestLocation::Latitude field"));
        //++ival;
      }
    }
    else {
      //std::cout << "Missing required PrevLocation::Latitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required DestLocation::Latitude field"));
    }
    
    // Longitude
    if (ival != val->elements_end() && ival->type() == ndn::tlv::location::Longitude) {
      //std::cout << "tlv::Longitude type: " << ival->type() << std::endl;
      //m_myLocation.m_longitude = ndn::encoding::readDouble(*ival);
      //m_myLocation.m_longitude = ndn::tlv::location::readDouble(*ival);
      ival->parse();
      ndn::Block::element_const_iterator it = ival->elements_begin();
      if (it != ival->elements_end() && it->type() == ndn::tlv::location::Double) {
        m_destLocation.m_longitude = *reinterpret_cast<const double*>(it->value());
        ++ival;
      }
      else {
        BOOST_THROW_EXCEPTION(Error("Missing required DestLocation::Longitude::Double field"));
      }
    }
    else {
      std::cout << "Missing required DestLocation::Longitude field. Got: " << ival->type() << std::endl;
      BOOST_THROW_EXCEPTION(Error("Missing required DestLocation::Longitude field"));
    }
    
    //std::cout << "m_destLocation: " << m_destLocation.m_longitude << ", " <<  m_destLocation.m_latitude << std::endl;
    ++val; // no next field
  }
  else {
    std::cout << "Missing required location::DestLocation field. Got: " << val->type() << std::endl;
    BOOST_THROW_EXCEPTION(Error("Missing required location::DestLocation field"));
  }
}
} // namespace lp
} // namespace ndn
