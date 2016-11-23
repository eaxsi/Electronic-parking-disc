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
    
  }
  
  Eink.RefreshScreen();
}

//Controller for the top button
void buttonController() {

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

