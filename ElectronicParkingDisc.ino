#include <SmartEink.h>

E_ink Eink;

typedef enum {
  PARKIFY_SPLASH,
  SCROLLER_USAGE,
  BUTTON_USAGE
} Screen;

typedef enum {
  UP,
  DOWN
} Direction;

struct State {
  Screen screen;
} state;

boolean stateChanged = false;

void setup() {
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  
  Eink.InitEink();

  //Initialize state
  state.screen = PARKIFY_SPLASH;

  //Draw the splash screen
  render();

  delay(3000);

  //Switch to the scroller usage state
  state.screen = SCROLLER_USAGE;
  render();

  
}

void loop() {
  if (stateChanged) {
    render();
    stateChanged = false;
  }
}

//Redraws the screen according to the current state
void render() {
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
      Eink.EinkP8x16Str(14, 8, "CONTINUE USING THE TOP BUTTON");
      break;
    
  }
  
  Eink.RefreshScreen();
}

//Controller for the top button
void buttonController() {
  
}

//Controller for the scroller
void buttonController(Direction direction) {
  
}

