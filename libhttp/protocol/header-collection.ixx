namespace http {
namespace protocol {

  inline void
  header_collection::set(std::string key, std::string value)
  {
    normalize_header_name(key);
    key_value_store_.erase(key);
    key_value_store_.emplace(std::move(key), std::move(value));
  }

  inline void
  header_collection::append(std::string key, std::string value)
  {
    normalize_header_name(key);
    key_value_store_.emplace(std::move(key), std::move(value));
  }

  inline void
  header_collection::erase(std::string key)
  {
    normalize_header_name(key);
    key_value_store_.erase(key);
  }

  inline bool
  header_collection::contains(std::string key) const
  {
    normalize_header_name(key);
    return key_value_store_.find(key) != key_value_store_.end();
  }

  inline std::vector< std::string >
  header_collection::get(std::string key) const
  {
    normalize_header_name(key);

    std::vector< std::string > strings;

    auto lower = key_value_store_.lower_bound(key);
    auto upper = key_value_store_.upper_bound(key);

    while (lower != upper) {
      strings.push_back(lower->second);
      ++lower;
    }

    return strings;
  }

  inline optional< std::string >
  header_collection::get_one(std::string key) const
  {
    normalize_header_name(key);

    auto lower = key_value_store_.lower_bound(key);
    auto upper = key_value_store_.upper_bound(key);

    if (lower != upper)
      return lower->second;

    return {};
  }

  inline header_collection::const_iterator
  header_collection::begin() const
  {
    return key_value_store_.begin();
  }

  inline header_collection::const_iterator
  header_collection::cbegin() const
  {
    return key_value_store_.cbegin();
  }

  inline header_collection::const_iterator
  header_collection::end() const
  {
    return key_value_store_.end();
  }

  inline header_collection::const_iterator
  header_collection::cend() const
  {
    return key_value_store_.cend();
  }

} // namespace protocol
} // namespace http
