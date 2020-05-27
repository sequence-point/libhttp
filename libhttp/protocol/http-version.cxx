#include <libunittest/unittest.hxx>

#include <libhttp/protocol/http-version.hxx>

DEFINE_TEST("Comparison")
{
  {
    http::protocol::version v1{ 1, 1 };
    http::protocol::version v2{ 1, 1 };

    TEST_EQUAL(v1, v2);
  }

  {
    http::protocol::version v1{ 1, 1 };
    http::protocol::version v2{ 1, 0 };

    TEST_DIFFERENT(v1, v2);
  }
}

DEFINE_TEST("to_string returns expected result")
{
  http::protocol::version v{ 2, 3 };

  auto text = to_string(v);

  TEST_EQUAL(text, "HTTP/2.3");
}
