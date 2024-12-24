#include <iostream>
#include "net.h"
using namespace std;

// Global variables
bool exit_program = false;

// External variables
extern bool connected;

int main() {
    string input;

    cout << "Welcome to Clowd client!\n" << endl;
    while (!exit_program) {
        cout << ">> ";
        cin >> input;

        // Verify input
        if (input == "connect") get_server_info();
        else if (input == "login") login();
        else if (input == "exit_program") {
            exit_program = true;
            if (connected) disconnect_from_server();
        }
        else if (input == "disconnect") {
            if (connected) disconnect_from_server();
            else cout << "Not connected." << endl;
        }
        else cout << "Unknown command: " << input << endl;
    }

    return 0;
}
