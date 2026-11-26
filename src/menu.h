#pragma once

#include <string>
#include <vector>

#include "main.h"

enum MenuAction
{
	MENU_INVALID = -1,

	MENU_MAIN,

	MENU_VIEWALL,
	MENU_VIEWCATEGORY,
	MENU_SAVE,

	MENU_QUIT,

	MENU_ACTIONEND,

	MENU_MAX,
};

extern MenuAction g_currentAction;
extern MenuAction g_lastAction;

void menu_set_action(MenuAction act);
bool menu_perform_action(string category = "");