#include <fstream>
#include <cstdio>
#include "main.h"
#include "printutils.h"
#include "order.h"

Order g_orders[MAX_ORDERS];
int g_orderCount = 0;

bool order_set_index(Order &ord, int &ind, string &str)
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

bool order_parse_line(string &str)
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
		if (!order_set_index(order, ordIndex, sub))
			return false;

		pos = off + 1;
	}

	string endsub = str.substr(pos, str.length() - pos);
	if (!order_set_index(order, ordIndex, endsub))
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

		if (!order_parse_line(str))
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
		fatal_error("Nepavyko sukurti faila.");
		return false;
	}

	file << str;

	file.close();
	return true;
}

void order_stringify_order_data(string &str, Order &ord, bool singleCategory)
{
	string currencyStr = " " + string(ORDER_CURRENCY);

	str += "Preke: " + ord.product + "\n";
	if (!singleCategory)
		str += "Kategorija: " + ord.category + "\n";

	str += "Vieneto kaina: " + get_precise(ord.basePrice) + currencyStr + "\n";
	str += "Uzsakytas kiekis: " + to_string(ord.orderedAmount) + "\n";

	string baseSum = get_precise(order_get_sum_price(ord)) + currencyStr;
	if (ord.orderedAmount >= 10)
	{
		str += "Suma (be nuolaidos): " + baseSum + "\n";
		
		string discountedPrice = get_precise(order_get_discounted_price(ord)) + currencyStr;
		str += "Vieneto kaina (su nuolaida): " + discountedPrice + "\n";

		string discountedSum = get_precise(order_get_discounted_sum_price(ord)) + currencyStr;
		str += "Suma (su nuolaida): " + discountedSum;
	}
	else
		str += "Suma: " + baseSum;
}

void order_stringify_full_sum(string &str, string category)
{
	string currencyStr = " " + string(ORDER_CURRENCY);

	str += "Uzsakymo suma (be nuolaidu): " + get_precise(order_get_full_price(category)) + currencyStr + "\n";
	str += "Nuolaidu suma: " + get_precise(order_get_discount_full_price(category)) + currencyStr + "\n";
	if (order_get_discount_full_price(category) > 0.0f)
		str += "Moketina suma (po nuolaidu): " + get_precise(order_get_discounted_full_price(category)) + currencyStr + "\n";
}

#define ORDER_DIVIDER		"------------------------------------------"
#define ORDER_TITLE_DIVIDER "=========================================="

string order_get_all()
{
	string str = "";

	string prevCategory = "";
	for (int i = 0; i < g_orderCount; i++)
	{
		Order ord = g_orders[i];
		if (ord.product == "")
			continue;

		str += ORDER_DIVIDER + string("\n");
		order_stringify_order_data(str, ord);
		str += string("\n") + ORDER_DIVIDER + string("\n");
	}

	str += string("\n") + ORDER_TITLE_DIVIDER + string("\n");
	order_stringify_full_sum(str);

	return str;
}

string order_get_category(string category)
{
	string str = "";

	bool containsOrders = false;
	for (Order ord : g_orders)
	{
		if (ord.category == category)
		{
			str += ORDER_DIVIDER + string("\n");
			order_stringify_order_data(str, ord, true);
			str += string("\n") + ORDER_DIVIDER + string("\n");

			containsOrders = true;
		}
	}

	if (containsOrders)
	{
		str += string("\n") + ORDER_TITLE_DIVIDER + string("\n");
		order_stringify_full_sum(str, category);
	}
	else
		str = "Kategorijoje nera prekiu.";

	return str;
}

double order_get_sum_price(Order &ord)
{
	return ord.basePrice * ord.orderedAmount;
}

double order_get_discounted_price(Order &ord)
{
	double perc = (100.0f - ORDER_DISCOUNT_PERCENT) / 100.0f;
	if (ord.orderedAmount < 10)
		perc = 1.0f;
	return ord.basePrice * perc;
}

double order_get_discounted_sum_price(Order &ord)
{
	double price = order_get_discounted_price(ord);
	return price * ord.orderedAmount;
}

double order_get_full_price(string category)
{
	double price = 0.0;
	for (Order ord : g_orders)
	{
		if ((category != "" && ord.category == category) || category == "")
			price += order_get_sum_price(ord);
	}

	return price;
}

double order_get_discount_full_price(string category)
{
	double price = 0.0;
	for (Order ord : g_orders)
	{
		if (ord.orderedAmount < 10)
			continue;

		if ((category != "" && ord.category == category) || category == "")
			price += order_get_discounted_price(ord);
	}

	return price;
}

double order_get_discounted_full_price(string category)
{
	double price = 0.0;
	for (Order ord : g_orders)
	{
		if ((category != "" && ord.category == category) || category == "")
			price += order_get_discounted_sum_price(ord);
	}

	return price;
}