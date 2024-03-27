#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/multicast.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/chrono.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <iostream>
#include <unordered_map>

#define multicast_port 30001
class receiver
{
public:
  receiver(boost::asio::io_context& io_context,
           const boost::asio::ip::address& multicast_address);

private:
  void do_receive();
  void print_copies();

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  std::array<char, 1024> data_;
  std::unordered_map<int, boost::posix_time::ptime> copies_alive;
};
