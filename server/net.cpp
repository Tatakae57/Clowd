#include "net.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <thread>
#include <map>

using namespace std;
#define UDP_PORT 8080

// Structures
struct TCP_Response {
    /*
        Types:
            0: Loged in
            1: Not loged.
            2: User created.
            3: User not created.
            4: Uploaded.
            5: Downloaded.
    */
    unsigned char type;
    uint32_t file_content;
};

struct TCP_Request {
    /*
        Types:
            0: Login.
            1: Upload file.
            2: Remove file.
            3: Change directory.
            4: New directory.
            5: Remove directory.
            6: User status.
            7: Create new user.
            8: Remove user.
            9: Change password.
            10: Disconnect.
    */
    unsigned char type;
    string param1; // User | File | File | Directory | Directory | Directory | User
    string param2; // Password | Password
    uint32_t file_content;
};

// Extern variables
extern bool exit_program;

// Global variables
string ip = "127.0.0.1";
unsigned short port = 8000, max_clients = 5, max_users = 5, max_space = 20;
bool server_live = false;

// Local variables
static unsigned char connected_clients = 0;
static int tcp_server, udp_server;
static map<string, string> users_list;

// Functions
//      UDP
static void wait_udp_requests() {
    sockaddr_in client_address;
    socklen_t len = sizeof(client_address);
    unsigned char message;
    while (!exit_program && server_live) {
        recvfrom(udp_server, (unsigned char *)&message, sizeof(message), MSG_WAITALL, (struct sockaddr *)&client_address, &len);

        if (message == 10) {
            if (connected_clients != max_clients) message = 1; // The client can connect.
            else message = 0; // The server is full.

            // Send message
            sendto(udp_server, (unsigned char *)&message, sizeof(message), MSG_CONFIRM, (struct sockaddr *)&client_address, len);
        }
    }
}

void start_udp_server() {
    // UDP (for ask if the server is not full)
    udp_server = socket(AF_INET, SOCK_DGRAM, 0); // Setting UDP socket

    cout << "Creating UDP server... ";
    if (udp_server < 0) {
        cout << "failed." << endl;
    }
    cout << "done." << endl;

    // Reusing socket
    cout << "Setting socket for reuse... ";
    int opt = 1;
    if (setsockopt(udp_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cout << "failed." << endl;
        exit(EXIT_FAILURE);
    }
    cout << "done." << endl;

    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(UDP_PORT);
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());

    cout << "Starting UDP server... ";
    if (bind(udp_server, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cout << "failed." << endl;
        exit(EXIT_FAILURE);
    }
    cout << "done." << endl;
    users_list["kae"] = "57";

    // Create udp thread
    thread wait_requests(wait_udp_requests);
    wait_requests.detach();
}

//      TCP
static void send_tcp_message(struct TCP_Response response, int client) {
    send(client, &response, sizeof(response), 0);
}

static void wait_tcp_requests(int client) {
    struct TCP_Request client_request;
    struct TCP_Response response;
    while (!exit_program && client_request.type != 10) {
        recv(client, &client_request, sizeof (client_request), MSG_WAITALL);

        switch (client_request.type) {
            case 0: //Login
                cout << client_request.param1 << client_request.param2 << endl;
                if (users_list[client_request.param1] == client_request.param2) response.type = 0;
                else response.type = 1;
                send_tcp_message(response, client);
                break;
            case 1: // Upload
                break;
            case 2: // Remove file
                break;
            case 3: // Change directory
                break;
            case 4: // New directory
                break;
            case 5: // Remove directory
                break;
            case 6: // User status
                break;
            case 7: // Create new user
                break;
            case 8: // Remove user
                break;
            case 9: // Change password
                break;
            case 10: // Disconnect
                connected_clients--;
                break;
        }
    }
}

static void wait_tcp_clients() {
    int new_client;
    while (!exit_program && server_live) {
        new_client = accept(tcp_server, nullptr, nullptr);
        connected_clients++;

        // Create thread for wait requests
        thread wait_messages(wait_tcp_requests, new_client);
        wait_messages.detach();
    }
}

void start_tcp_server(){
    // TCP (for login, messages and transactions)
    tcp_server = socket(AF_INET, SOCK_STREAM, 0); // Setting TCP socket

    cout << "Creating TCP server... ";
    if (tcp_server < 0) {
        cout << "failed." << endl;
        exit(EXIT_FAILURE);
    }
    cout << "done." << endl;

    // Reusing socket
    cout << "Setting socket for reuse... ";
    int opt = 1;
    if (setsockopt(tcp_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cout << "failed." << endl;
        exit(EXIT_FAILURE);
    }
    cout << "done." << endl;

    // Setting tcp server address
    sockaddr_in server_addr;
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // Binding tcp server
    cout << "Starting TCP server... ";
    if (bind (tcp_server, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cout << "failed." << endl;
        exit(EXIT_FAILURE);
    }
    cout << "done." << endl;
    listen(tcp_server, max_clients);

    cout << "TCP server started at: " << ip << ":" << port << endl;
    server_live = true;
    thread wait_clients(wait_tcp_clients);
    wait_clients.detach();
}

void stop_server() {
    connected_clients = 0;
    shutdown(tcp_server, SHUT_RDWR);
    shutdown(udp_server, SHUT_RDWR);
    server_live = false;
}
