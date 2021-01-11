#ifndef libhttp__server__middleware_hxx_
#define libhttp__server__middleware_hxx_

#include <memory>

#include <libhttp/server/request-handler.hxx>

namespace http::server {

class transaction;

class middleware {
public:
  template< typename Handler >
  middleware(Handler handler)
    : handler_{ std::make_shared< handler_model< Handler > >(
        std::move(handler)) }
  {}

  void
  invoke(transaction& tx, request_handler const& next) const
  {
    get_handler().invoke(tx, next);
  }

  template< typename Handler, typename... Args >
  friend middleware
  make_middleware(Args&&... args);

private:
  struct handler_concept {
    virtual ~handler_concept() = default;

    virtual void
    invoke(transaction&, request_handler const& next) const = 0;
  };

  template< typename Handler >
  struct handler_model : handler_concept {
    template< typename... Args >
    handler_model(Args&&... args) : handler{ std::forward< Args >(args)... }
    {}

    void
    invoke(transaction& tx, request_handler const& next) const override
    {
      handler(tx, next);
    }

    Handler handler;
  };

  struct from_handler_t {
  };

  inline static constexpr from_handler_t from_handler = from_handler_t{};

  middleware(from_handler_t, std::shared_ptr< handler_concept > handler)
    : handler_{ std::move(handler) }
  {}

  handler_concept const&
  get_handler() const
  {
    return *handler_;
  }

  // Request handlers are shared for performance and hence const.
  std::shared_ptr< handler_concept const > handler_;
};

template< typename Handler, typename... Args >
middleware
make_middleware(Args&&... args)
{
  using handler_model = middleware::handler_model< Handler >;
  return { middleware::from_handler,
           std::make_shared< handler_model >(std::forward< Args >(args)...) };
}

inline request_handler
make_middleware_chain(std::vector< middleware > middlewares,
                      request_handler handler)
{
  struct invoker {
    middleware mw;
    request_handler next;

    void
    operator()(transaction& tx) const
    {
      mw.invoke(tx, next);
    }
  };

  request_handler next{ std::move(handler) };

  for (auto it = middlewares.rbegin(); it != middlewares.rend(); ++it)
    next = invoker{ *it, next };

  return next;
}

} // namespace http::server

#endif
