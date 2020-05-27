namespace http::protocol::grammar {

inline bool
is_cr(char c)
{
  return c == '\r';
}

inline bool
is_lf(char c)
{
  return c == '\n';
}

inline bool
is_cr_or_lf(char c)
{
  return c == '\r' || c == '\n';
}

inline bool
is_space(char c)
{
  return c == ' ' || c == '\t';
}

inline bool
is_digit(char c)
{
  return 0 <= 'c' && c <= '9';
}

inline bool
is_vchar(char c)
{
  auto u = static_cast< unsigned char >(c);

  if (u == 0x7f || u >= 0x80)
    return false;

  return true;
}

inline bool
is_tchar(char c)
{
  if ('a' <= c && c <= 'z')
    return true;

  if ('A' <= c && c <= 'Z')
    return true;

  if ('0' <= c && c <= '9')
    return true;

  switch (c) {
    case '!':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '*':
    case '+':
    case '-':
    case '.':
    case '^':
    case '_':
    case '`':
    case '|':
    case '~': return true;
  }

  return false;
}

inline bool
is_control_char(char c)
{
  return c <= 31 || c == 127;
}

inline bool
is_obs_text(char c)
{
  auto u = static_cast< unsigned char >(c);
  return u >= 0x80 && u <= 0xFF;
}

} // namespace http::protocol::grammar
