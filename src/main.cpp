#include <iomanip>
#include <sstream>

#include "main.h"
#include "menu.h"
#include "printutils.h"
#include "order.h"

bool g_quit = false;

string get_precise(double num)
{
	stringstream str;
	str << fixed << setprecision(2) << num;

	return str.str();
}

int main()
{
	if (!order_read_file())
		return 1;

	while (!g_quit)
	{
		string category;
		if (g_currentAction == MENU_VIEWCATEGORY)
		{
			int result = menu_list("Iveskite kategorija pavadinima", {
				ORDER_CATEGORY_1,
				ORDER_CATEGORY_2,
				ORDER_CATEGORY_3
			});

			switch (result)
			{
				case 1: category = ORDER_CATEGORY_1; break;
				case 2: category = ORDER_CATEGORY_2; break;
				case 3: category = ORDER_CATEGORY_3; break;
			}
		}

		menu_perform_action(category);
	}

	return 0;
}