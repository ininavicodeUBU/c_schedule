#ifndef GUI_FUNC_H
#define GUI_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "./constants.h"
#include "./func.h"
#include "./GUI_constants.h"

// data types ++++++++++++++++++++++++++++++++++++++++
typedef struct GUI_t
{
    short unsigned main_menu_index;
    short unsigned option [MAX_SUBMENU_OPTIONS];
    date_t showing_date;

} GUI_t;
// --------------------------------------------------

// menu objects control ++++++++++++++++++++++++++++++
void SetButtonBackgroundColor(HWND hwndButton, COLORREF color);

void DrawCustomButton(LPDRAWITEMSTRUCT lpdis, COLORREF bgColor, char text[]);

// ---------------------------------------------------


// initialization +++++++++++++++++++++++++++++++++++++++
void create_buttons_days (HWND hwnd, HWND buttons_of_the_days []);

void create_buttons_schedules (HWND hwnd, HWND buttons_of_schedules []);

void create_combo_boxes (HWND hwnd, HWND months_combo_box, HWND year_combo_box, GUI_t *GUI);

void create_combo_box (HWND hwnd, HWND combo_box, char options[][256], unsigned left_margin,
unsigned top_margin, unsigned width, unsigned height, int combo_box_ID, int default_option);
// ---------------------------------------------------

// button_colors management ++++++++++++++++++++++++++++
void paint_day_buttons (HWND hwnd, HWND list_of_buttons [], date_event_t events_of_month[], COLORREF list_of_the_color_button []);

void make_visible_buttons_days (HWND hwnd, HWND buttons_of_the_days []);

// -----------------------------------------------------

#endif