/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#include "ppd_main.h"

#ifndef PPD_COIN
#define PPD_COIN

#ifndef DENOMS
#define DENOMS
#define NUM_DENOMS 8

#define COIN_DELIM ","
#define COINLEN 10
#define CENTLEN 2

#define MAX_PRICE_DOL 99
#define MAX_COIN_COUNT 99
#define MAX_PRICE 9995
#define LOWEST_DENOM 5
#define COIN_VALID_CHARS "0123456789"
#define PRICE_VALID_CHARS "0123456789."

#define MAX_MONEY_INPUTS 99


struct ppd_system;
/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

enum denomination_values
{
    fivecents = 5, tencents = 10, twentycents = 20, fiftycents = 50,
    onedollar = 100, twodollars = 200, fivedollars = 500, tendollars = 1000
};
/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

void init_cash_register(struct ppd_system * system);

BOOLEAN load_coins(struct ppd_system * system, FILE * coins_file);

void coin_refund(struct ppd_system * system, int input_change[MAX_MONEY_INPUTS],
        enum denomination_values denom_value[NUM_DENOMS]);

BOOLEAN validate_coins(struct ppd_system * system, char * den_str, 
        long den[NUM_DENOMS], int current_pt, long count, char * count_str);

BOOLEAN add_price_validation(char * string_input, int long * temp_price_dol, 
        char * temp_cent_str, int long * temp_price_cent);

#endif
#endif
