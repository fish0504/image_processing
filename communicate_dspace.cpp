#include "comunicate_dspace.h"

/**
* @brief constructor
* @param[in] queSend sending data
* @param[out] queRecv received data
* @param[in] destIP the IP address of the dSpace
* @param[in] destPort the port of the dSpace
* @param[in] srcPort the port of PC for receiving
*/
ComDspace::ComDspace(spsc_queue<sendData_t> &queSend, spsc_queue<recvData_t> &queRecv, const std::string &destIP, const std::string &destPort, const std::string &srcPort)
	: queSend_(queSend), queRecv_(queRecv)
	, destIP_(destIP), destPort_(destPort), srcPort_(srcPort)
	, isFinished_(false)
{
}

/**
* @brief sending the data to dspace
*/
void ComDspace::sendData()
{
	sendData_t objInfo;

	boost::asio::io_service io_service;
	Udp::resolver resolver(io_service);
	Udp::resolver::query query(Udp::v4(), destIP_, destPort_);
	Udp::endpoint receiver_endpoint = *resolver.resolve(query);
	Udp::socket socket(io_service);
	socket.open(Udp::v4());
	while (true) {
		//std::cout << isFinished_;
		if (isFinished_) {
			break; 
		}
		if (!queSend_.pop(objInfo)) {
			std::this_thread::yield();
			continue;
		}
		//std::cout << "onje\:" << objInfo[0] << std::endl;
		//objInfo[0] = 5;
		socket.send_to(boost::asio::buffer(objInfo), receiver_endpoint);
	}
	//to stop receiving, send the data to myself
	boost::asio::ip::udp::endpoint destination(
		boost::asio::ip::address::from_string("127.0.0.1"), std::stoi(srcPort_));
	socket.send_to(boost::asio::buffer(objInfo.data(), objInfo.size()), destination);
	std::cout << "FInish send to dSpace" << std::endl;
}

/**
* @brief receiving the data from dspace
*/
void ComDspace::recieveData()
{

	boost::asio::io_service io_service;
	Udp::socket socket(io_service, Udp::endpoint(Udp::v4(), std::stoi(srcPort_)));
	Udp::endpoint remote_endpoint;
	boost::system::error_code error;
	recvData_t recv_buf;
	size_t len;

	while (!isFinished_) {
		len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

		queRecv_.push(recv_buf);
	}
}