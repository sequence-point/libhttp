#include <libunittest/unittest.hxx>

#include <libhttp/util/query-params.hxx>

DEFINE_TEST("key and value")
{
  auto params = http::util::parse_query_params("hello=world");

  TEST_EQUAL(params.size(), 1);
  TEST_EQUAL(params["hello"], "world");
}

DEFINE_TEST("multiple keys and value values")
{
  auto params = http::util::parse_query_params("hello=world&hej=världen");

  TEST_EQUAL(params.size(), 2);
  TEST_EQUAL(params["hello"], "world");
  TEST_EQUAL(params["hej"], "världen");
}

DEFINE_TEST("empty key")
{
  auto params = http::util::parse_query_params("=value");

  TEST_EQUAL(params.size(), 1);
  TEST_EQUAL(params[""], "value");
}

DEFINE_TEST("empty value")
{
  auto params = http::util::parse_query_params("key=");

  TEST_EQUAL(params.size(), 1);
  TEST_EQUAL(params["key"], "");
}

DEFINE_TEST("empty key and value")
{
  auto params = http::util::parse_query_params("=");

  TEST_EQUAL(params.size(), 0);
}
