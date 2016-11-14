/* ** NOTE: undocumented in Doxygen
 * @file db_utils.c
 * @brief implementation of several tool functions for pictDB
 *
 * @date 2 Nov 2015
 */

#include "pictDB.h"
#include <stdint.h> // for uint8_t
#include <stdio.h> // for sprintf
#include <inttypes.h> // for PRIu
#include <openssl/sha.h> // for SHA256_DIGEST_LENGTH


/********************************************************************//**
 * Human-readable SHA
 */
static void
sha_to_string (const unsigned char* SHA,
               char* sha_string)
{
    if (SHA == NULL) {
        return;
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(&sha_string[i*2], "%02x", SHA[i]);
    }

    sha_string[2*SHA256_DIGEST_LENGTH] = '\0';
}

/********************************************************************//**
 * pictDB header display.
 */
void print_header (const struct pictdb_header* header)
{
    printf("*****************************************\n");
    printf("**********DATABASE HEADER START**********\n");
    printf("DB NAME: %31s\n", header->db_name);
    printf("VERSION: %" PRIu32 "\n", header->db_version);
    printf("IMAGE COUNT: %" PRIu32 "\t\t", header->num_files);
    printf("MAX IMAGES: %" PRIu32 "\n", header->max_files);
    printf("THUMBNAIL: %" PRIu16 " x %" PRIu16 "\t", header->res_resized[2*RES_THUMB], header->res_resized[(2*RES_THUMB)+1]);
    printf("SMALL: %" PRIu16 " x %" PRIu16 "\n", header->res_resized[2*RES_SMALL], header->res_resized[(2*RES_SMALL)+1]);
    printf("***********DATABASE HEADER END***********\n");
    printf("*****************************************\n");
}

/********************************************************************//**
 * Metadata display.
 */
void print_metadata (const struct pict_metadata* metadata)
{
    char sha_printable[2*SHA256_DIGEST_LENGTH+1];
    sha_to_string(metadata->SHA, sha_printable);

    printf("PICTURE ID: %s\n",metadata->pict_id);
    printf("SHA: %s\n", sha_printable);
    printf("VALID: %" PRIu16 "\n", metadata->is_valid);
    printf("UNUSED: %" PRIu16 "\n", metadata->unused_16);
    printf("OFFSET ORIG. : %" PRIu64 "\t\t", metadata->offset[RES_ORIG]);
    printf("SIZE ORIG. : %" PRIu32 "\n", metadata->size[RES_ORIG]);
    printf("OFFSET THUMB.: %" PRIu64 "\t\t", metadata->offset[RES_THUMB]);
    printf("SIZE THUMB.: %" PRIu32 "\n", metadata->size[RES_THUMB]);
    printf("OFFSET SMALL : %" PRIu64 "\t\t", metadata->offset[RES_SMALL]);
    printf("SIZE SMALL : %" PRIu32 "\n", metadata->size[RES_SMALL]);
    printf("ORIGINAL: %" PRIu32 " x %" PRIu32 "\n", metadata->res_orig[0], metadata->res_orig[1]);
    printf("*****************************************\n");
}

/**
 * @brief Open a pictdb_file and read its content (header and metadatas)
 *
 * @param pictdb_file* A pointer to a structure in memory with header and metadata.
 * @param const char* file_name name of the file
 * @param const char* opening_mode of the file
 *
 * @return 0 if no errors, otherwise an error.
 */
int do_open(const char* file_name, const char* opening_mode, struct pictdb_file* db_file)
{
    int result = 0;
    size_t header_result = 0;
    size_t metadata_result = 0;
    db_file->fpdb = fopen(file_name, opening_mode);
    if(db_file->fpdb == NULL) {
        result = ERR_IO;
    } else {
        header_result = fread(&db_file->header, sizeof(struct pictdb_header), 1, db_file->fpdb);
        metadata_result = fread(db_file->metadata, sizeof(struct pict_metadata), MAX_MAX_FILES, db_file->fpdb);
    }
    /* Correcteur : utilisation de MAX_MAX_FILES comme taille de metadata.
     * -0.5pt
     *
     *  De plus, on ne vérifie pas que header.max_files <= MAX_MAX_FILES avant de lire.
     *  -0.5pt
     */
    if(header_result != 1 || metadata_result != MAX_MAX_FILES) {
        result = ERR_IO;
    }
    return result;
}

/**
 * @brief Close the file of the pictdb_file structure
 *
 * @param pictdb_file* A pointer to a structure in memory with header and metadata.
 */
void do_close(struct pictdb_file* db_file)
{
    if(db_file->fpdb != NULL) {
        fclose(db_file->fpdb);
    }
}




