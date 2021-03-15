#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include "board.h"
using namespace std;


void initialization();

int main()
{
    Chessboard game_board;
    int id = 1;
    int send_len = 0;
    int recv_len = 0;
    int len = 0;
    char send_buf[100];
    char recv_buf[100];

    SOCKET s_server;
    SOCKET s_accept;
    SOCKADDR_IN server_addr;
    SOCKADDR_IN accept_addr;

    initialization();

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(5010);
    s_server = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        cout << "�׽��ְ�ʧ�ܣ�" << endl;
        WSACleanup();
    }
    else
    {
        cout << "�׽��ְ󶨳ɹ���" << endl;
    }

    if (listen(s_server, SOMAXCONN) < 0)
    {
        cout << "���ü���״̬ʧ�ܣ�" << endl;
        WSACleanup();
    }
    else
    {
        cout << "���ü���״̬�ɹ���" << endl;
    }
    cout << "��������ڼ������ӣ����Ժ�...." << endl;

    len = sizeof(SOCKADDR);
    s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
    if (s_accept == SOCKET_ERROR)
    {
        cout << "����ʧ�ܣ�" << endl;
        WSACleanup();
        return 0;
    }

    cout << "���ӽ�����׼����������" << endl;

    game_board.display();

    while (1)
    {
        cout << "����������XYλ����Ϣ:" << endl;
        cin >> send_buf;
        cout << int(send_buf[0]) << endl;
        if (game_board.drop_chess(int(send_buf[0] - 'A'), int(send_buf[1] - 'A'), id) != 0)
        {
            game_board.display();
            send_len = send(s_accept, send_buf, 100, 0);
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
        recv_len = recv(s_accept, recv_buf, 100, 0);
        if (recv_len < 0)
        {
            cout << "����ʧ�ܣ�" << endl;
            break;
        }
        else
        {
            cout << "�������Ӻ�:" << endl;
            if (game_board.drop_chess(int(recv_buf[0] - 'A'), int(recv_buf[1] - 'A'), id + 1) != 0)
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
    }

    closesocket(s_server);
    closesocket(s_accept);

    WSACleanup();
    system("pause");
    return 0;
}

void initialization()
{
    WORD w_req = MAKEWORD(2, 2);//�汾��
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0)
    {
        cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
    }
    else
    {
        cout << "��ʼ���׽��ֿ�ɹ���" << endl;
    }
    //���汾��
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


