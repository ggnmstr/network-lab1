#pragma once
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#define multicast_port 30001

class sender
{
public:
  sender(boost::asio::io_context& io_context,
         const boost::asio::ip::address& multicast_address);

private:
  void do_send();

  void do_timeout();

  boost::asio::ip::udp::endpoint endpoint_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::steady_timer timer_;
  std::string message_;
};
