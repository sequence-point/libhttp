#ifndef libhttp__models__model_hxx_
#define libhttp__models__model_hxx_

#include <libhttp/protocol/response.hxx>

#include <libhttp/protocol/rfc7231/content-type.hxx>
#include <libhttp/protocol/rfc7232/entity-tag.hxx>
#include <libhttp/protocol/rfc7232/etag.hxx>
#include <libhttp/protocol/rfc7232/last-modified.hxx>

#include <libhttp/models/traits.hxx>

namespace http {
namespace models {

  template< typename T >
  class model {
  public:
    using model_type = T;

    template< typename... Args >
    model(Args&&... args);

    model_type const&
    get_model() const;

    void
    serialize(std::ostream& o) const;

    static model< T >
    deserialize(std::istream& i);

  private:
    using model_traits = models::model_traits< model_type >;

    model_type model_;
  };

  template< typename T >
  model< T >
  wrap_model(T m)
  {
    return model< T >{ std::move(m) };
  }

} // namespace models
} // namespace http

#include <libhttp/models/model.txx>

#endif
