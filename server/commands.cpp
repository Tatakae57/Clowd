#include "commands.h"
#include <string>

using namespace std;

// Extern variables
extern string ip;
extern unsigned short port, max_clients, max_users, max_space;

void help() {
    printf("Help:\n");
    printf("    start_server: Start the server.");
    printf("    stop_server: Stop server :v");
    printf("    help: Show this message.");
    printf("    exit_program: Exit program.");
}

void server_configuration() {
    printf("Server configurarion: \n");
    printf("    IP: %s\n", ip.c_str());
    printf("    Port: %hu\n", port);
    printf("    Max clients: %hu\n", max_clients);
    printf("    Max users: %hu\n", max_users);
    printf("    Max space: %hu\n", max_space);
}
