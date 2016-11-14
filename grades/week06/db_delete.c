/**
 * @file db_delete.c
 * @brief Delete an image in a file
 *
 * @date 17 April 2016
 */

#include <string.h> // for strcmp
#include <stdio.h> // for sprintf
#include "pictDB.h"

/**
 * @brief Delete an image in the file
 *
 * @param pictdb_file* A pointer to a structure in memory with header and metadata.
 * @param const char* picture_name the picture we want to delete
 *
 * @return 0 if no errors, otherwise an error.
 */
int do_delete(const char* picture_name, struct pictdb_file* db_file)
{
    if(db_file->header.num_files == 0) {
        /* Correcteur : ERR_FILE_NOT_FOUND
         * -0.25pt
         */
        return ERR_IO;
    }
    unsigned int index = 0;   //Position of the image to delete
    while(strcmp(picture_name, db_file->metadata[index].pict_id) != 0 && index < MAX_MAX_FILES) {
        /* Correcteur : optimisation. D'abord tester si l'image est valide ou non
         * avant de comparer les noms d'images. (Les comparaisons sont couteuses)
         * -0.5pt
         */
        index += 1;
    }
    //If index == MAX_MAX_FILES, the image is not in the metadatas
    /* Correcteur : max_max_files n'est pas vraiment la taille de metadata
     * (pas toujours)
     * -0.5pt
     */
    if(index >= MAX_MAX_FILES) {
        return ERR_FILE_NOT_FOUND;
    }

    //Position of the image in the file
    size_t pict_position = sizeof(struct pictdb_header) + index * sizeof(struct pict_metadata);
    /* Correcteur : utiliser la constante EMPTY plutot que 0 
     * -0.5pt
     */
    db_file->metadata[index].is_valid = 0;
    int return_value_fseek = fseek(db_file->fpdb, pict_position, SEEK_SET);
    size_t return_value_fwrite = 0;

    if(return_value_fseek == 0) {
        /* Correcteur : (the whole METADATA) ;) */
        //Rewrite the whole image
        return_value_fwrite = fwrite(&db_file->metadata[index], sizeof(struct pict_metadata), 1, db_file->fpdb);
        if(return_value_fwrite == 1) {
            //Update the header
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
