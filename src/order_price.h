#pragma once

#include "order.h"

#define ORDER_CURRENCY "EUR"
#define ORDER_DISCOUNT_PERCENT 10

double order_product_sum(Order &ord);
double order_product_discount_price(Order &ord);
double order_product_discount_sum(Order &ord);

/////////////////////////////////////////////////////////////

double order_sum(string category = "");
double order_discount_total(string category = "");
double order_discount_sum(string category = "");