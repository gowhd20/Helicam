#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include "Socket.h"
#include <stdio.h>
#include <string>
#include <winsock2.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <string.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//SampleListener toDisplay;
SSocket::SSocket()
{
	len = 0;
	counter = 0;
	sec = 0.0;
	fps = 0.0;
	slen = sizeof(si_other);
	slen_s = sizeof(si_other_s);
	recv_len = 0;
	recv_len_s = 0;
	fflush(stdout);
	img = NULL;
}

SSocket::~SSocket()
{
	closesocket(s);
	WSACleanup();
}

BOOL SSocket::udpConnect()
{
	//Initialise winsock
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		return 0;
		exit(EXIT_FAILURE);
	}
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
	{
		return 2;
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( PORT );

	//Bind

	if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		return 3;
		//printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//puts("Bind done");
	time(&start);
	return 1;
}

BOOL SSocket::udpConnectForCommand()
{

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		return 0;
		exit(EXIT_FAILURE);
	}
	
	//Create a socket
	if((s_s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
	{
		return 2;
	}

	//Prepare the sockaddr_in structure
	server_s.sin_family = AF_INET;
	server_s.sin_addr.s_addr = INADDR_ANY;
	server_s.sin_port = htons( PORT_s );

	//Bind

	if( bind(s_s ,(struct sockaddr *)&server_s , sizeof(server_s)) == SOCKET_ERROR)
	{
		return 3;
		//printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//puts("Bind done");
	//time(&start);
	return 1;
}

void SSocket::camPlay()
{
	vector<uchar>videoBuffer;
	fflush(stdout);
	recv_len = 0;
	//clear the buffer by filling null, it might have previously received data
	memset(buf,'\0', BUFLEN);

	//print details of the client/peer and the data received			
	recv_len = recvfrom (s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
	videoBuffer.resize(recv_len);
	memcpy((char*)(&videoBuffer[0/*soc.recv_len*h*/]), buf, recv_len);
			
	Mat img_obj = imdecode(Mat(videoBuffer), CV_LOAD_IMAGE_COLOR);
	img = cvCloneImage(&(IplImage)img_obj);
	//isDataReceived = true;

	//get frame counts
	time(&end);
	++counter;
	sec = difftime(end, start);
	fps = counter/sec;
	//printf("\n%lf\n", fps);
}

void SSocket::receiveCommand()
{
	vector<uchar>commandBuffer;
	fflush(stdout);
	recv_len_s =0;
	memset(buf_s, '\0', BUFLEN_s);
	
	recv_len_s = recvfrom (s_s, buf_s, BUFLEN_s, 0, (struct sockaddr *) &si_other_s, &slen_s);
	
}
/*
void SSocket::getFPS()
{

}
*/