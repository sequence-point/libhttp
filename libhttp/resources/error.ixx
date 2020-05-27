namespace http {
namespace resources {

  inline void
  method_unknown(server::transaction& tx)
  {
    tx.send_error(server::common_error::not_implemented);
  }

  inline void
  method_disallowed(server::transaction& tx,
                    std::vector< std::string > allowed_methods)
  {

    // TODO promote to protocol::rfc7231::Allow

    tx.response().set_status(405);
    tx.response().set_header("Allow", str.str());
    tx.send(asio::const_buffer{});
  }

} // namespace resources
} // namespace http
