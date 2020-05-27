#include <libunittest/unittest.hxx>

#include <libhttp/protocol/media-type.hxx>

#include <string>

DEFINE_TEST("Parse")
{
  std::string media_type_str{ "text/plain" };

  auto media_type = http::protocol::media_type::try_parse(media_type_str);

  TEST_TRUE((bool)media_type);

  std::cout << "one" << media_type->type() << "\n";
  std::cout << "two" << media_type->subtype() << "\n";

  TEST_EQUAL(media_type->type(), "text");
  TEST_EQUAL(media_type->subtype(), "plain");
}

DEFINE_TEST("Parse with simple parameter")
{
  std::string media_type_str{ "text/plain;charset=utf-8" };

  auto media_type = http::protocol::media_type::try_parse(media_type_str);

  TEST_TRUE((bool)media_type);

  TEST_EQUAL(media_type->type(), "text");
  TEST_EQUAL(media_type->subtype(), "plain");

  std::cout << "thr" << media_type->type() << "\n";
  std::cout << "for" << media_type->subtype() << "\n";

  auto charset = media_type->params().at("charset");

  std::cout << "fiv" << charset << "\n";

  TEST_EQUAL(charset, "utf-8");
}

DEFINE_TEST("Parse with parameter with quoted string value")
{
  std::string media_type_str{ R"(text/plain;charset="name")" };
}

DEFINE_TEST("Parse complex")
{
  std::string media_type_str{ R"(text/plain;  charset="name";  lang=swe)" };

  auto media_type = http::protocol::media_type::try_parse(media_type_str);

  TEST_TRUE((bool)media_type);

  TEST_EQUAL(media_type->type(), "text");
  TEST_EQUAL(media_type->subtype(), "plain");

  auto charset = media_type->params().at("charset");
  auto lang = media_type->params().at("lang");

  TEST_EQUAL(charset, "name");
  TEST_EQUAL(charset, "swe");
}
