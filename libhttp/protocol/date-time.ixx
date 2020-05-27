namespace http {
namespace protocol {

  inline std::string
  format_http_date(std::chrono::system_clock::time_point const& time_point)
  {
    static constexpr const char time_format[] = "%a, %d %b %Y %H:%M:%S GMT";

    std::time_t now_c = std::chrono::system_clock::to_time_t(time_point);

    std::stringstream str;
    str.imbue(std::locale{});
    str << std::put_time(std::gmtime(&now_c), time_format);

    return str.str();
  }

} // namespace protocol
} // namespace http
