namespace http {
namespace client {

  template< typename RequestContent, typename ResponseContent >
  void
  user_agent::make_request(protocol::request request,
                           RequestContent content,
                           protocol::response& response,
                           ResponseContent response_content)
  {}

  template< typename RequestContent,
            typename ResponseContent,
            typename CompletionHandler >
  void
  user_agent::async_make_request(protocol::request request,
                                 RequestContent content,
                                 protocol::response& response,
                                 ResponseContent response_content,
                                 CompletionHandler&& handler)
  {}

} // namespace client
} // namespace http
