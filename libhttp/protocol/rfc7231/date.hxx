#ifndef libhttp__protocol__rc7231__date_hxx_
#define libhttp__protocol__rc7231__date_hxx_

#include <libhttp/protocol/date-time.hxx>

#include <chrono>

namespace http::protocol::rfc7231 {

// FIXME Implement parsing.
struct Date {
  using alias_type = std::chrono::system_clock::time_point;

  static constexpr const char name[] = "date";

  static std::string
  to_string(std::chrono::system_clock::time_point const& point_in_time)
  {
    return format_http_date(point_in_time);
  }
};

} // namespace http::protocol::rfc7231

#endif
