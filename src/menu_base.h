#pragma once

#include <string>
#include <vector>

#include "main.h"

extern int g_inputResult;
extern string g_inputString;

void menu_clear_input();
bool menu_get_string();
bool menu_get_input(int minval = -1, int maxval = -1);

void menu_print_list(vector<string> list);
int menu_list(string title, vector<string> list);