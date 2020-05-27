#ifndef libhttp__server__session_manager_hxx_
#define libhttp__server__session_manager_hxx_

#include <libhttp/server/session.hxx>

#include <memory>
#include <set>

#include <asio.hpp>

namespace http {
namespace server {

  class session_manager {
  public:
    session_manager();

    void
    adopt(std::shared_ptr< session > const& s);

  private:
    std::set< std::shared_ptr< session > > sessions_;
  };

} // namespace server
} // namespace http

#include <libhttp/server/session-manager.ixx>

#endif
