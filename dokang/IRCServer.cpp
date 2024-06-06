#include "IRCServer.hpp"

IRCServer::IRCServer(const char *port, const char* password) : server_pwd(password)
{
	this -> listen_fd = create_bind(port); // 주어진 포트에 바인딩된 소켓을 생성
	non_blocking(this -> listen_fd); // 소켓을 논블로킹 모드로 설정
	listen(this -> listen_fd, SOMAXCONN); // 소켓을 수신 대기 상태로 설정

	struct pollfd pfd; // 새로운 폴 파일 디스크립터 구조체를 만듭니다.
	pfd.fd = this -> listen_fd; // 폴 구조체의 파일 디스크립터를 listen 소켓으로 설정합니다.
	pfd.events = POLLIN; // 읽기 가능한 이벤트를 설정합니다.
	poll_fd.push_back(pfd); // listen 소켓을 폴링할 파일 디스크립터 목록에 추가합니다.
}

IRCServer::~IRCServer()
{
	close(listen_fd); // listen 소켓을 닫습니다.
	for (int i = 1; i < poll_fd.size(); ++i)  // i = 0 은 listen 소켓이다.
		close(poll_fd[i].fd); // 모든 클라이언트 소켓을 닫습니다.
}

int IRCServer::create_bind(const char* port)
{
	struct addrinfo want;
	struct addrinfo *result;
	struct addrinfo *rp;
	int sfd;

	std::memset(&want, 0, sizeof(struct addrinfo)); // want 구조체 초기화
	want.ai_family = AF_UNSPEC; // IPv4 와 IPv6 둘다 허용
	want.ai_socktype = SOCK_STREAM; // TCP 소켓을 사용
	want.ai_flags = AI_PASSIVE; // 서버 소켓을 생성
	//addrinfo 정보는 readme에 설명이 있음
	int s = getaddrinfo(NULL, port, &want, &result); //포트에 대한 주소 정보를 얻음
	if (s != 0)
	{
		std::cerr << "getaddrinfo_error: " << gai_strerror(s) << std::endl;
		exit(1);
	}
	for (rp = result; rp != NULL; rp = rp -> ai_next) // 주소 정보 리스트를 순회
	{
		sfd = socket(rp -> ai_family, rp -> ai_socktype, rp -> ai_protocol); // 소켓 생성
		if (sfd == -1)
			continue;
		int ok = 1;
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) == -1) // 소켓 옵션을 설정
		{
			std::cerr << "setsockopt" << std::endl;
			exit(1);
		}
		
		if (bind(sfd, rp -> ai_addr, rp -> ai_addrlen) == 0) // 소켓을 바인딩
			break;
		close(sfd);
	}
	if (rp == 0)
	{
		std::cerr << "not bind" << std::endl;
		exit(1);
	}

	freeaddrinfo(result); // 주소 정보 결과를 해제
	return sfd; // 생성된 소켓 파일 디스크립터를 반환
}

void IRCServer::non_blocking(int cfd)
{
	int flags = fcntl(cfd, F_SETFL, O_NONBLOCK); // 현재 파일 디스크립터을 논블로킹 모드로 설정
	if (flags == -1)
	{
		std::cerr << "fcntl error" << std::endl;
		exit(1);
	}
}

void IRCServer::run()
{
	while (true) // 서버가 종료될 때까지 실행
	{
		int count_poll = poll(&poll_fd[0], poll_fd.size(), -1); //폴링, 이벤트가 발생할 떄까지 대기
		if (count_poll == -1)
		{
			std::cerr << "poll error" << std::endl;
			exit(1);
		}
		
		for (int i = 0; i < poll_fd.size(); i++) // 모든 파일 디스크립터를 순회
		{
			if (poll_fd[i].revents & POLLIN) // 읽기 가능한 이벤트가 발생했는지 확인
			{
				if (poll_fd[i].fd == listen_fd) // 리스닝 소켓에서 새로운 연결이 들어왔는지 확인
				{
					connection_handling(); // 새로운 연결을 처리
				}
				else {
					message_handling(poll_fd[i].fd); // 클라이언트로부터의 메시지를 처리
				}

			} 
		}
	}
}