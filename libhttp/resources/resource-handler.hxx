#ifndef libhttp__resources__resource_handler_hxx_
#define libhttp__resources__resource_handler_hxx_

#include <libhttp/optional.hxx>

#include <libhttp/server/transaction.hxx>

#include <libhttp/protocol/rfc7232/if-modified-since.hxx>
#include <libhttp/protocol/rfc7232/if-none-match.hxx>

#include <libhttp/models/metadata.hxx>
#include <libhttp/models/send.hxx>
#include <libhttp/models/traits.hxx>

#include <libhttp/resources/metadata.hxx>
#include <libhttp/resources/traits.hxx>
#include <libhttp/resources/utility.hxx>

#include <string>
#include <vector>

namespace http {
namespace resources {

  template< typename Resource, typename Factory >
  class resource_handler;

  template< typename Resource,
            typename Factory,
            typename handler = resource_handler< Resource, Factory > >
  auto
  use_resource(Factory factory);

  template< typename Resource, typename Factory >
  class resource_handler {
  public:
    using resource = Resource; // TODO remove
    using resource_type = Resource;

    using resource_factory = Factory; // TODO remove
    using factory_type = Factory;

    explicit resource_handler(factory_type factory);

    void
    on_request(server::transaction& tx) const;

    void
    operator()(server::transaction& tx) const
    {
      on_request(tx);
    }

    virtual void
    on_method_not_allowed(
      server::transaction& tx,
      std::vector< std::string > const& allowed_methods) const;

    virtual void
    on_method_not_supported(server::transaction& tx) const;

    // TODO: virtual void on_method_not_allowed(...)
    // TODO: virtual void on_method_not_supported(...)

  private:
    using resource_traits = resources::resource_traits< resource_type >;

    void
    handle_resource(server::transaction& tx,
                    std::optional< resource >& r) const;
    void
    handle_resource(server::transaction& tx, resource& r) const;

    void
    on_get(server::transaction& tx, resource const& r, bool head_request) const;

    void
    on_update(server::transaction& tx, resource const& r) const;

    void
    on_create(server::transaction& tx, resource const& r) const;

    void
    on_erase(server::transaction& tx, resource const& r) const;

    auto
    factory() const
    {
      return utility::deref(factory_);
    }

    factory_type factory_;
  };

} // namespace resources
} // namespace http

#include <libhttp/resources/resource-handler.txx>

#endif
