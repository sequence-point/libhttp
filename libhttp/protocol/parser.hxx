#ifndef libhttp__protocol__parser_hxx_
#define libhttp__protocol__parser_hxx_

#include <libhttp/protocol/error.hxx>
#include <libhttp/protocol/grammar.hxx>
#include <libhttp/protocol/http-version.hxx>
#include <libhttp/protocol/message.hxx>
#include <libhttp/protocol/request.hxx>
#include <libhttp/protocol/response.hxx>
#include <libhttp/protocol/utility.hxx>

#include <liburi/uri-parser.hxx>

#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace http {
namespace protocol {

  // TODO rename basic_parser
  template< typename Iterator >
  class Basic_parser {
  public:
    struct Iterator_pair {
      Iterator first{};
      Iterator last{};
    };

    struct Header {
      Iterator_pair field;
      Iterator_pair value;
    };

    Iterator
    parse(Iterator first, Iterator last, std::error_code& ec);

    Iterator_pair const&
    version() const
    {
      return version_;
    }

    std::vector< Header > const&
    headers() const
    {
      return headers_;
    }

  protected:
    // TODO prefix request-expectations with req_.
    enum Expectation {
      // Requests.
      expect_method_start = 10,
      expect_method,

      expect_target_start = 20,
      expect_target,

      expect_version_h = 30,
      expect_version_ht,
      expect_version_htt,
      expect_version_http,
      expect_version_slash,
      expect_version_major,
      expect_version_period,
      expect_version_minor,
      expect_version_cr,
      expect_version_lf,

      // Response
      expect_response_version_h,
      expect_response_version_ht,
      expect_response_version_htt,
      expect_response_version_http,
      expect_response_version_slash,
      expect_response_version_major,
      expect_response_version_period,
      expect_response_version_minor,
      expect_response_version_space,

      expect_response_status_1,
      expect_response_status_2,
      expect_response_status_3,
      expect_response_status_space,

      expect_response_reason_start,
      expect_response_reason,
      expect_response_reason_lf,

      // Headers.
      expect_header_start,
      expect_header_field,
      expect_header_value_start,
      expect_header_value,
      expect_header_terminating_lf,

      expect_terminating_lf,

      done = 9999
    };

    explicit Basic_parser(Expectation expect) : expect_{ expect }
    {}

    Basic_parser(Basic_parser const&) = default;
    Basic_parser(Basic_parser&&) = default;

    Basic_parser&
    operator=(Basic_parser const&) = default;

    Basic_parser&
    operator=(Basic_parser&&) = default;

    Expectation expect_;

    Iterator_pair method_{};
    Iterator_pair target_{};
    Iterator_pair version_{};

    Iterator_pair status_{};
    Iterator_pair reason_{};

    std::vector< Header > headers_;

  private:
    Expectation
    parse_char(Iterator it, std::error_code& ec);
  };

  // TODO rename basic_requst_parser
  template< typename Iterator >
  class Basic_request_parser : public Basic_parser< Iterator > {
  public:
    Basic_request_parser()
      : Basic_parser< Iterator >{
          Basic_parser< Iterator >::expect_method_start
        }
    {}

    typename Basic_parser< Iterator >::Iterator_pair const&
    method() const
    {
      return Basic_parser< Iterator >::method_;
    }

    typename Basic_parser< Iterator >::Iterator_pair const&
    target() const
    {
      return Basic_parser< Iterator >::target_;
    }
  };

  // TODO rename basic_response_parser
  template< typename Iterator >
  class Basic_response_parser : public Basic_parser< Iterator > {
  public:
    Basic_response_parser()
      : Basic_parser< Iterator >{
          Basic_parser< Iterator >::expect_response_version_h
        }
    {}

    typename Basic_parser< Iterator >::Iterator_pair const&
    status() const
    {
      return Basic_parser< Iterator >::status_;
    }

    typename Basic_parser< Iterator >::Iterator_pair const&
    reason() const
    {
      return Basic_parser< Iterator >::reason_;
    }
  };

  template< typename Iterator >
  void
  extract_version(Basic_parser< Iterator > const& parser, message& m);

  template< typename Iterator >
  void
  extract_headers(Basic_parser< Iterator > const& parser, message& m);

  template< typename Iterator >
  void
  extract_method(Basic_request_parser< Iterator > const& parser, request& r);

  template< typename Iterator >
  void
  extract_target(Basic_request_parser< Iterator > const& parser, request& r);

  template< typename Iterator >
  void
  extract_status(Basic_response_parser< Iterator > const& parser, response& r);

  template< typename Iterator >
  void
  extract_reason(Basic_response_parser< Iterator > const& parser, response& r);

  template< typename Iterator >
  void
  extract_message(Basic_parser< Iterator > const& parser, message& m);

  template< typename Iterator >
  void
  extract_message(Basic_request_parser< Iterator > const& parser, request& r);

  template< typename Iterator >
  void
  extract_message(Basic_response_parser< Iterator > const& parser, response& r);

  template< typename InputIterator >
  InputIterator
  parse_request(request& r, InputIterator first, InputIterator last);

  template< typename InputIterator >
  InputIterator
  parse_request(request& r,
                InputIterator first,
                InputIterator last,
                std::error_code& ec);

  template< typename InputIterator >
  InputIterator
  parse_request(request& r,
                Basic_request_parser< InputIterator >& parser,
                InputIterator first,
                InputIterator last);

  template< typename InputIterator >
  InputIterator
  parse_request(request& r,
                Basic_request_parser< InputIterator >& parser,
                InputIterator first,
                InputIterator last,
                std::error_code& ec);

  template< typename InputIterator >
  InputIterator
  parse_response(response& r,
                 InputIterator first,
                 InputIterator last,
                 std::error_code& ec);

  template< typename InputIterator >
  InputIterator
  parse_response(response& r,
                 Basic_response_parser< InputIterator >& parser,
                 InputIterator first,
                 InputIterator last);

  template< typename InputIterator >
  InputIterator
  parse_response(response& r,
                 Basic_response_parser< InputIterator >& parser,
                 InputIterator first,
                 InputIterator last,
                 std::error_code& ec);

  class parser_not_ready : public std::logic_error {
  public:
    parser_not_ready() : std::logic_error{ "parser not ready" }
    {}
  };

} // namespace protocol
} // namespace http

#include <libhttp/protocol/parser.txx>

#endif
