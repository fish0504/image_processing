#include "comunicate_dspace.h"

/**
* @brief �R���X�g���N�^
* @param[in] queSend ���M�f�[�^
* @param[out] queRecv ��M�f�[�^
* @param[in] destIP ���M��(dSpace)IP�A�h���X
* @param[in] destPort ���M��(dSpace)�|�[�g
* @param[in] srcPort ��M(PC)�|�[�g
*/
ComDspace::ComDspace(spsc_queue<sendData_t> &queSend, spsc_queue<recvData_t> &queRecv, const std::string &destIP, const std::string &destPort, const std::string &srcPort)
	: queSend_(queSend), queRecv_(queRecv)
	, destIP_(destIP), destPort_(destPort), srcPort_(srcPort)
	, isFinished_(false)
{
}

/**
* @brief dspace�փf�[�^���M
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
	std::array<double,num_of_array>v;
	for(int i=0;i<v.size();i++)v[i]=sin(1.0*i);
	sendData_t now=v;
	while (true) {
		//std::cout << isFinished_;
		
		queSend_.push(now);// for test UDP communication
		if (isFinished_) {
			break; 
		}
		if (!queSend_.pop(objInfo)) {
			std::this_thread::yield();
			continue;
		}
		objInfo[0] = 5;
		//std::cout << "onje " << objInfo[0] << std::endl;
		
		if(int sentBytes=socket.send_to(boost::asio::buffer(objInfo), receiver_endpoint)){
			printf("sent %d bytes\n",sentBytes);
		};
		printf("sended objinfo {");
		for(auto now:objInfo){
			printf("%lf, ",now);
		}
		printf("}\n");
	}
	//to stop receiving, send data to myself.
	boost::asio::ip::udp::endpoint destination(
	boost::asio::ip::address::from_string("127.0.0.1"), std::stoi(srcPort_));
	socket.send_to(boost::asio::buffer(objInfo.data(), objInfo.size()), destination);
	std::cout << "Finish send to dSpace" << std::endl;
}

/**
* @brief dspace����f�[�^��M
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