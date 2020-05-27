#ifndef libhttp__client__get_hxx_
#define libhttp__client__get_hxx_

#include <libhttp/types.hxx>

#include <libhttp/protocol/read-message.hxx>
#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/write-message.hxx>

#include <libhttp/protocol/rfc7230/content-length.hxx>

#include <asio.hpp>

namespace http {
namespace client {

  template< typename T >
  T
  get(string const& host, protocol::request const& request);

}
} // namespace http

#include <libhttp/client/get.txx>

#endif
