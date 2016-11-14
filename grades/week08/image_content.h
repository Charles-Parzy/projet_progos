/* Correcteur : entête doxygen manquant pour le fichier en général 
 * -0.5pt*/

#include "pictDB.h"

/**
* @brief Resize an existing image.
* @param res Index of the new resolution.
* @param db_file Pointer to a pictdb_file structure.
* @param index Postion of the existing image in the metadata.
* @return 0 if successfull otherwise an error.
*/
int lazily_resize(const int res, struct pictdb_file* db_file, size_t index);
