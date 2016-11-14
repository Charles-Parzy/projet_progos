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
void do_list (const struct pictdb_file file) {
	print_header(file.header);
    // Correcteur : attenton à l'indentation
		if(file.header.num_files != 0) {
            // Correcteur : pas une bonne pratique d'utiliser une constante comme taille
            // de tableau, alors que cette taille se trouve dans pictdb_file.header.max_files
            // Cela complexifie un futur refactoring. 0.5/1pt
			for(int i = 0; i < MAX_MAX_FILES; i++) {
                // Correcteur : utiliser les constantes EMPTY/NON EMPTY -0.5pt
				if(file.metadata[i].is_valid != 0) {
					print_metadata(file.metadata[i]);
				}
			}
		} else {
			printf("<< empty database >>\n");
		}
}
