#ifndef libhttp__protocol__match_message_hxx_
#define libhttp__protocol__match_message_hxx_

#include <libhttp/protocol/error.hxx>

#include <system_error>

#include <asio.hpp>

namespace http {
namespace protocol {

  template< typename Parser, typename Message >
  class match_message {
  public:
    match_message(Message& m, std::error_code& ec);

    template< typename Iterator >
    std::pair< Iterator, bool >
    operator()(Iterator begin, Iterator end);

  private:
    Message& message_;
    std::error_code& ec_;
  };

} // namespace protocol
} // namespace http

namespace asio {

template< typename Parser, typename Message >
struct is_match_condition< http::protocol::match_message< Parser, Message > >
  : public std::true_type {
};

} // namespace asio

#include <libhttp/protocol/match-message.txx>

#endif
