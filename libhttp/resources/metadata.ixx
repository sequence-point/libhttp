namespace http {
namespace resources {

  inline void
  set_metadata(protocol::message& target, resource_metadata const& metadata)
  {
    namespace rfc7232 = protocol::rfc7232;

    if (auto val = metadata.etag; val)
      protocol::set< rfc7232::ETag >(target, *val);
  }

} // namespace resources
} // namespace http
