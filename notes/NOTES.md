## DEPENDENCIES
Compilation arguments for GUI functions:
    -lgdi32


## Flow of the GUI:

![[code_flow_01.excalidraw|1000]]

<hr style="border-color: violet;">
#### ![[code_flow_01.excalidraw#^u8rmEdtL]]
<div style="background-color: rgba(192, 87, 223, 0.225); border-radius: 10px; padding-left: 10px; white-space: pre;">Creation of all static objects and global variables 
(only the objects that are always on the same place):
1. Buttons (New schedule / Delete schedule)
2. Combo boxes (Schedule selection, Month, Year)
3. Input boxes (Any at the start)
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

#### Structures to save the elements of the GUI
```C
// 31 days of the month + 4 other buttons
#define N_BUTTONS 31 + 4  
HWND buttons[N_BUTTONS];

// 4 others + MAX_DISPLAYING_EVENTS * 5 (each events has 5 combo boxes)
#define N_COMBO_BOXES 4 + MAX_DISPLAYING_EVENTS * 5
HWND combo_boxes[N_COMBO_BOXES]

// name input for the new scheudle creation + MAX_DISPLAYING_EVENTS
#define N_INPUT_BOXES 1 + MAX_DISPLAYING_EVENTS
HWND text_input_boxes[N_INPUT_BOXES]
```
