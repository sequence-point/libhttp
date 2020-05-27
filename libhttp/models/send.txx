namespace http::models {

template< typename M >
void
send(server::transaction& tx, M const& model, bool head_request)
{
  set_metadata(tx.response(), get_metadata(model));

  if (head_request) {
    std::stringstream tmp_content;
    model.write_to(tmp_content);

    // set< protocol::rfc7231::ContentLength >(tx.response(), );
    // TODO set Content-Length
    tx.response().set_header("X-Content-Length", "0");
    tx.send({});

    return;
  }

  auto content_stream = tx.alloc_stream();
  model.write_to(content_stream);
  tx.send(content_stream);
}

} // namespace http::models
