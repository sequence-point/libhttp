#ifndef libhttp__resources__file_resource_hxx_
#define libhttp__resources__file_resource_hxx_

#include <libhttp/types.hxx>

#include <libhttp/protocol/media-type.hxx>

#include <libhttp/resources/entity-tag.hxx>

#include <fstream>
#include <functional>

namespace http {
namespace resources {

  template< typename S >
  class binary_content {
  public:
    using stream_type = S;

    binary_content(stream_type stream, protocol::media_type content_type);

    entity_tag
    etag() const;

    protocol::media_type const&
    content_type() const;

    void
    serialize(std::ostream& o) const;

  private:
    std::iostream&
    ios() const;

    stream_type mutable fs_;
    protocol::media_type content_type_;
  };

  class fs_resource {
  public:
    explicit file_resource(std::string file_path);

    entity_tag
    etag() const;

    binary_content< std::fstream >
    get() const;

  private:
    string const file_path_;
  };

  class fs_resource_factory {
  public:
    file_resource_factory(string prefix, string root);

    template< typename T >
    auto
    make(server::exchange& ex) const;

    string const&
    prefix() const;

    string const&
    root() const;

  private:
    optional< string >
    resolve_path(string target);

    string const prefix_;
    string const root_;
  };

  template<>
  optional< file_resource >
  file_resource_factory::make< file_resource >(server::exchange& ex) const;

  resource_handler< file_resource, file_resource_factory >
  make_file_server(string prefix, string root);

} // namespace resources
} // namespace http

#include <libhttp/resources/file-resource.ixx>
#include <libhttp/resources/file-resource.txx>

#endif
