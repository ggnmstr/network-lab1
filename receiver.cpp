#include "receiver.hpp"
using namespace boost::asio;

receiver::receiver(io_context& io_context,
                   const ip::address& multicast_address)
  : socket_(io_context)
{
  ip::udp::endpoint listen_endpoint;
  listen_endpoint.port(multicast_port);
  if (multicast_address.is_v4()) {
    listen_endpoint.address(ip::address_v4::any());
    // listen_endpoint.address(multicast_address);
  } else {
    listen_endpoint.address(ip::address_v6::any());
  }
  // listen_endpoint.address(multicast_address);
  socket_.open(listen_endpoint.protocol());
  socket_.set_option(ip::udp::socket::reuse_address(true));
  socket_.bind(listen_endpoint);

  socket_.set_option(ip::multicast::join_group(multicast_address));

  do_receive();
}

void
receiver::do_receive()
{
  socket_.async_receive_from(
    buffer(data_),
    sender_endpoint_,
    [this](boost::system::error_code ec, std::size_t length) {
      if (!ec) {
        copies_alive[sender_endpoint_.port()] =
          boost::posix_time::second_clock::local_time();
        print_copies();
        do_receive();
      }
    });
}
void
receiver::print_copies()
{
  std::cout << "----------------------------" << std::endl;
  boost::posix_time::ptime cur_time =
    boost::posix_time::second_clock::local_time();
  for (auto& it : copies_alive) {
    if (it.first == 0)
      continue;
    auto diff = (cur_time - it.second).total_seconds();
    if (diff <= 5) {
      std::cout << "Copy " << it.first << std::endl;
    }
  }
  std::cout << "----------------------------" << std::endl;
}
