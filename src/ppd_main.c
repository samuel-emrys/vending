/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "../includes/ppd_main.h"
#include "../includes/ppd_menu.h"
#include "../includes/ppd_options.h"
#include "../includes/ppd_utility.h"
#include "../includes/ppd_stock.h"
#include "../includes/ppd_coin.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{

    /* represents the data structures to manage the system */
    struct ppd_system system;
    int long selection;
    struct menu_item menu[MENU_SIZE];

    /* validate command line arguments */
    if (argc != NUM_ARGS)
    {
        fprintf(stderr, "Invalid Number of arguments entered. Please enter "
            "2 arguments only in the form of <coinfile> <stockfile>\n");
        return EXIT_FAILURE;
    }


    /* init the system */
    system_init(&system);

    /* load data */
    if(!load_data(&system, argv[FIRSTFILE], argv[SECONDFILE]))
    {
        return EXIT_FAILURE;
    }

    /* test if everything has been initialised correctly */

    /* initialise the menu system */
    init_menu(menu);
    /* loop, asking for options from the menu */
    while (TRUE)
    {
        selection = main_menu(menu, &system);
        if(!menu[selection-1].function(&system))
        {
            printf("\nFunction aborted.\n");
        }
    }
    /* run each option selected */

    /* until the user quits */

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */
    system_free(&system);

    return EXIT_SUCCESS;
}
