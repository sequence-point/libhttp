namespace http {
namespace client {

  template< typename ResponseModel >
  ResponseModel
  http_get(http_connection& connection, string target)
  {
    protocol::request request{ "GET", target, protocol::http_1_1 };
    protocol::response response;

    asio::streambuf response_content;
    connection.make_request(request, response, response_content);

    std::istream input{ &response_content };

    return common::model< ResponseModel >::deserialize(input).get_model();
  }

} // namespace client
} // namespace http
