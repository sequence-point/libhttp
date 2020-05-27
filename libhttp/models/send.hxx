#ifndef libhttp__models__send_hxx_
#define libhttp__models__send_hxx_

#include <libhttp/server/transaction.hxx>

#include <libhttp/models/metadata.hxx>

namespace http::models {

template< typename M >
void
send(server::transaction& tx, M const& model, bool head_request);

} // namespace http::models

#include <libhttp/models/send.txx>

#endif
