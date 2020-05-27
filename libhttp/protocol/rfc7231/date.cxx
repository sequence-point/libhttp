#include <libunittest/libunittest.hxx>

#include <libhttp/protocol/message.hxx>
#include <libhttp/protocol/rfc7231/date.hxx>

#include <chrono>

namespace {

// Work-around since message has a protected constructor.
class constructible_message : public http::protocol::message {
};

} // namespace

DEFINE_TEST("Date serializes correctly")
{
  static constexpr char expected[] = "Mon, 15 Apr 2019 16:37:27 GMT";

  auto time_since_epoch = std::chrono::duration_cast< http::duration >(
    std::chrono::seconds{ 1555346247 });

  std::chrono::system_clock::time_point time_point{ time_since_epoch };

  auto string = http::protocol::rfc7231::Date::to_string(time_point);

  TEST_EQUAL(string, expected);
}

DEFINE_TEST("Date is set correctly on message")
{
  static constexpr char expected[] = "Mon, 15 Apr 2019 16:37:27 GMT";

  auto time_since_epoch = std::chrono::duration_cast< http::duration >(
    std::chrono::seconds{ 1555346247 });

  http::time_point time_point{ time_since_epoch };

  constructible_message message;
  http::protocol::set< http::protocol::rfc7231::Date >(message, time_point);

  auto header = message.headers().get_one("date");

  TEST_TRUE((bool)header);
  TEST_EQUAL(*header, expected);
}
