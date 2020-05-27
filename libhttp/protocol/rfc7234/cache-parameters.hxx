#ifndef libhttp__protocol__rfc7234__cache_parameters_hxx_
#define libhttp__protocol__rfc7234__cache_parameters_hxx_

namespace http {
namespace protocol {
  namespace rfc7234 {

    struct cache_parameters {
      optional< bool > is_private;
      optional< bool > is_public;
      optional< bool > must_revalidate;
      optional< bool > no_cache;
      optional< bool > no_store;
      optional< bool > no_transform;
      optional< bool > only_if_cached;
      optional< bool > proxy_revalidate;

      optional< uint32_t > max_age;
      optional< uint32_t > max_stale;
      optional< uint32_t > min_fres;
      optional< uint32_t > s_maxage;
    };

    string
    to_string(cache_parameters const& params)
    {
      // FIXME Implement.
      return {};
    }

  } // namespace rfc7234
} // namespace protocol
} // namespace http

#endif
