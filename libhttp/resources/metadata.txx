namespace http {
namespace resources {

  template< typename T >
  resource_metadata
  get_metadata(T const& source)
  {
    using traits = resource_traits< T >;

    resource_metadata metadata;

    if constexpr (traits::has_entity_tag)
      metadata.etag = source.etag();

    return metadata;
  }

} // namespace resources
} // namespace http
