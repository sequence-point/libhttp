#include <libunittest/unittest.hxx>

DEFINE_TEST("EmptyBuffer")
{
  TEST_NOT_IMPLEMENTED;
  auto bytes_transferred = 0;
  TEST_EQUAL(bytes_transferred, 40);
}

DEFINE_TEST("PartiallyFilledBuffer")
{
  TEST_NOT_IMPLEMENTED;
  auto bytes_transferred = 0;
  TEST_EQUAL(bytes_transferred, 40);
}

DEFINE_TEST("CompletelyFilledBuffer")
{
  TEST_NOT_IMPLEMENTED;
  auto bytes_transferred = 40;
  TEST_EQUAL(bytes_transferred, 0);
}
