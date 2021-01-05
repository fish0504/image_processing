/**
* @brief �^��`
*/
#pragma once

using sendData_t = std::array<double, 3>; //data type:=double the number of data=14
//using sendData_t = std::array<double, 14>;
//originally position of center gravity (2D)+ lean(1D)+angles of eleven desirable angles(11D)=14
using recvData_t = std::array<double, 12>; //data type:=double the number of data=12
//! @brief �\���f�[�^�̌^
struct dispData_t {
	unsigned long frameCount;
	//cv::Mat1b image;
	cv::Mat3b image;
	cv::Point2d centroid;
	cv::Point2d worldPoints;
	cv::RotatedRect brect;
	std::vector< cv::Point > approx;
	recvData_t recvData;
};

constexpr std::size_t capacity = 3; //! spsc_queue�̃T�C�Y
template <typename T>
using spsc_queue = boost::lockfree::spsc_queue<T, boost::lockfree::capacity<capacity>>; //! �X���b�h�ԒʐM�p�̃��b�N�t���[�L���[�̌^