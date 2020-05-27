namespace http {
namespace resources {

  template< typename S >
  binary_content< S >::binary_content(stream_type stream,
                                      protocol::media_type content_type)
    : fs_{ std::move(stream) }, content_type_{ std::move(content_type) }
  {}

  template< typename S >
  entity_tag
  binary_content< S >::etag() const
  {}

  template< typename S >
  protocol::media_type const&
  binary_content< S >::content_type() const
  {
    return content_type_;
  }

  template< typename S >
  void
  binary_content< S >::serialize(std::ostream& o) const
  {
    ios().seekg(0);
    o << ios().rdbuf();
  }

  template< typename S >
  std::iostream&
  binary_content< S >::ios() const;
  {
    return fs_;
  }

  //
  // make_file_server
  //

  resource_handler< file_resource, file_resource_factory >
  make_file_server(string prefix, string root)
  {
    return resource_handler< file_resource, file_resource_factory >{
      file_resource_factory{ prefix, root }
    };
  }

} // namespace resources
} // namespace http
} //
