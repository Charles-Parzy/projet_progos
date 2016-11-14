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
    /* Correcteur : utilisation de MAX_MAX_FILES au lieu de 
     * header.max_files. -1pt
     * */
    int return_value = 0;
    size_t number_header = 0;
    size_t number_metadata = 0;
    // Sets the DB header name
    strncpy(db_file->header.db_name, CAT_TXT,  MAX_DB_NAME);
    db_file->header.db_name[MAX_DB_NAME] = '\0';
    //Initialisation
    db_file->header.db_version = 0;
    db_file->header.num_files = 0;
    /* Correcteur : utilisation de MAX_MAX_FILES au lieu de 
     * db_file->header.max_files et donc pas de vérification non
     * plus que le max_files fourni n'est pas > MAX_MAX_FILES. -0.5pt
     */
    for(int i = 0; i < MAX_MAX_FILES; i++) {
        /* Correcteur : pas d'utilisation des constantes NON EMPTY/EMPTY
         * pour initialiser -0.5pt
         */
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

    /* Correcteur : que se passe-t-il si il y a eu erreur précédemment ? 
     * Cette ligne ne sera pas affichée dans ce cas là. -0.5pt
     */
    printf("%zu item(s) written\n", number_header+number_metadata);

    /* Correcteur : aucun fclose pour fermer db_file->fpdb. do_create 
     * est "one shot" donc une fois qu'elle est appellée, le programme 
     * se termine. Du coup, il faut fermer le fichier ouvert précedemment. 
     * -1.5pt
     */
    return return_value;
}
