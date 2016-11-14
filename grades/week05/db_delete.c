#include <string.h> // for strcmp
#include <stdio.h> // for sprintf
#include "pictDB.h"




int do_delete(const char* picture_name, struct pictdb_file* db_file)
{
    //A changer ...
    if(db_file->header.num_files == 0) {
        return ERR_IO;
    }
    unsigned int index = 0;   //position de l'image à supprimer.
    while(strcmp(picture_name, db_file->metadata[index].pict_id) != 0 && index < MAX_MAX_FILES) {
        index += 1;
    }
    //si index == MAX_MAX_FILE, l'image recherchée n'est pas dans la base de donnée.
    if(index >= MAX_MAX_FILES) {
        return ERR_FILE_NOT_FOUND;
    }

    size_t pict_position = sizeof(struct pictdb_header) + index * sizeof(struct pict_metadata);
    db_file->metadata[index].is_valid = 0;

    int return_value_fseek = fseek(db_file->fpdb, sizeof(struct pictdb_header) + index*sizeof(struct pict_metadata), SEEK_SET);
    size_t return_value_fwrite = 0;
    if(return_value_fseek == 0) {
        return_value_fwrite = fwrite(&db_file->metadata[index], sizeof(struct pict_metadata), 1, db_file->fpdb);
        if(return_value_fwrite == 1) {
            db_file->header.db_version += 1;
            db_file->header.num_files -= 1;
            return_value_fseek = fseek(db_file->fpdb, 0, SEEK_SET);
            return_value_fwrite = fwrite(&db_file->header, sizeof(struct pictdb_header), 1, db_file->fpdb);
        }
    }
    if(return_value_fseek != 0 || return_value_fwrite != 1) {
        return ERR_IO;
    }
    return 0;
}