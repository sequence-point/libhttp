#ifndef libhttp__resources__error_hxx_
#define libhttp__resources__error_hxx_

#include <libhttp/server/transaction.hxx>

#include <sstream>
#include <string>
#include <vector>

namespace http {
namespace resources {

  void
  method_unknown(server::transaction& tx);

  void
  method_disallowed(server::transaction& tx,
                    std::vector< std::string > allowed_methods);

} // namespace resources
} // namespace http

#include <libhttp/resources/error.ixx>

#endif
