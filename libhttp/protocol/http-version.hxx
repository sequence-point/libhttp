#ifndef libhttp__protocol__http_version_hxx_
#define libhttp__protocol__http_version_hxx_

#include <asio.hpp>

#include <cstdint>
#include <ostream>
#include <string>

namespace http {
namespace protocol {

  struct version {
    constexpr version() noexcept : major{ 0 }, minor{ 0 }
    {}

    constexpr version(std::uint16_t major, std::uint16_t minor) noexcept
      : major{ major }, minor{ minor }
    {}

    std::uint16_t major;
    std::uint16_t minor;
  };

  version constexpr http_1_0{ 1, 0 };

  version constexpr http_1_1{ 1, 1 };

  bool
  operator==(version const& lhs, version const& rhs) noexcept;

  bool
  operator!=(version const& lhs, version const& rhs) noexcept;

  std::ostream&
  to_stream(std::ostream& o, version const& v);

  std::string
  to_string(version const& v);

  std::ostream&
  operator<<(std::ostream& o, version const& v);

  void
  to_buffers(std::vector< asio::const_buffer >& buffers, version const& v);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/http-version.ixx>

#endif
