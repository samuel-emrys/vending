/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "../includes/ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, 
const char * stock_name)
{
    FILE *coins_file, *stock_file;
    Stock * data = malloc(sizeof(*data));

    coins_file = fopen(coins_name, "r");
    stock_file = fopen(stock_name, "r");
    if (coins_file == NULL)
    {
        fprintf(stderr, "Unable to open file %s\n", coins_name);
        return FALSE;
    }
    else if (stock_file == NULL)
    {
        fprintf(stderr, "Unable to open file %s\n", stock_name);
        return FALSE;
    }
    else if(check_empty(stock_file) || check_empty(coins_file))
    {
        printf("ERROR: One of the imported files is empty.\n");
        return FALSE;
    }
    else if(!load_stock(system, stock_file, data) || !load_coins(system, coins_file))
    {
        return FALSE;
    }
    else
    {
        system->stock_file_name = stock_name;
        system->coin_file_name = coins_name;
    }
    fclose(stock_file);
    fclose(coins_file);
    free(data);
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    init_cash_register(system);
    system->item_list = initialiseLinkedList();
    system->coin_file_name=NULL;
    system->stock_file_name=NULL;
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
    freeLinkedList(system->item_list);
}

enum input_result get_input(int long * select,char string_input[LSIZE],int type, int size)
{
    char * charSelectEndPtr=NULL;
    char charSelect[LSIZE];
    int long selection;

    while(TRUE)
    {
        if(fgets(charSelect, LSIZE, stdin) == NULL)
        /* Ctrl-D check, return to main menu. */
        {
            /*read_rest_of_line();*/
            /*can't be here for coreteaching servers, has to be here for osx*/
            return RTM;
        }
        else
        {    
            if (charSelect[strlen(charSelect)-1] != '\n')
            /*ensures length of string is within allocated string length*/
            {
                printf("Error: Input too long. Please Re-enter \n");
                read_rest_of_line();
                /*clears left over input from the input buffer */
            }

            else if (charSelect[0] == '\n')
            /*returns to menu if new line command is given on a new line */
            {
                return RTM;
            }
            else 
            {
                charSelect[strlen(charSelect)-1] = '\0';

                if (type == 0)
                /*if output required is a number, i.e. column/menu selection*/
                {
                    selection = strtol(charSelect, &charSelectEndPtr, 10);
                    *select = selection;
                    break;
                }
                else if (type == 1)
                /*if output required is a string i.e. name entry*/
                {
                    if (size <= strlen(charSelect))
                    {
                        printf("Error: Input too long. Please Re-enter.\n");
                        return FAILURE;
                    }
                    else
                    {
                        strcpy(string_input,charSelect);
                    }
                    /*copies contents of charSelect into string_input*/
                    break;
                }
            }
        }
    }
    return SUCCESS;
}

BOOLEAN abort_program(struct ppd_system * system)
{
    printf("You have aborted the program\n");
    system_free(system);
    exit(0);
    return TRUE;
}

BOOLEAN check_empty(FILE *file)
{
/*    long offset = ftell(file);*/
    fseek(file,0,SEEK_END);

    if (ftell(file)==0)
    {
        return TRUE;
    }

    rewind(file);
    return FALSE;
}

