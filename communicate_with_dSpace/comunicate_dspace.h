/**
* @brief�@dSpace�Ƃ̒ʐM(����M)
*/

#pragma once

/**
* @struct comDspace
* @brief�@dSpace�Ƃ̒ʐM�N���X
*/
struct ComDspace{

private:
	using Udp = boost::asio::ip::udp;
	spsc_queue<sendData_t> &queSend_; //! ���M�f�[�^�̃��b�N�t���[�L���[
	spsc_queue<recvData_t> &queRecv_; //! ��M�f�[�^�̃��b�N�t���[�L���[
	const std::string destIP_; //! ���M��(dSpace)IP�A�h���X
	const std::string destPort_; //! ���M��̃|�[�g
	const std::string srcPort_; //! ���M���̃|�[�g�i��M�p�j

	bool isFinished_; //! �������I������t���O

public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] queSend ���M�f�[�^
	* @param[out] queRecv ��M�f�[�^
	* @param[in] destIP 
	* @param[in] destPort ���M��(dSpace)�|�[�g
	* @param[in] srcPort ��M(PC)�|�[�g
	*/
	ComDspace(spsc_queue<sendData_t> &queSend, spsc_queue<recvData_t> &queRecv, const std::string &destIP, const std::string &destPort, const std::string &srcPort);

	/**
	* @brief dspace�փf�[�^���M
	*/
	void sendData();

	/**
	* @brief dspace����f�[�^��M
	*/
	void recieveData();

	/**
	* @brief�@�������I������
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