namespace http {
namespace protocol {

  template< typename Iterator >
  std::tuple< Iterator, Iterator >
  trim_whitespace(Iterator first, Iterator last)
  {
    while (first != last && grammar::is_space(*first))
      ++first;

    while (last != first && grammar::is_space(*last))
      --last;

    return { first, last };
  }

} // namespace protocol
} // namespace http
