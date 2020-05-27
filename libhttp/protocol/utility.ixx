namespace http {
namespace protocol {

  inline void
  normalize_header_name(std::string& name)
  {
    std::locale loc{ "C" };

    for (auto& c : name)
      c = std::tolower(c, loc);
  }

} // namespace protocol
} // namespace http
