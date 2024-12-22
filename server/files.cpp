#include "files.h"
#include <string.h>
#include <string>
#include <map>

using namespace std;

// Local variables
static map <string, string> users_map;

// Extern variables
extern string ip;
extern unsigned short port, max_clients, max_users, max_space;

// Functions
//      Lines
static void split_line(char line[], char param1[], char param2[]) {
    bool space = false;
    unsigned char p1_counter = 0, p2_counter = 0;
    for (unsigned char i = 0; line[i] != '\0'; i++) {
        if (line[i] == ' ') space = true;
        else if (space == false)
            param1[p1_counter++] = line[i];
        else if (space == true)
            param2[p2_counter++] = line[i];
    }
    param1[p1_counter] = '\0';
    param2[p2_counter] = '\0';
}

//      Users
static void read_users() {

}

//      Config
void write_config() {
    // Create config file
    FILE *config_file = fopen("config.txt", "w");

    // Writing config
    fprintf(config_file, "IP %s\n", ip.c_str());
    fprintf(config_file, "PORT %d\n", port);
    fprintf(config_file, "MAX_CLIENTS %d\n", max_clients);
    fprintf(config_file, "MAX_USERS %d\n", max_users);
    fprintf(config_file, "MAX_SPACE %d\n", max_space);

    fclose(config_file);
}

void read_config() {
    // Open and read config
    printf("Loading config... ");
    FILE *config_file = fopen("config.txt", "r");
    if (config_file == NULL) {
        // If the config file dont exists, create config and read it
        printf("\nConfig file not found, creating... ");
        write_config();
        printf("done.\n");
        read_config();
        return;
    }

    printf("founded. \nReading... ");
    char line[1024], part1[512], part2[512];
    while (fgets(line, sizeof(line), config_file)) { // Read file line by line
        if (line[0] == '\0' || line[0] == '#') continue; // Ignore empty lines or comments

        // Split line in two
        split_line(line, part1, part2);

        // Setting values
        if (!strcmp(part1, "IP")) {ip = part2; ip.pop_back();}
        else if (!strcmp(part1, "PORT")) port = stoi(part2);
        else if (!strcmp(part1, "MAX_CLIENTS")) max_clients = stoi(part2);
        else if (!strcmp(part1, "MAX_USERS")) max_users = stoi(part2);
        else if (!strcmp(part1, "MAX_SPACE")) max_space = stoi(part2);
    }
    fclose(config_file);
    printf("done.\n");
}
