#ifndef libhttp__server__extract_query_params_hxx_
#define libhttp__server__extract_query_params_hxx_

#include <libhttp/server/exchange.hxx>
#include <libhttp/server/query-params.hxx>

#include <map>
#include <string>

namespace http {
namespace util {

  struct query_parameters {
    std::map< std::string, std::string > params;
  };

  class extract_query_parameters {
  public:
    void
    on_request(exchange& ex) const;
  };

} // namespace util
} // namespace http

#endif
