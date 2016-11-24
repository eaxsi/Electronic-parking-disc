#include <SmartEink.h>

E_ink Eink;

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
  UP,
  DOWN
} Direction;

struct State {
  Screen screen;
  char hour;
  char minute;
} state;

boolean stateChanged = false;
char *time_string = (char *) malloc(6 * sizeof(char)); //Used by the method get_time_string to return the state's current time as a string

void setup() {
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);

  Eink.InitEink();
  
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
  
  if (stateChanged) {
    render();
    stateChanged = false;
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
      stateChanged = true;
      break;
  }
}

//Controller for the scroller
void scrollController(Direction direction) {
  switch (state.screen) {
    case SCROLLER_USAGE:
      state.screen = BUTTON_USAGE;
      stateChanged = true;
      break;
  }
}

