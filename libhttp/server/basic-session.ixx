namespace http {
namespace server {

  inline io::stream&
  session::get_stream()
  {
    return stream_;
  }

  inline io::stream const&
  session::get_stream() const
  {
    return stream_;
  }

  void
  session::start()
  {
    auto self = shared_from_this();

    std::cout << "starting session\n";

    tx_.reset(new default_transaction{ *this });
    tx_->init();

    // we're now in a transaction
  }

  void
  session::resume()
  {}

} // namespace server
} // namespace http
