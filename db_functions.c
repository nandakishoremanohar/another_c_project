#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_database(const char *db_name) {
    if (db_name == NULL) {
        printf("Error: Database name is NULL.\n");
        return;
    }
    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);
    // Allocate memory for full filename
    char *full_name = (char *)malloc(name_len + ext_len + 1);
    if (full_name == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    // Construct full filename
    strcpy(full_name, db_name);
    strcat(full_name, extension);
    // Open file for writing
    FILE *file = fopen(full_name, "w");
    if (file == NULL) {
        printf("Error: Could not create database file %s\n", full_name);
        free(full_name);
        return;
    }
    // Write initial JSON structure
    fprintf(file, "{\n\t\"database_name\": \"%s\",\n\t\"records\": []\n}", db_name);
    printf("Database created successfully: %s\n", full_name);
    fclose(file);
    free(full_name);
}
// Function to insert a record with a specific key and pair into the JSON database
void insert_record_with_key(const char *db_name, const char *key_name, const char *record) {
    if (db_name == NULL || key_name == NULL || record == NULL) {
        printf("Error: Database name, key name, or record is NULL.\n");
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = (char *)malloc(name_len + ext_len + 1);
    if (!full_name) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (!file) {
        printf("Error: Could not open database file %s\n", full_name);
        free(full_name);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        free(full_name);
        return;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    char *pos = strrchr(buffer, ']');
    if (!pos) {
        printf("Error: Invalid JSON format in database.\n");
        free(buffer);
        free(full_name);
        return;
    }

    int last_id = 0;
    char *id_pos = buffer;
    while ((id_pos = strstr(id_pos, "\"id\":")) != NULL) {
        int temp_id = 0;
        sscanf(id_pos, "\"id\": %d", &temp_id);
        if (temp_id > last_id)
            last_id = temp_id;
        id_pos += 5;
    }

    int new_id = last_id + 1;

    char new_record[512];
    if (*(pos - 1) == '[') {
        // No existing records
        sprintf(new_record, "\n\t\t{\"id\": %d, \"%s\": \"%s\"}\n\t", new_id, key_name, record);
    } else {
        // Existing records
        sprintf(new_record, ",\n\t\t{\"id\": %d, \"%s\": \"%s\"}\n\t", new_id, key_name, record);
    }

    file = fopen(full_name, "w");
    if (!file) {
        printf("Error: Could not open database file for writing.\n");
        free(buffer);
        free(full_name);
        return;
    }

    *pos = '\0';
    fprintf(file, "%s%s]", buffer, new_record);

    printf("Record inserted successfully with ID %d into %s\n", new_id, full_name);

    fclose(file);
    free(buffer);
    free(full_name);
}

void read_database(const char *db_name) {
    if (db_name == NULL) {
        printf("Error: Database name is null\n");
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = malloc(name_len + ext_len + 1);
    if (!full_name) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        free(full_name);
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s\n", buffer);
    }

    fclose(file);
    free(full_name);
}

void delete_database(const char *db_name) {

    if (db_name == NULL) {
        printf("Error: Database name is NULL.\n");
        return;
    }
    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);
    // Allocate memory for full filename
    char *full_name = (char *)malloc(name_len + ext_len + 1);
    if (full_name == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    // Construct full filename
    strcpy(full_name, db_name);
    strcat(full_name, extension);
    // Delete the file
    if (remove(full_name) == 0) {
        printf("Database deleted successfully: %s\n", full_name);
    } else {
        printf("Error: Could not delete database file %s\n", full_name);
    }
    free(full_name);
}

void update_record(const char *db_name, int target_id, const char *new_value) {
    if (!db_name || !new_value) {
        printf("Error: Invalid arguments or no values presented\n");
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = malloc(name_len + ext_len + 1);
    if (!full_name) {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (!file) {
        printf("Could not open file\n");
        free(full_name);
        return;
    }

    // Read entire file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        free(full_name);
        return;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    // Find the record with the given ID
    char search[32];
    sprintf(search, "\"id\": %d", target_id);

    char *id_pos = strstr(buffer, search);
    if (!id_pos) {
        printf("Record with ID %d not found\n", target_id);
        free(buffer);
        free(full_name);
        return;
    }

    // Find the start and end of the value string
    char *value_start = strchr(id_pos, ':'); // after "id"
    value_start = strchr(value_start + 1, '"'); // opening quote
    char *value_end = strchr(value_start + 1, '"'); // closing quote

    if (!value_start || !value_end) {
        printf("Malformed record\n");
        free(buffer);
        free(full_name);
        return;
    }

    // Build new buffer
    size_t prefix_len = value_start + 1 - buffer;
    size_t suffix_len = strlen(value_end);

    char *new_buffer = malloc(prefix_len + strlen(new_value) + suffix_len + 1);
    if (!new_buffer) {
        free(buffer);
        free(full_name);
        return;
    }

    strncpy(new_buffer, buffer, prefix_len);
    strcpy(new_buffer + prefix_len, new_value);
    strcpy(new_buffer + prefix_len + strlen(new_value), value_end);

    // Write updated file
    file = fopen(full_name, "w");
    if (!file) {
        printf("Could not write file\n");
        free(buffer);
        free(new_buffer);
        free(full_name);
        return;
    }

    fputs(new_buffer, file);

    fclose(file);
    free(buffer);
    free(new_buffer);
    free(full_name);

    printf("Record with ID %d updated successfully\n", target_id);
}

void delete_record_by_id(const char *db_name, int target_id) {
    if (!db_name) {
        printf("Error: Invalid database name\n");
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = malloc(name_len + ext_len + 1);
    if (!full_name) {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (!file) {
        printf("Could not open file\n");
        free(full_name);
        return;
    }

    // Read entire file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        free(full_name);
        return;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    // Find record start
    char search[32];
    sprintf(search, "\"id\": %d", target_id);

    char *id_pos = strstr(buffer, search);
    if (!id_pos) {
        printf("Record with ID %d not found\n", target_id);
        free(buffer);
        free(full_name);
        return;
    }

    // Find start of object '{'
    char *record_start = id_pos;
    while (record_start > buffer && *record_start != '{')
        record_start--;

    // Find end of object '}'
    char *record_end = strchr(id_pos, '}');
    if (!record_end) {
        printf("Malformed record\n");
        free(buffer);
        free(full_name);
        return;
    }
    record_end++; // include '}'

    // Handle trailing comma
    if (*record_end == ',')
        record_end++;

    // Build new buffer without the record
    size_t prefix_len = record_start - buffer;
    size_t suffix_len = strlen(record_end);

    char *new_buffer = malloc(prefix_len + suffix_len + 1);
    if (!new_buffer) {
        free(buffer);
        free(full_name);
        return;
    }

    strncpy(new_buffer, buffer, prefix_len);
    strcpy(new_buffer + prefix_len, record_end);

    // Write back to file
    file = fopen(full_name, "w");
    if (!file) {
        printf("Could not write file\n");
        free(buffer);
        free(new_buffer);
        free(full_name);
        return;
    }

    fputs(new_buffer, file);

    fclose(file);
    free(buffer);
    free(new_buffer);
    free(full_name);

    printf("Record with ID %d deleted successfully\n", target_id);
}
