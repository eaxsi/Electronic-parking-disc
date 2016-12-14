#include <SmartEink.h>

E_ink Eink;
const int BUTTON_PORT = 2;
const int SCROLL_DOWN_PORT = 4;
const int SCROLL_UP_PORT = 3;

typedef enum {
  PARKIFY_SPLASH,
  SCROLLER_USAGE,
  BUTTON_USAGE,
  SET_HOUR1,
  SET_HOUR2,
  SET_MIN1,
  SET_MIN2,
  SET_YEAR,
  SET_MONTH,
  SET_DAY,
  SHOW_TIME,
  SAMPLE_TIME
} Screen;

typedef enum {
  UP = 1,
  DOWN = -1
} Direction;

struct State {
  Screen screen;
  char hour;
  char minute;
  short year;
  char month;
  char day;
} state;

boolean state_changed = false;
boolean has_been_clicked = false;
boolean has_been_scrolled_down = false;
boolean has_been_scrolled_up = false;
char *time_string = (char *) malloc(6 * sizeof(char)); //Used by the method get_time_string to return the state's current time as a string
char *date_string = (char *) malloc(11 * sizeof(char)); //As above, but for the date

const uint8_t sample_time_bitmap[] PROGMEM = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 127, 255, 255, 255, 255, 255, 255, 192, 0, 7, 255, 255, 255, 255, 255, 254, 0, 0, 0, 255, 255, 255, 255, 255, 252, 0, 0, 0, 127, 255, 255, 255, 255, 248, 0, 0, 0, 63, 255, 255, 255, 255, 240, 0, 0, 0, 31, 255, 255, 255, 255, 224, 0, 0, 0, 15, 255, 255, 255, 255, 192, 3, 255, 128, 7, 255, 255, 255, 255, 192, 31, 255, 248, 7, 255, 255, 255, 255, 192, 127, 255, 252, 7, 255, 255, 255, 255, 192, 255, 255, 254, 7, 255, 255, 255, 255, 129, 255, 255, 254, 3, 255, 255, 255, 255, 129, 255, 255, 255, 3, 255, 255, 255, 255, 129, 255, 255, 255, 3, 255, 255, 255, 255, 129, 255, 255, 254, 3, 255, 255, 255, 255, 192, 255, 255, 254, 7, 255, 255, 255, 255, 192, 127, 255, 252, 7, 255, 255, 255, 255, 192, 31, 255, 240, 7, 255, 255, 255, 255, 224, 0, 254, 0, 15, 255, 255, 255, 255, 224, 0, 0, 0, 15, 255, 255, 255, 255, 240, 0, 0, 0, 31, 255, 255, 255, 255, 248, 0, 0, 0, 63, 255, 255, 255, 255, 252, 0, 0, 0, 127, 255, 255, 255, 255, 255, 0, 0, 1, 255, 255, 255, 255, 255, 255, 192, 0, 7, 255, 255, 255, 255, 255, 255, 254, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 15, 255, 255, 255, 255, 255, 255, 31, 224, 1, 255, 255, 255, 255, 255, 252, 31, 192, 0, 255, 255, 255, 255, 255, 248, 31, 128, 0, 63, 255, 255, 255, 255, 240, 31, 0, 0, 31, 255, 255, 255, 255, 224, 30, 0, 0, 31, 255, 255, 255, 255, 224, 28, 0, 0, 15, 255, 255, 255, 255, 192, 28, 1, 240, 7, 255, 255, 255, 255, 192, 124, 7, 252, 7, 255, 255, 255, 255, 192, 252, 15, 254, 7, 255, 255, 255, 255, 193, 252, 31, 254, 3, 255, 255, 255, 255, 129, 252, 31, 255, 3, 255, 255, 255, 255, 129, 252, 31, 255, 3, 255, 255, 255, 255, 193, 252, 31, 255, 3, 255, 255, 255, 255, 193, 252, 31, 254, 7, 255, 255, 255, 255, 192, 254, 15, 254, 7, 255, 255, 255, 255, 192, 126, 7, 252, 7, 255, 255, 255, 255, 224, 31, 3, 240, 7, 255, 255, 255, 255, 224, 1, 128, 0, 15, 255, 255, 255, 255, 240, 0, 0, 0, 15, 255, 255, 255, 255, 248, 0, 0, 0, 31, 255, 255, 255, 255, 252, 0, 0, 0, 63, 255, 255, 255, 255, 254, 0, 0, 0, 127, 255, 255, 255, 255, 255, 128, 0, 1, 255, 255, 255, 255, 255, 255, 240, 0, 7, 255, 255, 255, 255, 255, 255, 255, 131, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 192, 63, 240, 31, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 63, 255, 255, 255, 255, 255, 255, 255, 192, 31, 255, 255, 255, 255, 255, 255, 255, 192, 15, 255, 255, 255, 255, 255, 255, 255, 192, 3, 255, 255, 255, 255, 255, 255, 255, 192, 1, 255, 255, 255, 255, 255, 255, 255, 192, 0, 255, 255, 255, 255, 255, 255, 255, 192, 128, 63, 255, 255, 255, 255, 255, 255, 192, 224, 31, 255, 255, 255, 255, 255, 255, 192, 240, 15, 255, 255, 255, 255, 255, 255, 192, 248, 3, 255, 255, 255, 255, 255, 255, 192, 254, 1, 255, 255, 255, 255, 255, 255, 192, 255, 0, 255, 255, 255, 255, 255, 255, 192, 255, 128, 63, 255, 255, 255, 255, 255, 192, 255, 224, 31, 255, 255, 255, 255, 255, 192, 255, 240, 15, 255, 255, 255, 255, 192, 0, 0, 0, 7, 255, 255, 255, 255, 192, 0, 0, 0, 7, 255, 255, 255, 255, 192, 0, 0, 0, 7, 255, 255, 255, 255, 192, 0, 0, 0, 7, 255, 255, 255, 255, 192, 0, 0, 0, 7, 255, 255, 255, 255, 192, 0, 0, 0, 7, 255, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 195, 255, 255, 255, 255, 255, 255, 255, 255, 192, 255, 255, 7, 255, 255, 255, 255, 255, 192, 63, 255, 1, 255, 255, 255, 255, 255, 192, 31, 255, 0, 127, 255, 255, 255, 255, 192, 15, 255, 0, 63, 255, 255, 255, 255, 192, 7, 255, 0, 31, 255, 255, 255, 255, 192, 3, 255, 0, 15, 255, 255, 255, 255, 192, 1, 255, 0, 15, 255, 255, 255, 255, 192, 0, 255, 240, 7, 255, 255, 255, 255, 192, 0, 127, 252, 7, 255, 255, 255, 255, 192, 128, 127, 254, 7, 255, 255, 255, 255, 192, 224, 63, 254, 3, 255, 255, 255, 255, 192, 224, 31, 254, 3, 255, 255, 255, 255, 192, 240, 31, 255, 3, 255, 255, 255, 255, 192, 248, 15, 254, 3, 255, 255, 255, 255, 192, 252, 7, 254, 7, 255, 255, 255, 255, 192, 252, 3, 252, 7, 255, 255, 255, 255, 192, 254, 1, 248, 7, 255, 255, 255, 255, 192, 255, 0, 0, 7, 255, 255, 255, 255, 192, 255, 0, 0, 15, 255, 255, 255, 255, 192, 255, 128, 0, 15, 255, 255, 255, 255, 192, 255, 192, 0, 31, 255, 255, 255, 255, 192, 255, 224, 0, 63, 255, 255, 255, 255, 192, 255, 240, 0, 127, 255, 255, 255, 255, 192, 255, 252, 1, 255, 255, 255, 255, 255, 192, 255, 255, 223, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };

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
  state.hour = 13;
  state.minute = 25;
  state.year = 2017;
  state.month = 6;
  state.day = 15;

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
      scroll_controller(DOWN);
    } else if (in == 2) {
      scroll_controller(UP);
    } else {
      button_controller();
    }
  }

  if (digitalRead(BUTTON_PORT) == LOW && !has_been_clicked) {
    has_been_clicked = true;
    button_controller();
  }

  if (digitalRead(BUTTON_PORT) == HIGH && has_been_clicked) {
    has_been_clicked = false;
  }

  if (digitalRead(SCROLL_DOWN_PORT) == LOW && !has_been_scrolled_down) {
    has_been_scrolled_down = true;
    scroll_controller(DOWN);
  }

  if (digitalRead(SCROLL_UP_PORT) == LOW && !has_been_scrolled_up) {
    has_been_scrolled_up = true;
    scroll_controller(UP);
  }

  if (digitalRead(SCROLL_DOWN_PORT) == HIGH && has_been_scrolled_down) {
    has_been_scrolled_down = false;
  }

  if (digitalRead(SCROLL_UP_PORT) == HIGH && has_been_scrolled_up) {
    has_been_scrolled_up = false;
  }

  if (state_changed) {
    Serial.println((int) state.hour);
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
      Eink.EinkP8x16Str(7, 72, "Parkify");
      break;

    case SCROLLER_USAGE:
      Eink.EinkP8x16Str(9, 54, "SCROLL USING");
      Eink.EinkP8x16Str(5, 54, "THE SELECTOR");
      break;

    case BUTTON_USAGE:
      Eink.EinkP8x16Str(9, 48, "CONTINUE USING");
      Eink.EinkP8x16Str(5, 48, "THE TOP BUTTON");
      break;

    case SET_HOUR1:
      Eink.DisplayChar(6, 76, '_');
      Eink.EinkP8x16Str(7, 76, get_time_string());
      break;


    case SET_HOUR2:
      Eink.DisplayChar(6, 84, '_');
      Eink.EinkP8x16Str(7, 76, get_time_string());
      break;


    case SET_MIN1:
      Eink.DisplayChar(6, 100, '_');
      Eink.EinkP8x16Str(7, 76, get_time_string());
      break;


    case SET_MIN2:
      Eink.DisplayChar(6, 108, '_');
      Eink.EinkP8x16Str(7, 76, get_time_string());
      break;

    case SET_YEAR:
      Eink.EinkP8x16Str(6, 62, "____");
      Eink.EinkP8x16Str(7, 62, get_date_string());
      break;

    case SET_MONTH:
      Eink.EinkP8x16Str(6, 102, "__");
      Eink.EinkP8x16Str(7, 62, get_date_string());
      break;

    case SET_DAY:
      Eink.EinkP8x16Str(6, 126, "__");
      Eink.EinkP8x16Str(7, 62, get_date_string());
      break;

    case SHOW_TIME:
      Eink.EinkP8x16Str(7, 76, get_time_string());
      break;

    case SAMPLE_TIME:
      Eink.ShowBitMapFromProgmem(sample_time_bitmap);
      break;

  }

  Eink.RefreshScreen();
}

//Returns the state's current time in the format 13:19 with trailing zeroes (e.g. 00:00)
char *get_time_string() {
  time_string[0] = (char) (state.hour / 10 + '0');
  time_string[1] = (char) (state.hour % 10 + '0');
  time_string[2] = ':';
  time_string[3] = (char) (state.minute / 10 + '0');
  time_string[4] = (char) (state.minute % 10 + '0');
  time_string[5] = 0;
  return time_string;
}

//Returns the state's current date in the format YYYY-MM-DD with trailing zeroes
char *get_date_string() {
  date_string[0] = (char) (state.year / 1000 + '0');
  date_string[1] = (char) ((state.year / 100) % 10 + '0');
  date_string[2] = (char) ((state.year / 10) % 10 + '0');
  date_string[3] = (char) (state.year % 10 + '0');
  date_string[4] = '-';
  date_string[5] = (char) (state.month / 10 + '0');
  date_string[6] = (char) (state.month % 10 + '0');
  date_string[7] = '-';
  date_string[8] = (char) (state.day / 10 + '0');
  date_string[9] = (char) (state.day % 10 + '0');
  date_string[10] = 0;
  return date_string;
}

//Controller for the top button
void button_controller() {
  switch (state.screen) {
    case BUTTON_USAGE:
      state.screen = SET_HOUR1;
      state_changed = true;
      break;

    case SET_HOUR1:
      state.screen = SET_HOUR2;
      state_changed = true;
      break;

    case SET_HOUR2:
      state.screen = SET_MIN1;
      state_changed = true;
      break;


    case SET_MIN1:
      state.screen = SET_MIN2;
      state_changed = true;
      break;

    case SET_MIN2:
      state.screen = SET_YEAR;
      state_changed = true;
      break;

    case SET_YEAR:
      state.screen = SET_MONTH;
      state_changed = true;
      break;

    case SET_MONTH:
      state.screen = SET_DAY;
      state_changed = true;
      break;

    case SET_DAY:
      state.screen = SHOW_TIME;
      state_changed = true;
      break;

    case SHOW_TIME:
      state.screen = SAMPLE_TIME;
      state_changed = true;
      break;

    case SAMPLE_TIME:
      state.screen = SHOW_TIME;
      state_changed = true;
      break;
  }
}

//Controller for the scroller
void scroll_controller(Direction direction) {

  char minute_before;
  char hour_before;

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

    case SET_HOUR2:
      hour_before = state.hour;
      state.hour += direction;
      if (direction == UP && state.hour % 10 == 0) {
        state.hour -= 10;
      } else if (direction == DOWN && hour_before % 10 == 0) {
        state.hour += 10;
      }
      state_changed = true;
      break;

    case SET_MIN1:
      state.minute += 10 * direction;
      if (state.minute >= 60) {
        state.minute -= 60;
      } else if (state.minute < 0) {
        state.minute += 60;
      }
      state_changed = true;
      break;

    case SET_MIN2:
      minute_before = state.minute;
      state.minute += direction;
      if (direction == UP && state.minute % 10 == 0) {
        state.minute -= 10;
      } else if (direction == DOWN && minute_before % 10 == 0) {
        state.minute += 10;
      }
      state_changed = true;
      break;

    case SET_YEAR:
      state.year += direction;
      state_changed = true;
      break;

    case SET_MONTH:
      state.month += direction;
      if (state.month > 12) {
        state.month -= 12;
      } else if (state.month <= 0) {
        state.month += 12;
      }
      state_changed = true;
      break;

    case SET_DAY:
      state.day = wrapped_day(state.year, state.month, state.day + direction);
      state_changed = true;
      break;
  }
}

/* Wraps the day if it is invalid and returns it (2016, 01, 32) -> (1)
 * This method takes leap years into account */
char wrapped_day(int year, char month, char day) {
  boolean is_leap_year = (year % 4 != 0) ? false : ((year % 100 != 0) ? true : ((year % 400 != 0) ? false : true));
  char days_in_month;
  if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
    days_in_month = 31; 
  } else if (month == 2) {
    days_in_month = is_leap_year ? 29 : 28;
  } else {
    days_in_month = 30;
  }
  if (day > days_in_month) {
    return day - days_in_month;
  } else if (day <= 0) {
    return day + days_in_month;
  } else {
    return day;
  }
}

