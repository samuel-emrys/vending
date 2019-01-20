/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "../includes/ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
    Node * node = system->item_list->head;
    printf("Items Menu\n----------\n");
    printf("%-6s|%-41s|%-10s|%-6s\n","ID","Name","Available","Price");
    printf("---------------------------------------------------------------------\n");
    while (node != NULL)
    {
        printf("%-6s|%-41s|%-10u|$%u.%02u\n",node->data->id, node->data->name, node->data->on_hand, node->data->price.dollars, node->data->price.cents);
        node = node->next;
    }
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
    char string_input[IDLEN+1];
    enum input_result input_success, input_success1;
    int long payment_input=0;
    float amount_owing=0, final_change[COINLEN] = {0};
    int i, j,k=0, n_change=0, temp_change, input_change[MAX_MONEY_INPUTS] = {0};
    Node * node;
    enum denomination_values denom_value[NUM_DENOMS] = { fivecents, tencents, twentycents, fiftycents, onedollar, twodollars, fivedollars, tendollars }; 

    printf("Purchase Item\n----------\n");
    while(TRUE)
    {
        printf("Please enter the ID if the item you wish to purchase: ");
        node = system->item_list->head;
        input_success = get_input(0,string_input,1,IDLEN+1);
        switch (input_success)
        {
            case RTM:
            return FALSE;
            break;

            case SUCCESS:
            printf("String Input: %s\n", string_input);
            while (TRUE)
            {
                if (strcmp(node->data->id,string_input) != 0)
                {
                    node = node->next;
                    if (node == NULL)
                    {
                        printf("The ID '%s' is not a valid selection. Please select an ID of the format I000x.\n", string_input);
                        break; 
                    }
                }
                else if (node->data->on_hand == 0)
                {
                    printf("Unfortunately we are out of stock! Please make another selection.\n");
                    break;
                }

                else
                {
                    printf("You have selected '%s'. This will cost you $%u.%u.\n", node->data->name, node->data->price.dollars, node->data->price.cents);
                    printf("Please hand over the money - type in the value of each note/coin in cents.\nPress enter or ctrl-d on a new line to cancel this purchase:\n");
                    amount_owing = (node->data->price.dollars)*100+(node->data->price.cents);

                    while(amount_owing > 0)
                    {
                    printf("You still need to give us $%.2f: ", amount_owing/100);

                    input_success1 = get_input(&payment_input,0,0,0);

                        switch (input_success1)
                        {
                            case RTM:
                            printf("Aborting Purchase. Processing Refund Now..\n");
                            coin_refund(system, input_change, denom_value);

                            return FALSE;
                            break;

                            case SUCCESS:

                            for (i=NUM_DENOMS-1;i>=0;i--)
                            {
                                if (payment_input == denom_value[i])
                                {
                                    if (i<=MAX_MONEY_INPUTS)
                                    {
                                        amount_owing = amount_owing - payment_input;
                                        system->cash_register[i].count++;
                                        for (j=0;j<MAX_MONEY_INPUTS;j++)
                                        {
                                            if (input_change[j] == 0)
                                            {
                                            input_change[j] = payment_input;
                                            break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        printf("Error: Maximum number of inputs exceeded. Quitting.\n");
                                        coin_refund(system, input_change, denom_value);
                                        return FALSE;
                                    }
                                    break;
                                }
                                else if (i == 0 && payment_input != denom_value[i])
                                {
                                    printf("Error: %ld is not a valid denomination of money.\n", payment_input);
                                    break;
                                }
                            }
                            
                            break;

                            case FAILURE:
                            /*error message in get_input function*/
                            break;
                        }
                    }
                    if (amount_owing < 0)
                    {
                        temp_change = -amount_owing;
                        for (i=TEN_DOLLARS;i>=FIVE_CENTS;i--)
                        {
                            n_change = temp_change/(denom_value[i]); /*check if denomination is a valid refund option*/
                            if (n_change > 0 && system->cash_register[i].count >= n_change) /*ensures denom is valid and enough change of this denomination exists*/
                            {
                                temp_change = temp_change - (denom_value[i] * n_change); /*subtracts change given by this denom from total change outstanding*/
                                system->cash_register[i].count = system->cash_register[i].count-n_change; /*subtracts change from cash register*/

                                for (j=k;j<(k+n_change);j++) /*adds the number of denoms divisible into total to the array. i.e. for change of $9, array will contain $5 $2 $2 */
                                {
                                    final_change[j]=denom_value[i];
                                }
                                k=k+n_change; /*increases array position to next blank spot*/
                            }
                            else if (i==NUM_DENOMS-1 && temp_change > 0 && system->cash_register[i].count < n_change)
                            {
                                    printf("Error: The system does not have enough money to provide you with correct change.");
                                    coin_refund(system, input_change, denom_value);
                                    return FALSE;
                            }
                        }

                        printf("Thank you. Here is your %s, and your change of $%.2f: \n", node->data->name, -amount_owing/100);
                        for(i=0;i<k;i++)
                        {
                            printf("$%0.2f ",final_change[i]/100);
                        }
                        printf("\n");
                        node->data->on_hand--;
                        return TRUE;
                    }
                    else if (amount_owing == 0)
                    {
                        printf("Thank you. Here is your %s.\n",node->data->name);
                        node->data->on_hand--;
                        return TRUE;
                    }

                    break;

                    case FAILURE:
                    /*error message in get_input function*/
                    break;
                }
            }
        }
    }
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    FILE *stock_file, *coins_file;
    int i;
    enum denomination_values denom_value[NUM_DENOMS] = { fivecents, tencents, twentycents, fiftycents, onedollar, twodollars, fivedollars, tendollars }; 
    Node * node = system->item_list->head;
    Stock * data = malloc(sizeof(*data));

    stock_file = fopen(system->stock_file_name, "w");
    coins_file = fopen(system->coin_file_name, "w");

    if (coins_file == NULL || stock_file == NULL)
    {
        printf("Error opening files.\nAborting Operation.\n");
        fclose(stock_file);
        fclose(coins_file);
        return FALSE;
    }
    for (i=TEN_DOLLARS;i>=FIVE_CENTS;i--)
    {
        fprintf(coins_file, "%u,%u\n",denom_value[i],system->cash_register[i].count);
    }

    while (node != NULL)
    {
        fprintf(stock_file, "%s|%s|%s|%u.%02u|%u\n", node->data->id, node->data->name, node->data->desc, node->data->price.dollars, node->data->price.cents, node->data->on_hand);
        node=node->next;
    }

    fclose(stock_file);
    fclose(coins_file);
    free(data);
    abort_program(system);
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
    enum input_result input_success;
    char string_input[DESCLEN], * temp_cent_str=NULL;
    char * bufferEndPtr;
    int highest_id=0/*, i=0*/;
    int long current_id=0, temp_price_dol=0, temp_price_cent=0;
    Node * node = system->item_list->head;
    Stock * data = malloc(sizeof(*data));

    while (node != NULL)
    {
        current_id = strtol(node->data->id+1, &bufferEndPtr, 10);
        if (current_id > highest_id)
        {
            highest_id = current_id;
        }
        if (current_id >= MAX_ID)
        {
            printf("ERROR: The maximum number of items storable by the system has been reached. Unable to add item.\n");
            return FALSE;
        }
        node=node->next;
    }
    sprintf(data->id, "I%04d",highest_id+1);
    printf("This new meal item will have the Item id of %s.\nEnter the item name: ",data->id);

    while(TRUE)
    {
    input_success = get_input(0, string_input,1,NAMELEN+1);
        switch (input_success)
        {
            case RTM:
            return FALSE;
            break;

            case FAILURE:
            /*error message in get_input function*/
            break;

            case SUCCESS:
            strcpy(data->name,string_input);
            break;
        }
        if (input_success == SUCCESS)
        {
            break;
        }
    }

    printf("Enter the item description: ");
    while (TRUE)
    {
        input_success = get_input(0, string_input,1, DESCLEN+1);
        switch (input_success)
        {
            case RTM:
            return FALSE;
            break;

            case FAILURE:
            /*error message in get_input function*/
            break;

            case SUCCESS:
            strcpy(data->desc,string_input);
            break;
        }
        if (input_success == SUCCESS)
        {
            break;
        }
    }

    printf("Enter the price for this item (in the format dollars.cents): ");
    while(TRUE)
    {
        input_success = get_input(0, string_input, 1, COINLEN);
        switch (input_success)
        {
            case RTM:
            return FALSE;
            break;

            case FAILURE:
            /*error message in get_input function*/
            break;

            case SUCCESS:

            if(!add_price_validation(string_input, &temp_price_dol, temp_cent_str, &temp_price_cent))
            {
                break;
            }
            else
            {
                data->price.dollars = temp_price_dol;
                data->price.cents = temp_price_cent;
                data->on_hand = DEFAULT_STOCK_LEVEL;
                AddToLinkedList(system->item_list,data);
                printf("This item '%s - %s' has now been added to the menu\n", data->name, data->desc);
                free(data);
                return TRUE;
                break;
            }
        }
    }
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
    char remove_id[IDLEN+1];
    enum input_result input_success=0;

    printf("Enter the item id of the item to remove from the menu: ");
    while(TRUE)
    {
        input_success = get_input(0,remove_id,1,IDLEN+1);
        switch (input_success)
        {
            case RTM:
            return FALSE;
            break;

            case FAILURE:
            /*error message in get_input function*/
            break;

            case SUCCESS:
            if(RemoveFromLinkedList(system->item_list,remove_id))
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
            break;
        }
    }
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
    Node * node = system->item_list->head;

    while (node != NULL)
    {
        node->data->on_hand = DEFAULT_STOCK_LEVEL;
        node=node->next;
    }
    printf("Stock Levels have been reset.\n");

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    int i;

    for (i=0;i<NUM_DENOMS;i++)
    {
        system->cash_register[i].count = DEFAULT_COIN_COUNT;
    }
    printf("Coin levels have been reset.\n");
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
    int i;
    enum denomination_values denom_value[NUM_DENOMS] = { fivecents, tencents, twentycents, fiftycents, onedollar, twodollars, fivedollars, tendollars }; 

    printf("Coins Summary\n-------------\n");
    printf("%-15s|%6s\n", "Denomination", "Count");
    printf("----------------------\n");
    for (i=FIVE_CENTS;i<=TEN_DOLLARS;i++)
    {
        if (denom_value[i]/100 > 1)
        {
            printf("%-2u %-12s|    %-d\n", denom_value[i]/100, "dollars", system->cash_register[i].count);
        }
        else if (denom_value[i]/100 == 1)
        {
            printf("%-2u %-12s|    %-d\n", denom_value[i]/100, "dollar", system->cash_register[i].count);
        }
        else
        {
            printf("%-2u %-12s|    %-d\n", denom_value[i], "cents", system->cash_register[i].count);
        }
    }
    return TRUE;
}
