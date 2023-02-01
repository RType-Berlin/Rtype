#ifndef PROTOCOL_UDPCLIENT_HPP_
#define PROTOCOL_UDPCLIENT_HPP_

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../../Protocol/IProtocol.hpp"

using boost::asio::ip::udp;

class UdpClient : public IProtocol {
 public:
  UdpClient(boost::asio::io_service &t_io_service, const std::string &t_host,
            const std::string &t_port, const std::size_t &t_ownPort);
  ~UdpClient();
  void sendMessage(const std::string &);
  void receiveClient();
  void handleReceive(const boost::system::error_code &error, std::size_t size);
  void handleSend(std::string t_msg, const boost::system::error_code &t_error,
                  std::size_t t_size);

 private:
  udp::socket m_socket;
  udp::endpoint m_remoteEndpoint;
  std::array<char, 1024> m_recvBuffer;
  boost::asio::io_service &m_io_service;
};

#endif /* !PROTOCOL_UDPCLIENT_HPP_ */
