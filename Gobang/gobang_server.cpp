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
        cout << "套接字绑定失败！" << endl;
        WSACleanup();
    }
    else
    {
        cout << "套接字绑定成功！" << endl;
    }

    if (listen(s_server, SOMAXCONN) < 0)
    {
        cout << "设置监听状态失败！" << endl;
        WSACleanup();
    }
    else
    {
        cout << "设置监听状态成功！" << endl;
    }
    cout << "服务端正在监听连接，请稍候...." << endl;

    len = sizeof(SOCKADDR);
    s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
    if (s_accept == SOCKET_ERROR)
    {
        cout << "连接失败！" << endl;
        WSACleanup();
        return 0;
    }

    cout << "连接建立，准备接受数据" << endl;

    game_board.display();

    while (1)
    {
        cout << "请输入落子XY位置信息:" << endl;
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
                cout << "发送失败！" << endl;
                break;
            }
        }
        recv_len = recv(s_accept, recv_buf, 100, 0);
        if (recv_len < 0)
        {
            cout << "接受失败！" << endl;
            break;
        }
        else
        {
            cout << "对手落子后:" << endl;
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
    WORD w_req = MAKEWORD(2, 2);//版本号
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0)
    {
        cout << "初始化套接字库失败！" << endl;
    }
    else
    {
        cout << "初始化套接字库成功！" << endl;
    }
    //检测版本号
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
    {
        cout << "套接字库版本号不符！" << endl;
        WSACleanup();
    }
    else
    {
        cout << "套接字库版本正确！" << endl;
    }
}


