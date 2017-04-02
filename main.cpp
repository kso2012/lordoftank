#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <mutex>
#include <vector>
#include <set>
#include "protocol.h"
using namespace std;

#define OP_RECV 1
#define OP_SEND 2

HANDLE g_hIocp;

struct OverlapEx
{
	WSAOVERLAPPED original_overlap;
	int	operation;
	WSABUF recv_buf;
	unsigned char socket_buf[MAX_BUF_SIZE];
	int packet_size;
};

struct Client {
	int	m_id;
	bool is_connected;
	SOCKET m_s;
	mutex vl_lock;
	OverlapEx m_recv_overlap;
	int previous_data;
	unsigned char packet[MAX_BUF_SIZE];
};

Client clients[MAX_USER];
bool g_isshutdown = false;

void error_display(char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s", msg);
	//wprintf(L"에러%s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void Initialize_Server()
{
	WSADATA	wsadata;
	//_wsetlocale(LC_ALL, L"korean");
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	for (auto i = 0; i < MAX_USER; ++i) {
		clients[i].m_recv_overlap.recv_buf.buf =
			reinterpret_cast<CHAR *>(clients[i].m_recv_overlap.socket_buf);
		clients[i].m_recv_overlap.recv_buf.len = MAX_BUF_SIZE;
		clients[i].m_recv_overlap.operation = OP_RECV;
		clients[i].is_connected = false;
	}
}

void worker_thread()
{
	DWORD io_size, key;
	OverlapEx *overlap;
	bool result;

	while (true)
	{
		result = GetQueuedCompletionStatus(g_hIocp, &io_size, &key,
			reinterpret_cast<LPOVERLAPPED *>(&overlap), INFINITE);

		if (false == result) {
			// Error를 처리한다.
		}

		if (io_size == 0)
		{

		}

		if (overlap->operation == OP_RECV)
		{
			unsigned char *buf_ptr = overlap->socket_buf;
			int remained = io_size;


			DWORD flags = 0;
			WSARecv(clients[key].m_s,
				&clients[key].m_recv_overlap.recv_buf,
				1, NULL, &flags,
				reinterpret_cast<LPWSAOVERLAPPED>(&clients[key].m_recv_overlap),
				NULL);
		}
		else if (overlap->operation == OP_SEND)
		{

		}
		else
		{
			cout << "Unknown Event on worker_thread" << endl;
			while (true);
		}
	}
}

void accept_thread()
{
	struct sockaddr_in listen_addr;

	SOCKET accept_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&listen_addr, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(SERVER_PORT);
	ZeroMemory(&listen_addr.sin_zero, 8);
	::bind(accept_socket,
		reinterpret_cast<sockaddr *>(&listen_addr),
		sizeof(listen_addr));

	listen(accept_socket, 10);

	while (true)
	{
		struct sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);

		SOCKET new_client = WSAAccept(accept_socket,
			reinterpret_cast<sockaddr *>(&client_addr),
			&addr_size, NULL, NULL);

		int new_id = -1;
		for (auto i = 0; i < MAX_USER; ++i) {
			if (clients[i].is_connected == false) {
				new_id = i;
				break;
			}
		}

		if (-1 == new_id) {
			cout << "Maximum User Number Fail\n";
			closesocket(new_client);
			continue;
		}

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client),
			g_hIocp, new_id, 0);

		clients[new_id].m_s = new_client;
		clients[new_id].is_connected = true;
		clients[new_id].m_id = new_id;
		clients[new_id].m_recv_overlap.operation = OP_RECV;
		clients[new_id].m_recv_overlap.packet_size = 0;
		clients[new_id].previous_data = 0;

		DWORD flags = 0;
		int res = WSARecv(new_client,
			&clients[new_id].m_recv_overlap.recv_buf, 1, NULL,
			&flags,
			&clients[new_id].m_recv_overlap.original_overlap, NULL);

		if (0 != res) {
			int error_no = WSAGetLastError();
			if (WSA_IO_PENDING != error_no) {
				error_display("AcceptThread:WSARecv", error_no);
			}
		}
	}
}

int main()
{
	vector <thread *> worker_threads;
	thread accept_threads;

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	Initialize_Server();
	for (auto i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
		worker_threads.push_back(new thread{ worker_thread });
	accept_threads = thread{ accept_thread };
	while (g_isshutdown == false)
	{
		Sleep(1000);
	}
	for (auto th : worker_threads) {
		th->join();
		delete th;
	}
	accept_threads.join();
	WSACleanup();
}