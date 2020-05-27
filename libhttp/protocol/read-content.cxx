#include <libunittest/unittest.hxx>

#include <libhttp/protocol/read-content.hxx>

#include "asio-mocks.hxx"
#include <asio.hpp>

namespace {

struct never_called {
  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const&)
  {
    return 0;
  }

  template< typename MutableBufferSequence >
  std::size_t
  read_some(MutableBufferSequence const&, std::error_code&)
  {
    return 0;
  }
};

} // namespace

namespace asio {

template<>
struct is_mutable_buffer_sequence< never_called > : std::true_type {
};

} // namespace asio

#if 0
read_content(SyncReadStream& s,
             size_t content_length,
             DynamicBuffer& read_buffer,
             MutableBufferSequence const& response_content);

size_t
read_content(SyncReadStream& s,
             size_t content_length,
             DynamicBuffer& read_buffer,
             MutableBufferSequence const& response_content,
             error_code& ec);

size_t
read_content(SyncReadStream& s,
             protocol::response const& response,
             DynamicBuffer& read_buffer,
             DynamicBuffer& response_content);

size_t
read_content(SyncReadStream& s,
             protocol::response const& response,
             DynamicBuffer& read_buffer,
             DynamicBuffer& response_content,
             error_code& ec);
#endif

DEFINE_TEST("Pre-populated DynamicBuffer does not cause a read")
{
  // Populate the dynamic buffer.
  asio::streambuf read_buffer;
  asio::buffer_copy(read_buffer.prepare(4), asio::buffer("HELO", 4), 4);
  read_buffer.commit(4);

  // Read "response content", passing dynamic buffer.

  never_called dummy_stream;

  char response_content[4];

  auto bytes_transferred = http::protocol::read_content(
    dummy_stream, read_buffer, 4, asio::buffer(response_content));

  TEST_EQUAL(bytes_transferred, 0);
  TEST_EQUAL(response_content[0], 'H');
  TEST_EQUAL(response_content[1], 'E');
  TEST_EQUAL(response_content[2], 'L');
  TEST_EQUAL(response_content[3], 'O');
}

DEFINE_TEST("Read with content_length == 0 does not perform a read")
{
  asio::streambuf read_buffer;
  never_called dummy_stream;
  char response_content[4];

  auto bytes_transferred = http::protocol::read_content(
    dummy_stream, read_buffer, 0, asio::buffer(response_content));

  TEST_EQUAL(bytes_transferred, 0);
}

DEFINE_TEST("Read with dynamic buffer filled with half contents causes read of "
            "half the data")
{
  // Populate the dynamic buffer.
  asio::streambuf read_buffer;
  asio::buffer_copy(read_buffer.prepare(4), asio::buffer("HELO", 4), 4);
  read_buffer.commit(4);

  auto s = asio_mocks::make_sync_read_stream_once(asio::buffer("OLEH", 48));

  char response_content[8];

  auto bytes_transferred = http::protocol::read_content(
    s, read_buffer, 8, asio::buffer(response_content));

  TEST_EQUAL(bytes_transferred, 4);
  TEST_EQUAL(response_content[0], 'H');
  TEST_EQUAL(response_content[1], 'E');
  TEST_EQUAL(response_content[2], 'L');
  TEST_EQUAL(response_content[3], 'O');
  TEST_EQUAL(response_content[4], 'O');
  TEST_EQUAL(response_content[5], 'L');
  TEST_EQUAL(response_content[6], 'E');
  TEST_EQUAL(response_content[7], 'H');
}
