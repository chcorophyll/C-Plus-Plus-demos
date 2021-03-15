#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>
#include "board.h"
using namespace std;

void initialization();
int main()
{
	Chessboard game_board;
	int id = 2;
	int send_len = 0;
	int recv_len = 0;

	char send_buf[100];
	char recv_buf[100];
	string ip_addr;

	SOCKET s_server;

	SOCKADDR_IN server_addr;
	initialization();

	server_addr.sin_family = AF_INET;
	cout << "������IP";
	cin >> ip_addr;
	server_addr.sin_addr.S_un.S_addr = inet_addr(ip_addr.c_str());
	/*server_addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.33")*/;
	server_addr.sin_port = htons(5010);

	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "����������ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else
	{
		cout << "���������ӳɹ���" << endl;
	}

	game_board.display();

	while (1)
	{
		cout << "�ȴ���������:" << endl;
		recv_len = recv(s_server, recv_buf, 100, 0);
		if (recv_len < 0)
		{
			cout << "����ʧ�ܣ�" << endl;
			break;
		}
		else
		{
			cout << "�������Ӻ�:" << endl;
			if (game_board.drop_chess(int(recv_buf[0] - 'A'), int(recv_buf[1] - 'A'), id - 1) != 0)
			{
				game_board.display();
				if (game_board.board_end() != 0)
				{
					cout << "Game Over" << endl;
					if (game_board.board_end() == 1)
					{
						cout << "Winner is ID 1" << endl;
						break;
					}
					else
					{
						cout << "Winner is ID 2" << endl;
						break;
					}
				}
			}

		}
		cout << "����������XYλ����Ϣ:" << endl;
		cin >> send_buf;
		cout << send_buf << endl;
		if (game_board.drop_chess(int(send_buf[0] -  'A'), int(send_buf[1] - 'A'), id) != 0)
		{
			game_board.display();
			send_len = send(s_server, send_buf, 100, 0);
			if (game_board.board_end() != 0)
			{
				cout << "Game Over" << endl;
				if (game_board.board_end() == 1)
				{
					cout << "Winner is ID 1" << endl;
					break;
				}
				else
				{
					cout << "Winner is ID 2" << endl;
					break;
				}
			}
			if (send_len < 0)
			{
				cout << "����ʧ�ܣ�" << endl;
				break;
			}
		}
	}

	closesocket(s_server);

	WSACleanup();
	system("pause");
	return 0;
}
void initialization()
{

	WORD w_req = MAKEWORD(2, 2);
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0)
	{
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
	}
	else {
		cout << "��ʼ���׽��ֿ�ɹ���" << endl;
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		cout << "�׽��ֿ�汾�Ų�����" << endl;
		WSACleanup();
	}
	else
	{
		cout << "�׽��ֿ�汾��ȷ��" << endl;
	}

}