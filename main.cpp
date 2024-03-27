// Разработать приложение, обнаруживающее копии себя в локальной сети с помощью
// обмена multicast UDP сообщениями. Приложение должно отслеживать моменты
// появления и исчезновения других копий себя в локальной сети и при изменениях
// выводить список IP адресов "живых" копий.

// Адрес multicast-группы необходимо передавать параметром в приложение.
// Приложение должно поддерживать работу как в IPv4 так и в IPv6 сетях,
// выбирая протокол автоматически в зависимости от переданного адреса группы.

#include "receiver.hpp"
#include "sender.hpp"
#include <iostream>
#include <thread>

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: ./app.out <multicast_address>\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    ./app.out 239.255.0.1\n";
    std::cerr << "  For IPv6, try:\n";
    std::cerr << "    ./app.out ff31::8000:1234\n";
    return 0;
  }
  try {
    char key = 0;
    if (argv[1][0] == '-' && (argv[1][1] == 's' || argv[1][1] == 'r')) {
      key = argv[1][1];
    }
    if (key == 's') {
      boost::asio::ip::address multicast_address =
        boost::asio::ip::make_address(argv[2]);
      boost::asio::io_context io_context2;
      sender s(io_context2, multicast_address);
      std::thread t2{ [&io_context2]() { io_context2.run(); } };
      t2.join();
    } else if (key == 'r') {

      boost::asio::ip::address multicast_address =
        boost::asio::ip::make_address(argv[2]);
      boost::asio::io_context io_context;
      receiver r(io_context, multicast_address);
      std::thread t1{ [&io_context]() { io_context.run(); } };
      t1.join();
    } else {
      boost::asio::ip::address multicast_address =
        boost::asio::ip::make_address(argv[1]);
      boost::asio::io_context io_context;
      boost::asio::io_context io_context2;
      sender s(io_context2, multicast_address);
      receiver r(io_context, multicast_address);
      std::thread t1{ [&io_context]() { io_context.run(); } };
      std::thread t2{ [&io_context2]() { io_context2.run(); } };
      t1.join();
      t2.join();
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
