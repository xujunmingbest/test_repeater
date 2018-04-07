#include "tcp客户端.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

void handleSend(string src_mac, string des_mac);
void handleRecv();
void handlet(string port);

SOCKET sockClient;
string mac;
void main(int argc,char *argv[]) {


	if (argc != 4){
		cout << "用法:" << "exe + 源mac地址 + 目的mac地址  + Port" <<endl;
		system("pause");
		return;
	}
	handlet(argv[3]);


	thread t1(handleSend, argv[1], argv[2]);
    t1.detach();
	thread t2(handleRecv);
	t2.detach();

	while (1)
	{
		Sleep(1000);
	}
	system("pause");
	//关闭套接字
	WSACleanup();
}


void handlet(string port)
{
	//加载套接字  
	WSADATA wsaData;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return;
	}


	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(atoi(port.c_str()));
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	//创建套接字  
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == sockClient) {
		printf("Socket() error:%d", WSAGetLastError());
		return;
	}
	int nSendBuf = 100;//设置为32K
	setsockopt(sockClient, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));

	//向服务器发出连接请求  
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET) {
		printf("Connect failed:%d", WSAGetLastError());
		return;
	}
}

void handleSend(string src_mac, string des_mac){
	char Headbuff[5];
	char Bodybuff[9];

	//发送数据
	for(int i=0;i<10000000;i++){
		if (i == 1) {
			getchar();
		}
		Sleep(rand()%1000);
		Json::Value root;
		root["src_mac"] = Json::Value(src_mac);
		root["des_mac"] = Json::Value(des_mac);
		Json::FastWriter w;
		string s = w.write(root);
		memset(Headbuff, 0x00, 5);
		snprintf(Headbuff, 5, "%d", s.length());
		string lh(Headbuff, 4);
		char buffs[1000];
		memset(buffs, 0x00, 1000);
		memset(buffs, 'a', 300);
		memset(buffs+300, 'b', 300);
		memset(buffs+600, 'c', 399);
		string bb(buffs);
		memset(Bodybuff, 0x00, 9);
		snprintf(Bodybuff, 9, "%d", bb.length());
		string lb(Bodybuff, 8);

		string sfinally;
		sfinally = sfinally + lh + lb + s + bb;
		int sendlen = send(sockClient, sfinally.c_str(), sfinally.length(), 0);
		//cout << "发送数据" << sendlen << endl;
	}
}

string R(int rl);

void handleRecv() {
	char buff[1000];

	while (1) {
		string lh = R(4);
		cout << " " << lh << endl;
		string lb = R(8);
		cout << " " << lh << endl;
		string head = R(atoi(lh.c_str()));
		cout << " " << head << endl;
		string body = R(atoi(lb.c_str()));
		cout << " " << body << endl;
	}
}

string R(int rl) {
	char *buff = new char[rl];
	int lena = 0;
	while (lena < rl) {
		int templ = recv(sockClient, buff + lena, rl - lena, 0);
		if (templ == 0 || templ == -1) {
			system("pause");
			exit(-1);
		}
		lena += templ;
	}
	string r(buff, rl);
	return r;
}