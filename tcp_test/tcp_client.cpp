// tcp_client.cpp : Defines the entry point for the console application.
//
// #ifdef __cplusplus
// extern "C" {
// #endif

#include "nd_typedef.h"
#include "testmarc.h"
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>

class snd_file
{
public:
	snd_file()
	{
		this->buf_ = new char[SIZE_MTU];//(char*)malloc(SIZE_MTU);
		this->file_ = -1;
		this->s_ = -1;
	}
	~snd_file()
	{
		if (this->s_ != -1)
		{
			close(this->s_);
		}
/*
		if (this->file_ != -1)
		{
			//CloseHandle(this->file_);
			close(this->file_);
			this->file_ = -1;
		}
*/
		delete []this->buf_;
		//free(this->buf_);
	}

	bool open_t(const char* fn, const char* addr)
	{
		printf("open start, fn :%s, addr :%s\n", fn, addr);
		this->addr_ = addr;
		std::string::size_type pos = this->addr_.find(':');
		if (std::string::npos == pos || pos < 5) {
			printf("invalid addr\r\n");
			return false;
		}

		std::string ip = this->addr_.substr(0, pos);
		this->ip_ = inet_addr(ip.c_str());
		this->port_ = atoi(this->addr_.substr(pos + 1).c_str());

		this->fn_ = fn;
/*
		this->file_ = CreateFile(fn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == this->file_) 
			return false;

		LARGE_INTEGER lg;
		if (!GetFileSizeEx(this->file_, &lg)) {
			return false;
		}

		this->fsize_ = lg.QuadPart;
*/
		if ((this->file_ = open(fn, O_RDONLY|O_CREAT)) == -1){
			printf("Open %s Error\n", fn);
			return false;
		}
		struct stat statbuff;
		if (stat(fn, &statbuff) < 0){
			printf("stat Error\n");
			return false;
		}
		else{
			this->fsize_ = statbuff.st_size;
		}
/*
		WSADATA wsaDate;
		int err = WSAStartup(MAKEWORD(2, 2), &wsaDate);
		if (0 != err)
		{
			printf("WSAStartup err\n");
			return false;
		}
*/
		struct sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(client_addr));
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(this->port_);
		client_addr.sin_addr.s_addr = this->ip_; //("127.0.0.1"); //this->ip_;

		this->s_ = socket(AF_INET, SOCK_STREAM, 0);

		if (this->s_ == -1)
		{
			//WSACleanup();
			printf("socket() failed\n");
			return false;
		}
		int opt;
		opt = 512 * 1024;
		setsockopt(this->s_, SOL_SOCKET, SO_RCVBUF, (char*)&opt, sizeof(opt));
		opt = 512 * 1024;
		setsockopt(this->s_, SOL_SOCKET, SO_SNDBUF, (char*)&opt, sizeof(opt));

		int ret;
		ret = connect(this->s_, (struct sockaddr *)&client_addr, sizeof(client_addr));
		if (ret == -1)
		{
			printf("connect failed\t\n");
			//WSACleanup();
			return false;
		}

		int len = 0;
		//int tmo = 10*1000;
		char *buf = this->buf_;

		std::string short_fn = "";
		char *c = &this->fn_[this->fn_.size() - 1];
		int i = 0;
		for (; i != this->fn_.size(); i++) {
			if (*c == '/' || *c == '\\') {
				short_fn = c + 1;
				break;
			}
			c--;
		}
		if (short_fn.size() == 0) {
			printf("invalid fn: %s\r\n", this->fn_.c_str());
			return false;
		}

		*((uint16_t*)buf) = htons(8 + 2 + short_fn.size());
		buf[2] = 1;
		buf[3] = 0;
		*((uint64_t*)(buf + HDR_LEN)) = nd_htonll(this->fsize_);
		*((uint16_t*)(buf + HDR_LEN + 8)) = htons(short_fn.size());

		memcpy(buf + HDR_LEN + 8 + 2, short_fn.c_str(), short_fn.size());
		len = HDR_LEN + 8 + 2 + short_fn.size();

		//memcpy(buf, short_fn.c_str(), short_fn.size());

		if ( send(this->s_, buf, len, 0) == -1)
		{
			printf("send file name failed!\t\n");
			return false;
		}

		printf("start send...\t\n");
		//DWORD tm_begin = GetTickCount();
		//DWORD tm = GetTickCount();
		//DWORD tm_inter = GetTickCount();
		struct timeval t;	
		uint64_t tm;
		uint64_t tm_inter;
		gettimeofday(&t, 0);
		tm = t.tv_sec * 1000 + t.tv_usec/1000;
		uint64_t tm_begin = tm;
		uint64_t send_rate = 0;
		uint64_t send_bytes = 0;

		while (1) {
			int rd_len = 0;
			rd_len = read(this->file_, buf + HDR_LEN, SIZE_MTU - HDR_LEN);

			if (rd_len <= 0)
				break;
			
			*((uint16_t*)buf) = htons(rd_len);
			buf[2] = 2;
			buf[3] = 0;
			
			if ( send(this->s_, buf, rd_len + HDR_LEN, 0) == -1)
			{
				printf("send data failed!\t\n");
				return false;
			}
			send_bytes += rd_len;
			gettimeofday(&t, 0);
			tm_inter = t.tv_sec * 1000 + t.tv_usec/1000;
			if (tm_inter - tm > 1000)
			{
				send_rate = ((double)(send_bytes) / (double)(tm_inter - tm)) * 1000.0;
				printf("inter_time=%.2f, send_rate=%llu(Bytes/S)\n", (double)(tm_inter - tm) / 1000.0, send_rate);
				tm = tm_inter;
				send_bytes = 0;
			}

		}

		if (this->file_ != -1) {
			close(this->file_);
			this->file_ = -1;
		}

		if (recv(this->s_, buf, HDR_LEN, 0) == -1)
		{
			printf("client recv failed\t\n");
			return false;

		}

		unsigned long tm_end = 0;	
		gettimeofday(&t, 0);
		tm_end = t.tv_sec * 1000 + t.tv_usec/1000;

		printf("file send finished, rate=%.2fMB\r\n", ((double)this->fsize_) / (double(tm_end - tm_begin) / 1000.0f  + 0.1) / (double)(1024*1024));

		return true;
	}
protected:
private:

public:
	std::string fn_;
	std::string addr_;
	uint64_t fsize_;
	long ip_;
	char* buf_;
	int port_;
	int file_;
	int s_;
};

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("invalid argc, tcp exit...\r\n");
		system("pause");
		return -1;
	}

	snd_file sf;
	if (!sf.open_t(argv[1], argv[2])) {
		printf("open error, app exit...\r\n");
		system("pause");
		return 0;
	}

	//system("pause");
	return 0;
}

