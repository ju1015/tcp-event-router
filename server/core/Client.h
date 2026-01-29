#ifndef CLIENT_H
#define CLIENT_H
#include <chrono>
#include <string>
#include <vector>
using namespace std;

class Client {
	private:
		int socketFd;
		std::string recvBuffer;
		bool authenticated = false;
		std::string identity;

	public:
		explicit Client(int fd);
		~Client();

		int getFd() const;


		// Networking
		bool receiveData();                 // calls recv()
		std::vector<std::string> popMessages(); // extract complete messages

		// Sending
		bool sendRaw(const std::string& data);
		bool sendText(const std::string& text);

		// Connection management
		void closeConnection();

		bool isAuthenticated() const;

		void authenticate();

		// ---- Future-ready fields (safe to add now) ----
		std::string username;

		struct RateLimit {
			int tokens = 10; // max events
			std::chrono::steady_clock::time_point lastRefill =
				std::chrono::steady_clock::now();
		};
		RateLimit rate;
};

#endif

