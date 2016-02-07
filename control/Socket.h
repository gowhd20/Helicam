#include<stdio.h>
#include<winsock2.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <vector>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

class SSocket
{
public:
	#define BUFLEN 50000	//Max length of buffer
	#define BUFLEN_s 2		
	#define PORT 8888	//The port on which to listen for incoming data
	#define PORT_s 5555 
	SSocket();
	~SSocket();
	void camPlay();
	void receiveCommand();
	BOOL udpConnect();
	BOOL udpConnectForCommand();

public:
	int len;
	time_t start, end;
	int counter;
	double sec;
	double fps;
	SOCKET s;
	SOCKET s_s;
	IplImage *img;
	struct sockaddr_in server, si_other;
	struct sockaddr_in server_s, si_other_s;
	int slen , recv_len;
	int slen_s, recv_len_s;

	char buf[BUFLEN];
	char buf_s[BUFLEN_s];
	WSADATA wsa;
	//WSADATA wsa_s;
};


