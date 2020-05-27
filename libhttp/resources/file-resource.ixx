namespace http {
namespace resources {

  inline file_resource::file_resource(std::string file_path)
    : path_{ std::move(file_path) }
  {}

  inline entity_tag
  file_resource::etag() const
  {
    return {};
  }

  inline binary_content< std::fstream >
  file_resource::get() const
  {
    std::fstream fs{ path(), std::ios::in | std::ios::binary };
    return binary_content
  }

  inline file_resource_factory::file_resource_factory(string prefix,
                                                      string root)
    : prefix_{ std::move(prefix) }, root_{ std::move(root) }
  {}

  template<>
  inline optional< file_resource >
  file_resource_factory::make< file_resource >(server::exchange& ex) const
  {
    auto path = resolve_path(ex.request().target());

    if (!path)
      return {};

    return file_resource{ std::move(path) };
  }

  inline string const&
  file_resource_factory::prefix() const
  {
    return prefix_;
  }

  inline string const&
  file_resource_factory::root() const
  {
    return root_;
  }

  inline optional< string >
  file_resource_factory::resolve_path(string target)
  {
    // TODO use C++'s std::path for this

    // FIXME Make sure /prefix/ IS a prefix of /target/

    // FIXME Remove /prefix/ from /target/

    // FIXME Append /target/ to /root/

    return root() + target;
  }

} // namespace resources
} // namespace http
