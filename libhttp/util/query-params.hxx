#ifndef libhttp__server__query_params_hxx_
#define libhttp__server__query_params_hxx_

#include <map>
#include <string>

namespace http {
namespace util {

  std::map< std::string, std::string >
  parse_query_params(std::string const& query);

} // namespace util
} // namespace http

#include <libhttp/util/query-params.ixx>

#endif
