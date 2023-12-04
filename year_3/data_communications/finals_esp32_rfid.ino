#include <WiFi.h>
#include <HTTPClient.h>

const String URL = "http://192.168.254.191/some_php/data.php";

const char* ssid = "ESP-32 Test Access Point";
const char* password = "thepasswordis";

int pinLED = 33;

int some_row = 1;

// new stuff
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

const int TFT_SCK = 14;
const int TFT_SDA = 13;
const int TFT_A0 = 2;
const int TFT_RESET = 12;
const int TFT_CS = 15;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_A0, TFT_SDA, TFT_SCK, TFT_RESET);
// end new stuff

// void connectWiFi() {
//   Serial.println();
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println();
//   Serial.println("WiFi connected.");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
//   Serial.println();
// }

String getLocalIPAddress() {
  return String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + WiFi.localIP()[2] + "." + WiFi.localIP()[3];
}

void connectWiFiTFT() {
  tft.print("Connecting to ");
  tft.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }

  tft.println();
  tft.println("WiFi connected.");
  tft.println();
  tft.println("IP address: ");
  tft.println(getLocalIPAddress());
  tft.println();

  delay(5000);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);

  // new stuff
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  // end new stuff

  connectWiFiTFT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFiTFT();
  }

  // String postData = "some_row=" + String(some_row);
  String getData = "?some_row=" + String(some_row);
  String fullURL = URL + getData;

  digitalWrite(pinLED, LOW);

  HTTPClient http;
  http.begin(fullURL);

  // int httpResponseCode = http.POST(postData);
  int httpResponseCode = http.GET();
  // http.addHeader("Content-Type", "application/x-www-from-urlencoded");

  // Serial.print("URL: ");
  // Serial.println(fullURL);

  // if (httpResponseCode > 0) {
  //   Serial.print("HTTP Response code: ");
  //   Serial.println(httpResponseCode);
  //   String payload = http.getString();
  //   Serial.println(payload);
  // } else {
  //   Serial.print("Error code: ");
  //   Serial.println(httpResponseCode);
  // }

  // Serial.println("--------------------------------------------------");

  tft.print("URL: ");
  tft.println(fullURL);
  tft.println();

  if (httpResponseCode > 0) {
    tft.print("HTTP Response code: ");
    tft.println(httpResponseCode);

    tft.println();

    tft.print("Payload: ");
    tft.println(http.getString());
  } else {
    tft.print("Error code: ");
    tft.println(httpResponseCode);
  }

  // tft.println("--------------------------");

  http.end();

  digitalWrite(pinLED, HIGH);

  delay(5000);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);

  some_row += 1;
}