#include "main.h"
#include "menu.h"
#include "printutils.h"
#include "order.h"

bool g_quit = false;

int main()
{
	if (!order_read_file())
		return 1;

	while (!g_quit)
	{
		menu_perform_action();
	}

	return 0;
}