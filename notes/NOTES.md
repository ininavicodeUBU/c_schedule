## DEPENDENCIES
Compilation arguments for GUI functions:
    -lgdi32


## Flow of the GUI:

![[code_flow_01.excalidraw|1000]]

<hr style="border-color: violet;">
#### ![[code_flow_01.excalidraw#^u8rmEdtL]]
<div style="background-color: rgba(192, 87, 223, 0.225); border-radius: 10px; padding-left: 10px; white-space: pre;">Procedures to do:
	- Get&Save the .exe file path
	- Get&Save the path for the schedules folder
	- Get&Save the available schedules

	- Create all the elements of the GUI (hide all)
	- Show the elements of the no schedule selected menu (and set
		its default values)

	- Get the local time and save it on the showing date

	- Set the menu state to the correct
</div>

<hr style="border-color: violet;">
#### ![[code_flow_01.excalidraw#^MaRTkqLA]]
<div style="background-color: rgba(192, 87, 223, 0.225); border-radius: 10px; padding-left: 10px; white-space: pre">Each displayed event has 4 modificable things:
1. Date
	1.1. Day (Combo box)
	1.2. Month (Combo box)
	1.3. Year (Combo box)
2. Description (Input text)
</div>

<hr style="border-color: violet;">


## Definitions of the ID of the buttons

### Ranges:
(0 - 99) -> Buttons
(100 - 199) -> Combo boxes
(200 - 299) -> Input text

## Data structures

### Menu state control

#### Analysis of Possible states

| *nº state* |        *var1*        |        *var2*         |
| ---------- | :------------------: | :-------------------: |
| 1          | no schedule selected |      no defined       |
| 2          | no schedule selected | creating new schedule |
| 3          |  schedule selected   |      no defined       |
| 4          |  schedule selected   |     day selected      |
##### var1 <hr style="border-color: rgb(90, 224, 168)">

###### Possible states
| *nº state* |        *var1*        |
| ---------- | :------------------: |
| 1          | no schedule selected |
| 2          | no schedule selected |
| 3          |  schedule selected   |
| 4          |  schedule selected   |
**Total count = 2**
**Variable type =** <span class="C-variable">uint8_t</span> *{0 - 255}*
##### var2 <hr style="border-color: rgb(90, 224, 168)">

###### Possible states
| *nº state* |        *var2*         |
| ---------- | :-------------------: |
| 1          |      no defined       |
| 2          | creating new schedule |
| 3          |      no defined       |
| 4          |     day selected      |
**Total count = 3**
**Variable type =** <span class="C-variable">uint8_t</span> *{0 - 255}*

#### Data Structure for control menu state
```C
// actually of len 2, modificable for possible ampliations
#define MENU_STATE_LEN 2
uint8_t menu_state[MENU_STATE_LEN];
```

### Elements of the GUI visualization

##### State 1
- Button: New schedule
- Combo box: Delete schedule
- Combo box: Select schedule  

##### State 2
- Window: Where the user interacts to enter the name
- Text: Intro the name
- Text input box: Where the user enters the name of the new schedule
- Button: Save new schedule name
- Button: Cancel new schedule name

##### State 3
- Text: Actual schedule selected
- Buttons: Days of the month *{1 - 31}*
- Button: Save
- Combo box: Month *(actual selected showing month)*
- Combo box: Year *(actual selected showing year)*

##### State 4
- All [[NOTES#State 3|State 3]]
- Combo boxes: Day/Month/Year Hour/Minute by number of events
- Text input box: Description for each event
- Text: *"Date:"* before the *Combo box* of the date
- Text: *"Description:"* on top of the *Input box* of the description

#### Windows screen
This is the main screen of the program, so to add any element to the screen we have to tell to the screen to do it (more or less).

```C
HWND GUI_main_screen;
```

#### Structures to save the elements of the GUI
```C
// 31 days of the month + 4 other buttons
#define N_BUTTONS 31 + 4  
HWND buttons[N_BUTTONS];

// 4 others + MAX_DISPLAYING_EVENTS * 5 (each events has 5 combo boxes)
#define N_COMBO_BOXES 4 + MAX_DISPLAYING_EVENTS * 5
HWND combo_boxes[N_COMBO_BOXES];

// name input for the new scheudle creation + MAX_DISPLAYING_EVENTS
#define N_INPUT_BOXES 1 + MAX_DISPLAYING_EVENTS
HWND text_input_boxes[N_INPUT_BOXES];

#define N_TEXTS 4
HWND text[N_TEXTS];
```

#### Other important variables

##### Local time
We will save that date to keep it there constant,it will be necessary to actualize the combo boxes relatively to this time

```C
date_t local_time_date;
```

##### Showing date
Here will be stored the actual Month/Year for the showing days,
and the selected day to visualize it's events.

```C
// this data_type is defined on func.h
date_t showing_date;
```

##### Path to the .exe file
Unfortunately it's necessary to get our paths relatively from the .exe path.
If the user executes the program from a folder different from the .exe file, all the paths of the program will be wrong, so it's necessary to set a base path to get the other paths from there.

We will use this variable type for more data so we create a data_type.

```C
#define SCHEDULE_PATH_MAX_LEN (defined on func.h)
typedef char path_t [SCHEDULE_PATH_MAX_LEN];

path_t exe_file_path;
```
##### Folder for the schedules
This variable stores the path of the folder where all the schedules are stored.

```C
// variable to store the path for the folder of the schedules
path_t schedules_folder_path;
```

##### Actual selected schedule
This variable stores the path for the selected schedule

```C
path_t selected_schedule_path;
```

##### Available schedules to select
This variable stores **names** and not the **paths** of the available schedules.
For this we will create a data_type because is a list of strings.

```C
// MAX_FILENAME_LEN defined on constants.h
typedef char* names_of_available_schedules_t [MAX_FILENAME_LEN];

names_of_available_schedules_t names_of_available_schedules;
```
##### Last downloaded events
Here will be stored the events from the selected schedule.
The data type of an event is already defined on **func.h**

```C
// date_event_t already defined on func.h
date_event_t last_downloaded_events[MAX_EVENTS];
```

##### Events of the showing date
There are the filtered last_downloaded_events by the showing date.

```C
date_event_t events_of_showing_date[MAX_EVENTS];
```


> [!warning] Adding dynamic colors to the buttons of the months
> For problems on the arhitechture of the windows API we have to save the colors of the buttons to assignate these on the painting call

```C
// for each color change we have to save that change here
unsigned colors_of_buttons[N_BUTTONS];
```

##### Structure to display each event
When the GUI shows the events of the selected day is tricky to
access the ID of each element, so it could be convenient to create an structure to save the **combo boxes and elements** of each event.

It would be perfect to do I with an *object* but this is not possible in C, instead we can simulate that *constructor*, and *methods* with specific functions.

But another problem appears with the screen space, it's going to be necessary to add pages, to show more events than can fit on a page.
So this page is a variable that we have to store.

It's so easy to create new elements that we will rewrite the existing events with new default values at the page to *'simulate'* that we are in other page.

```C
// structure for each event
typedef struct
{
	date_event_t date_event;
	HWND GUI_elements [6];
	unsigned block_id;
	unsigned n_page;

} GUI_event_t;

GUI_event_t GUI_events_list [MAX_EVENTS];
```

![[showing_events_analysis.excalidraw|1000]]



#### Data structure conclusion <hr style="border-color: rgb(50, 200, 200);">

> [!important] Conclusion for the data structure
> All the data has to be accessible to every function, but it is
> not so eficient to add every object as a parameter for each function.
> So the decision is to get all this data in a struct that stores all the necessary data for anything

```C
typedef struct
{
	// GUI elements +++++++++++++++++++++
	HWND GUI_main_screen;
	HWND buttons[N_BUTTONS];
	HWND combo_boxes[N_COMBO_BOXES];
	HWND text_input_boxes[N_INPUT_BOXES];
	HWND text[N_TEXTS];
	GUI_event_t GUI_events_list [MAX_EVENTS];
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
	path_t schedules_folder_path;
	// -----------------------------------
	// list of events of the selected schedule
	date_event_t last_downloaded_events[MAX_EVENTS];
	
	date_event_t events_of_showing_date[MAX_EVENTS];
	// -----------------------------------

} GUI_data_t;
```


