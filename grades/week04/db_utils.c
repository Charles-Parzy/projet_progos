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
void print_header (const struct pictdb_header header) {
    printf("*****************************************\n");
    printf("**********DATABASE HEADER START**********\n");
    printf("DB NAME: %31s\n", header.db_name);
    printf("VERSION: %" PRIu32 "\n", header.db_version);
    printf("IMAGE COUNT: %" PRIu32 "\t\t", header.num_files);
    printf("MAX IMAGES: %" PRIu32 "\n", header.max_files);
    /* Correcteur : utiliser les constantes RES_SMALL ET RES_THUMBNAIL pour les indices
     * de tableau. -1pt
     */
    printf("THUMBNAIL: %" PRIu16 " x %" PRIu16 "\t", header.res_resized[0], header.res_resized[1]);
    printf("SMALL: %" PRIu16 " x %" PRIu16 "\n", header.res_resized[2], header.res_resized[3]);
    printf("***********DATABASE HEADER END***********\n");
    printf("*****************************************\n");
}

/********************************************************************//**
 * Metadata display.
 */
void
print_metadata (const struct pict_metadata metadata) {
    char sha_printable[2*SHA256_DIGEST_LENGTH+1];
    sha_to_string(metadata.SHA, sha_printable);

     printf("PICTURE ID: %s\n",metadata.pict_id);
     printf("SHA: %s\n", sha_printable);
     printf("VALID: %" PRIu16 "\n", metadata.is_valid);
     printf("UNUSED: %" PRIu16 "\n", metadata.unused_16);
     printf("OFFSET ORIG. : %" PRIu64 "\t\t", metadata.offset[RES_ORIG]);
     printf("SIZE ORIG. : %" PRIu32 "\n", metadata.size[RES_ORIG]);
     printf("OFFSET THUMB.: %" PRIu64 "\t\t", metadata.offset[RES_THUMB]);
     printf("SIZE THUMB.: %" PRIu32 "\n", metadata.size[RES_THUMB]);
     printf("OFFSET SMALL : %" PRIu64 "\t\t", metadata.offset[RES_SMALL]);
     printf("SIZE SMALL : %" PRIu32 "\n", metadata.size[RES_SMALL]);
     printf("ORIGINAL: %" PRIu32 " x %" PRIu32 "\n", metadata.res_orig[0], metadata.res_orig[1]);
     printf("*****************************************\n");
}
