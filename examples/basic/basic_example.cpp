#include "ESP-Config.h"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>

// Server stuff
const char *   ssid     = ""; // Enter SSID here
const char *   password = ""; // Enter Password here
AsyncWebServer server(80);

// Config stuff
ConfigPage page("page", "Page", "This is an example page.");

ConfigSection section(page, "section", "Section A", "This is an example section: A.");
ConfigString  s(section, "string", "String", "This is a string example", "teststring");
ConfigBool    bt(section, "bool_true", "Boolean", "This is a bool example", true);
ConfigBool    bf(section, "bool_false", "Boolean", "This is a 2. bool example", false);
ConfigInt     i(section, "int", "Int", "This is a int example", 0);
ConfigDecimal d(section, "decimal", "Decimal", "This is a decimal example", 1234.5678, 6);

ConfigSection section_hello(page, "hello_world", "Hello World");
ConfigString  hello(section_hello, "hello", "Hello", "This is a hello world!", "world");

ConfigHTML    html;
ConfigFactory fact;

void setup() {
  Serial.begin(115200);

  delay(500);
  SPIFFS.begin(true);

  // finish config
  html.addPage(&page);
  fact.addPage(&page);

  // wifi config
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  html.registerWebServer(server, "/");
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });

  server.on("/test.json", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/test.json", "text/json");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  if (html.wasSaved()) {
    fact.saveConfig("/test.json");
  }
}
