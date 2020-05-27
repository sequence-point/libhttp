#ifndef libhttp__protocol__status_code_hxx_
#define libhttp__protocol__status_code_hxx_

#include <stdexcept>
#include <string>

namespace http {
namespace protocol {

  class status_code {
  public:
    status_code() = default;

    status_code(unsigned short code);

    unsigned short
    code() const;

    operator unsigned short();

  private:
    unsigned short code_{ 0 };
  };

  bool
  operator==(status_code const& lhs, status_code const& rhs);

  bool
  operator==(status_code const& lhs, unsigned short rhs);

  bool
  operator==(unsigned short lhs, status_code const& rhs);

  bool
  operator!=(status_code const& lhs, status_code const& rhs);

  bool
  operator!=(status_code const& lhs, unsigned short rhs);

  bool
  operator!=(unsigned short lhs, status_code const& rhs);

  bool
  is_informational(status_code const& sc);

  bool
  is_success(status_code const& sc);

  bool
  is_redirection(status_code const& sc);

  bool
  is_client_error(status_code const& sc);

  bool
  is_server_error(status_code const& sc);

} // namespace protocol
} // namespace http

#include <libhttp/protocol/status-code.ixx>

#endif
