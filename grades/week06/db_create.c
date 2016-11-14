/**
 * @file db_create.c
 * @brief pictDB library: do_create implementation.
 *
 * @date 2 Nov 2015
 */

#include "pictDB.h"
#include <string.h> // for strncpy

/********************************************************************//**
 * Creates the database called db_filename. Writes the header and the
 * preallocated empty metadata array to database file.
 */
int do_create(const char* file_name, struct pictdb_file* db_file)
{
    int return_value = 0;
    size_t number_header = 0;
    size_t number_metadata = 0;
    // Sets the DB header name
    strncpy(db_file->header.db_name, CAT_TXT,  MAX_DB_NAME);
    db_file->header.db_name[MAX_DB_NAME] = '\0';
    //Initialisation
    db_file->header.db_version = 0;
    db_file->header.num_files = 0;
    for(int i = 0; i < MAX_MAX_FILES; i++) {
        db_file->metadata[i].is_valid = 0;
    }

    db_file->fpdb = NULL;
    db_file->fpdb = fopen(file_name, "wb");

    if(db_file->fpdb != NULL) {
        number_header = fwrite(&db_file->header, sizeof(struct pictdb_header), 1, db_file->fpdb);
        number_metadata = fwrite(db_file->metadata, sizeof(struct pict_metadata), MAX_MAX_FILES, db_file->fpdb);
    }

    if(number_header != 1 || number_metadata != MAX_MAX_FILES || db_file->fpdb == NULL) {
        return_value = ERR_IO;
    }

    printf("%zu item(s) written\n", number_header+number_metadata);
    return return_value;
}
