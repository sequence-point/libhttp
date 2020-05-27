namespace http {
namespace common {

  inline protocol::request const&
  http_exchange::request() const
  {
    return request_;
  }

  inline void
  http_exchange::replace_request(protocol::request request)
  {
    request_ = std::move(request);
  }

  inline void
  http_exchange::replace_request(protocol::request request,
                                 const_buffer content)
  {
    request_ = std::move(request);
    request_content_ = std::move(content);
  }

  inline void
  http_exchange::replace_request_content(const_buffer content)
  {
    request_content_ = std::move(content);
  }

  inline protocol::response const&
  http_exchange::response() const
  {
    return response_;
  }

  inline void
  http_exchange::replace_response(protocol::response response)
  {
    response_ = std::move(response);
  }

  inline void
  http_exchange::replace_response(protocol::response response,
                                  const_buffer content)
  {
    response_ = std::move(response);
    response_content_ = std::move(content);
  }

  inline void
  http_exchange::replace_response_content(const_buffer content)
  {
    response_content_ = std::move(content);
  }

  inline http_exchange::stream
  http_exchange::alloc_stream()
  {
    return stream{ &alloc< asio::streambuf >() };
  }

} // namespace common
} // namespace http
