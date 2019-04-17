// #include "location.hpp"
// #include "tlv.hpp"

// namespace ndn {
// namespace lp {

// Location::Location() = default;

// Location::Location(double longitude, double latitude)
//   : m_longitude(longitude)
//   , m_latitude(latitude)
//   {
//   }

// template<encoding::Tag TAG>
// size_t
// Location::wireEncode(EncodingImpl<TAG>& encoder) const
// {
//   size_t totalLength = 0;
//   size_t doubleLength = 10;

//   const uint8_t* longitudeBytes = reinterpret_cast<const uint8_t*>(&m_longitude);
//   totalLength += encoder.prependByteArrayBlock(ndn::lp::Location::Double, longitudeBytes, 8);
//   totalLength += encoder.prependVarNumber(doubleLength);
//   totalLength += encoder.prependVarNumber(ndn::lp::Location::Longitude);

//   const uint8_t* latitudeBytes = reinterpret_cast<const uint8_t*>(&m_latitude);
//   totalLength += encoder.prependByteArrayBlock(ndn::lp::Location::Double, latitudeBytes, 8);
//   totalLength += encoder.prependVarNumber(doubleLength);
//   totalLength += encoder.prependVarNumber(ndn::lp::Location::Latitude);

//   totalLength += encoder.prependVarNumber(totalLength);
//   totalLength += encoder.prependVarNumber(ndn::lp::tlv::Location);

//   return totalLength;
// }

// NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(Location);

// // const Block&
// // Location::wireEncode() const
// // {
// //   EncodingEstimator estimator;
// //   size_t estimatedSize = wireEncode(estimator);

// //   EncodingBuffer buffer(estimatedSize, 0);
// //   wireEncode(buffer);

// //   m_wire = buffer.block();

// //   return m_wire;
// //   return nullptr;
// // }

// void
// Location::wireDecode(const Block& wire)
// {
//   if (wire.type() != ndn::lp::tlv::Location) {
//     BOOST_THROW_EXCEPTION(Error("Unexpected TLV-TYPE " + to_string(wire.type())));
//   }

//   wire.parse();

//   ndn::Block::element_const_iterator val = wire.elements_begin();

//   if (val != wire.elements_end() && val->type() == ndn::lp::Location::Latitude) {
//     val->parse();
//     ndn::Block::element_const_iterator it = val->elements_begin();
//     if (it != val->elements_end() && val->type() == ndn::lp::Location::Double) {
//       m_latitude = *reinterpret_cast<const double*>(it->value());

//       ++val;
//     }
//     else {
//       std::cout << "Missing required Location Double field" << std::endl;
//       BOOST_THROW_EXCEPTION(Error("Missing required Double field"));
//     }
//   }
//   else {
//     std::cout << "Location Latitude: Missing required Location Latitude field" << std::endl;
//     BOOST_THROW_EXCEPTION(Error("Location Latitude : Missing required Location Latitude field"));
//   }

//   if (val != wire.elements_end() && val->type() == ndn::lp::Location::Longitude) {
//     val->parse();
//     ndn::Block::element_const_iterator it = val->elements_begin();
//     if (it != val->elements_end() && val->type() == ndn::lp::Location::Double) {
//       m_longitude = *reinterpret_cast<const double*>(it->value());
//     }
//     else {
//       std::cout << "Location Longitude: Missing required Location Double field" << std::endl;
//       BOOST_THROW_EXCEPTION(Error("Missing required Double field"));
//     }
//   }
//   else {
//     std::cout << "Missing required Location Longitude field" << std::endl;
//     BOOST_THROW_EXCEPTION(Error("Missing required Location Longitude field"));
//   }
// }

// }
// }