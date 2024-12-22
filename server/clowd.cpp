#include "files.h"
#include "net.h"
#include "commands.h"
#include <iostream>
using namespace std;

// Global variables
bool exit_program = false;

// Extern variables
extern bool server_live;
extern string ip;
extern unsigned short port, max_clients, max_users, max_space;

int main() {
    // Local variables
    string input;

    read_config(); // Read or write config

    cout << "Welcome to Clowd!\n" << endl;
    // Loop
    while (!exit_program) {
        cout << ">> ";
        cin >> input;

        // Verify input
        //      Server
        if (input == "start_server") {
            start_udp_server();
            start_tcp_server();
        }
        else if (input == "set_max_clients") {
            cout << "Max clients: ";
            cin >> input;
            max_clients = stoi(input);
            write_config();
        }
        else if (input == "set_max_users") {
            cout << "Max users: ";
            cin >> input;
            max_users = stoi(input);
            write_config();
        }
        else if (input == "set_max_space") {
            cout << "Max space: ";
            cin >> input;
            max_space = stoi(input);
            write_config();
        }
        else if (input == "set_port") {
            cout << "Server port: ";
            cin >> input;
            port = stoi(input);
            write_config();
        }
        else if (input == "set_ip") {
            cout << "Server ip: ";
            cin >> ip;
            write_config();
        }

        //      Others
        else if (input == "exit_program") {
            stop_server();
            exit_program = true;
        }
        else if (input == "stop_server") {
            if (server_live) {
                stop_server();
                cout << "Server stopped." << endl;
            }
            else
                cout << "The server is not live." << endl;
        }
        else if (input == "server_configuration") server_configuration();

        else cout << "Unknown command: " << input << "\n" << endl;
    }
}
