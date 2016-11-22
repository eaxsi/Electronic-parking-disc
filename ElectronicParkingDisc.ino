#include <SmartEink.h>

E_ink Eink;

typedef enum {
  PARKIFY_SPLASH,
  SCROLLER_USAGE,
  BUTTON_USAGE
} Screen;

struct State {
  Screen screen;
};

boolean stateChanged = false;

void setup() {
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  
  Eink.InitEink();

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
  Eink.EinkP8x16Str(14, 8, "Parkified");
  Eink.RefreshScreen();
}

