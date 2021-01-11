namespace http::models {

template< typename M >
void
send(server::transaction& tx, M const& model, bool head_request)
{
  set_metadata(tx.response(), get_metadata(model));

  auto content_stream = tx.alloc_stream();
  model.write_to(content_stream);

  tx.send(content_stream);
}

} // namespace http::models
