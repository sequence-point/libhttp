namespace http {
namespace client {

  inline user_agent::user_agent()
    : ua_identity_{ { "libhttp", version::version_str } }
  {}

  inline user_agent::~user_agent()
  {}

  inline void
  user_agent::set_identity(protocol::rfc7231::products identity)
  {
    ua_identity_ = std::move(identity);
  }

} // namespace client
} // namespace http
