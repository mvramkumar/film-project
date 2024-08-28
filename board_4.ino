#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ESP32_AP_4";
const char* password = "12345678";
AsyncWebServer server(80);

String subStationNames[38] = {
    "NAP", "VRPD", "DC", "MVPM", "SGE", "ANU", "CV",      // Index 0-6
    "VRT", "PNDM", "ICGH", "SNDI", "OTK", "KLIH", "KKPM", "KITR", "VLDE", "VKV", "SRGM", "TLNR", "ICG", "MDUR", "VER", "ALU", "KLGM", "PMP", "LLI", "BXS", "GOC",  // Index 7-27
    "KKM", "ALLP", "TMPY", "MAKM", "SRTL", "VAY", "TUVR", "ZEZB", "KUM", "ERS" //28-37
};

bool subStationStatus[38] = {false};


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
}

void handlePage1(AsyncWebServerRequest *request) {
  String page = getPageHeader();
  page += "<div class='container'>";

  // Station 1: Salem
  page += "<div class='section station'><h2>Station 1: Salem</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String kanyakumariTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(0, i, kanyakumariTracks[i]);
  }
  page += "</div></div>";

  // Sub-Stations for  Salem
  page += "<div class='section substations'><h2>Sub-Stations (Salem)</h2><div class='controls'>";
  for (int i = 0; i < 7; i++) {
    page += getSubstationButton(1, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";

  // Station 2: Erode
  page += "<div class='section station'><h2>Station 2: Erode</h2>";
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

  // Debugging
  // Serial.println("Loading Station 1: Chengalpattu");

  // Station 1: Vriddhachalam
  page += "<div class='section station'><h2>Station 1:  Vriddhachalam</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String egmoreTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(2, i, egmoreTracks[i]);
  }
  page += "</div></div>";

  // Debugging
  // Serial.println("Adding Sub-Stations for Chengalpattu");

  // Sub-Stations for Villupuram
  page += "<div class='section substations'><h2>Sub-Stations (Vriddhachalam)</h2><div class='controls'>";
  for (int i = 7; i < 27; i++) {
    page += getSubstationButton(2, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";

  // Station 2: Trichirappali
  page += "<div class='section station'><h2>Station 2: Trichirappali</h2>";
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

  // Station 1: Ernakulam
  page += "<div class='section station'><h2>Station 1: Ernakulam</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String mgrTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(4, i, mgrTracks[i]);
  }
  page += "</div></div>";

  // Sub-Stations
  page += "<div class='section substations'><h2>Sub-Stations (Thiruvananthapuram) </h2><div class='controls'>";
  for (int i = 28; i < 37; i++) {
    page += getSubstationButton(3, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";

  // Station 2:Palakad
  page += "<div class='section station'><h2>Station 2: Palakad</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String mgrTracks1[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(4, i, mgrTracks1[i]);
  }  page += "</div></div>";

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
  String statusText = status ? "ON" : "OFF";
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

    String stationName;
    switch (pageIndex) {
      case 0: stationName = "Salem"; break;
      case 1: stationName = "Erode"; break;
      case 2: stationName = "Vriddhachalam"; break;
      case 3: stationName = "Trichirapalli"; break;
      case 4: stationName = "Ernakulam"; break;
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
      if ((pageIndex < 3 && subStationIndex < 38) || (pageIndex >= 3 && subStationIndex < 38)) {
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
      case 0: stationName = "Salem"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 1: stationName = "Erode"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 2: stationName = "Vriddhachalam"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 3: stationName = "Trichirapalli"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 4: stationName = "Ernakulam"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      default: stationName = "Unknown"; trackName = "Unknown";
    }
    
    Serial.printf("Speed for %s at Station %s changed to %d\n", trackName.c_str(), stationName.c_str(), speed);
  }
  request->send(204);
}

void loop() {
}