#include<iostream> //printing messages
#include<sys/socket.h> //create a socket,attach ip+port,waits for clients,accept client connection
#include <netinet/in.h> //contains definitions for the internet protocol family. 
#include <arpa/inet.h> //Helps convert values into network format
#include <unistd.h> //used for systemcalls
#include <sys/select.h> 
#include <unordered_map>
#include "Client.h"
using namespace std;

int main() {

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		cerr << "Socket creation failed\n";
		return 1;
	}

	sockaddr_in serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) == -1) {

		cerr << "Bind failed\n";
		return 1;
	}

	if (listen(serverSocket, 5) == -1) {
		cerr << "Listen failed\n";
		return 1;
	}

	cout << "Server is listening...\n";

	unordered_map<int, Client*> clients;	
	fd_set readfds;

	while (true) {

		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds);
		int maxFd = serverSocket;

		for (const auto& pair : clients) {
			int clientFd = pair.first;

			FD_SET(clientFd, &readfds);

			if (clientFd > maxFd)
				maxFd = clientFd;
		}

		int activity = select(maxFd + 1, &readfds, NULL, NULL, NULL);

		if (activity < 0) {
			cerr << "select error\n";
			break;
		}

		// New client
		if (FD_ISSET(serverSocket, &readfds)) {

			sockaddr_in clientAddress{};
			socklen_t clientSize = sizeof(clientAddress);

			int clientSocket = accept(serverSocket,(struct sockaddr*)&clientAddress,&clientSize);

			if (clientSocket != -1) {
				Client* client = new Client(clientSocket);
				clients[clientSocket] = client;
				cout << "New client connected. Total clients: "
					<< clients.size() << endl;
			}
		}

		// Existing clients
		for (auto it = clients.begin(); it != clients.end(); ) {

			Client* client = it->second;
			int fd = client->getFd();

			if (FD_ISSET(fd, &readfds)) {

				if (!client->receiveData()) {
					cout << "Client disconnected\n";
					client->closeConnection();
					delete client;
					it = clients.erase(it);
					continue;
				}

				auto messages = client->popMessages();

				for (const auto& msg : messages) {
					cout << "Client says: " << msg << endl;

					// broadcast
					for (const auto& pair : clients) {
						if (pair.second != client) {
							pair.second->sendText(msg);
						}
					}
				}
			}

			++it;
		}
	}

	for (auto& pair : clients) {
		pair.second->closeConnection();
		delete pair.second;
	}
	clients.clear();	
	close(serverSocket);
	return 0;
}

