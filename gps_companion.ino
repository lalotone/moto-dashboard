#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

// Define the RX and TX pins for Serial 2
#define RXD2 19
#define TXD2 18

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define GPS_BAUD 9600


// The TinyGPS++ object
TinyGPSPlus gps;

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  resetScreen();
  display.println("HELLO :)");
  display.display();
  delay(1000);
}

void loop() {
  unsigned long start = millis();

  while (millis() - start < 1000) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    if (gps.location.isUpdated()) {
      // Not used them ATM
      // Serial.print("LAT: ");
      // Serial.println(gps.location.lat(), 6);
      // Serial.print("LONG: "); 
      // Serial.println(gps.location.lng(), 6);
      resetScreen();
      display.println("SPEED kmh\n");
      display.print(gps.speed.kmph());
      display.display();
      resetScreen();
      display.println("ALT\n");
      display.print(gps.altitude.meters());
      display.display();
      resetScreen();
      display.println("SATS\n");
      display.print(gps.satellites.value());
      display.display();
      // DATE MIGHT BE INTERESTING IN A FUTURE
      // Serial.print("Time in UTC: ");
      // Serial.println(String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day()) + "," + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
      // Serial.println("");
    } else {
      display.clearDisplay();
      display.setCursor(20, 20);
      display.println("NO GPS\n   DATA");
      display.display();
      delay(100);
    }
  }
}

void resetScreen (){
  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 0);
}