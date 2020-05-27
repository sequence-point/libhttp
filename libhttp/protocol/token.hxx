#ifndef libhttp__protocol__token_hxx_
#define libhttp__protocol__token_hxx_

#include <libhttp/protocol/grammar.hxx>

#include <stdexcept>
#include <string>
#include <vector>

namespace http {
namespace protocol {

  class token {
  public:
    token(std::string the_token);

    std::string const&
    str() const;

  private:
    std::string the_token_;
  };

  template< typename InputIterator >
  std::string
  try_parse_token(InputIterator& begin, InputIterator end)
  {
    std::string token;

    while (begin != end && grammar::is_tchar(*begin))
      token += *begin;

    return token;
  }

  template< typename InputIterator >
  std::vector< std::string >
  try_parse_tokens(InputIterator& begin, InputIterator end)
  {
    std::vector< std::string > tokens;

    while (begin != end) {
      auto token = try_parse_token(begin, end);
      tokens.push_back(token);
    }

    return tokens;
  }

} // namespace protocol
} // namespace http

#include <libhttp/protocol/token.ixx>

#endif
