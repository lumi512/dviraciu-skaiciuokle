#include <iostream>
#include <vector>

#include "menu_base.h"
#include "printutils.h"

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