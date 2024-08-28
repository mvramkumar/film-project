#include <WiFi.h>
#include <PCF8575_library.h>
#include <ESPAsyncWebServer.h>
#include "control_board_power_status.h"
#include "control_board_run_status.h"
#include "control_board_speed.h"
 
const char* ssid = "ESP32_AP_1";
const char* password = "12345678";
AsyncWebServer server(80);
 
String subStationNames[47] = {
    // Group 1: "VPY" to "PLMG" (Index 0-30)
    "VPY", "PER", "PEW", "VLK", "PVM", "FRT", "SAN", "NTD",  // Index 0-7
    "AVD", "PAB", "TI", "SVR", "TRL", "KBT", "MAF", "MSU",   // Index 8-15
    "BBQ", "VJN", "PCW", "ICF", "KOTR", "ABU", "TMVL",       // Index 16-22
    "ANNR", "HC", "NEC", "VEU", "PTLR", "EGT", "SPAM", "TO", // Index 23-30
    "PLMG",                                                   // Index 31
 
    // Group 2: "MSC" to "CTN" (Index 32-41)
    "MSC", "NBK", "MBM", "GDY", "PZA", "TLM", "CMP", "TBM",  // Index 32-39
    "VDR", "GI" ,                                      // Index 40-41
 
    // Group 3: "TEN" to "NYY" (Index 42-45)
    "TEN", "SAP", "KCVL", "TVC", "NEM" // Index 42-45
};
 
 void gpio_speed_channel_sel(int channel)
{
  for (int i=0; i<GPIO_SPEED_SEL_PINS; i++)
  {
    digitalWrite(gpio_speed_sel_arr[i], (channel>>i)&1);
  }
  delay(5);
//  delay(10000);
}

int gpio_speed_sel_arr[GPIO_SPEED_SEL_PINS] = {GPIO_SPEED_SEL_0, GPIO_SPEED_SEL_1, GPIO_SPEED_SEL_2, GPIO_SPEED_SEL_3};
 
bool subStationStatus[46] = {false};
int subStationLedState[46] = {0};  // New array to track LED states (0: off, 1: blinking)
unsigned long lastBlinkTime[46] = {0};  // Array to track last blink time for each LED
const long blinkInterval = 500;  // Blink interval in milliseconds
 
void gpio_run_status_init()
 
{
  pcf_run.begin();
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_1, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_2, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_3, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_4, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_5, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_6, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_7, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_8, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_9, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_10, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_11, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_12, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_13, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_14, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_15, OUTPUT);
  pcf_run.pinMode(GPIO_RUN_STATUS_TRAIN_16, OUTPUT);  
 
}
 
void gpio_power_status_init()
 
{
  pcf_power.begin();
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_1, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_2, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_3, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_4, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_5, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_6, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_7, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_8, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_9, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_10, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_11, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_12, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_13, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_14, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_15, OUTPUT);
  pcf_power.pinMode(GPIO_POWER_STATUS_TRAIN_16, OUTPUT);
}

 
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Welcome to the project");
  Serial.println("Setting up ESP32 as an Access Point");
 
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/", HTTP_GET, handlePage1);
  server.on("/page2", HTTP_GET, handlePage2);
  server.on("/page3", HTTP_GET, handlePage3);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/speed", HTTP_GET, handleSpeedChange);
 
  server.begin();
  Serial.println("Server started");

  pinMode(25, OUTPUT); //GPIO_SPEED_SIG
  pinMode(26, OUTPUT); //GPIO_SPEED_SEL_0
  pinMode(27, OUTPUT); //GPIO_SPEED_SEL_1
  pinMode(32, OUTPUT); //GPIO_SPEED_SEL_2
  pinMode(33, OUTPUT); //GPIO_SPEED_SEL_3
 
  initializeGPIO();  // Initialize all assigned GPIO pins
}
 
void handlePage1(AsyncWebServerRequest *request) {
  String page = getPageHeader();
  page += "<div class='container'>";
 
  // Station 1: MGR Central
  page += "<div class='section station'><h2>Station 1: MGR Central</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String kanyakumariTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(0, i, kanyakumariTracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations for MGR Central
  page += "<div class='section substations'><h2>Sub-Stations (MGR Central)</h2><div class='controls'>";
  for (int i = 0; i < 32; i++) {
    page += getSubstationButton(1, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Station 2: Arakonam
  page += "<div class='section station'><h2>Station 2: Arakonam</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String trivandrumTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(1, i, trivandrumTracks[i]);
  }
  page += "</div></div>";
 
  page += "</div>";  // End of container
  page += getPageFooter();
  request->send(200, "text/html", page);
}
 
void handlePage2(AsyncWebServerRequest *request) {
  String page = getPageHeader();
  page += "<div class='container'>";
 
  // Station 1: Egmore
  page += "<div class='section station'><h2>Station 1: Egmore</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String egmoreTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(2, i, egmoreTracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations for Egmore
  page += "<div class='section substations'><h2>Sub-Stations (Egmore)</h2><div class='controls'>";
  for (int i = 32; i < 42; i++) {
    page += getSubstationButton(1, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Station 2: Chengalpattu
  page += "<div class='section station'><h2>Station 2: Chengalpattu</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String chengalpattuTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(3, i, chengalpattuTracks[i]);
  }
  page += "</div></div>";
 
  page += "</div>";  // End of container
  page += getPageFooter();
  request->send(200, "text/html", page);
}
 
 
void handlePage3(AsyncWebServerRequest *request) {
  String page = getPageHeader();
  page += "<div class='container'>";
 
  // Station 1: Kanyakumari
  page += "<div class='section station'><h2>Station 1: Kanyakumari</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String mgrTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(4, i, mgrTracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations
  page += "<div class='section substations'><h2>Sub-Stations</h2><div class='controls'>";
  for (int i = 42; i < 46; i++) {
    page += getSubstationButton(3, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Station 2: Arakkonam
  page += "<div class='section station'><h2>Station 2: Arakkonam</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  page += "<p>No up/down tracks available</p>";
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
  header += ".nav { position: fixed; bottom: 0; width: 100%; background-color: #ccc; padding: 10px; }";
  header += ".nav a { margin: 5px; padding: 10px; background-color: #aaa; color: white; text-decoration: none; }";
  header += "</style>";
  header += "</head><body>";
  return header;
}
 
String getPageFooter() {
  String footer = "<div class='nav'>";
  footer += "<a href='/'>Page 1</a>";
  footer += "<a href='/page2'>Page 2</a>";
  footer += "<a href='/page3'>Page 3</a>";
  footer += "</div>";
  footer += "<script>";
  footer += "function togglePin(page, pin, type) {";
  footer += "  fetch('/toggle?page=' + page + '&pin=' + pin + '&type=' + type).then(response => location.reload());";
  footer += "}";
  footer += "function updateSpeed(track, speed, trackNumber) {";
  footer += "  fetch('/speed?track=' + track + '&speed=' + speed + '&trackNumber=' + trackNumber).then(response => location.reload());";
  footer += "}";
  footer += "</script>";
  footer += "</body></html>";
  return footer;
}
 
String getTrackControls(int trackGroup, int trackIndex, String trackName) {
  String controls = "<button class='control-button' onclick=\"togglePin(" + String(trackGroup) + "," + String(trackIndex) + ",'track')\">";
  controls += trackName;
  controls += "</button>";
  controls += "<button class='control-button' onclick=\"togglePin(" + String(trackGroup) + "," + String(trackIndex) + ",'front')\">Front</button>";
  controls += "<button class='control-button' onclick=\"togglePin(" + String(trackGroup) + "," + String(trackIndex) + ",'back')\">Back</button>";
  controls += "<input type='range' min='1' max='3' value='2' class='speed-slider' ";
  controls += "id='speedControl" + String(trackGroup) + "_" + String(trackIndex) + "' onchange=\"updateSpeed(" + String(trackGroup) + ", this.value, " + String(trackIndex) + ")\">";
  return controls;
}
 
String getSubstationButton(int page, int index, String name, bool status) {
    String statusText = subStationLedState[index] ? "BLINKING" : "OFF";
    String button = "<button class='control-button' onclick=\"togglePin(" + String(page) + "," + String(index) + ",'substation')\">";
    button += name;
    button += "</button><span class='status'>" + statusText + "</span>";
    return button;
}
 
void handleToggle(AsyncWebServerRequest *request) {
  if (request->hasParam("page") && request->hasParam("pin") && request->hasParam("type")) {
    int pageIndex = request->getParam("page")->value().toInt();
    int pinIndex = request->getParam("pin")->value().toInt();
    String type = request->getParam("type")->value();
 
    if (type == "substation") {
            toggleSubstation(pinIndex);
        }
 
    String stationName;
    switch (pageIndex) {
      case 0: stationName = "MGR Central"; break;
      case 1: stationName = "Arakonam"; break;
      case 2: stationName = "Egmore"; break;
      case 3: stationName = "Chengalpattu"; break;
      case 4: stationName = "Kanyakumari"; break;
      default: stationName = "Unknown";
    }
 
    if (type == "track" || type == "front" || type == "back") {
      String trackName;
      trackName = (pinIndex == 0) ? "UpTrack 1" : "DownTrack 1";
      String action = (type == "track") ? "toggled" : "activated";
      Serial.printf("%s %s %s %s.\n", stationName.c_str(), trackName.c_str(), type.c_str(), action.c_str());
    } else if (type == "substation") {
      bool* statusArray;
      String* nameArray;
      int subStationIndex = pinIndex;  // Default to pinIndex
 
      if (pageIndex >= 3) {
        // This should refer to Thiruvananthapuram (subStationNames)
        statusArray = subStationStatus;
        nameArray = subStationNames;
      } else {
        // For stations like Arakonam and Jolarpettai
        statusArray = subStationStatus;
        nameArray = subStationNames;
       
        if (pageIndex == 2) {
          subStationIndex = pinIndex;  // Adjust index to fit Chengalpattu range
        } else if (pageIndex == 1) {
          // Jolarpettai uses pinIndex directly
          subStationIndex = pinIndex;  // Adjust to start from the beginning
        }
      }
 
      // Ensure the subStationIndex is within bounds
      if ((pageIndex < 3 && subStationIndex < 46) || (pageIndex >= 3 && subStationIndex < 46)) {
        String subStationName = nameArray[subStationIndex];
        statusArray[subStationIndex] = !statusArray[subStationIndex];
        String status = statusArray[subStationIndex] ? "ON" : "OFF";
        Serial.printf("Sub-station %s (%s) toggled. New status: %s\n", subStationName.c_str(), stationName.c_str(), status.c_str());
      } else {
        Serial.printf("Invalid subStationIndex %d for pageIndex %d.\n", subStationIndex, pageIndex);
      }
    }
  }
  request->send(204);
}
 
 
void handleSpeedChange(AsyncWebServerRequest *request) {
  if (request->hasParam("track") && request->hasParam("speed") && request->hasParam("trackNumber")) {
    int track = request->getParam("track")->value().toInt();
    int speed = request->getParam("speed")->value().toInt();
    int trackNumber = request->getParam("trackNumber")->value().toInt();
   
    String stationName;
    String trackName;
    switch (track) {
        case 0: stationName = "MGR Central"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 1: stationName = "Arakonam"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 2: stationName = "Egmore"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 3: stationName = "Chengalpattu"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 4: stationName = "Kanyakumari"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      default: stationName = "Unknown"; trackName = "Unknown";
    }
   
    Serial.printf("Speed for %s at Station %s changed to %d\n", trackName.c_str(), stationName.c_str(), speed);
  }
  request->send(204);
}
 
void toggleSubstation(int index) {
  if (index < 16) {
    // First 16 pins: Run status
    uint8_t currentState = pcf_run.digitalRead(index);
    pcf_run.digitalWrite(index, !currentState);
    Serial.printf("Toggled Run status for substation %d using PCF_RUN. New state: %s\n", 
                  index, !currentState ? "HIGH" : "LOW");
  } else if (index < 32) {
    // Next 16 pins: Power status
    uint8_t currentState = pcf_power.digitalRead(index - 16);
    pcf_power.digitalWrite(index - 16, !currentState);
    Serial.printf("Toggled Power status for substation %d using PCF_POWER. New state: %s\n", 
                  index, !currentState ? "HIGH" : "LOW");
  } else {
    gpio_speed_channel_sel(index - 32);
    uint8_t currentState = digitalRead(GPIO_SPEED_SIG);
    Serial.printf("Toggled power status for substation %d using CD74. New state: %s\n", index, !currentState ? "HIGH":"LOW");
    return;
  }
 
  subStationLedState[index] = !subStationLedState[index];
  if (subStationLedState[index]) {
    Serial.printf("Substation %s LED started blinking\n", subStationNames[index].c_str());
  } else {
    Serial.printf("Substation %s LED turned off\n", subStationNames[index].c_str());
  }
}

void initializeGPIO()
{
  gpio_run_status_init();
  gpio_power_status_init();
  // The remaining pins (33-46) are left unassigned for now
}
 
// Replace your existing loop() function with this:
void loop() {
  unsigned long currentMillis = millis();
  gpio_run_status_init();
  gpio_power_status_init();
 
  for (int i = 0; i < 46; i++) {
    if (subStationLedState[i] == 1) {
      if (currentMillis - lastBlinkTime[i] >= blinkInterval) {
        lastBlinkTime[i] = currentMillis;
        subStationStatus[i] = !subStationStatus[i];
        if (i < 16) {
          pcf_run.digitalWrite(i, subStationStatus[i] ? HIGH : LOW);
        } else if (i < 32) {
          pcf_power.digitalWrite(i - 16, subStationStatus[i] ? HIGH : LOW);
        }  else {
          digitalWrite(i- 32, subStationStatus[i] ? HIGH : LOW);
        }
      }
    }
  }
  // Add any additional loop logic here
}