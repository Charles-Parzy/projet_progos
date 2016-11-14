/**
 * @file pictDBM.c
 * @brief pictDB Manager: command line interpretor for pictDB core commands.
 *
 * Picture Database Management Tool
 *
 * @date 2 Nov 2015
 */

#include "pictDB.h"

#include <stdlib.h>
#include <string.h>

/********************************************************************//**
 * Opens pictDB file and calls do_list command.
 ********************************************************************** */
int
do_list_cmd (const char* filename)
{
    int return_value = 0;
    struct pictdb_file myfile;

    return_value = do_open(filename, "rb", &myfile);
    if(return_value == 0) {
        do_list(&myfile);
    }

    /* Correcteur : ton do_close vérifie déjà si fpdb != NULL.
     * -0.25pt
     */
    if(myfile.fpdb != NULL) {
        do_close(&myfile);
    }
    return return_value;
}

/********************************************************************//**
 * Prepares and calls do_create command.
********************************************************************** */
int
do_create_cmd (const char* filename)
{
    // This will later come from the parsing of command line arguments
    const uint32_t max_files =  10;
    const uint16_t thumb_res =  64;
    const uint16_t small_res = 256;

    puts("Create");

    struct pictdb_file file;

    file.header.res_resized[0] = thumb_res;
    file.header.res_resized[1] = thumb_res;
    file.header.res_resized[2] = small_res;
    file.header.res_resized[3] = small_res;
    file.header.max_files = max_files;
    int return_value;
    return_value = do_create(filename, &file);
    if(return_value == 0) {
        print_header(&file.header);
    }
    return return_value;
}

/********************************************************************//**
 * Displays some explanations.
 ********************************************************************** */
int
help (void)
{
    /* Correcteur : différence très mineure avec ce qui est demandé
     * -0.25pt
     */
    printf("pictDBM [COMMAND] [ARGUMENTS]\n");
    printf("  help: displays this help.\n");
    printf("  list <dbfilename>: list pictDB content.\n");
    printf("  create <dbfilename>: create a new pictDB.\n");
    printf("  delete <dbfilename> <pictID>: delete picture pictID from pictDB\n");
    return 0;
}

/********************************************************************//**
 * Deletes a picture from the database.
 */
int
do_delete_cmd (const char* filename, const char* pictID)
{
    if(strlen(pictID) == 0 || strlen(pictID) > MAX_PIC_ID) {
        return ERR_INVALID_PICID;
    }

    int check_do_open = 0;
    int check_do_delete = 0;
    struct pictdb_file db_file;

    //Open the file in Read and write
    check_do_open = do_open(filename, "rb+", &db_file);
    if(check_do_open == 0) {
        check_do_delete = do_delete(pictID, &db_file);
    }
    if(db_file.fpdb != NULL) {
        do_close(&db_file);
    }

    if(check_do_open != 0 || check_do_delete != 0) {
        /* Correcteur : ce serait mieux de retourner les valeurs check_do_open
         * ou check_do_delete elles-même.
         * -0.25pt
         */
        return ERR_IO;
    }

    return 0;
}

/********************************************************************//**
 * MAIN
 */
int main (int argc, char* argv[])
{
    int ret = 0;

    if (argc < 2) {
        ret = ERR_NOT_ENOUGH_ARGUMENTS;
    } else {
        /* **********************************************************************
         * TODO WEEK 08: THIS PART SHALL BE REVISED THEN (WEEK 09) EXTENDED.
         * **********************************************************************
         */
        argc--;
        argv++; // skips command call name
        if (!strcmp("list", argv[0])) {
            if (argc < 2) {
                ret = ERR_NOT_ENOUGH_ARGUMENTS;
            } else {
                ret = do_list_cmd(argv[1]);
            }
        } else if (!strcmp("create", argv[0])) {
            if (argc < 2) {
                ret = ERR_NOT_ENOUGH_ARGUMENTS;
            } else {
                ret = do_create_cmd(argv[1]);
            }
        } else if (!strcmp("delete", argv[0])) {
            if (argc < 3) {
                ret = ERR_NOT_ENOUGH_ARGUMENTS;
            } else {
                ret = do_delete_cmd(argv[1], argv[2]);
            }
        } else if (!strcmp("help", argv[0])) {
            ret = help();
        } else {
            ret = ERR_INVALID_COMMAND;
        }
    }

    if (ret) {
        fprintf(stderr, "ERROR: %s\n", ERROR_MESSAGES[ret]);
        (void)help();
    }

    return ret;
}
