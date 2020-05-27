#ifndef undefined__libhttp__client__http_client_hxx_
#define undefined__libhttp__client__http_client_hxx_

namespace http {
namespace client

{

  struct http_client_options {
    size_t max_response_length;
    size_t max_content_length;
  };

  class http_client {
  public:
    http_client();

  private:
    class http_session;
  };

  // Sessions can be cached based on their connection and protocol endpoint.
  class http_client::session {
  public:
  private:
    asio::streambuf buffer_;
  };
} // namespace client

} // namespace http

#endif
