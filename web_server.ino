/*

  ESP32 Web Server

  Simple server sketch to use as a template for projects or testing

  See README.md for more info!

*/

// The libraries are needed for WiFi and creating the server.
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"

// Place your WiFi credentials in this file.
// If using Git to save your work, make sure this file is added to your .gitignore file!
#include "secrets.h"

/*

  The html the Esp serves is in the form of a string literal.
  It can be a little painful to edit, especially for a complex UI.
  However, it is a little easier to work with than uploading html files
  directly to the Esp.
  To upload files directly, please read more on Esp32 filesystems.

  You can edit this file directly to add UI functions.
  What's a little easier is to create a sepearte html file,
  create your UI in this html file,
  then copy the html into index.h,
  making sure the html you paste is inbetween 
  'const char index_html[] PROGMEM = R"ESPHTML('  
  and 
  ')ESPHTML"';

  For example:
  
  const char index_html[] PROGMEM = R"ESPHTML(

  <!DOCTYPE HTML>
  <html>
  <head>
  R"ESPHTML(
        <title>Example HTML</title>
  </head>
  <body>
    <h1>Hello World!</h1>
  </body>
  </html>

  )ESPHTML";

  With this way, you can see your html by opening the html file with your browser
  before uploading it to the Esp.

  Please note, you will need to upload the html in order to test any interactivity with the Esp.
*/
#include "index.h"

// Create a server instance.
AsyncWebServer server(80);

// We can add event handling to the server.
AsyncEventSource events("/events");

// We can create functions to handle HTTP requests.

// This function handles 404 errors.
static void not_found_handler(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "PAGE NOT FOUND");
}

// This function is an example for handling GET requests.
// This can be expanded for more complexe GEt requetss.
static void get_handler(AsyncWebServerRequest *request) {
  Serial.println("/GET esp");
  request->send_P(200, "application/json", "{\"msg\": \"success\"}");
}

// We can also handle form submissions.
// Form data must be sent as URl params in a POST request.
static void form_handler(AsyncWebServerRequest *request) {
  Serial.println("/POST esp-form");

  AsyncWebParameter *p1 = request->getParam(0);
  const char *text = p1->value().c_str();

  AsyncWebParameter *p2 = request->getParam(1);
  const char *value = p2->value().c_str();

  Serial.println("Params:");
  Serial.print(text);
  Serial.print("\t");
  Serial.println(value);

  request->send_P(200, "application/json", "{\"msg\": \"success\"}");
}

// In setup, we connect to WiFi and create our server.
void setup() {

  Serial.begin(115200);
  Serial.println("ESP32 Web Server");

  Serial.println("Connecting to WiFi");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

  Serial.println("Setting WiFi mode");
  WiFi.mode(WIFI_STA);

  Serial.println("WiFi begin");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("WiFi connceting...");

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }

  Serial.println("Esp Connected to WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // The index file is our homepage when a client first connects to the Esp.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Here are our HTTP request handlers.
  server.on("/esp-form", HTTP_POST, form_handler);
  server.on("/esp", HTTP_GET, get_handler);
  server.onNotFound(not_found_handler);

  // Add a events handler for when we want the Esp to send data to our webpage.
  server.addHandler(&events);

  // Start the server.
  server.begin();
}

void loop() {
  // There is no need for a loop when only serving static content.
  // However, we can definitely add some code here to
  // read sensors, interact with hardware or update our webpage with data.
}
