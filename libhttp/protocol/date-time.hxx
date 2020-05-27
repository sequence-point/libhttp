#ifndef libhttp__protocol__http_date_time_hxx_
#define libhttp__protocol__http_date_time_hxx_

#include <libhttp/protocol/optional.hxx>

#include <chrono>
#include <iomanip>
#include <locale>
#include <string>

namespace http {
namespace protocol {

  std::string
  format_http_date(std::chrono::system_clock::time_point const& time_point);

  optional< std::chrono::system_clock::time_point >
  try_parse_http_date(std::string const& text);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/date-time.ixx>

#endif
