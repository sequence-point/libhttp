
#include <libunittest/unittest.hxx>

#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/write-message.hxx>

#include <sstream>

namespace {

struct stream_wrapper {
  std::ostream& output;

  template< typename ConstBufferSequence >
  std::size_t
  write_some(ConstBufferSequence const& buffers)
  {
    std::size_t count = 0;

    // Highly inefficient.
    for (auto it = asio::buffers_begin(buffers);
         it != asio::buffers_end(buffers); ++it) {
      char c = *it;
      output.write(&c, 1);
      ++count;
    }

    return count;
  }

  template< typename ConstBufferSequence >
  std::size_t
  write_some(ConstBufferSequence const& buffers, std::error_code&)
  {
    return write_some(buffers);
  }
};

} // namespace

DEFINE_TEST("write_request(buffers)")
{
  http::protocol::request request{ "GET", "/", http::protocol::http_1_1 };
  request.set_header("Content-Type", "application/json");

  std::stringstream output;
  stream_wrapper wrapper{ output };
  http::protocol::write_request(wrapper, request);

  TEST_EQUAL(output.str(), "GET / HTTP/1.1\r\n"
                           "content-type: application/json\r\n"
                           "\r\n");
}

DEFINE_TEST("write_request(buffers, error_code)")
{
  http::protocol::request request{ "GET", "/", http::protocol::http_1_1 };
  request.set_header("Content-Type", "application/json");

  std::stringstream output;
  stream_wrapper wrapper{ output };
  http::protocol::write_request(wrapper, request);

  TEST_EQUAL(output.str(), "GET / HTTP/1.1\r\n"
                           "content-type: application/json\r\n"
                           "\r\n");
}
