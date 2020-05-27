#ifndef libhttp__routing__match_path_hxx_
#define libhttp__routing__match_path_hxx_

#include <libhttp/routing/route-parameters.hxx>

#include <map>
#include <string>

namespace http {
namespace routing {

  bool
  match_path(std::string const& pattern,
             std::string const& subject,
             route_parameters& params);

  std::string
  render_path(std::string const& pattern, route_parameters const& params);

} // namespace routing
} // namespace http

#include <libhttp/routing/match-path.ixx>

#endif
