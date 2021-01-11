namespace http {
namespace protocol {

  template< typename Iterator >
  Iterator
  Basic_parser< Iterator >::parse(Iterator first,
                                  Iterator last,
                                  std::error_code& ec)
  {
    while (first != last) {
      expect_ = parse_char(first++, ec);

      if (expect_ == done || ec)
        return first;
    }

    if (first == last)
      ec = parse_error::incomplete_message;

    return first;
  }

  /**
   * @throws parser_not_ready Thrown if the parser is not ready.
   */
  template< typename Iterator >
  typename Basic_parser< Iterator >::Expectation
  Basic_parser< Iterator >::parse_char(Iterator it, std::error_code& ec)
  {
    char const c{ *it };

    // std::cout << "got: " << c << " (expect: " << expect_ << ")" << '\n';

    switch (expect_) {
      case expect_method_start:
        if (grammar::is_cr_or_lf(c)) // ignore before request
          return expect_method_start;
        if (!grammar::is_tchar(c)) {
          ec = parse_error::bad_method;
          return done;
        }
        method_.first = it;
        return expect_method;

      case expect_method:
        if (grammar::is_space(c)) {
          method_.last = it;
          return expect_target_start;
        }
        if (!grammar::is_tchar(c)) {
          ec = parse_error::bad_method;
          return done;
        }
        return expect_method;

      case expect_target_start:
        // TODO validate grammar
        target_.first = it;
        return expect_target;

      case expect_target:
        if (grammar::is_space(c)) {
          target_.last = it;
          return expect_version_h;
        }
        // TODO validate grammar
        return expect_target;

      case expect_version_h:
        if ('H' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_ht;

      case expect_version_ht:
        if ('T' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_htt;

      case expect_version_htt:
        if ('T' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_http;

      case expect_version_http:
        if ('P' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_slash;

      case expect_version_slash:
        if ('/' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_major;

      case expect_version_major:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        version_.first = it;
        return expect_version_period;

      case expect_version_period:
        if ('.' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_minor;

      case expect_version_minor:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        version_.last = it;
        return expect_version_cr;

      case expect_version_cr:
        if (!grammar::is_cr(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_version_lf;

      case expect_version_lf:
        if (!grammar::is_lf(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_header_start;

      case expect_response_version_h:
        if ('H' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_version_ht;

      case expect_response_version_ht:
        if ('T' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_version_htt;

      case expect_response_version_htt:
        if ('T' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_version_http;

      case expect_response_version_http:
        if ('P' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_version_slash;

      case expect_response_version_slash:
        if ('/' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_version_major;

      case expect_response_version_major:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        version_.first = it;
        return expect_response_version_period;

      case expect_response_version_period:
        if ('.' != c) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_version_minor;

      case expect_response_version_minor:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        version_.last = it;
        return expect_response_version_space;

      case expect_response_version_space:
        if (!grammar::is_space(c)) {
          ec = parse_error::bad_version;
          return done;
        }
        return expect_response_status_1;

      case expect_response_status_1:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_status;
          return done;
        }
        status_.first = status_.last = it;
        return expect_response_status_2;

      case expect_response_status_2:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_status;
          return done;
        }
        return expect_response_status_3;

      case expect_response_status_3:
        if (!grammar::is_digit(c)) {
          ec = parse_error::bad_status;
          return done;
        }
        return expect_response_status_space;

      case expect_response_status_space:
        if (' ' != c) {
          ec = parse_error::bad_status;
          return done;
        }
        status_.last = it;
        return expect_response_reason_start;

      case expect_response_reason_start:
        if (c != '\t' && c != ' ' && !grammar::is_vchar(c) &&
            grammar::is_obs_text(c)) {
          ec = parse_error::bad_reason;
          return done;
        }
        reason_.first = it;
        return expect_response_reason;

      case expect_response_reason:
        if ('\r' == c) {
          reason_.last = it;
          return expect_response_reason_lf;
        }
        if (c != '\t' && c != ' ' && !grammar::is_vchar(c) &&
            grammar::is_obs_text(c)) {
          ec = parse_error::bad_reason;
          return done;
        }
        return expect_response_reason;

      case expect_response_reason_lf:
        if (c != '\t' && c != ' ' && !grammar::is_vchar(c) &&
            grammar::is_obs_text(c)) {
          ec = parse_error::bad_reason;
          return done;
        }
        return expect_header_start;

      case expect_header_start:
        if (grammar::is_cr(c))
          return expect_terminating_lf;
        if (!grammar::is_tchar(c)) {
          ec = parse_error::bad_header_field;
          return done;
        }
        headers_.emplace_back();
        headers_.back().field.first = it;
        return expect_header_field;

      case expect_header_field:
        if (':' == c) {
          headers_.back().field.last = it;
          return expect_header_value_start;
        }
        if (!grammar::is_tchar(c)) {
          ec = parse_error::bad_header_field;
          return done;
        }
        return expect_header_field;

      case expect_header_value_start:
        // TODO check through grammar
        if (*it == ' ' || *it == '\t')
          return expect_header_value_start;

        headers_.back().value.first = it;
        if (grammar::is_cr(c)) {
          headers_.back().value.last = it;
          return expect_header_terminating_lf;
        }
        if (!grammar::is_space(c) && grammar::is_control_char(c)) {
          ec = parse_error::bad_header_value;
          return done;
        }
        return expect_header_value;

      case expect_header_value:
        if (grammar::is_cr(c)) {
          headers_.back().value.last = it;
          return expect_header_terminating_lf;
        }
        if (!grammar::is_space(c) && grammar::is_control_char(c)) {
          ec = parse_error::bad_header_value;
          return done;
        }
        return expect_header_value;

      case expect_header_terminating_lf:
        if (!grammar::is_lf(c)) {
          ec = parse_error::bad_header_value;
          return done;
        }
        return expect_header_start;

      case expect_terminating_lf:
        if (!grammar::is_lf(c))
          ec = parse_error::bad_terminator;
        return done;

      case done: throw parser_not_ready{};
    }

    assert(false);
    throw std::logic_error{ "parser in invalid state" };
  }

  template< typename Iterator >
  void
  extract_version(Basic_parser< Iterator > const& parser, message& m)
  {
    version v{ static_cast< unsigned short >(*parser.version().first - '0'),
               static_cast< unsigned short >(*parser.version().last - '0') };

    m.set_version(std::move(v));
  }

  template< typename Iterator >
  void
  extract_headers(Basic_parser< Iterator > const& parser, message& m)
  {
    header_collection headers;

    for (auto const& j : parser.headers()) {
      std::string key{ j.field.first, j.field.last };
      std::string value{ j.value.first, j.value.last };

      // Calling header.set() would replace any earlier set
      // header with the same key.
      headers.append(std::move(key), std::move(value));
    }

    m.set_headers(std::move(headers));
  }

  template< typename Iterator >
  void
  extract_method(Basic_request_parser< Iterator > const& parser, request& r)
  {
    std::string method{ parser.method().first, parser.method().last };
    r.set_method(std::move(method));
  }

  template< typename Iterator >
  void
  extract_target(Basic_request_parser< Iterator > const& parser, request& r)
  {
    std::string target{ parser.target().first, parser.target().last };

    r.set_target(std::move(target));

    auto target_uri =
      uri::try_parse_uri(parser.target().first, parser.target().last);

    if (target_uri)
      r.set_target_uri(std::move(*target_uri));
  }

  template< typename Iterator >
  void
  extract_status(Basic_response_parser< Iterator > const& parser, response& r)
  {
    auto digit1 = parser.status().first;
    auto digit2 = digit1 + 1;
    auto digit3 = digit1 + 2;

    unsigned long long cdig1 = (*digit1 - '0');
    unsigned long long cdig2 = (*digit2 - '0');
    unsigned long long cdig3 = (*digit3 - '0');

    r.set_status((cdig1 * 100) + (cdig2 * 10) + cdig3);
  }

  template< typename Iterator >
  void
  extract_reason(Basic_response_parser< Iterator > const& parser, response& r)
  {
    std::string reason{ parser.reason().first, parser.reason().last };
    r.set_reason(std::move(reason));
  }

  template< typename Iterator >
  void
  extract_message(Basic_parser< Iterator > const& parser, message& m)
  {
    extract_version(parser, m);
    extract_headers(parser, m);
  }

  template< typename Iterator >
  void
  extract_message(Basic_request_parser< Iterator > const& parser, request& r)
  {
    extract_message(parser, static_cast< message& >(r));
    extract_method(parser, r);
    extract_target(parser, r);
  }

  template< typename Iterator >
  void
  extract_message(Basic_response_parser< Iterator > const& parser, response& r)
  {
    extract_message(parser, static_cast< message& >(r));
    extract_status(parser, r);
    extract_reason(parser, r);
  }

  template< typename InputIterator >
  InputIterator
  parse_request(request& r, InputIterator first, InputIterator last)
  {
    Basic_request_parser< InputIterator > parser;
    return parse_request(r, parser, first, last);
  }

  template< typename InputIterator >
  InputIterator
  parse_request(request& r,
                InputIterator first,
                InputIterator last,
                std::error_code& ec)
  {
    Basic_request_parser< InputIterator > parser;
    return parse_request(r, parser, first, last, ec);
  }

  template< typename InputIterator >
  InputIterator
  parse_request(request& r,
                Basic_request_parser< InputIterator >& parser,
                InputIterator first,
                InputIterator last)
  {
    first = parser.parse(first, last);
    extract_message(parser, r);
    return first;
  }

  template< typename InputIterator >
  InputIterator
  parse_request(request& r,
                Basic_request_parser< InputIterator >& parser,
                InputIterator first,
                InputIterator last,
                std::error_code& ec)
  {
    first = parser.parse(first, last, ec);

    if (ec)
      return first;

    extract_message(parser, r);

    return first;
  }

  template< typename InputIterator >
  InputIterator
  parse_response(response& r, InputIterator first, InputIterator last)
  {
    Basic_response_parser< InputIterator > parser;
    return parse_response(r, parser, first, last);
  }

  template< typename InputIterator >
  InputIterator
  parse_response(response& r,
                 InputIterator first,
                 InputIterator last,
                 std::error_code& ec)
  {
    Basic_response_parser< InputIterator > parser;
    return parse_response(r, parser, first, last, ec);
  }

  template< typename InputIterator >
  InputIterator
  parse_response(response& r,
                 Basic_response_parser< InputIterator >& parser,
                 InputIterator first,
                 InputIterator last)
  {
    first = parser.parse(first, last);
    extract_message(parser, r);
    return first;
  }

  template< typename InputIterator >
  InputIterator
  parse_response(response& r,
                 Basic_response_parser< InputIterator >& parser,
                 InputIterator first,
                 InputIterator last,
                 std::error_code& ec)
  {
    first = parser.parse(first, last, ec);

    if (ec)
      return first;

    extract_message(parser, r);

    return first;
  }

} // namespace protocol
} // namespace http
