#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database_functions.h"

int main() {
    char command[256];
    char db_name[128];
    printf("compiled by nandakishore raghunath\n");
    printf("Welcome to nandaDB Terminal!\n");
    printf("Enter database name: ");
    scanf("%127s", db_name);

    while (1) {
        printf("\nnandadb> ");
        scanf(" %255[^\n]", command); // read full line

        if (strcmp(command, "exit") == 0) {
            printf("Exiting byeeeeeeeeeeeeee\n");
            break;
        }
        else if (strcmp(command, "create") == 0) {
            create_database(db_name);
        }
        else if (strcmp(command, "read") == 0) {
            read_database(db_name);
        }
        else if (strncmp(command, "insert ", 7) == 0) {
            char key[128], value[128];
            // extract key and value from input: "insert key value"
            if (sscanf(command + 7, "%127s %127[^\n]", key, value) == 2) {
                insert_record_with_key(db_name, key, value);
            } else {
                printf("Usage: insert <key> <value>\n");
            }
        }
        else if (strncmp(command, "update ", 7) == 0) {
            int id;
            char value[128];
            if (sscanf(command + 7, "%d %127[^\n]", &id, value) == 2) {
                update_record(db_name, id, value);
            } else {
                printf("Usage: update <id> <new_value>\n");
            }
        }
        else if (strncmp(command, "delete_record ", 14) == 0) {
            int id;
            if (sscanf(command + 14, "%d", &id) == 1) {
                delete_record_by_id(db_name, id);
            } else {
                printf("Usage: delete_record <id>\n");
            }
        }
        else if (strcmp(command, "delete_db") == 0) {
            delete_database(db_name);
        }
        else {
            printf("Unknown command: %s\n", command);
            printf("Available commands: create, read, insert, update, delete_record, delete_db, exit\n");
        }

        // clear stdin buffer in case of leftover input
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    return 0;
}
