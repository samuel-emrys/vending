/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "../includes/ppd_utility.h"
#include "../includes/ppd_coin.h"

void init_cash_register(struct ppd_system * system)
{
    int i;
    for (i=FIVE_CENTS;i<=TEN_DOLLARS;i++)
    {
        /*initialises respective denom cells to enum value*/
        system->cash_register[i].denom=i;
        /*sets the count of each denom to 0*/
        system->cash_register[i].count=0;
    }
}

BOOLEAN load_coins(struct ppd_system * system, FILE * coins_file)
{
	char coinbuffer[COINLEN], * den_str, *count_str;
    char * bufferEndPtr;
    int current_pt=TEN_DOLLARS;
    int long den[NUM_DENOMS] = {0},count=0;

	while(fgets(coinbuffer, COINLEN, coins_file) != NULL)
	{
	    if (current_pt >= 0)
	    {
            den_str = strtok(coinbuffer, COIN_DELIM);
    	    den[current_pt] = strtol(den_str,&bufferEndPtr, 10);
            count_str = strtok(NULL,COIN_DELIM);
            /*check to see if count is actually a field*/
            if (count_str == NULL)
            {
                printf("ERROR: The imported coin file contains an "
                    "invalid number of fields.\n");
                return FALSE;
            }
            count = strtol(count_str, &bufferEndPtr, 10);
            if (!validate_coins(system, den_str, den, current_pt, 
                count, count_str))
            {
                return FALSE;
            }
        }
	    current_pt--;
	}
    return TRUE;
}

void coin_refund(struct ppd_system * system,int input_change[MAX_MONEY_INPUTS],
             enum denomination_values denom_value[NUM_DENOMS])
{
	int i,j;
    float coin_display=0, temp_holder=0;
	
    printf("Refunding your coins: ");
	for (i=0;i<MAX_MONEY_INPUTS;i++)
    {
        if (input_change[0] == 0)
        {
            printf("No coins to refund.\n");
            break;
        }
        if (input_change[i] == 0)
        {
            break;
        }
        else
        {
            for (j=TEN_DOLLARS;j>=FIVE_CENTS;j--)
            {
                /*determines which denomination to deduct*/
                if (input_change[i]==denom_value[j])
                {
                    system->cash_register[j].count--;
                    temp_holder = input_change[i];
                    coin_display = temp_holder/100;
                    printf("$%.2f ", coin_display);
                }
            }
        }
    }
    printf("\n");
}

BOOLEAN validate_coins(struct ppd_system * system, char * den_str,
     long den[NUM_DENOMS], int current_pt, long count, char * count_str)
{
    float denomination=den[current_pt];
    int i=0,j=0;

    /*allocates denomination to correct index in denom array*/
    switch (den[current_pt])
    {
        case fivecents:
        system->cash_register[FIVE_CENTS].count=count;
        break;

        case tencents:
        system->cash_register[TEN_CENTS].count=count;
        break;  
        
        case twentycents:
        system->cash_register[TWENTY_CENTS].count=count;
        break;

        case fiftycents:
        system->cash_register[FIFTY_CENTS].count=count;
        break;  

        case onedollar:
        system->cash_register[ONE_DOLLAR].count=count;
        break;

        case twodollars:
        system->cash_register[TWO_DOLLARS].count=count;
        break;  

        case fivedollars:
        system->cash_register[FIVE_DOLLARS].count=count;
        break;

        case tendollars:
        system->cash_register[TEN_DOLLARS].count=count;
        break;

        default:
        printf("ERROR: Coin file contains invalid price denomination "
                "'%ld'\n", den[current_pt]);
        return FALSE;
    }
    /*ensures denomination count is within limits*/
    if (count > MAX_COIN_COUNT)
    {
        printf("ERROR: Imported coin file contains invalid number of "
                "the following denomination: $%.2f. Valid range is between "
                "0 and 99.\n", denomination/100);
        return FALSE;
    }
    /*checks that the length of valid chars = length of the string.
    otherwise indicates an invalid character in string*/
    if (strspn(den_str, COIN_VALID_CHARS) != strlen(den_str) || 
            strspn(count_str, COIN_VALID_CHARS)+1 != strlen(count_str))
    {
        printf("ERROR: Imported coin file contains invalid characters. "
            "Denominations and their respective count must be "
                "numeric characters only.\n");
        return FALSE;
    }
    for (i=TEN_DOLLARS;i>0;i--)
    {
        for (j=i-1;j>=0;j--)
        {
            /*test to see if two elements are the same except if they're
            zero*/
            if (den[i]==den[j] && den[i] != 0)
            {
                printf("ERROR: Imported coins file contains duplicate "
                        "denominations.\n");
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOLEAN add_price_validation(char * string_input, int long * temp_price_dol,
         char * temp_cent_str, int long * temp_price_cent)
{
    char * bufferEndPtr;

    if (strspn(string_input, PRICE_VALID_CHARS) != strlen(string_input))
    {
        printf("ERROR: Invalid price format. Please re-enter a valid price "
                    "in the format of x.xx: ");
        return FALSE;
    }
    else
    {
        *temp_price_dol = strtol(strtok(string_input,PRICE_DELIM),
                &bufferEndPtr,10);
        temp_cent_str = strtok(NULL,PRICE_DELIM);

        if (temp_cent_str == NULL)
        {
            printf("ERROR: Invalid price format. Please re-enter a valid "
                    "price in the format of x.xx: ");
            return FALSE;
        }
        *temp_price_cent = strtol(temp_cent_str,&bufferEndPtr,10);

        /*case for only 1 cent digit entered*/
        if (strcmp(&temp_cent_str[CENTLEN-1],"\0") == 0) 
        {
            *temp_price_cent = *temp_price_cent*10;
        }
        /*ensures only 2 digits for cents*/
        else if (strcmp(&temp_cent_str[CENTLEN],"\0") != 0) 
        {
            printf("ERROR: Price input too long. Please re-enter a valid "
                    "price in the format of x.xx: ");
            return FALSE;
        }
        /*ensures price divisible by lowest denomination*/
        if (*temp_price_cent%LOWEST_DENOM != 0)
        {
            printf("ERROR: Incompatible price selected. Price must be a "
                    "multiple of 5 cents. Please Enter a New Price: ");
            return FALSE;
        }
        else if(*temp_price_dol > MAX_PRICE_DOL)
        {
            printf("ERROR: Price entered is too high. The maximum price is "
                "$99.95. Please enter a price equal to or lower than this: ");
            return FALSE;              
        }
        /*ensures theres no extra fields*/
        else if(strtok(NULL,PRICE_DELIM) != NULL)
        {
            printf("ERROR: Invalid price format. Please re-enter a valid "
                    "price in the format of x.xx: ");
            return FALSE;
        }
    }
    return TRUE;
}
 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/

