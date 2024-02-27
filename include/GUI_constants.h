#define MAX_SUBMENU_OPTIONS 10

#define MAX_LABELS_BY_MENU 10

#define MAX_MENUS 10

#define MAX_BUTTONS_BY_MENU 40

// menu index constants +++++++++++++++++
#define MENU_SELECT_SCHEDULE 0
#define MENU_ON_SCHEDULE 1
// -------------------------------------

// options of the menus +++++++++++++++++
#define DELETE_SCHEDULE 0
#define NEW_SCHEDULE 1

#define RETURN_TO_MENU_ON_SCHEDULE 1
// -------------------------------------

// style +++++++++++++++++++++++++++++
// days
#define DAY_BUTTONS_HEIGHT 40
#define DAY_BUTTONS_WIDTH 50

#define DAY_BUTTONS_LEFT_MARGIN 50
#define DAY_BUTTONS_TOP_MARGIN 190

// schedules
#define SCH_BUTTONS_HEIGHT 20
#define SCH_BUTTONS_WIDTH 130

#define SCH_BUTTONS_LEFT_MARGIN 50
#define SCH_BUTTONS_TOP_MARGIN NEW_SCH_BUTTONS_TOP_MARGIN + DEL_SCH_BUTTONS_HEIGHT + 10

// delete schedule
#define DEL_SCH_BUTTONS_HEIGHT 20
#define DEL_SCH_BUTTONS_WIDTH 130

#define DEL_SCH_BUTTONS_LEFT_MARGIN 50
#define DEL_SCH_BUTTONS_TOP_MARGIN 10

// new schedule
#define NEW_SCH_BUTTONS_HEIGHT 20
#define NEW_SCH_BUTTONS_WIDTH 130

#define NEW_SCH_BUTTONS_LEFT_MARGIN 50
#define NEW_SCH_BUTTONS_TOP_MARGIN 10 + DEL_SCH_BUTTONS_HEIGHT

// combo boxes
// days
#define CMB_BOX_DAYS_HEIGHT 150
#define CMB_BOX_DAYS_WIDTH 70

#define CMB_BOX_DAYS_LEFT_MARGIN 200
#define CMB_BOX_DAYS_TOP_MARGIN 40

#define CMB_BOX_DAYS_ID 200

// months
#define CMB_BOX_MONTHS_HEIGHT 150
#define CMB_BOX_MONTHS_WIDTH 70

#define CMB_BOX_MONTHS_LEFT_MARGIN 200 + CMB_BOX_DAYS_WIDTH
#define CMB_BOX_MONTHS_TOP_MARGIN 40 

#define CMB_BOX_MONTHS_ID 201

// years
#define CMB_BOX_YEARS_HEIGHT 150
#define CMB_BOX_YEARS_WIDTH 90

#define CMB_BOX_YEARS_LEFT_MARGIN CMB_BOX_MONTHS_LEFT_MARGIN + CMB_BOX_MONTHS_WIDTH
#define CMB_BOX_YEARS_TOP_MARGIN 40

#define CMB_BOX_YEARS_ID 202
// -------------------------------------
