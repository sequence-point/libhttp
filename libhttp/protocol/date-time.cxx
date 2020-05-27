#include <libunittest/unittest.hxx>

#include <libhttp/protocol/date-time.hxx>

static constexpr char example[] = "Mon, 15 Apr 2019 16:37:27 GMT";

DEFINE_TEST("Format HTTP date/time")
{
  using namespace std::chrono;

  auto time_since_epoch = duration_cast< system_clock::duration >(
    std::chrono::seconds{ 1555346247LL });

  system_clock::time_point tp{ time_since_epoch };

  auto text = http::protocol::format_http_date(tp);

  TEST_EQUAL(text, example);
}
