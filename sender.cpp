#include "sender.hpp"
using namespace boost::asio;
sender::sender(io_context& io_context,
               const ip::address& multicast_address)
  : endpoint_(multicast_address, multicast_port)
  , socket_(io_context, endpoint_.protocol())
  , timer_(io_context)
{
  do_send();
}

void
sender::do_send()
{

  socket_.async_send_to(
    buffer(
      boost::lexical_cast<std::string>(socket_.local_endpoint().port())),
    endpoint_,
    [this](boost::system::error_code ec, std::size_t /*length*/) {
      if (!ec)
        do_timeout();
    });
}

void
sender::do_timeout()
{
  timer_.expires_after(std::chrono::seconds(1));
  timer_.async_wait([this](boost::system::error_code ec) {
    if (!ec)
      do_send();
  });
}
