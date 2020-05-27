namespace http {
namespace resources {

  template< typename R >
  representation< R >::representation(representation_type rep)
    : rep_{ std::move(rep) }
  {}

  template< typename R >
  protocol::media_type
  representation< R >::type() const
  {
    return traits_type::type(rep());
  }

  template< typename R >
  void
  representation< R >::serialize(std::ostream& o)
  {
    traits_type::serialize(o, rep());
  }

  template< typename R >
  typename representation< R >::representation_type&
  representation< R >::rep()
  {
    return rep_;
  }

} // namespace resources
} // namespace http
