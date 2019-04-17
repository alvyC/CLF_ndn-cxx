#ifndef NDN_CXX_LOCATION_HPP
#define NDN_CXX_LOCATION_HPP

#include "../data.hpp"

namespace ndn {
namespace lp {

namespace location {
  enum {
    Double = 134,
    Longitude = 135,
    Latitude = 136
  };
}

// class Location {

// public:
// class Error : public ndn::tlv::Error
// {
// public:
//   explicit
//   Error(const std::string& what)
//     : ndn::tlv::Error(what)
//   {
//   }
// };

//   enum {
//     Double = 134,
//     Longitude = 135,
//     Latitude = 136
//   };

//   Location();

//   Location(double, double);

//   template<encoding::Tag TAG>
//   size_t
//   wireEncode(EncodingImpl<TAG>& encoder) const;

//   // const Block&
//   // wireEncode() const;

//   void
//   wireDecode(const Block& wire);

//   double
//   getLongitude() const {
//     return m_longitude;
//   }

//   void
//   setLongitude(const double lg) {
//     m_longitude = lg;
//   }

//   double
//   getLatitude() const {
//     return m_latitude;
//   }

//   void
//   setLaitude(const double lt) {
//     m_latitude = lt;
//   }

// private:
//   double m_longitude;
//   double m_latitude;
// };

}
}

#endif