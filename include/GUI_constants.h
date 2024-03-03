#define MAX_DISPLAYING_EVENTS 10

// LENGTH OF THE ARRAYS THAT CONTAIN THE OBJECTS OF THE GUI ++++++++++++++++++

// 31 days of the month + 4 other buttons
#define N_BUTTONS 31 + 4  

// 4 others + MAX_DISPLAYING_EVENTS * 5 (each events has 5 combo boxes)
#define N_COMBO_BOXES 4 + MAX_DISPLAYING_EVENTS * 5


// name input for the new scheudle creation + MAX_DISPLAYING_EVENTS
#define N_INPUT_BOXES 1 + MAX_DISPLAYING_EVENTS

#define N_TEXTS 4
// ---------------------------------------------------------------------------
// ID for menus ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// [0]
#define NO_SCHEDULE_SELECTED 0
#define SCHEDULE_SELECTED 1

// [1]
#define UNDEFINED_MENU_STATE 0
#define DAY_SELECTED 1

// ---------------------------------------------------------------------------

/*
RANGES FOR THE ID'S
(0 - 99) -> Buttons
(100 - 199) -> Combo boxes
(200 - 299) -> Input text
*/

// ID'S +++++++++++++++++++++++++++
// buttons
#define ID_NEW_SCHEDULE_BUTTON 0
#define ID_SAVE_NEW_SCHEDULE_NAME 1
#define ID_CANCEL_NEW_SCHEDULE_NAME 2
#define ID_SAVE_SELECTED_SCHEDULE 3
#define ID_FIRST_DAY_OF_MONTH 4
#define ID_LAST_DAY_OF_MONTH ID_FIRST_DAY_OF_MONTH + 30
#define ID_FIRST_EVENT_BLOCK 300

// combo boxes
#define ID_COMBO_BOX_OFFSET -100

#define ID_DELETE_SCHEDULE_CBX 100
#define ID_SELECT_SCHEDULE_CBX 101
#define ID_MONTH_SHOWING_DATE_CBX 102
#define ID_YEAR_SHOWING_DATE_CBX 103

// --------------------------------

// STYLES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SCREEN +++++++++++++++++++
#define WIDTH_SCREEN 900
#define HEIGTH_SCREEN 500


// buttons ++++++++++++++++++
// RGB
#define RGB_NEW_SCHEDULE_BUTTON RGB(87, 155, 18)
#define RGB_SAVE_NEW_SCHEDULE_NAME RGB(87, 155, 18)
#define RGB_CANCEL_NEW_SCHEDULE_NAME RGB(235, 48, 48)
#define RGB_SAVE_SELECTED_SCHEDULE RGB(87, 155, 18)
#define RGB_DEFAULT_DAY RGB(179, 183, 183)
#define RGB_DAY_WITH_EVENT RGB(140, 122, 255)
#define RGB_DAY_SELECTED RGB(131, 255, 122)

// POSITION AND SIZE
#define X_NEW_SCHEDULE_BUTTON 10
#define Y_NEW_SCHEDULE_BUTTON 10

#define WIDTH_SAVE_SELECTED_SCHEDULE 20
#define HEIGTH_SAVE_SELECTED_SCHEDULE 20
#define X_SAVE_SELECTED_SCHEDULE X_SELECT_SCHEDULE_CBX + WIDTH_SELECT_SCHEDULE_CBX
#define Y_SAVE_SELECTED_SCHEDULE Y_SELECT_SCHEDULE_CBX

#define WIDTH_MONTH_SHOWING_DATE_CBX 120
#define HEIGTH_MONTH_SHOWING_DATE_CBX 100
#define X_MONTH_SHOWING_DATE_CBX X_SELECT_SCHEDULE_CBX
#define Y_MONTH_SHOWING_DATE_CBX Y_SELECT_SCHEDULE_CBX + HEIGTH_SELECT_SCHEDULE_CBX

#define WIDTH_YEAR_SHOWING_DATE_CBX 100
#define HEIGTH_YEAR_SHOWING_DATE_CBX 100
#define X_YEAR_SHOWING_DATE_CBX X_MONTH_SHOWING_DATE_CBX + WIDTH_MONTH_SHOWING_DATE_CBX
#define Y_YEAR_SHOWING_DATE_CBX Y_MONTH_SHOWING_DATE_CBX

#define WIDTH_FIRST_DAY_OF_MONTH 40
#define HEIGTH_FIRST_DAY_OF_MONTH 40
#define X_FIRST_DAY_OF_MONTH X_MONTH_SHOWING_DATE_CBX
#define Y_FIRST_DAY_OF_MONTH Y_MONTH_SHOWING_DATE_CBX + HEIGTH_MONTH_SHOWING_DATE_CBX

// day events ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define HEIGTH_EVENT_BLOCK 100
#define X_FIRST_EVENT_BLOCK 300
#define Y_FIRST_EVENT_BLOCK 40

#define HEIGTH_EVENTS_CBXS 200

#define WIDTH_DAY_EVENT_CBX 75
#define WIDTH_MONTH_EVENT_CBX 150
#define WIDTH_YEAR_EVENT_CBX 100
#define WIDTH_HOUR_EVENT_CBX 75
#define WIDTH_MIN_EVENT_CBX 75

// combo boxes +++++++++++++++
// RGB
#define RGB_DELETE_SCHEDULE_CBX RGB(235, 48, 48)

// POSITION AND SIZE
#define WIDTH_SELECT_SCHEDULE_CBX 150
#define HEIGTH_SELECT_SCHEDULE_CBX 100
#define X_SELECT_SCHEDULE_CBX X_NEW_SCHEDULE_BUTTON
#define Y_SELECT_SCHEDULE_CBX 60

#define WIDTH_DELETE_SCHEDULE_CBX 150
#define HEIGTH_DELETE_SCHEDULE_CBX 100
#define X_DELETE_SCHEDULE_CBX X_NEW_SCHEDULE_BUTTON + 150
#define Y_DELETE_SCHEDULE_CBX Y_NEW_SCHEDULE_BUTTON
// ---------------------------------------------------------------------------


#define MENU_STATE_LEN 2