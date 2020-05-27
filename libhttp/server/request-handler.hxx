#ifndef libhttp__server__request_handler_hxx_
#define libhttp__server__request_handler_hxx_

#include <memory>

namespace http::server {

class transaction;

class request_handler {
public:
  template< typename Handler >
  request_handler(Handler handler)
    : handler_{ std::make_shared< handler_model< Handler > >(
        std::move(handler)) }
  {}

  void
  invoke(transaction& tx) const
  {
    get_handler().invoke(tx);
  }

  template< typename Handler, typename... Args >
  friend request_handler
  make_request_handler(Args&&... args);

private:
  struct handler_concept {
    virtual ~handler_concept() = default;

    virtual void
    invoke(transaction&) const = 0;
  };

  template< typename Handler >
  struct handler_model : handler_concept {
    template< typename... Args >
    handler_model(Args&&... args) : handler{ std::forward< Args >(args)... }
    {}

    void
    invoke(transaction& tx) const override
    {
      handler(tx);
    }

    Handler handler;
  };

  request_handler(std::shared_ptr< handler_concept > handler)
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
request_handler
make_request_handler(Args&&... args)
{
  using handler_model = request_handler::handler_model< Handler >;
  return std::make_shared< handler_model >(std::forward< Args >(args)...);
}

} // namespace http::server

#endif
