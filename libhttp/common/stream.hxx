#ifndef libhttp__common__stream_hxx_
#define libhttp__common__stream_hxx_

#include <asio.hpp>

namespace http {
namespace common {
  namespace io {

    class stream {
    public:
      using executor_type = asio::executor;

      using read_handler = std::function< void(std::error_code, std::size_t) >;

      using write_handler = std::function< void(std::error_code, std::size_t) >;

      template< typename StreamImplementation >
      stream(StreamImplementation stream)
        : stream_{ new stream_implementation< StreamImplementation >{
            std::move(stream) } }
      {}

      asio::executor
      get_executor();

      std::size_t
      read_some(asio::mutable_buffer const& buffer);

      std::size_t
      read_some(asio::mutable_buffer const& buffer, std::error_code& ec);

      void
      async_read_some(asio::mutable_buffer const& buffer, read_handler handler);

      std::size_t
      write_some(asio::const_buffer const& buffer);

      std::size_t
      write_some(asio::const_buffer const& buffer, std::error_code& ec);

      void
      async_write_some(asio::const_buffer const& buffer, write_handler handler);

    private:
      struct stream_concept {
        virtual ~stream_concept() = default;

        virtual asio::executor
        get_executor() = 0;

        virtual std::size_t
        read_some(asio::const_buffer const& buffer) = 0;

        virtual std::size_t
        read_some(asio::const_buffer const& buffer, std::error_code& ec) = 0;

        virtual void
        async_read_some(asio::mutable_buffer const& buffer,
                        write_handler& handler) = 0;

        virtual std::size_t
        write_some(asio::const_buffer const& buffer) = 0;

        virtual std::size_t
        write_some(asio::const_buffer const& buffer, std::error_code& ec) = 0;

        virtual void
        async_write_some(asio::const_buffer const& buffer,
                         write_handler& handler) = 0;
      };

      template< typename StreamImplementation >
      struct stream_implementation : stream_concept {
        stream_implementation(StreamImplementation stream)
          : stream{ std::move(stream) }
        {}

        asio::executor
        get_executor() override
        {
          return stream.get_executor();
        }

        std::size_t
        read_some(asio::const_buffer const& buffer) override
        {
          return stream.read_some(buffer);
        }

        std::size_t
        read_some(asio::const_buffer const& buffer,
                  std::error_code& ec) override
        {
          return stream.read_some(buffer, ec);
        }

        void
        async_read_some(asio::mutable_buffer const& buffer,
                        write_handler& handler) override
        {
          stream.async_read_some(buffer, std::move(handler));
        }

        std::size_t
        write_some(asio::const_buffer const& buffer) override
        {
          return stream.write_some(buffer);
        }

        std::size_t
        write_some(asio::const_buffer const& buffer,
                   std::error_code& ec) override
        {
          return stream.write_some(buffer, ec);
        }

        void
        async_write_some(asio::const_buffer const& buffer,
                         write_handler& handler) override
        {
          stream.async_write_some(buffer, std::move(handler));
        }

        StreamImplementation stream;
      };

      std::unique_ptr< stream_concept > stream_;
    };

  } // namespace io
} // namespace common
} // namespace http

#endif
