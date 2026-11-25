#include <iostream>
#include <sstream>

#include "menu.h"
#include "printutils.h"
#include "order.h"

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
	switch (g_currentAction)
	{
		case MENU_MAIN:
			result = menu_list("Meniu", {
				"Perziureti visas uzsakymo prekes",
				"Perziureti pasirinktos kategorijos prekes",
				"Perziureti viso uzsakymo sumas",
				"Perziureti uzsakymo sumas pagal kategorijas",
				"Parodyti visa ataskaita",
				"Iseiti"
			});

			switch (result)
			{
				case 1: menu_set_action(MENU_VIEWALL); break;
				case 2: menu_set_action(MENU_VIEWCATEGORY); break;
				case 3: menu_set_action(MENU_VIEWSUM); break;
				case 4: menu_set_action(MENU_VIEWCATEGORYSUM); break;
				case 5: menu_set_action(MENU_SAVE); break;
				case 6: menu_set_action(MENU_QUIT); break;
			}
			break;

		case MENU_VIEWALL:
			print_line(order_get_all());
			menu_set_action(MENU_ACTIONEND);
			break;

		case MENU_VIEWCATEGORY:
			if (category != "")
			{
				print_title("Perziureti kategorija: " + category);
				print_line(order_get_category(category));
			}
			menu_set_action(MENU_ACTIONEND);
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

void menu_print_list(vector<string> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		string str = list[i];
		print_line(to_string(i + 1) + ". " + str);
	}
}

int menu_list(string title, vector<string> list)
{
	print_title(title);
	menu_print_list(list);

	do menu_get_input(1, list.size());
	while (g_inputResult == -1);

	return g_inputResult;
}

int g_inputResult = -1;
string g_inputString = "";

void menu_clear_input()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool menu_get_string()
{
	cout << "> ";

	string str;
	cin >> str;

	if (cin.fail())
	{
		print_line("Neteisingas ivedimas.");
		menu_clear_input();

		g_inputString = "";
		return false;
	}

	g_inputString = str;
	return true;
}

bool menu_get_input(int minval, int maxval)
{
	cout << "> ";

	string str;
	cin >> str;

	int val = atoi(str.c_str());
	if (cin.fail() || (minval != -1 && val < minval) || (maxval != -1 && val > maxval))
	{
		print_line("Neteisingas pasirinkimas.");
		menu_clear_input();

		g_inputResult = -1;
		return false;
	}

	g_inputResult = val;
	return true;
}