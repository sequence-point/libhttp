#include <asio/ssl.hpp>
#include <iostream> // TODO remove

namespace http {
namespace client {

  template< typename T, typename SyncReadStream >
  T
  get(SyncReadStream& socket, protocol::request const& the_request);

  template< typename T >
  T
  get(string const& host, protocol::request const& the_request)
  {
    using asio::ip::tcp;
    namespace ssl = asio::ssl;
    typedef ssl::stream< tcp::socket > ssl_socket;

    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv23);

    auto verify_all = [](bool, ssl::verify_context&) { return true; };

    ctx.set_verify_callback(verify_all);

    // open
    asio::io_context io_context;
    ssl_socket sock(io_context, ctx);
    // tcp::resolver resolver(io_context);
    // tcp::resolver::query query(host, "https");
    // asio::connect(sock.lowest_layer(), resolver.resolve(query));
    asio::ip::tcp::endpoint endpoint{
      asio::ip::address::from_string("127.0.0.1"), 9090
    };

    sock.lowest_layer().connect(endpoint);
    sock.lowest_layer().set_option(tcp::no_delay(true));

    // handshake
    sock.set_verify_mode(ssl::verify_none);
    sock.set_verify_callback(ssl::rfc2818_verification(host));
    // sock.set_verify_callback(verify_all);
    sock.handshake(ssl_socket::client);

    return get< T >(sock, the_request);
  }

  template< typename T >
  T
  get(char const* host, protocol::request const& the_request)
  {
    return get< T >(string{ host }, the_request);
  }

  template< typename T, typename SyncReadStream >
  T
  get(SyncReadStream& socket, protocol::request const& the_request)
  {
    // Send message

    write_request(socket, the_request);

    char input[1 * 1024 * 1024];

    // Read response
    protocol::response the_response;
    std::error_code read_error;
    std::size_t consumed = 0;
    auto read_count =
      read_response(socket, asio::buffer(input, 1 * 1024 * 1024), the_response,
                    consumed, read_error);

    std::cout << "Read a total of    : " << read_count << '\n';
    std::cout << "Consumed a total of: " << consumed << '\n';

    if (read_error)
      throw read_error;

    auto data_length = the_response.get< protocol::rfc7230::ContentLength >();

    if (!data_length)
      throw "FAILED!";

    auto avail = read_count - consumed;
    auto content_length = *data_length;

    std::cout << "Content length: " << content_length << '\n';
    std::cout << "Available     : " << avail << '\n';

    char* begin_data = input + consumed;
    char* end_data = begin_data + content_length; // TODO max out

    while (avail < content_length) {
      std::cout << "Reading more content!\n";
      std::error_code read_ec;
      auto read_start = begin_data + avail;
      auto capacity = end_data - read_start;
      auto length =
        socket.read_some(asio::buffer(read_start, capacity), read_ec);
      if (read_ec)
        throw read_ec;
      std::cout << "Read more data: " << length << '\n';
      avail += length;
    }

#if 0


      for (;;) {
        rray< char, 128 > buf;
        system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
          break; // Connection closed cleanly by peer.
        else if (error)
          throw boost::system::system_error(error); // Some other error.

        std::cout.write(buf.data(), len);
      }
#endif

    // char const data[]{ "{\"name\":\"jane\"}" };
    return T::read_from(begin_data, end_data - begin_data);
  }

} // namespace client
} // namespace http
