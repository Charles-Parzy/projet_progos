/**
 * @file db_list.c
 * @brief Print the content of the the file.
 * Print first the header and then the metadata of the image if possible.
 *
 * @date 13 March 2016
 */

#include "pictDB.h"

/**
 * @brief Print the information of an image.
 *
 * @param db_file In memory structure with header and metadata.
 */
void do_list (const struct pictdb_file* file)
{
    print_header(&file->header);
    if(file->header.num_files != 0) {
        for(int i = 0; i < MAX_MAX_FILES; i++) {
            if(file->metadata[i].is_valid != 0) {
                print_metadata(&file->metadata[i]);
            }
        }
    } else {
        printf("<< empty database >>\n");
    }
}