#include <iostream>
#include "net.h"
using namespace std;

// Global variables
bool exit_program = false;

int main() {
    string input;
    int client;

    cout << "Welcome to Clowd client!\n" << endl;
    while (!exit_program) {
        cout << ">> ";
        cin >> input;

        // Verify input
        if (input == "connect") get_server_info(client);
        else if (input == "exit_program") exit_program = true;
        else cout << "Unknown command: " << input << endl;
    }

    return 0;
}
