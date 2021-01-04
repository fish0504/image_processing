/**
* @brief�communication with dSpace
*/

#pragma once

/**
* @struct comDspace
* @brief class for communication with dSpace
*/
struct ComDspace{

private:
	using Udp = boost::asio::ip::udp;
	spsc_queue<sendData_t> &queSend_; //! lock free queue for sending data
	spsc_queue<recvData_t> &queRecv_; //! lock free queue for received data
	const std::string destIP_; //! the IP address of the dSpace
	const std::string destPort_; //! the port of the dSpace
	const std::string srcPort_; //! the port of PC for receiving
	bool isFinished_; //! flag to finish processing

public:
	/**

	* @brief constructor
	* @param[in] queSend sending data
	* @param[out] queRecv received data
	* @param[in] destIP the IP address of the dSpace
	* @param[in] destPort the port of the dSpace
	* @param[in] srcPort the port of PC for receiving

	*/
	ComDspace(spsc_queue<sendData_t> &queSend, spsc_queue<recvData_t> &queRecv, const std::string &destIP, const std::string &destPort, const std::string &srcPort);

	/**
	* @brief sending data to dspace
	*/
	void sendData();

	/**
	* @brief receiving date from dspace
	*/
	void recieveData();

	/**
	* @brief confirm the flag for process finishing
	*/
	int finishProc() {
		isFinished_ = true;
		return 0;
	}

	/**
	* @brief�@�I���t���O�̊m�F
	*/
	bool isFinishProc() {
		return isFinished_;
	}
};