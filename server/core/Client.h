#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
using namespace std;

class Client {
	private:
		int socketFd;
		std::string recvBuffer;

	public:
		explicit Client(int fd);
		~Client();

		int getFd() const;

		// Buffer management
		/*void appendToBuffer(const char* data, size_t len);
		  string& getBuffer();
		  void clearBuffer();*/

		// Networking
		bool receiveData();                 // calls recv()
		std::vector<std::string> popMessages(); // extract complete messages

		// Sending
		bool sendRaw(const std::string& data);
		bool sendText(const std::string& text);

		// Connection management
		void closeConnection();

		// ---- Future-ready fields (safe to add now) ----
		std::string username;
		bool authenticated = false;
};

#endif

