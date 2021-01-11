namespace http::server {

inline transaction::stream
transaction::alloc_stream()
{
  return stream{ &memory().alloc< asio::streambuf >() };
}

inline system::allocator&
transaction::memory()
{
  return allocator_;
}

inline system::extension_context&
transaction::extensions()
{
  return extensions_;
}

} // namespace http::server
