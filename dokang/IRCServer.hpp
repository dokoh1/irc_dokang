#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

#include <iostream>  // 입출력 스트림
#include <cstring>  // C 스타일 문자열 처리
#include <vector> // 동적 배열을 사용하기 위해 포함
#include <map> // 키-값 쌍을 저장할 수 있는 맵 컨테이너 사용
#include <algorithm> // 표준 알고리즘 함수를 사용하기 위해 포함
#include <sys/types.h> // 시스템 타입 정의를 포함
#include <sys/socket.h> // 소켓 프로그래밍을 위한 정의를 포함
#include <netdb.h> // 네트워크 데이터베이스 작업을 위한 정의를 포함
#include <fcntl.h> // 파일 제어 옵션을 설정하기 위해 포함
#include <unistd.h> // 유닉스 표준 함수 정의를 포함
#include <poll.h> // I/O 다중화를 위해 "poll"함수를 사용하기 위해 포함

#define BUFFER_SIZE 512 // 버퍼 크기를 512 바이트로 정의

class IRCServer
{
	public:
		IRCServer(const char* port, const char* password); //서버 초기화(생성자)
		~IRCServer(); //소멸자
		void run(); //서버의 메인 루프를 실행
	private:
		int create_bind(const char* port); // 소캣을 생성 및 포트에 바인딩
		void non_blocking(int cfd); // 소켓을 논블로킹 모드로 설정
		void connection_handling(); //새 클라이언트 연결을 처리
		void message_handling(int client_fd); // 클라이언트로부터의 메시지를 처리
		void client_remove(int client_fd); // 클라이언트를 제거

		int listen_fd; // listen 소켓 파일 디스크립터
		std::string server_pwd; // 서버 연결 비밀번호
		std::vector<struct pollfd> poll_fd; // 폴링할 파일 디스크립터 목록 
		//struct pollfd는 구조체는 readme에 설명이 있음
		std::map<int, std::string> client_buffers; // 클라이언트 별로 수신된 데이터 버퍼를 저장
};


#endif