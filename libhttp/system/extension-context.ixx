namespace http {
namespace system {

  inline void*
  extension_context::use(std::type_index const& key) const
  {
    auto it = extensions_.find(key);

    if (it == extensions_.end())
      throw extension_not_found{};

    return it->second;
  }

  inline void
  extension_context::extend(std::type_index const& key, void* ptr)
  {
    if (0 != extensions_.count(key))
      throw duplicate_extension{};

    if (!ptr)
      throw std::invalid_argument{ "invalid pointer" };

    extensions_.emplace(key, ptr);
  }

  inline void
  extension_context::erase_extension(std::type_index const& key)
  {
    extensions_.erase(key);
  }

} // namespace system
} // namespace http
