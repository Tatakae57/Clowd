#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#define UDP_PORT 8080

using namespace std;

// Local variables
static int client;

// Global variables
char ip[15] = "127.0.0.1";
unsigned short port;
bool connected = false;

// External variables
extern bool exit_program;

// Structures
struct Request {
    unsigned char type;
    char param1[50];
    char param2[50];
    uint32_t file_content;
};

struct Response {
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
    return false;
}

//      TCP
static void connect_to_server() {
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
    else {
        printf("Connected.\n");
        connected = true;
    }
}

//          Sending messages
static void send_message(struct Request &message) {
    send(client, &message, sizeof(message), 0);
}

void send_login(string user, string password) {
    struct Request message = {};
    message.type = 0; // Login type
    sprintf(message.param1, "%s", user.c_str());
    sprintf(message.param2, "%s", password.c_str());
    send_message(message);

    struct Response response;
    response.type = 20;
    recv(client, &response, sizeof(response), MSG_WAITALL);
    if (response.type == 0) cout << "Logged in." << endl;
    else cout << "Not loged in: incorrect user or password." << endl;
}

//          Closing
void close_socket() {
    connected = false;
    shutdown(client, SHUT_RDWR);
    close(client);
}

void disconnect_from_server() {
    if (!connected) {
        printf("Not connected.\n");
        return;
    }

    // Sending disconnection message
    struct Request message;
    message.type = 10;
    send_message(message);

    close_socket();
}

//      General
void get_server_info() {
    printf("IP: ");
    cin >> ip;
    printf("Port: ");
    cin >> port;
    connect_to_server();
}

void login() {
    string user, password;
    cout << "User: ";
    cin >> user;
    cout << "Password: ";
    cin >> password;

    send_login(user, password);
}
