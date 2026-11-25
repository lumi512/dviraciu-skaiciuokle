#pragma once

#include <string>
using namespace std;

struct Order
{
	string category;
	string product;
	double basePrice;
	int orderedAmount;
};

#define MAX_ORDERS 100
#define ORDER_FILENAME "uzsakymas.txt"

#define ORDER_CATEGORY_1 "Dviraciai"
#define ORDER_CATEGORY_2 "Dalys"
#define ORDER_CATEGORY_3 "Aksesuarai"

#define ORDER_CURRENCY "EUR"
#define ORDER_DISCOUNT_PERCENT 10

extern Order g_orders[MAX_ORDERS];
extern int g_orderCount;

bool order_read_file();

void order_stringify_order_data(string &str, Order &ord, bool singleCategory = false);
void order_stringify_full_sum(string &str, string category = "");

string order_get_all();
string order_get_category(string category);

double order_get_sum_price(Order &ord);
double order_get_discounted_price(Order &ord);
double order_get_discounted_sum_price(Order &ord);

double order_get_full_price(string category = "");
double order_get_discount_full_price(string category = "");
double order_get_discounted_full_price(string category = "");