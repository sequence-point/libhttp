#ifndef libhttp__protocol__grammar_hxx_
#define libhttp__protocol__grammar_hxx_

namespace http::protocol::grammar {

bool
is_cr(char c);

bool
is_lf(char c);

bool
is_cr_or_lf(char c);

bool
is_space(char c);

bool
is_digit(char c);

bool
is_vchar(char c);

bool
is_tchar(char c);

bool
is_control_char(char c);

bool
is_obs_text(char c);

} // namespace http::protocol::grammar

#include <libhttp/protocol/grammar.ixx>

#endif
