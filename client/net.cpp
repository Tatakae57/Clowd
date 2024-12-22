#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

#define UDP_PORT 8080

// Global variables
char ip[15] = "127.0.0.1";
unsigned short port;

// External variables
extern bool exit_program;

// Functions
//      UDP
static bool verify_connection() {
    int client = socket(AF_INET, SOCK_DGRAM, 0); // UDP client

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(UDP_PORT);
    server_address.sin_addr.s_addr = inet_addr(ip);

    // Sending request
    unsigned char message = 10; // Ask if the server can accept client
    sendto(client, (const unsigned char *)&message, sizeof(message), MSG_CONFIRM, (const struct sockaddr *)&server_address, sizeof(server_address));

    // Receiving response
    socklen_t len;
    recvfrom(client, (unsigned char *)&message, sizeof(message), MSG_WAITALL, (struct sockaddr *)&server_address, &len);

    printf("Received: %hu\n", message);
    if (message == 1) return true;
    else if (message == 0) return false;
}

// TCP
static void connect_to_server(int &client) {
    if (!verify_connection()) {
        printf("The server is full.\n");
        return;
    }

    printf("Trying connection... ");
    // Create socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(ip);

    if (connect(client, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("failed.\n");
    }
    else
        printf("Connected.\n");
}

void disconnect_from_server(int &client) {

}

//  General
void get_server_info(int &client) {
    printf("IP: ");
    cin >> ip;
    printf("Port: ");
    cin >> port;
    connect_to_server(client);
}
