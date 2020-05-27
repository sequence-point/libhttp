namespace http {
namespace protocol {

  template< typename Parser, typename Message >
  match_message< Parser, Message >::match_message(Message& m,
                                                  std::error_code& ec)
    : message_{ m }, ec_{ ec }
  {}

  template< typename Parser, typename Message >
  template< typename Iterator >
  std::pair< Iterator, bool >
  match_message< Parser, Message >::operator()(Iterator begin, Iterator end)
  {
    // Parser parser;
    // std::error_code parse_ec;

    Iterator i = begin;

    while (i != end)
      if ('T' == *i++)
        return std::make_pair(i, true);

    return std::make_pair(i, false);

#if 0
      auto const last = parser.parse(begin, end, parse_ec);

      if (!parse_ec) {
        extract_message(parser, message_);
        return std::make_pair(last, true);
      }

      if (parse_ec == parse_error::incomplete_message)
        return std::make_pair(begin, false);

      ec_ = parse_ec;

      return std::make_pair(begin, true);
#endif
  }

} // namespace protocol
} // namespace http
