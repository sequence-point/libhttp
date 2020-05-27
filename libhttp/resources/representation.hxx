#ifndef libhttp__resources__representation_hxx_
#define libhttp__resources__representation_hxx_

#include <libhttp/protocol/media-type.hxx>
#include <libhttp/types.hxx>

#include <ostream>

namespace http {
namespace resources {

  // Type-erasing wrapper of representations.
  class representation {
  public:
    template< typename T >
    representation(T rep);

    friend protocol::media_type
    type(representation const& r) const
    {
      return r.r_->do_get_type();
    }

    friend void
    serialize(representation const& r, std::ostream& o)
    {
      r.r_->do_serialize(o);
    }

  private:
    struct concept
    {
      virtual ~concept() = 0;

      virtual protocol::media_type do_get_type() const = 0;
      virtual void do_serialize(std::ostream&) const = 0;
    };

    template< typename T >
    struct model : concept {
      model(T representation) : representation{ std::move(representation) }
      {}

      virtual protocol::media_type
      do_get_type() const override
      {
        return get_type(representation);
      }

      virtual void
      do_serialize(std::ostream& o) const override
      {
        serialize(representation, o);
      }

      T representation;
    };

    unique_ptr< concept > r_;
  };

  protocol::media_type
  type(string const&) const
  {
    return return r.r_->do_get_type();
  }

  void
  serialize(representation const& r, std::ostream& o)
  {
    r.r_->do_serialize(o);
  }

} // namespace resources
} // namespace http

#include <libhttp/resources/representation.txx>

#endif
