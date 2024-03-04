#ifndef GUI_FUNC_H
#define GUI_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>

#include "./constants.h"
#include "./func.h"
#include "./GUI_constants.h"

// data types ++++++++++++++++++++++++++++++++++++++++
typedef char path_t [MAX_PATH_LEN];
typedef char names_of_available_schedules_t [MAX_SCHEDULES][MAX_FILENAME_LEN];

typedef struct
{
	date_event_t date_event;
	HWND GUI_elements [6];
	unsigned block_id;
	unsigned n_page;

} GUI_event_t;

typedef struct
{
    // GUI elements +++++++++++++++++++++
	HWND GUI_main_screen;
	HWND buttons[N_BUTTONS];
	HWND combo_boxes[N_COMBO_BOXES];
	HWND text_input_boxes[N_INPUT_BOXES];
	HWND text[N_TEXTS];
	GUI_event_t GUI_events_list [4];
	// -----------------------------------
	// STYLING variables +++++++++++++++++
	// for each color change we have to save that change here
	unsigned colors_of_buttons[N_BUTTONS];
	// -----------------------------------
	// menu management variables +++++++++
	uint8_t menu_state[MENU_STATE_LEN];
	// -----------------------------------

	// dates ++++++++++++++++++++++
	date_t local_time_date;
	date_t showing_date;
	// -----------------------------------

    // PATHS +++++++++++++++++++++++++++++
	// path to the .exe file
	path_t exe_file_path;

	// path for the folder where the schedules are stored
	path_t schedules_folder_path;
	
	// selected schedule path
	path_t selected_schedule_path;
	// -----------------------------------

    names_of_available_schedules_t names_of_available_schedules;

	// list of events of the selected schedule ++++
	date_event_t last_downloaded_events[MAX_EVENTS];
    date_event_t events_of_showing_date[MAX_EVENTS];
	date_event_t events_of_selected_day[MAX_EVENTS];
	// --------------------------------------------

} GUI_data_t;

// --------------------------------------------------

// GUI_event_t functions +++++++++++++++++++++++++++++++++++++++++++++++++++
void GUI_event_constructor (GUI_data_t* GUI_data, GUI_event_t* GUI_event, unsigned block_id);

void hide_GUI_event_elements (GUI_event_t* GUI_event);

void show_GUI_event_elements (GUI_event_t* GUI_event);

void GUI_event_refresh_values (GUI_data_t* GUI_data, GUI_event_t* GUI_event);

// -------------------------------------------------------------------------


// menu objects control ++++++++++++++++++++++++++++++
void SetButtonBackgroundColor(HWND hwndButton, COLORREF color);

void DrawCustomButton(LPDRAWITEMSTRUCT lpdis, COLORREF bgColor, char text[]);

// ---------------------------------------------------

// elements creation ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
HWND create_in_box(HWND hwnd, char* default_text, unsigned X, unsigned Y, unsigned width, unsigned heigth);

// ------------------------------------------------------------------------------------------

// initialization +++++++++++++++++++++++++++++++++++++++
void GUI_init (GUI_data_t*);

// ---------------------------------------------------

// modifying elements ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void set_in_box_text(HWND in_box, const char* newText);
//  ----------------------------------------------------------------------------------------------------------------------------------------------------

// hiding elements +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void hide_no_schedule_selected_menu (GUI_data_t* GUI_data);

void hide_selected_schedule_menu (GUI_data_t* GUI_data);

// -------------------------------------------------------------------------------------------

// showing elements +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void show_date_showing_combo_boxes (GUI_data_t* GUI_data);

void show_days_of_selected_showing_date (GUI_data_t* GUI_data);

void show_save_button (GUI_data_t* GUI_data);

void show_no_schedule_selected_menu (GUI_data_t* GUI_data);

// -------------------------------------------------------------------------------------------

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * @brief This functions just refreshes the options on the combo boxes of 
 * <Select schedule> and <Delete schedule>, is getting the values from the list
 * of names of the available schedules so its necessary to refresh h that list previously
*/
void refresh_available_schedules_combo_boxes (GUI_data_t* GUI_data);

// -------------------------------------------------------------------------------------------

// painting ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void paint_days_to_default_color (GUI_data_t* GUI_data);

void paint_days_with_events (GUI_data_t* GUI_data);

void paint_selected_day (GUI_data_t* GUI_data);
// ---------------------------------------------------------------------------------------------

// data getting +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void get_in_box_text(HWND in_box, char* buffer, int bufferSize);
// --------------------------------------------------------------------------------------------

// data saving ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void save_elements_data_to_events (GUI_data_t* GUI_data);
// ----------------------------------------------------------------------------------------------------------------------------

#endif