#include <SmartEink.h>

E_ink Eink;
const int BUTTON_PORT = 2;
const int SCROLL_DOWN_PORT = 3;
const int SCROLL_UP_PORT = 4;

typedef enum {
  PARKIFY_SPLASH,
  SCROLLER_USAGE,
  BUTTON_USAGE,
  SET_HOUR1,
  SET_HOUR2,
  SET_MIN1,
  SET_MIN2
} Screen;

typedef enum {
  UP = 1,
  DOWN = -1
} Direction;

struct State {
  Screen screen;
  char hour;
  char minute;
} state;

boolean state_changed = false;
boolean has_been_clicked = false;
boolean has_been_scrolled_down = false;
boolean has_been_scrolled_up = false;
char *time_string = (char *) malloc(6 * sizeof(char)); //Used by the method get_time_string to return the state's current time as a string

void setup() {
  //Setup for the Eink display
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  Eink.InitEink();

  //Setup for the button and scrollers
  pinMode(BUTTON_PORT, INPUT);
  pinMode(SCROLL_DOWN_PORT, INPUT);
  pinMode(SCROLL_UP_PORT, INPUT);
  digitalWrite(BUTTON_PORT, HIGH);
  digitalWrite(SCROLL_DOWN_PORT, HIGH);
  digitalWrite(SCROLL_UP_PORT, HIGH);
  
  //Initialize state
  state.screen = PARKIFY_SPLASH;
  state.hour = 0;
  state.minute = 0;

  //Draw the splash screen
  render();

  delay(3000);

  //Switch to the scroller usage state
  state.screen = SCROLLER_USAGE;
  render();

  //For remote controlling (mostly for debugging purposes)
  Serial.begin(9600);
  
}

void loop() {
  if (Serial.available()) {
    int in = Serial.parseInt();
    if (in == 1) {
      scrollController(DOWN);
    } else if (in == 2) {
      scrollController(UP);
    } else {
      buttonController();
    }
  }

  if (digitalRead(BUTTON_PORT) == LOW && !has_been_clicked) {
    has_been_clicked = true;
    buttonController();
  }

  if (digitalRead(BUTTON_PORT) == HIGH && has_been_clicked) {
    has_been_clicked = false;
  }

  if (digitalRead(SCROLL_DOWN_PORT) == LOW && !has_been_scrolled_down) {
    has_been_scrolled_down = true;
    scrollController(DOWN);
  }

  if (digitalRead(SCROLL_UP_PORT) == LOW && !has_been_scrolled_up) {
    has_been_scrolled_up = true;
    scrollController(UP);
  }

  if (digitalRead(SCROLL_DOWN_PORT) == HIGH && has_been_scrolled_down) {
    has_been_scrolled_down = false;
  }

  if (digitalRead(SCROLL_UP_PORT) == HIGH && has_been_scrolled_up) {
    has_been_scrolled_up = false;
  }
  
  if (state_changed) {
    render();
    state_changed = false;
  }
}

//Redraws the screen according to the current state
void render() {
  Eink.InitEink();
  Eink.ClearScreen();

  switch (state.screen) {

    case PARKIFY_SPLASH:
      Eink.EinkP8x16Str(14, 8, "Parkify");
      break;

    case SCROLLER_USAGE:
      Eink.EinkP8x16Str(14, 8, "SCROLL USING");
      Eink.EinkP8x16Str(10, 8, "THE SELECTOR");
      break;

    case BUTTON_USAGE:
      Eink.EinkP8x16Str(14, 8, "CONTINUE USING");
      Eink.EinkP8x16Str(10, 8, "THE TOP BUTTON");
      break;

    case SET_HOUR1:
      Eink.DisplayChar(13, 8, '_');
      Eink.EinkP8x16Str(14, 8, get_time_string());
      break; 
    
  }
  
  Eink.RefreshScreen();
}

//Returns the state's current time in the format 13:19 with trailing zeroeos (e.g. 00:00) 
char *get_time_string() {
  time_string[0] = (char) (state.hour/10 + '0');
  time_string[1] = (char) (state.hour%10 + '0');
  time_string[2] = ':';
  time_string[3] = (char) (state.minute/10 + '0');
  time_string[4] = (char) (state.minute%10 + '0');
  time_string[5] = 0;
  return time_string;
}

//Controller for the top button
void buttonController() {
  switch (state.screen) {
    case BUTTON_USAGE:
      state.screen = SET_HOUR1;
      state_changed = true;
      break;
  }
}

//Controller for the scroller
void scrollController(Direction direction) {
  switch (state.screen) {
    case SCROLLER_USAGE:
      state.screen = BUTTON_USAGE;
      state_changed = true;
      break;

    case SET_HOUR1:
      state.hour += direction * 10;
      if (state.hour >= 24) {
        state.hour -= 30;
      } else if (state.hour < 0) {
        state.hour += 30;
      }
      state_changed = true;
      break;
  }
}

