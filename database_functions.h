#ifndef DATABASE_FUNCTIONS_H
#define DATABASE_FUNCTIONS_H

// Creates a new JSON database with the given name
void create_database(const char *db_name);

// Inserts a record into an existing JSON database
void insert_record_with_key(const char *db_name, const char *key_name, const char *record); 

// Reads and displays the contents of the JSON database
void read_database(const char *db_name);

// Deletes the JSON database file
void delete_database(const char *db_name);

// Updates a record in the JSON database with the specified ID
void update_record(const char *db_name, int target_id, const char *new_value);

// Deletes a record from the JSON database with the specified ID
void delete_record_by_id(const char *db_name, int target_id);
#endif // DATABASE_H
