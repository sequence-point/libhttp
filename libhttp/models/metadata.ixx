namespace http::models {

inline bool
operator==(model_metadata const& lhs, model_metadata const& rhs)
{
  if (lhs.content_type != rhs.content_type)
    return false;

  if (lhs.etag != rhs.etag)
    return false;

  if (lhs.last_modified != rhs.last_modified)
    return false;

  return true;
}

inline bool
operator!=(model_metadata const& lhs, model_metadata const& rhs)
{
  return !(lhs == rhs);
}

inline void
set_metadata(protocol::message& m, model_metadata const& metadata)
{
  namespace rfc7231 = protocol::rfc7231;
  namespace rfc7232 = protocol::rfc7232;

  if (auto val = metadata.content_type; val)
    protocol::set< rfc7231::ContentType >(m, *val);

  if (auto val = metadata.etag; val)
    protocol::set< rfc7232::ETag >(m, *val);

  if (auto val = metadata.last_modified; val)
    protocol::set< rfc7232::LastModified >(m, *val);
}

} // namespace http::models
