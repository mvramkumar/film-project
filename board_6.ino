#include <WiFi.h>
#include <ESPAsyncWebServer.h>
 
const char* ssid = "ESP32_AP_6";
const char* password = "12345678";
AsyncWebServer server(80);
 
String subStationNames[44] = {
    // Group 1: "VNJ" to "PLMD" (Index 0-5)
    "VNJ", "SNO", "VO", "IGU", "SHI", "PLMD",
   
    // Group 2: "TD" to "TAY" (Index 6-16)
    "TD", "KGD", "TY", "CVP", "MEJ",
    "TMQ", "TPV", "SRT", "KBT", "GDN", "TAY",
   
    // Group 3: "PPNS" to "ULL" (Index 17-24)
    "BPJ","MAU","ME","MAHE","JGE","TLY","BMD","ETK",
   
    // Group 4: "PPNS" to "ULL" (Index 25-43)
    "PPNS", "PAZ", "PAY", "CDRA", "NLE", "BFR", "KLAD", "KMQ", "MJS", "CS", "KPQ", "ELN", "TKQ", "CHV", "KZE", "KOK", "KGQ", "UAA", "ULL"
};
 
bool subStationStatus[44] = {false};
 
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
  server.on("/page4", HTTP_GET, handlePage4);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/speed", HTTP_GET, handleSpeedChange);
 
  server.begin();
  Serial.println("Server started");
}
 
void handlePage1(AsyncWebServerRequest *request) {
  String page = getPageHeader();
  page += "<div class='container'>";
 
  // Station 1: Tiruppur
  page += "<div class='section station'><h2>Station 1: Tiruppur</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String kanyakumariTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(0, i, kanyakumariTracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations for  Tiruppur
  page += "<div class='section substations'><h2>Sub-Stations (Tiruppur)</h2><div class='controls'>";
  for (int i = 0; i < 6; i++) {
    page += getSubstationButton(1, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Station 2: Coimbatore
  page += "<div class='section station'><h2>Station 2: Coimbatore</h2>";
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
 
  // Station 1: Madurai
  page += "<div class='section station'><h2>Station 1:  Madurai</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String egmoreTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(2, i, egmoreTracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations for Madurai
  page += "<div class='section substations'><h2>Sub-Stations (Madurai)</h2><div class='controls'>";
  for (int i = 6; i < 17; i++) {
    page += getSubstationButton(2, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Station 2: Tirunelveli
  page += "<div class='section station'><h2>Station 2:  Tirunelveli</h2>";
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
 
  // Station 1: palakad
  page += "<div class='section station'><h2>Station 1: Palakad</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String mgrTracks[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(4, i, mgrTracks[i]);
  }
  page += "</div></div>";
 
  // Sub-Stations
  page += "<div class='section substations'><h2>Sub-Stations (Palakad) </h2><div class='controls'>";
  for (int i = 17; i < 25; i++) {
    page += getSubstationButton(3, i, subStationNames[i], subStationStatus[i]);
  }
  page += "</div></div>";
 
  // Station 2: kozhikkode
  page += "<div class='section station'><h2>Station 2: Kozhikkode</h2>";
  page += "<div class='controls'><h3>Track Controls</h3>";
  String mgrTracks1[] = {"UpTrack 1", "DownTrack 1"};
  for (int i = 0; i < 2; i++) {
    page += getTrackControls(4, i, mgrTracks1[i]);
  }  
  page += "</div></div>";
 
  page += "</div>";  // End of container
  page += getPageFooter();
  request->send(200, "text/html", page);
}
 
void handlePage4(AsyncWebServerRequest *request) {
    String page = getPageHeader();
    page += "<div class='container'>";
 
    // Station 1: Kozhikode
    page += "<div class='section station'><h2>Station 1: Kozhikode</h2>";
    page += "<div class='controls'><h3>Track Controls</h3>";
    String kozhikodeTracks[] = {"UpTrack 1", "DownTrack 1"};
    for (int i = 0; i < 2; i++) {
        page += getTrackControls(5, i, kozhikodeTracks[i]);
    }
    page += "</div></div>";
 
    // Sub-Stations for Kozhikode
    page += "<div class='section substations'><h2>Sub-Stations (Kozhikode)</h2><div class='controls'>";
    for (int i = 25; i < 44; i++) {
        page += getSubstationButton(4, i, subStationNames[i], subStationStatus[i]);
    }
    page += "</div></div>";
 
    // Station 2: Mangaluru
    page += "<div class='section station'><h2>Station 2: Mangaluru</h2>";
    page += "<div class='controls'><h3>Track Controls</h3>";
    String mangaluruTracks[] = {"UpTrack 1", "DownTrack 1"};
    for (int i = 0; i < 2; i++) {
        page += getTrackControls(6, i, mangaluruTracks[i]);
    }
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
  footer += "<a href='/page4'>Page 4</a>";
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
            case 0: stationName = "Tiruppur"; break;
            case 1: stationName = "Coimbatore"; break;
            case 2: stationName = "Madurai"; break;
            case 3: stationName = "Tirunelveli"; break;
            case 4: stationName = "Palakkad"; break;
            case 5: stationName = "Kozhikode"; break;
            case 6: stationName = "Mangaluru"; break;
            default: stationName = "Unknown"; break;
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
 
      if (pageIndex >= 4) {
        // This should refer to Kozhikode-Mangaluru (subStationNames)
        statusArray = subStationStatus;
        nameArray = subStationNames;
        subStationIndex = pinIndex;
      } else {
        // For other stations
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
      if (subStationIndex < 44) {String subStationName = nameArray[subStationIndex];
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
      case 0: stationName = "Tiruppur"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 1: stationName = "Coimbatore"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 2: stationName = "Madurai"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 3: stationName = "Tirunelveli"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 4: stationName = "Palakkad"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 5: stationName = "Kozhikode"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      case 6: stationName = "Mangaluru"; trackName = (trackNumber == 0) ? "UpTrack 1" : "DownTrack 1"; break;
      default: stationName = "Unknown"; trackName = "Unknown";
    }
   
    Serial.printf("Speed for %s at Station %s changed to %d\n", trackName.c_str(), stationName.c_str(), speed);
  }
  request->send(204);
}
 
void loop() {
  //Serial.println(ESP.getFreeHeap());  // Print free heap memory for debugging
}