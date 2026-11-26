#include <iostream>
#include <sstream>

#include "menu.h"
#include "menu_base.h"
#include "printutils.h"
#include "order.h"
#include "order_stringify.h"

MenuAction g_currentAction = MENU_MAIN;
MenuAction g_lastAction = MENU_INVALID;

void menu_set_action(MenuAction act)
{
	g_lastAction = g_currentAction;
	g_currentAction = act;
}

bool menu_perform_action(string category)
{
	int result;
	string order;
	switch (g_currentAction)
	{
		case MENU_MAIN:
			result = menu_list("Meniu", {
				"Perziureti visas uzsakymo prekes",
				"Perziureti pasirinktos kategorijos prekes",
				"Parodyti visa ataskaita",
				"Iseiti"
			});

			switch (result)
			{
				case 1: menu_set_action(MENU_VIEWALL); break;
				case 2: menu_set_action(MENU_VIEWCATEGORY); break;
				case 3: menu_set_action(MENU_SAVE); break;
				case 4: menu_set_action(MENU_QUIT); break;
			}
			break;

		case MENU_VIEWALL:
			print_line(order_to_string());
			menu_set_action(MENU_ACTIONEND);
			break;

		case MENU_VIEWCATEGORY:
			if (category != "")
			{
				print_title("Perziureti kategorija: " + category);
				print_line(order_category_to_string(category));
			}
			menu_set_action(MENU_ACTIONEND);
			break;

		case MENU_SAVE:
			order = order_to_string();
			print_line(order);

			result = menu_list("Issaugoti ataskaita i faila?", {
				"Taip",
				"Ne"
			});

			if (result == 1 && order_write_file(order))
				print_line("Ataskaita issaugota.");

			menu_set_action(MENU_MAIN);
			break;

		case MENU_QUIT:
			g_quit = true;
			break;

		case MENU_ACTIONEND:
			result = menu_list("Ka dabar?", {
				"Is naujo vykdyti veiksma",
				"Grizti i meniu",
				"Iseiti"
			});

			switch (result)
			{
				case 1:
					menu_set_action(g_lastAction);
					break;
				case 2:
					menu_set_action(MENU_MAIN);
					break;
				case 3:
					menu_set_action(MENU_QUIT);
					break;
			}
			break;

		default:
			fatal_error("Nezinomas veiksmas - " + to_string(g_currentAction));
			return false;
	}

	return true;
}