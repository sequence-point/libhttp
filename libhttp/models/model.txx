namespace http {
namespace models {

  template< typename T >
  template< typename... Args >
  model< T >::model(Args&&... args) : model_{ std::forward< Args >(args)... }
  {}

  template< typename T >
  typename model< T >::model_type const&
  model< T >::get_model() const
  {
    return model_;
  }

  template< typename T >
  void
  model< T >::serialize(std::ostream& o) const
  {
    get_model().serialize(o);
  }

  template< typename T >
  model< T >
  model< T >::deserialize(std::istream& i)
  {
    return T::deserialize(i);
  }

} // namespace models
} // namespace http
