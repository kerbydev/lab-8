
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

// ==================================================
// 🎀 ESP32-S3 + SSD1306 SETTINGS
// ==================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

// ==================================================
// ✨ ANIMATION SETTINGS
// ==================================================
const unsigned long INTRO_DURATION = 2200;
const unsigned long FRAME_INTERVAL = 65;
const unsigned long TRANSITION_TIME = 260;

// Change these after testing against your audio.
// Durations are in milliseconds, not BPM.
struct Lyric {
  const char* line1;
  const char* line2;
  unsigned long duration;
};

Lyric lyrics[] = {
  {"Come on, just", "say it, himitsu", 2600},
  {"no girls talk", "Amaku bitter,", 3200},
  {"tokidoki spicy", "Everything", 3000},
  {"uch iakete yo", "hush, hush", 3200},
  {"We can't stop,", "nee hora", 3000},
  {"kiss and tell", "", 2300}
};

const int LYRIC_COUNT =
  sizeof(lyrics) / sizeof(lyrics[0]);

// ==================================================
// ⭐ STAR POSITIONS
// ==================================================
struct Star {
  int x;
  int y;
  byte phase;
};

Star stars[] = {
  {7, 10, 0},
  {19, 24, 2},
  {112, 10, 1},
  {120, 25, 3},
  {9, 53, 2},
  {117, 53, 0},
  {96, 6, 3},
  {30, 57, 1}
};

const int STAR_COUNT = sizeof(stars) / sizeof(stars[0]);

// ==================================================
// ⏱️ PLAYBACK STATE
// ==================================================
unsigned long introStart = 0;
unsigned long lyricStart = 0;
unsigned long lastFrame = 0;

int currentLyric = 0;
bool introFinished = false;
bool playbackFinished = false;

// ==================================================
// 📝 CENTER TEXT
// ==================================================
void centeredText(const char* text, int y, int size) {
  if (text == nullptr || text[0] == '\0') return;

  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(
    text, 0, y, &x1, &y1, &w, &h
  );

  int x = (SCREEN_WIDTH - (int)w) / 2;
  if (x < 0) x = 0;

  display.setCursor(x, y);
  display.print(text);
}

// ==================================================
// 🎀 PIXEL BOW
// ==================================================
void drawRibbon(int x, int y, int scale) {
  display.fillTriangle(
    x - 8 * scale, y,
    x - 2 * scale, y - 5 * scale,
    x - 2 * scale, y + 5 * scale,
    SSD1306_WHITE
  );

  display.fillTriangle(
    x + 8 * scale, y,
    x + 2 * scale, y - 5 * scale,
    x + 2 * scale, y + 5 * scale,
    SSD1306_WHITE
  );

  display.fillRect(
    x - scale, y - scale,
    2 * scale + 1, 2 * scale + 1,
    SSD1306_WHITE
  );

  display.drawLine(
    x - scale, y + 3 * scale,
    x - 4 * scale, y + 7 * scale,
    SSD1306_WHITE
  );

  display.drawLine(
    x + scale, y + 3 * scale,
    x + 4 * scale, y + 7 * scale,
    SSD1306_WHITE
  );
}

// ==================================================
// ⭐ TWINKLING STARS
// ==================================================
void drawStar(int x, int y, bool large) {
  if (!large) {
    display.drawPixel(x, y, SSD1306_WHITE);
    return;
  }

  display.drawLine(x, y - 2, x, y + 2, SSD1306_WHITE);
  display.drawLine(x - 2, y, x + 2, y, SSD1306_WHITE);
  display.drawPixel(x - 1, y - 1, SSD1306_WHITE);
  display.drawPixel(x + 1, y - 1, SSD1306_WHITE);
  display.drawPixel(x - 1, y + 1, SSD1306_WHITE);
  display.drawPixel(x + 1, y + 1, SSD1306_WHITE);
}

void drawStars(unsigned long now) {
  unsigned long tick = now / 180;

  for (int i = 0; i < STAR_COUNT; i++) {
    bool visible = ((tick + stars[i].phase) % 4 != 0);
    bool large = ((tick + stars[i].phase) % 5 == 0);

    if (visible) {
      drawStar(stars[i].x, stars[i].y, large);
    }
  }
}

// ==================================================
// 🎀 BOUNCING BOW
// ==================================================
int ribbonBounce(unsigned long now) {
  int frame = (now / 150) % 4;

  if (frame == 0) return 0;
  if (frame == 1) return -1;
  if (frame == 2) return -2;

  return -1;
}

// ==================================================
// 🌟 INTRO
// ==================================================
void drawIntro(unsigned long now) {
  display.clearDisplay();
  drawStars(now);

  centeredText("KISS & TELL", 17, 1);
  centeredText("aespa", 30, 2);

  drawRibbon(64, 51 + ribbonBounce(now), 1);

  display.display();
}

// ==================================================
// ✨ QUICK SPARKLE TRANSITION
// ==================================================
void drawTransition(unsigned long elapsed) {
  int frame = elapsed / 65;
  int distance = 3 + frame * 4;

  drawStar(64 - distance, 34, frame % 2 == 0);
  drawStar(64 + distance, 34, frame % 2 == 0);
}

// ==================================================
// 🎵 DRAW CURRENT LYRIC
// Text automatically switches size to avoid clipping.
// ==================================================
void drawLyric(unsigned long now) {
  display.clearDisplay();
  drawStars(now);

  drawRibbon(64, 8 + ribbonBounce(now), 1);

  const char* line1 = lyrics[currentLyric].line1;
  const char* line2 = lyrics[currentLyric].line2;

  int len1 = strlen(line1);
  int len2 = strlen(line2);

  // Default large font; use small font for long lines.
  int textSize = 2;

  if (len1 > 10 || len2 > 10) {
    textSize = 1;
  }

  int lineHeight = 8 * textSize;
  int y1;
  int y2 = -1;

  if (len2 == 0) {
    y1 = (SCREEN_HEIGHT - lineHeight) / 2;
  } else {
    y1 = (SCREEN_HEIGHT - (lineHeight * 2 + 2)) / 2;
    y2 = y1 + lineHeight + 2;
  }

  unsigned long elapsed = now - lyricStart;
  int slide = 0;

  if (elapsed < TRANSITION_TIME) {
    slide = 4 - (int)(
      elapsed * 4 / TRANSITION_TIME
    );
    drawTransition(elapsed);
  }

  centeredText(line1, y1 + slide, textSize);

  if (len2 > 0) {
    centeredText(line2, y2 + slide, textSize);
  }

  display.drawPixel(64, 60, SSD1306_WHITE);
  display.display();
}

// ==================================================
// 💗 FINAL SCREEN
// ==================================================
void drawFinalScreen(unsigned long now) {
  display.clearDisplay();
  drawStars(now);

  drawRibbon(64, 8 + ribbonBounce(now), 1);

  const char* line1 = lyrics[currentLyric].line1;
  const char* line2 = lyrics[currentLyric].line2;

  int textSize = (strlen(line1) > 10 ||
                  strlen(line2) > 10) ? 1 : 2;

  int lineHeight = 8 * textSize;
  int y1;
  int y2 = -1;

  if (line2[0] == '\0') {
    y1 = (SCREEN_HEIGHT - lineHeight) / 2;
  } else {
    y1 = (SCREEN_HEIGHT - (lineHeight * 2 + 2)) / 2;
    y2 = y1 + lineHeight + 2;
  }

  centeredText(line1, y1, textSize);

  if (line2[0] != '\0') {
    centeredText(line2, y2, textSize);
  }

  display.drawPixel(60, 60, SSD1306_WHITE);
  display.drawPixel(62, 59, SSD1306_WHITE);
  display.drawPixel(64, 60, SSD1306_WHITE);
  display.drawPixel(62, 61, SSD1306_WHITE);

  display.display();
}

// ==================================================
// 🔄 PLAYBACK ENGINE
// ==================================================
void updateAnimation(unsigned long now) {
  if (!introFinished) {
    if (now - introStart < INTRO_DURATION) {
      drawIntro(now);
      return;
    }

    introFinished = true;
    currentLyric = 0;
    lyricStart = now;
  }

  if (!playbackFinished) {
    if (now - lyricStart >= lyrics[currentLyric].duration) {
      if (currentLyric < LYRIC_COUNT - 1) {
        currentLyric++;
        lyricStart = now;
      } else {
        playbackFinished = true;
      }
    }
  }

  if (playbackFinished) {
    drawFinalScreen(now);
  } else {
    drawLyric(now);
  }
}

// ==================================================
// 🚀 SETUP
// ==================================================
void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 initialization failed!");
    while (true) {}
  }

  display.clearDisplay();
  display.display();

  introStart = millis();
  lastFrame = introStart;

  Serial.println("aespa Kiss & Tell player ready!");
}

// ==================================================
// 🔁 MAIN LOOP
// ==================================================
void loop() {
  unsigned long now = millis();

  if (now - lastFrame >= FRAME_INTERVAL) {
    lastFrame = now;
    updateAnimation(now);
  }
}
