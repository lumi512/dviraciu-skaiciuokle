#pragma once

#include <string>

using namespace std;

#define ARRSIZE(x) sizeof(x) / sizeof(x[0])

extern bool g_quit;

string get_precise(double num);