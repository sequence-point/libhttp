#ifndef libhttp__resources__metadata_hxx_
#define libhttp__resources__metadata_hxx_

#include <libhttp/optional.hxx>

#include <libhttp/protocol/message.hxx>

#include <libhttp/protocol/rfc7232/entity-tag.hxx>
#include <libhttp/protocol/rfc7232/etag.hxx>

#include <libhttp/resources/traits.hxx>

#include <chrono>

namespace http::resources {

struct resource_metadata {
  //
  // From RFC 7232.
  //

  std::optional< protocol::rfc7232::entity_tag > etag;
};

template< typename T >
resource_metadata
get_metadata(T const& source);

void
set_metadata(protocol::message& target, resource_metadata const& meta);

} // namespace http::resources

#include <libhttp/resources/metadata.ixx>
#include <libhttp/resources/metadata.txx>

#endif
