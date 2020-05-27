namespace http {
namespace models {

  template< typename T >
  model_metadata
  get_metadata(T const& model)
  {
    using traits = model_traits< T >;

    model_metadata metadata;

    if constexpr (traits::has_content_type)
      metadata.content_type = model.content_type();

    if constexpr (traits::has_entity_tag)
      metadata.etag = model.etag();

    if constexpr (traits::has_last_modified)
      metadata.last_modified = model.last_modfied();

    return metadata;
  }

} // namespace models
} // namespace http
