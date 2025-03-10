#ifndef CONFIG_H
#define CONFIG_H

#include <WiFiClientSecure.h>

// Deklaration der globalen Variablen (keine Definition!)
extern const char *ssid;
extern const char *password;
extern const char *server;
extern const char *apiKey;
extern const char *root_ca;
extern WiFiClientSecure client;
extern const char *dataUrl;
extern const int httpsPort;


#include <U8g2lib.h>

// OLED-Display initialize
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2; // notice "extern"

// Debug-Mode activate/deactivate
#define DEBUG false

#if DEBUG
// Debug-Initialisierung
#define DEBUG_INIT() Debug::init()

#endif

// Debug-Klasse mit statischen Methoden
class Debug
{
public:
  static void init(); // Initialisierungsmethode
  static void drawWrappedText(U8G2& display, const char* text, int x, int y, int width, int lineHeight);
};

void delayMillis(unsigned long startTime);

#endif
