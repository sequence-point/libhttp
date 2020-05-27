#include <libunittest/unittest.hxx>

#include <libhttp/protocol/rfc7232/entity-tag.hxx>

#include <string>

DEFINE_TEST("Entity tag comparison")
{
  using namespace http::protocol::rfc7232;

  // Test cases taken from Section 2.3.3 of RFC7232.

  {
    entity_tag etag1{ "1", entity_tag::weak };
    entity_tag etag2{ "1", entity_tag::weak };

    TEST_EQUAL(false, strong_compare(etag1, etag2));
    TEST_EQUAL(true, weak_compare(etag1, etag2));
  }

  {
    entity_tag etag1{ "1", entity_tag::weak };
    entity_tag etag2{ "2", entity_tag::weak };

    TEST_EQUAL(false, strong_compare(etag1, etag2));
    TEST_EQUAL(false, weak_compare(etag1, etag2));
  }

  {
    entity_tag etag1{ "1", entity_tag::weak };
    entity_tag etag2{ "1", entity_tag::strong };

    TEST_EQUAL(false, strong_compare(etag1, etag2));
    TEST_EQUAL(true, weak_compare(etag1, etag2));
  }

  {
    entity_tag etag1{ "1", entity_tag::strong };
    entity_tag etag2{ "1", entity_tag::strong };

    TEST_EQUAL(true, strong_compare(etag1, etag2));
    TEST_EQUAL(true, weak_compare(etag1, etag2));
  }
}
