
#include "stdafx.h"    //ʹ��MFC�⣬�ڹ��̵�ÿ��Դ�ļ��ж���Ҫ��Ӵ�ͷ�ļ�

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>  
#include <WinSock2.h>  
#include <fstream>
#include <string>
#include <vector>
#include "string.h"
#include "stdio.h"
using namespace std;


const char* SERVER_IP = "192.168.1.50";
const unsigned short SERVER_PORT = 9100;
#pragma comment(lib,"ws2_32.lib") //ws2_32.lib�ļ����ṩ�˶������������API��֧�֣���ʹ�����е�API����Ӧ�ý�ws2_32.lib���빤��

void strReplace(char* cp, int n, char* str)
{
	int lenDiffer = strlen(str) - n;
	if (lenDiffer < 0)
	{
		int i = 0;
		for (; i < strlen(cp) + n; i++) {
			cp[i] = cp[i - lenDiffer];
		}
		cp[i] = '\0';
	}
	else if (lenDiffer > 0)
	{
		int i = strlen(cp) - 1;
		cp[i + 1 + lenDiffer] = '\0';
		for (; i >= n; i--) {
			cp[i + lenDiffer] = cp[i];
		}
	}

	memcpy_s(cp, strlen(str), str, strlen(str));
}


void findAndReplace(char* src, char* str1, char* str2) {
	char* p = strstr(src, str1);
	if (p) {
		strReplace(p, strlen(str1), str2);
	}
	printf("%s\n", src);
}

int ZBP_TEST(char* id, char* value)
{
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (0 != err){
		cout << "WSAStartup failed. return: " << err << endl;
		AfxMessageBox("WSAStartup failed");
		return -1;
	}

	if (wsaData.wVersion != wVersionRequested){
		cout << "wsaData.wVersion: " << wsaData.wVersion << " is not equal to wVersionRequested: " << wVersionRequested << endl;
		AfxMessageBox("wsaData.wVersion is not equal to wVersionRequested");
		WSACleanup();
		return -2;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock){
		cout << "socket error: " << WSAGetLastError() << endl;
		AfxMessageBox("socket error");
		return -3;
	}

	SOCKADDR_IN addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(SERVER_PORT);
	addr_server.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

	err = connect(sock, (SOCKADDR*)&addr_server, sizeof(addr_server));
	if (SOCKET_ERROR == err)
	{
		cout << "connect error: " << WSAGetLastError() << endl;
		AfxMessageBox("��������ʧ��");
		return -4;
	}
	cout << "Connection established,input 'quit' or 'exit' to exit." << endl;

	char send_buf[1024] = { "\0" };
	char send_buf_to_print[1024] = { "\0" };
	//��ȡģ��
	{
		ifstream inFile("ZPR.prn", ios::in | ios::binary); //�����ƶ���ʽ��
		if (!inFile) {
			cout << "���ļ�����" << endl;
	
			AfxMessageBox("���ļ�ʧ��");
			return 0;
		}

		int num = 0;
		while (inFile.read((char*)&send_buf, sizeof(send_buf))) { //һֱ�����ļ�����
			int readedBytes = inFile.gcount(); //���ղŶ��˶����ֽ�
		}
		inFile.close();
	}

	//�Բɼ���������ֵ��panel ID�滻��ģ����
	char str1[10] = "<end>", str2[10] = "<lv>", str3[10] = "<start>", str4[10] = "<all>";
	char str11[100] = "\0", str33[100] = "\0", str44[100] = "\0";

	char* ptr = id + strlen(id) - 4;
	memcpy_s(str11, 4, ptr, 4);
	memcpy_s(str33, strlen(id) - 4, id, strlen(id) - 4);
	memcpy_s(str44, strlen(id), id, strlen(id));
	ptr = str44 + strlen(id);
	*ptr = '#';
	ptr++;
	memcpy_s(ptr, strlen(value), value, strlen(value));


	findAndReplace(send_buf, str1, str11);
	findAndReplace(send_buf, str2, value);
	findAndReplace(send_buf, str3, str33);
	findAndReplace(send_buf, str4, str44);


	ofstream outFile("send.txt", ios::out | ios::binary);
	if (!outFile) {
	}
	else {
		outFile << send_buf << endl;
		outFile.close();
	}

	err = send(sock, send_buf, strlen(send_buf) + 1, 0);
	if (SOCKET_ERROR == err) {
		cout << "send error: " << WSAGetLastError() << endl;
		AfxMessageBox("send error");
	}

	err = closesocket(sock);
	if (SOCKET_ERROR == err){
		cout << "closesocket error: " << WSAGetLastError() << endl;
		AfxMessageBox("closesocket error");
	}
	WSACleanup();

	return 0;
}




