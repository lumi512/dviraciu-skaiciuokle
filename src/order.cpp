#include <fstream>

#include "main.h"
#include "printutils.h"
#include "order.h"

Order g_orders[MAX_ORDERS];
int g_orderCount = 0;

bool set_index(Order &ord, int &ind, string &str)
{
	switch (ind++)
	{
		case 0:
			if (str == ORDER_CATEGORY_1 ||
				str == ORDER_CATEGORY_2 ||
				str == ORDER_CATEGORY_3)
				ord.category = str;
			else
			{
				fatal_error("Uzsakymo kategorija yra nezinoma: " + str);
				return false;
			}
			break;

		case 1:
			ord.product = str;
			break;

		case 2:
			ord.basePrice = atof(str.c_str());
			if (ord.basePrice < 0)
			{
				fatal_error("Netinkama uzsakymo kaina: " + str);
				return false;
			}
			break;

		case 3:
			ord.orderedAmount = atoi(str.c_str());
			if (ord.orderedAmount < 0)
			{
				fatal_error("Netinkama uzsakymo kiekis: " + str);
				return false;
			}
			break;

		default:
			fatal_error("Nezinomas uzsakymo indeksas: " + to_string(ind));
			return false;
	}

	return true;
}

bool parse_line(string &str)
{
	if (g_orderCount >= MAX_ORDERS)
	{
		fatal_error("Per daug prekiu irasu!");
		return false;
	}

	Order order;
	int ordIndex = 0;

	size_t off = string::npos;
	int pos = 0;
	while ((off = str.find(';', off + 1)) && off != string::npos)
	{
		string sub = str.substr(pos, off - pos);
		if (!set_index(order, ordIndex, sub))
			return false;

		pos = off + 1;
	}

	string endsub = str.substr(pos, str.length() - pos);
	if (!set_index(order, ordIndex, endsub))
		return false;

	int ind = g_orderCount++;
	g_orders[ind] = order;

	return true;
}

bool order_read_file()
{
	ifstream file(ORDER_FILENAME);
	if (!file.is_open())
	{
		fatal_error("Nepavyko atidaryti faila '" ORDER_FILENAME "'.");
		return false;
	}

	string str;
	getline(file, str);

	int count = atoi(str.c_str());
	if (count <= 0)
	{
		fatal_error("Nesuprantamas irasu skaicius: '" + str + "'.");
		file.close();
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		getline(file, str);
		if (str == "")
		{
			fatal_error("Yra nustatyta " + to_string(count) + " irasu bet yra tik " + to_string(i));
			file.close();
			return false;
		}

		if (!parse_line(str))
		{
			file.close();
			return false;
		}
	}
	
	file.close();
	return true;
}

bool order_write_file(string &str)
{
	ofstream file(ORDER_SAVEFILENAME);
	if (!file.is_open())
	{
		print_line("Nepavyko sukurti faila '" ORDER_SAVEFILENAME "'.");
		return false;
	}

	file << str;

	file.close();
	return true;
}
