#ifndef libhttp__client__resumable_hxx_
#define libhttp__client__resumable_hxx_

namespace http {
namespace client {

  class resumable {
  public:
    virtual void
    resume() = 0;

  protected:
    resumable() = default;
    ~resumable() = default;
  };
} // namespace client
} // namespace http

#endif
