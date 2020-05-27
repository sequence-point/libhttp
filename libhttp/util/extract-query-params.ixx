namespace http {
namespace util {

  inline void
  extract_query_parameters::on_request(exchange& ex) const
  {
    auto params = parse_query_params(ex.request().query());
    ex.extend(&ex.alloc_emplace< query_parameters >(std::move(params)));
  }

} // namespace util
} // namespace http
