
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_database(const char *db_name) {
    if (db_name == NULL) {
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = (char *)malloc(name_len + ext_len + 1);
    if (full_name == NULL) {
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "w");
    if (file == NULL) {
        free(full_name);
        return;
    }

    fprintf(file, "{\n\t\"database_name\": \"%s\",\n\t\"records\": []\n}", db_name);

    fclose(file);
    free(full_name);
}

void insert_record_with_key(const char *db_name, const char *key_name, const char *record) {
    if (db_name == NULL || key_name == NULL || record == NULL) {
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = (char *)malloc(name_len + ext_len + 1);
    if (!full_name) {
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (!file) {
        free(full_name);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        free(full_name);
        return;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    char *pos = strrchr(buffer, ']');
    if (!pos) {
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
        sprintf(new_record, "\n\t\t{\"id\": %d, \"%s\": \"%s\"}\n\t", new_id, key_name, record);
    } else {
        sprintf(new_record, ",\n\t\t{\"id\": %d, \"%s\": \"%s\"}\n\t", new_id, key_name, record);
    }

    file = fopen(full_name, "w");
    if (!file) {
        free(buffer);
        free(full_name);
        return;
    }

    *pos = '\0';
    fprintf(file, "%s%s]", buffer, new_record);

    fclose(file);
    free(buffer);
    free(full_name);
}

void read_database(const char *db_name) {
    if (db_name == NULL) {
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = malloc(name_len + ext_len + 1);
    if (!full_name) {
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (file == NULL) {
        free(full_name);
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        /* no output */
    }

    fclose(file);
    free(full_name);
}

void delete_database(const char *db_name) {
    if (db_name == NULL) {
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = (char *)malloc(name_len + ext_len + 1);
    if (full_name == NULL) {
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    remove(full_name);

    free(full_name);
}

void update_record(const char *db_name, int target_id, const char *new_value) {
    if (!db_name || !new_value) {
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = malloc(name_len + ext_len + 1);
    if (!full_name) {
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (!file) {
        free(full_name);
        return;
    }

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

    char search[32];
    sprintf(search, "\"id\": %d", target_id);

    char *id_pos = strstr(buffer, search);
    if (!id_pos) {
        free(buffer);
        free(full_name);
        return;
    }

    char *value_start = strchr(id_pos, ':');
    value_start = strchr(value_start + 1, '"');
    char *value_end = strchr(value_start + 1, '"');

    if (!value_start || !value_end) {
        free(buffer);
        free(full_name);
        return;
    }

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

    file = fopen(full_name, "w");
    if (!file) {
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
}

void delete_record_by_id(const char *db_name, int target_id) {
    if (!db_name) {
        return;
    }

    const char *extension = ".json";
    size_t name_len = strlen(db_name);
    size_t ext_len = strlen(extension);

    char *full_name = malloc(name_len + ext_len + 1);
    if (!full_name) {
        return;
    }

    strcpy(full_name, db_name);
    strcat(full_name, extension);

    FILE *file = fopen(full_name, "r");
    if (!file) {
        free(full_name);
        return;
    }

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

    char search[32];
    sprintf(search, "\"id\": %d", target_id);

    char *id_pos = strstr(buffer, search);
    if (!id_pos) {
        free(buffer);
        free(full_name);
        return;
    }

    char *record_start = id_pos;
    while (record_start > buffer && *record_start != '{')
        record_start--;

    char *record_end = strchr(id_pos, '}');
    if (!record_end) {
        free(buffer);
        free(full_name);
        return;
    }
    record_end++;

    if (*record_end == ',')
        record_end++;

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

    file = fopen(full_name, "w");
    if (!file) {
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
}
