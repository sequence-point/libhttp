#ifndef libhttp__routing__route_parameters_hxx_
#define libhttp__routing__route_parameters_hxx_

#include <map>
#include <string>

namespace http::routing {

class route_parameters : public std::map< std::string, std::string > {
public:
  // Inherit constructor from std::map.
  using std::map< std::string, std::string >::map;

  // ... and also inherit operator=.
  using std::map< std::string, std::string >::operator=;
};

} // namespace http::routing

#endif
