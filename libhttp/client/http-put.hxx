
#ifndef undefined__libhttp__client__http_get_hxx_
#define undefined__libhttp__client__http_get_hxx_

namespace http {
namespace client {

  template< typename ResponseModel >
  ResponseModel
  http_get(http_connection& connection, string target);

  template< typename RequestModel, typename ResponseModel >
  ResponseModel
  http_put(http_connection& connection,
           string target,
           RequestModel const& request);

  template< typename RequestModel, typename ResponseModel >
  ResponseModel
  http_post(http_connection& connection,
            string target,
            RequestModel const& request);

} // namespace client

} // namespace http
} // namespace http

#endif
