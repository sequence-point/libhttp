#ifndef libhttp__models__metadata_hxx_
#define libhttp__models__metadata_hxx_

#include <libhttp/optional.hxx>

#include <libhttp/protocol/media-type.hxx>

#include <libhttp/protocol/rfc7231/content-type.hxx>
#include <libhttp/protocol/rfc7232/entity-tag.hxx>
#include <libhttp/protocol/rfc7232/etag.hxx>
#include <libhttp/protocol/rfc7232/last-modified.hxx>

#include <libhttp/models/traits.hxx>

#include <chrono>

namespace http {
namespace models {

  struct model_metadata {
    //
    // From RFC 7231
    //

    std::optional< protocol::media_type > content_type;

    //
    // From RFC 7232
    //

    std::optional< protocol::rfc7232::entity_tag > etag;
    std::optional< std::chrono::system_clock::time_point > last_modified;
  };

  bool
  operator==(model_metadata const& lhs, model_metadata const& rhs);
  bool
  operator!=(model_metadata const& lhs, model_metadata const& rhs);

  template< typename T >
  model_metadata
  get_metadata(T const& model);

  void
  set_metadata(protocol::message& m, model_metadata const& metadata);

} // namespace models
} // namespace http

#include <libhttp/models/metadata.ixx>
#include <libhttp/models/metadata.txx>

#endif
