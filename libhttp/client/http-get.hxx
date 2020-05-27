#ifndef undefined__libhttp__client__http_get_hxx_
#define undefined__libhttp__client__http_get_hxx_

#include <libhttp/types.hxx>

#include <libhttp/common/model.hxx>

#include <libhttp/client/http-connection.hxx>

namespace http {
namespace client {

  template< typename ResponseModel >
  ResponseModel
  http_get(http_connection& connection, string target);

} // namespace client
} // namespace http

#include <libhttp/client/http-get.txx>

#endif
