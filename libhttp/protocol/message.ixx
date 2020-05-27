namespace http {
namespace protocol {

  inline protocol::version const&
  message::version() const
  {
    return version_;
  }

  inline void
  message::set_version(protocol::version v)
  {
    version_ = std::move(v);
  }

  inline header_collection const&
  message::headers() const
  {
    return headers_;
  }

  inline void
  message::set_header(std::string name, std::string value)
  {
    headers_.set(std::move(name), std::move(value));
  }

  inline void
  message::append_header(std::string name, std::string value)
  {
    headers_.append(std::move(name), std::move(value));
  }

  inline void
  message::erase_header(std::string name)
  {
    headers_.erase(name);
  }

  inline void
  message::set_headers(header_collection headers)
  {
    headers_ = std::move(headers);
  }

} // namespace protocol
} // namespace http
