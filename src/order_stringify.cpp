#include "order.h"
#include "order_stringify.h"
#include "order_price.h"

#define DIVIDER			"------------------------------------------"
#define TITLE_DIVIDER	"=========================================="

#define CURRENCY_STR " " + string(ORDER_CURRENCY)
#define ENDL string("\n")

void stringify_product(string &str, Order &ord, bool singleCategory = false)
{
	str += "Preke: " + ord.product + ENDL;
	if (!singleCategory)
		str += "Kategorija: " + ord.category + ENDL;

	str += "Vieneto kaina: " + get_precise(ord.basePrice) + CURRENCY_STR + ENDL;
	str += "Uzsakytas kiekis: " + to_string(ord.orderedAmount) + ENDL;

	string baseSum = get_precise(order_product_sum(ord)) + CURRENCY_STR;
	if (ord.orderedAmount >= 10)
	{
		str += "Suma (be nuolaidos): " + baseSum + ENDL;

		string discountedPrice = get_precise(order_product_discount_price(ord)) + CURRENCY_STR;
		str += "Vieneto kaina (su nuolaida): " + discountedPrice + ENDL;

		string discountedSum = get_precise(order_product_discount_sum(ord)) + CURRENCY_STR;
		str += "Suma (su nuolaida): " + discountedSum;
	}
	else
		str += "Suma: " + baseSum;
}

void stringify_sum(string &str, string category = "")
{
	double orderSum = order_sum(category);
	str += "Uzsakymo suma (be nuolaidu): " + get_precise(orderSum) + CURRENCY_STR + ENDL;

	double discountTotal = order_discount_total(category);
	str += "Nuolaidu suma: " + get_precise(discountTotal) + CURRENCY_STR + ENDL;

	if (discountTotal > 0.0f)
	{
		double discountSum = order_discount_sum(category);
		str += "Moketina suma (po nuolaidu): " + get_precise(discountSum) + CURRENCY_STR + ENDL;
	}
}

string order_to_string()
{
	string str = "";

	for (int i = 0; i < g_orderCount; i++)
	{
		Order ord = g_orders[i];
		if (ord.product == "")
			continue;

		str += DIVIDER + ENDL;
		stringify_product(str, ord);
		str += ENDL + DIVIDER + ENDL;
	}

	str += ENDL + TITLE_DIVIDER + ENDL;
	stringify_sum(str);
	str += TITLE_DIVIDER + ENDL;

	return str;
}

string order_category_to_string(string category)
{
	string str = "";

	bool containsOrders = false;
	for (int i = 0; i < g_orderCount; i++)
	{
		Order ord = g_orders[i];
		if (ord.category == category)
		{
			str += DIVIDER + ENDL;
			stringify_product(str, ord, true);
			str += ENDL + DIVIDER + ENDL;

			containsOrders = true;
		}
	}

	if (containsOrders)
	{
		str += ENDL + TITLE_DIVIDER + ENDL;
		stringify_sum(str, category);
		str += TITLE_DIVIDER + ENDL;
	}
	else
		str = "Kategorijoje nera prekiu.";

	return str;
}