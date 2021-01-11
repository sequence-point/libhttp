#ifndef libhttp__server__common_error_hxx_
#define libhttp__server__common_error_hxx_

namespace http::server {

enum class common_error {
  // RFC 7231
  //

  // Client Error
  bad_request,
  payment_required,
  forbidden,
  not_found,
  method_not_allowed,
  not_acceptable,
  request_timeout,
  conflict,
  gone,
  length_required,
  payload_too_large,
  uri_too_long,
  unsupported_media_type,
  expectation_failed,
  upgrade_required,

  // Server Error
  internal_server_error,
  not_implemented,
  bad_gateway,
  service_unavailable,
  gateway_timeout,
  http_version_not_supported
};

} // namespace http::server

#endif
