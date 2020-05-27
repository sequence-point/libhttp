namespace http {
namespace protocol {

  inline token::token(std::string the_token)
    : the_token_{ std::move(the_token) }
  {
    for (auto const& c : str()) {
      if (!grammar::is_tchar(c))
        throw std::invalid_argument{ "invalid token" };
    }
  }

  inline std::string const&
  token::str() const
  {
    return the_token_;
  }

} // namespace protocol
} // namespace http
