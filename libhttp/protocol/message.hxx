#ifndef libhttp__protocol__message_hxx_
#define libhttp__protocol__message_hxx_

#include <libhttp/protocol/header-collection.hxx>
#include <libhttp/protocol/http-version.hxx>
#include <libhttp/protocol/optional.hxx>
#include <libhttp/protocol/traits.hxx>
#include <libhttp/protocol/utility.hxx>

namespace http {
namespace protocol {

  class message {
  public:
    protocol::version const&
    version() const;

    void
    set_version(protocol::version v);

    header_collection const&
    headers() const;

    void
    set_headers(header_collection headers);

    void
    set_header(std::string name, std::string value);

    void
    append_header(std::string name, std::string value);

    void
    erase_header(std::string name);

  protected:
    message() = default;

  private:
    protocol::version version_;
    header_collection headers_;
  };

  template< typename Header >
  bool
  has(message const& m);

  template< typename Header >
  bool
  has_quick(message const& m);

  template< typename Header >
  optional< traits::alias_type_t< Header > >
  get(message const& m);

  template< typename Header >
  optional< traits::alias_type_t< Header > >
  get(message const& m, std::error_code& ec);

  template< typename Header, typename... Args >
  void
  set(message& m, Args&&... args);

  template< typename Header, typename... Args >
  void
  set_if_not_set(message& m, Args&&... args);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/message.ixx>
#include <libhttp/protocol/message.txx>

#endif
