#include <WiFi.h>
#include <PCF8575_library.h>
#include <ESPAsyncWebServer.h>
#include "control_board_power_status.h"
#include "control_board_run_status.h"
#include "control_board_speed.h"
#include <FastLED.h>
 
#define LED_PIN_1     19  // The pin for the first LED strip (UpTrack)
#define LED_PIN_2     18  // The pin for the second LED strip (DownTrack)
#define NUM_LEDS      20  // Number of LEDs in each strip
#define BRIGHTNESS    64  
 
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
 
const char* ssid = "ESP32_AP_1";
const char* password = "12345678";
AsyncWebServer server(80);
 
String subStationNames[32] = {
    // Group 1: "VPY" to "PLMG" (Index 0-30)
    "VPY", "PER", "PEW", "VLK", "PVM", "FRT", "SAN", "NTD",  // Index 0-7
    "AVD", "PAB", "TI", "SVR", "TRL", "KBT", "MAF", "MSU",   // Index 8-15
    "BBQ", "VJN", "PCW", "ICF", "KOTR", "ABU", "TMVL",       // Index 16-22
    "ANNR", "HC", "NEC", "VEU", "PTLR", "EGT", "SPAM", "TO", // Index 23-30
    "PLMG"
};
 
bool subStationStatus[32] = {false};
int subStationLedState[32] = {0};  // 0: off, 1: blinking
unsigned long lastBlinkTime[32] = {0};
const long blinkInterval = 500;  // milliseconds
 
bool trackStatus[2][2] = {{false, false}, {false, false}}; // [station][track]
int trackSpeed[2][2] = {{1, 1}, {1, 1}}; // [station][track]
 
void gpio_run_status_init()
{
  pcf_run.begin();
  for (int i = 0; i < 16; i++) {
    pcf_run.pinMode(i, OUTPUT);
  }
}
 
void gpio_power_status_init()
{
  pcf_power.begin();
  for (int i = 0; i < 16; i++) {
    pcf_power.pinMode(i, OUTPUT);
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Welcome to the MGR Central to Arakonam Control System");
  Serial.println("Setting up ESP32 as an Access Point");
 
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/", HTTP_GET, handlePage1);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/speed", HTTP_GET, handleSpeedChange);
 
  server.begin();
  Serial.println("Server started");
 
  pinMode(25, OUTPUT); // GPIO_SPEED_SIG
  pinMode(26, OUTPUT); // GPIO_SPEED_SEL_0
  pinMode(27, OUTPUT); // GPIO_SPEED_SEL_1
  pinMode(32, OUTPUT); // GPIO_SPEED_SEL_2
  pinMode(33, OUTPUT); // GPIO_SPEED_SEL_3
 
  pinMode(19,OUTPUT);
  pinMode(18, OUTPUT);
  initializeGPIO();
 
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);  FastLED.setBrightness(BRIGHTNESS);
}
 
void handlePage1(AsyncWebServerRequest *request) {
  String page = getPageHeader();
  page += "<div class='container'>";
 
  // MGR Central
  page += "<div class='section station'><h2>MGR Central</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String tracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(0, i, tracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations
  page += "<div class='section substations'><h2>Sub-Stations (MGR Central to Arakonam)</h2><div class='controls'>";
  for (int i = 0; i < 23; i++) {
    page += getSubstationButton(0, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Arakonam
  page += "<div class='section station'><h2>Arakonam</h2>";
  page += "<div class='controls'><h3>No tracks</h3>";
  page += "</div></div>";
 
  page += "</div>";  // End of container
  page += getPageFooter();
  request->send(200, "text/html", page);
}
 
String getPageHeader() {
  String header = "<html><head>";
  header += "<style>";
  header += "body { font-family: Arial; text-align: center; margin: 0; padding: 0; background-color: #f0f0f0; }";
  header += ".container { display: flex; flex-direction: column; width: 100%; height: 100vh; }";
  header += ".section { flex: 1; padding: 20px; }";
  header += ".station { background-color: #d0e0f0; }";
  header += ".substations { background-color: #f0d0d0; }";
  header += ".controls { display: flex; flex-direction: row; justify-content: center; flex-wrap: wrap; }";
  header += ".control-button { width: 80px; margin: 5px; padding: 10px; font-size: 14px; }";
  header += ".status { margin-left: 10px; }";
  header += ".speed-slider { width: 150px; margin: 5px; }";
  header += "</style>";
  header += "</head><body>";
  return header;
}
 
String getPageFooter() {
  String footer = "<script>";
  footer += "function togglePin(page, pin, type) {";
  footer += "  fetch('/toggle?page=' + page + '&pin=' + pin + '&type=' + type)";
  footer += "    .then(response => response.json())";
  footer += "    .then(data => {";
  footer += "      if (data.status) {";
  footer += "        document.getElementById('status_' + page + '_' + pin).innerText = data.status;";
  footer += "      }";
  footer += "    });";
  footer += "}";
  footer += "function updateSpeed(track, speed, trackNumber) {";
  footer += "  fetch('/speed?track=' + track + '&speed=' + speed + '&trackNumber=' + trackNumber)";
  footer += "    .then(response => response.json())";
  footer += "    .then(data => {";
  footer += "      if (data.speed) {";
  footer += "        document.getElementById('speed_' + track + '_' + trackNumber).innerText = 'Speed: Level ' + data.speed;";
  footer += "      }";
  footer += "    });";
  footer += "}";
  footer += "</script>";
  footer += "</body></html>";
  return footer;
}
 
String getTrackControls(int trackGroup, int trackIndex, String trackName) {
  String controls = "<button class='control-button' onclick=\"togglePin(" + String(trackGroup) + "," + String(trackIndex) + ",'track')\">";
  controls += trackName;
  controls += "</button>";
  controls += "<span class='status' id='status_" + String(trackGroup) + "_" + String(trackIndex) + "'>" + (trackStatus[trackGroup][trackIndex] ? "ON" : "OFF") + "</span>";
  controls += "<button class='control-button' onclick=\"togglePin(" + String(trackGroup) + "," + String(trackIndex) + ",'front')\">Front</button>";
  controls += "<button class='control-button' onclick=\"togglePin(" + String(trackGroup) + "," + String(trackIndex) + ",'back')\">Back</button>";
  controls += "<input type='range' min='1' max='6' value='" + String(trackSpeed[trackGroup][trackIndex]) + "' class='speed-slider' ";
  controls += "id='speedControl" + String(trackGroup) + "_" + String(trackIndex) + "' onchange=\"updateSpeed(" + String(trackGroup) + ", this.value, " + String(trackIndex) + ")\">";
  controls += "<span class='status' id='speed_" + String(trackGroup) + "_" + String(trackIndex) + "'>Speed: Level " + String(trackSpeed[trackGroup][trackIndex]) + "</span>";
  return controls;
}
 
String getSubstationButton(int page, int index, String name, bool status) {
  String statusText = subStationLedState[index] ? "BLINKING" : "OFF";
  String button = "<button class='control-button' onclick=\"togglePin(" + String(page) + "," + String(index) + ",'substation')\">";
  button += name;
  button += "</button><span class='status' id='status_" + String(page) + "_" + String(index) + "'>" + statusText + "</span>";
  return button;
}
 
void handleToggle(AsyncWebServerRequest *request) {
    if (request->hasParam("page") && request->hasParam("pin") && request->hasParam("type")) {
        int pageIndex = request->getParam("page")->value().toInt();
        int pinIndex = request->getParam("pin")->value().toInt();
        String type = request->getParam("type")->value();
        String stationName = (pageIndex == 0) ? "MGR Central" : "Arakonam";

        if (type == "substation") {
            toggleSubstation(pinIndex);
            String newStatus = subStationLedState[pinIndex] ? "BLINKING" : "OFF";
            int pcfIndex = (pinIndex < 16) ? pinIndex : (pinIndex - 16);
            String pcfType = (pinIndex < 16) ? "PCF_RUN" : "PCF_POWER";
            Serial.printf("Toggled %s index %d. New state: %s\n", pcfType.c_str(), pcfIndex, newStatus.c_str());
            request->send(200, "application/json", "{\"status\":\"" + newStatus + "\"}");
        } else if (type == "track" || type == "front" || type == "back") {
            String trackName = (pinIndex == 0) ? "UpTrack 1" : "DownTrack 1";
            if (type == "track") {
                trackStatus[pageIndex][pinIndex] = !trackStatus[pageIndex][pinIndex];
                String newStatus = trackStatus[pageIndex][pinIndex] ? "ON" : "OFF";
                int ledPin = (pinIndex == 0) ? LED_PIN_1 : LED_PIN_2;
                digitalWrite(ledPin, trackStatus[pageIndex][pinIndex] ? HIGH : LOW);
                Serial.printf("%s %s toggled. LED strip on pin %d is %s\n", stationName.c_str(), trackName.c_str(), ledPin, newStatus.c_str());
                request->send(200, "application/json", "{\"status\":\"" + newStatus + "\"}");
            }

            Serial.printf("%s %s %s activated.\n", stationName.c_str(), trackName.c_str(), type.c_str());
        }
    } else {
        request->send(400, "text/plain", "Bad Request");
    }
}

void handleSpeedChange(AsyncWebServerRequest *request) {
    if (request->hasParam("track") && request->hasParam("speed") && request->hasParam("trackNumber")) {
        int track = request->getParam("track")->value().toInt();
        int speed = request->getParam("speed")->value().toInt();
        int trackNumber = request->getParam("trackNumber")->value().toInt();
        String stationName = (track == 0) ? "MGR Central" : "Arakonam";
        String trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1";
        trackSpeed[track][trackNumber] = speed;
        Serial.printf("Speed for %s at Station %s changed to Level %d\n", trackName.c_str(), stationName.c_str(), speed);
        request->send(200, "application/json", "{\"speed\":\"" + String(speed) + "\"}");
    } else {
        request->send(400, "text/plain", "Bad Request");
    }
}
 
void toggleSubstation(int index) {
  subStationLedState[index] = !subStationLedState[index];
  if (subStationLedState[index]) {
    Serial.printf("Substation %s LED started blinking\n", subStationNames[index].c_str());
  } else {
    Serial.printf("Substation %s LED turned off\n", subStationNames[index].c_str());
    if (index < 16) {
      pcf_run.digitalWrite(index, LOW);
    } else if (index < 32) {
      pcf_power.digitalWrite(index - 16, LOW);
    }
  }
}
 
void initializeGPIO()
{
  gpio_run_status_init();
  gpio_power_status_init();
}

void loop() {
  unsigned long currentMillis = millis();
 
  for (int i = 0; i < 32; i++) {
    if (subStationLedState[i] == 1) {
      if (currentMillis - lastBlinkTime[i] >= blinkInterval) {
        lastBlinkTime[i] = currentMillis;
        subStationStatus[i] = !subStationStatus[i];
        if (i < 16) {
          pcf_run.digitalWrite(i, subStationStatus[i] ? HIGH : LOW);
        } else {
          pcf_power.digitalWrite(i - 16, subStationStatus[i] ? HIGH : LOW);
        }
      }
    }
  }
}
 