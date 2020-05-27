#ifndef libhttp__server__session_hxx_
#define libhttp__server__session_hxx_

#include <memory>

namespace http {
namespace server {

  class session : public std::enable_shared_from_this< session > {
  public:
  protected:
    session() = default;
    ~session() = default;
  };

} // namespace server
} // namespace http

#endif
