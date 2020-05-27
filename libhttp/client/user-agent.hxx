#ifndef libhttp__client__user_agent_hxx_
#define libhttp__client__user_agent_hxx_

#include <libhttp/client/version.hxx>

#include <libhttp/protocol/read-message.hxx>
#include <libhttp/protocol/write-message.hxx>

#include <libhttp/protocol/rfc7231/product.hxx>

namespace http {
namespace client {

  class user_agent {
  public:
    user_agent();
    ~user_agent();

    //! Override User Agent identity.
    void
    set_identity(protocol::rfc7231::products identity);

    template< typename RequestContent, typename ResponseContent >
    void
    make_request(protocol::request request,
                 RequestContent content,
                 protocol::response& response,
                 ResponseContent response_content);

    template< typename RequestContent,
              typename ResponseContent,
              typename CompletionHandler >
    void
    async_make_request(protocol::request request,
                       RequestContent content,
                       protocol::response& response,
                       ResponseContent response_content,
                       CompletionHandler&& handler);

  private:
    void
    prepare_request(protocol::request& r);

    protocol::rfc7231::products ua_identity_;

    connection_pool pool_;
  };

} // namespace client
} // namespace http

#endif
