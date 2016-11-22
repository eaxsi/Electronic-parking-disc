#include <SmartEink.h>

E_ink Eink;

typedef enum {
  PARKIFY_SPLASH,
  SCROLLER_USAGE,
  BUTTON_USAGE
} Screen;

typedef struct State {
  Screen screen;
};

void setup() {
  pinMode(8,OUTPUT);
  digitalWrite(8, LOW);
  
  Eink.InitEink();
  Eink.ClearScreen();

  Eink.EinkP8x16Str(14, 8, "Parkify");

  Eink.RefreshScreen();
}

void loop() {

}
