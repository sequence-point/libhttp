#ifndef libhttp__protocol__error_hxx_
#define libhttp__protocol__error_hxx_

#include <string>
#include <system_error>

namespace http::protocol {

enum class parse_error {
  incomplete_message = 1,

  // Common.
  bad_version,
  bad_header_field,
  bad_header_value,
  bad_terminator,

  // Request-specific.
  bad_method,
  bad_target,

  // Response-specific.
  bad_status,
  bad_reason
};

std::error_category const&
get_parse_error_category();

std::error_code
make_error_code(parse_error error);

enum class protocol_error { invalid_content_length = 1, request_too_large };

std::error_category const&
get_protocol_error_category();

std::error_code
make_error_code(protocol_error error);

} // namespace http::protocol

namespace std {

template<>
struct is_error_code_enum< http::protocol::parse_error > : true_type {
};

template<>
struct is_error_code_enum< http::protocol::protocol_error > : true_type {
};

} // namespace std

#include <libhttp/protocol/error.ixx>

#endif
