namespace http::resources {

template< typename R, typename F, typename H >
auto
use_resource(F factory)
{
  return H{ factory };
}

template< typename R, typename F >
resource_handler< R, F >::resource_handler(resource_factory factory)
  : factory_{ std::move(factory) }
{}

template< typename R, typename F >
void
resource_handler< R, F >::on_request(server::transaction& tx) const
{
  auto r = factory().template make< resource >(tx);
  handle_resource(tx, r);
}

template< typename R, typename F >
void
resource_handler< R, F >::on_method_not_allowed(
  server::transaction& tx,
  std::vector< std::string > const& allowed_methods) const
{
  std::stringstream str;

  // TODO Make hard-coded header type out of this.
  if (auto it = allowed_methods.begin(); it != allowed_methods.end()) {
    str << *it;

    while (++it != allowed_methods.end())
      str << ", " << *it;
  }
  tx.response().set_header("Allow", str.str());

  tx.response().set_status(405);
  tx.send(asio::const_buffer{});
}

template< typename R, typename F >
void
resource_handler< R, F >::on_method_not_supported(server::transaction& tx) const
{
  std::cout << "method not supported\n";

  tx.response().set_status(501);
  tx.send(asio::const_buffer{});
}

template< typename R, typename F >
void
resource_handler< R, F >::handle_resource(server::transaction& tx,
                                          std::optional< resource >& r) const
{
  if (r)
    handle_resource(tx, *r);

  // TODO allow the use of an external 404 handler
  tx.response().set_status(404);
  tx.send({});
}

template< typename R, typename F >
void
resource_handler< R, F >::handle_resource(server::transaction& tx,
                                          resource& r) const
{
  auto const& method = tx.request().method();

  if (method == "GET" || method == "HEAD") {
    if constexpr (resource_traits::can_get) {
      on_get(tx, r, method == "HEAD");
      return;
    }

    on_method_not_allowed(tx, resource_traits::allowed_methods());
    return;
  }

  if (method == "PUT") {
    if constexpr (resource_traits::can_update) {
      on_update(tx, r);
      return;
    }

    std::cout << "Resource does not support PUT/update...\n";

    on_method_not_allowed(tx, resource_traits::allowed_methods());
    return;
  }

  if (method == "POST") {
    if constexpr (resource_traits::can_create) {
      on_create(tx, r);
      return;
    }

    on_method_not_allowed(tx, resource_traits::allowed_methods());
    return;
  }

  if (method == "DELETE") {
    if constexpr (resource_traits::can_erase) {
      on_erase(tx, r);
      return;
    }

    on_method_not_allowed(tx, resource_traits::allowed_methods());
    return;
  }

  on_method_not_supported(tx);
}

template< typename R, typename F >
void
resource_handler< R, F >::on_get(server::transaction& tx,
                                 resource const& r,
                                 bool head_request) const
{
  using protocol::rfc7232::IfModifiedSince;
  using protocol::rfc7232::IfNoneMatch;

  using protocol::get;

  tx.response().set_status(200);

  auto rdata = get_metadata(r);

  if (rdata.etag) {
    auto const etag = *rdata.etag;

    if (auto opt_val = get< IfNoneMatch >(tx.request()); opt_val) {
      auto const val = *opt_val;

      // TODO Implement.
      // if (strong_compare(val.etag(), etag)) {
      //  // TODO Respond Not Modified
      //  return;
      //}
    }

    if (auto opt_val = get< IfModifiedSince >(tx.request()); opt_val) {
      auto const val = *opt_val;

      // TODO Implement.
      //  // TODO Respond Not Modified
    }
  }

  set_metadata(tx.response(), rdata);

  auto model = r.get();

  // TODO Check If-None-Match against model
  // TODO Check If-Modified-Since against model
  // TODO Check If-Range after the above checks

  models::send(tx, model, head_request);
}

template< typename R, typename F >
void
resource_handler< R, F >::on_update(server::transaction& tx,
                                    resource const& r) const
{
  using input_model_type = typename resource_traits::model_type_for_update;

  // TODO handle If-Match
  auto input = input_model_type::read_from(tx.request_content());
  auto updated_resource = r.update(input);

  using updated_resource_type = decltype(updated_resource);

  auto updated_resource_metadata = get_metadata(updated_resource);
  set_metadata(tx.response(), updated_resource_metadata);

  // Handle resources without a get() method differently.
  constexpr auto can_get =
    resource_traits::template rebind< updated_resource_type >::other::can_get;

  if constexpr (can_get) {
    tx.response().set_status(200);

    auto model = updated_resource.get();
    models::send(tx, updated_resource.get(), false);
  }
  else {
    tx.response().set_status(204);
    tx.send({});
  }
}

template< typename R, typename F >
void
resource_handler< R, F >::on_create(server::transaction& tx,
                                    resource const& r) const
{
  using model_type = typename resource_traits::model_type_for_create;
  using model_traits = models::model_traits< model_type >;

  // Deserialize the input model.
  auto input = model_traits::read_from(tx.request_content());

  // Pass the deserialized input model to the handler.

  // TODO handle If-Unmodified-Since

  if constexpr (std::is_same_v< decltype(r.create(input)), void >) {
    r.create(input);
    tx.response().set_status(204);
    tx.send({});
  }
  else {
    auto new_resource = r.create(input);

    auto new_resource_metadata = get_metadata(new_resource);
    set_metadata(tx.response(), new_resource_metadata);

    tx.response().set_status(201);
    models::send(tx, new_resource.get(), false);
  }
}

template< typename R, typename F >
void
resource_handler< R, F >::on_erase(server::transaction& tx,
                                   resource const& r) const
{}

} // namespace http::resources
