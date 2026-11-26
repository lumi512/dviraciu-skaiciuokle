#include "order_price.h"

double order_product_sum(Order &ord)
{
	return ord.basePrice * ord.orderedAmount;
}

double order_product_discount_price(Order &ord)
{
	double perc = (100.0f - ORDER_DISCOUNT_PERCENT) / 100.0f;
	if (ord.orderedAmount < 10)
		perc = 1.0f;

	return ord.basePrice * perc;
}

double order_product_discount_sum(Order &ord)
{
	double price = order_product_discount_price(ord);
	return price * ord.orderedAmount;
}

/////////////////////////////////////////////////////////////////////////////////

double order_sum(string category)
{
	double price = 0.0;
	for (Order ord : g_orders)
	{
		if ((category != "" && ord.category == category) || category == "")
			price += order_product_sum(ord);
	}

	return price;
}

double order_discount_total(string category)
{
	double price = 0.0;
	for (Order ord : g_orders)
	{
		if (ord.orderedAmount < 10)
			continue;

		if ((category != "" && ord.category == category) || category == "")
			price += order_product_discount_price(ord);
	}

	return price;
}

double order_discount_sum(string category)
{
	double price = 0.0;
	for (Order ord : g_orders)
	{
		if ((category != "" && ord.category == category) || category == "")
			price += order_product_discount_sum(ord);
	}

	return price;
}