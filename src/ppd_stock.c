/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "../includes/ppd_stock.h"
 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/

LinkedList * initialiseLinkedList()
{
    LinkedList * item_list = malloc(sizeof(*item_list));
    if (item_list != NULL)
    {
        item_list->head = NULL;
        item_list->count = 0;
    }
    return item_list;
}

BOOLEAN AddToLinkedList(LinkedList * item_list, Stock * data)
{
    Node * prev_node, * curr_node;

    Node * new_node = malloc(sizeof(*new_node));
    if (new_node==NULL)
    {
        return FALSE;
    }
    new_node->data = malloc(sizeof(*new_node->data));
    if(new_node->data == NULL)
    {
        free(new_node);
        return FALSE;
    }

    memcpy(new_node->data, data, sizeof(*data));
    new_node->next = NULL;
    prev_node = NULL;
    curr_node = item_list->head;

    while (curr_node != NULL)
    {
        if(strcmp(data->name,curr_node->data->name) < 0)
        {
            break;
        }

        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    if (item_list->head == NULL)
    {
        /*Empty List Case*/
        item_list->head = new_node;
    }
    else if(prev_node == NULL)
    {
        new_node->next = item_list->head;
        item_list->head = new_node;
    }
    else
    {
        prev_node->next = new_node;
        new_node->next = curr_node;
    }
    item_list->count++;

    return TRUE;
}

void printLinkedList(LinkedList * item_list)
{
    Node * node = item_list->head;
    printf("Items Menu\n----------");
    printf("%-6s|%-41s|%-10s|%-6s","ID","Name","Available","Price");
    printf("-----------------------------------------------");
    while (node != NULL)
    {
        printf("%-6s|%-41s|%-10u|$%u.%u\n",node->data->id, node->data->name, node->data->on_hand, node->data->price.dollars, node->data->price.cents);
    }
}

void freeLinkedList(LinkedList * item_list)
{
    Node * node = item_list->head;
    while (node != NULL)
    {
        Node * temp = node;
        node = node->next;

        free(temp->data);
        free(temp);
    }
    free(item_list);
}

BOOLEAN RemoveFromLinkedList(LinkedList * item_list, char remove_id[IDLEN+1])
{
    Node * prev_node, * curr_node, *temp_node;
    prev_node = NULL;
    curr_node = item_list->head;

    if (item_list->head == NULL)
    {
        printf("The list is empty! Nothing to remove!\n");
        return FALSE;
    }

    while (curr_node != NULL)
    {
        if (strcmp(curr_node->data->id,remove_id) == 0)
        {
            break;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    if (curr_node == NULL)
    {
        printf("Error: The id '%s' you have entered was not found as a valid product id. Please enter a valid ID: ", remove_id);
        return FALSE;
    }
    else if (curr_node == item_list->head)
    {
        temp_node = item_list->head;
        item_list->head = curr_node->next;
        printf("'%s - %s - %s' has been removed from the system\n", temp_node->data->id, temp_node->data->name, temp_node->data->desc);
        free(temp_node->data);
        free(temp_node);
    }
    else
    {
        prev_node->next = curr_node->next;
        printf("'%s - %s - %s' has been removed from the system\n", curr_node->data->id, curr_node->data->name, curr_node->data->desc);
        free(curr_node->data);
        free(curr_node);
    }
    return TRUE;
}

BOOLEAN load_stock(struct ppd_system * system, FILE * stock_file, Stock * data)
{
    char stockbuffer[DESCLEN], * full_price_temp, * temp_cents, *stock_id, *stock_name, *stock_desc, *stock_on_hand, *temp_dollars;
    char * bufferEndPtr, **validate_ID;
    int current_line=0, i;
    int character, lines=0;
    int soh_len = 0, soh_valid = 0;

    do
    {
        character = fgetc(stock_file);
        if (character == '\n')
        {
            lines++;
        }
    } while (character != EOF);

    rewind(stock_file);

    validate_ID = malloc(lines*sizeof(validate_ID));
    for (i=0;i<lines;i++)
    {
        validate_ID[i]=malloc((IDLEN+1)*sizeof(validate_ID));
    }
    while (fgets(stockbuffer, DESCLEN, stock_file) != NULL)
    {
        stock_id = strtok(stockbuffer, STOCK_DELIM);
        stock_name = strtok(NULL, STOCK_DELIM);
        stock_desc = strtok(NULL, STOCK_DELIM);
        full_price_temp = strtok(NULL, STOCK_DELIM);
        stock_on_hand = strtok(NULL, STOCK_DELIM);
        
        if (stock_id == NULL || stock_name == NULL || stock_desc == NULL || full_price_temp == NULL || stock_on_hand == NULL)
        {
            printf("ERROR: The imported stock file contains an invalid number of fields.\n");
            return FALSE;
        }

        temp_dollars = strtok(full_price_temp,PRICE_DELIM);
        temp_cents = strtok(NULL,PRICE_DELIM);
        if (temp_dollars == NULL || temp_cents == NULL)
        {
            printf("ERROR: The imported stock file contains an invalid number of fields.\n");
            return FALSE;
        }

        soh_valid = strspn(stock_on_hand, COUNT_VALID_CHARS)+NEWLINE_CHARLEN;
        soh_len = strlen(stock_on_hand);
        if (soh_len != soh_valid)
        {
            printf("ERROR: The imported stock file contains an invalid stock on hand value due to invalid characters.\n");
            return FALSE;
        }


        strcpy(data->id,stock_id);
        strcpy(data->name,stock_name);
        strcpy(data->desc,stock_desc);
        data->on_hand = strtol(stock_on_hand,&bufferEndPtr,10);
        data->price.dollars = strtol(temp_dollars,&bufferEndPtr,10);
        data->price.cents = strtol(temp_cents,&bufferEndPtr,10);

        if (!validate_stock(data, validate_ID, lines, current_line, temp_cents))
        {
            free(validate_ID);
            return FALSE;
        }

        AddToLinkedList(system->item_list,data);
        current_line++;
    }
    for (i=0;i<lines;i++)
    {
        free(validate_ID[i]);
    }
    free(validate_ID);
    return TRUE;
}

BOOLEAN validate_stock(Stock * data, char *validate_ID[IDLEN+1], int lines, int current_line, char * temp_cents)
{
    int i, j;

    if (data->id[0] != ID_IDENT)
    {
        printf("ERROR: Stock file contains invalid product ID: '%s'. Product ID string must begin with 'I'. Program Exiting.\n",data->id);
        return FALSE;
    }
    else if (strlen(data->id) > IDLEN+1)
    {
        printf("ERROR: Stock file contains invalid product ID which exceeds allowable length of 5 characters. Please ensure the file contains valid information and try again.\n");
        return FALSE;
    }
    else if (strlen(data->name) > NAMELEN+1)
    {
        printf("ERROR: Name for product with ID: '%s' exceeds allowable length of 40 characters. Please ensure the file contains valid information and try again.\n", data->id);
        return FALSE;
    }
    else if (strlen(data->desc) > DESCLEN+1)
    {
        printf("ERROR: Description for product with ID: '%s' exceeds allowable length of 255 characters. Please ensure the file contains valid information and try again.\n", data->id);
        return FALSE;
    }
    else if (data->on_hand > MAX_ONHAND)
    {
        printf("ERROR: The imported stock file contains an on-hand value for '%s' that exceeds the range of acceptable inputs. Valid range is 0-99\n", data->name);
        return FALSE;
    }
    else if (data->price.dollars > MAX_PRICE_DOL)
    {
        printf("ERROR: The Price for ID: '%s' in the imported stock file exceeds the maximum system price\n", data->id);
        return FALSE;
    }
    if (strcmp(&temp_cents[CENTLEN-1],"\0") == 0) /*case for only 1 cent digit entered*/
    {
        data->price.cents = data->price.cents*10;
    }
    else if (strcmp(&temp_cents[CENTLEN],"\0") != 0) /*ensures only 2 digits for cents*/
    {
        printf("ERROR: The Price for ID: '%s' in the imported stock file does not fit the approriate format\n", data->id);
        return FALSE;
    }
    if (data->price.cents%5 != 0)
    {
        printf("ERROR: The price for ID: '%s' in the imported stock file is not a valid denomination. Denominations must be multiples of 5c\n", data->id);
        return FALSE;
    }
    else if(strtok(NULL,PRICE_DELIM) != NULL)
    {
        printf("ERROR: The price for ID: '%s' in the imported stock file is not a valid format. Format must be x.xx\n", data->id);
        return FALSE;
    }
    strcpy(validate_ID[current_line],data->id);
    for (i=0;i<current_line;i++)
    {
        for (j=i+1;j<=current_line;j++)
        {
            if (strcmp(validate_ID[i],validate_ID[j])==0)
            {
                printf("ERROR: The imported stock file contains duplicate IDs.\n");
                return FALSE;
            }
        }
    }
    return TRUE;
}

