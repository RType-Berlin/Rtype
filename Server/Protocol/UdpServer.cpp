#include "./UdpServer.hpp"

UdpServer::UdpServer(boost::asio::io_service &t_io_service)
    : m_io_service(t_io_service),
      m_socket(t_io_service, udp::endpoint(udp::v4(), 50000)) {
  m_flag = 0;
  receiveClient();

  m_thread = boost::thread([&t_io_service]() { t_io_service.run(); });
}

UdpServer::~UdpServer() { m_socket.close(); }

void UdpServer::sendMessage(const std::string &t_msg) {
  m_socket.send_to(boost::asio::buffer(t_msg, t_msg.size()), m_remoteEndpoint);
}

void UdpServer::handleSend(std::string t_msg,
                           const boost::system::error_code &t_error,
                           std::size_t t_size) {}

void UdpServer::receiveClient() {
  std::cout << "listening" << std::endl;
  m_socket.async_receive_from(
    boost::asio::buffer(m_recvBuffer), m_remoteEndpoint,
    boost::bind(&UdpServer::handleReceive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void UdpServer::handleReceive(const boost::system::error_code &t_error,
                              std::size_t t_size) {
  if (!t_error) {
    std::cout << "Received: '"
              << std::string(m_recvBuffer.begin(),
                             m_recvBuffer.begin() + t_size)
              << "' (" << t_error.message() << ")\n";
    if (std::string(m_recvBuffer.begin(), m_recvBuffer.begin() + t_size) !=
        "END\n") {
      m_flag = 1;
      receiveClient();
    }
  } else {
    m_socket.close();
  }
}