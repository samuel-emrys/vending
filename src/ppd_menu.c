/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Author        : Samuel Dowling
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "../includes/ppd_menu.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
	struct menu_item menu_list[MENU_SIZE] =
	{

		{ "Display Items", display_items },
		{ "Purchase Items", purchase_item },
		{ "Save and Exit", save_system },
		{ "Add Item", add_item },
		{ "Remove Item", remove_item },
		{ "Display Coins", display_coins },
		{ "Reset Stock", reset_stock },
		{ "Reset Coins", reset_coins },
		{ "Abort Program", abort_program },

	};
	/*copy list into menu struct*/
	memcpy(menu, menu_list, sizeof(menu_list));
}

int main_menu(struct menu_item * menu, struct ppd_system * system)
{
	int i;
	int long selection=0;
	enum input_result input_success;


	printf("Main Menu:\n");
	for (i=0;i<MENU_SIZE;i++)
	{
		if (i==ADMIN_MENU)
		{
			printf("Administrator-Only Menu:\n");
		}
		printf(" %d. %s\n",i+1,menu[i].name);
	}
	while(TRUE)
	{
		printf("Select your option(1-9): \n");

		input_success = get_input(&selection,0,0,0);

		switch (input_success)
		{
			case RTM:
			break;

			case SUCCESS:
			if (selection > 0 && selection <= MENU_SIZE)
			{
				return selection;
			}
			else
			{
				printf("Error: Invalid selection. Please enter a "
					"value between integers 1-9\n");
			}
			break;

			case FAILURE:
			printf("You shouldn't be here\n");
			break;
		}
	}
}

