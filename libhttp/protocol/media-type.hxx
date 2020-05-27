#ifndef libhttp__protocol__media_type_hxx_
#define libhttp__protocol__media_type_hxx_

#include <libhttp/protocol/grammar.hxx>
#include <libhttp/protocol/optional.hxx>

#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace http {
namespace protocol {

  class media_type {
  public:
    media_type() = default;

    media_type(std::string type, std::string subtype);

    media_type(std::string type,
               std::string subtype,
               std::map< std::string, std::string > params);

    std::string const&
    type() const noexcept;

    std::string const&
    subtype() const noexcept;

    std::map< std::string, std::string > const&
    params() const noexcept;

    template< typename InputIterator >
    static optional< media_type >
    try_parse(InputIterator& begin, InputIterator const& end);

    static optional< media_type >
    try_parse(std::string const& str);

  private:
    std::string type_;
    std::string subtype_;
    std::map< std::string, std::string > params_;
  };

  bool
  operator==(media_type const& lhs, media_type const& rhs);

  bool
  operator!=(media_type const& lhs, media_type const& rhs);

  std::ostream&
  to_stream(std::ostream& o, media_type const& mt);

  std::string
  to_string(media_type const& mt);

  std::ostream&
  operator<<(std::ostream& o, media_type const& mt);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/media-type.ixx>
#include <libhttp/protocol/media-type.txx>

#endif
