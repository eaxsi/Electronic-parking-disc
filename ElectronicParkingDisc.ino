#include <SmartEink.h>

E_ink Eink;

typedef enum {
  PARKIFY_SPLASH,
  SCROLLER_USAGE,
  BUTTON_USAGE
} Screen;

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
  stateChanged = true;
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
      Eink.EinkP8x16Str(14, 8, "SCROLL USING THE SELECTOR");
      break;

    case BUTTON_USAGE:
      Eink.EinkP8x16Str(14, 8, "CONTINUE USING THE TOP BUTTON");
      break;
    
  }
  
  Eink.RefreshScreen();
}

