#ifndef libhttp__protocol__header_collection_hxx_
#define libhttp__protocol__header_collection_hxx_

#include <libhttp/protocol/optional.hxx>
#include <libhttp/protocol/utility.hxx>

#include <map>
#include <string>
#include <vector>

namespace http {
namespace protocol {

  class header_collection {
  public:
    using const_iterator =
      std::multimap< std::string, std::string >::const_iterator;

    void
    set(std::string key, std::string value);

    void
    append(std::string key, std::string value);

    void
    erase(std::string key);

    bool
    contains(std::string key) const;

    std::vector< std::string >
    get(std::string key) const;

    std::optional< std::string >
    get_one(std::string key) const;

    const_iterator
    begin() const;

    const_iterator
    cbegin() const;

    const_iterator
    end() const;

    const_iterator
    cend() const;

  private:
    std::multimap< std::string, std::string > key_value_store_;
  };

} // namespace protocol
} // namespace http

#include <libhttp/protocol/header-collection.ixx>

#endif
