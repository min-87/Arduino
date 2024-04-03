#include "Arduino.h"
#include "Wav.h"
#include "I2S.h"
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ESPAsyncWebServer.h"
#include "esp_task_wdt.h"
#include <SD.h>

#define TIME_FOR_CONNECTING 20000
#define I2S_MODE I2S_MODE_ADC_BUILT_IN

const char* ESP_SSID = "Wall-e Access Point";
const char* ESP_SSID_PASS = "12345678";

const char* WIFI_SSID;
const char* WIFI_PASS;

const char* serverName = "http://www.google.com/speech-api/v2/recognize?client=chromium&lang=ru-RU&key=AIzaSyBOti4mM-6x9WDnZIjIeyEU21OpBXqWBgw";

AsyncWebServer server(80);
HTTPClient http;

const int record_time = 3;  // second
const int headerSize = 44;
const int waveDataSize = record_time * 32000;
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData / 4;

byte header[headerSize];
char communicationData[numCommunicationData];
char partWavData[numPartWavData];

void setup() {
  WiFi.mode(WIFI_MODE_APSTA);
  Serial.begin(115200);
  Serial2.begin(9600);
  if (!SD.begin()) {
    Serial.println("Error, while init SD card!");
  }
  connectToWiFiWithSavedSettings();
  createAccessPoint();
  setupServer();

  SD.remove("/data.wav");

  I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_32BIT);

  http.begin(serverName);
}

long lastCheckTime = 0;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Plese, speak");
    Serial2.print("S");
    record();
    Serial.println("We try ro recognize speech");
    Serial2.print("R");
    String text = recognize();
    JSONVar obj;
    Serial.println(text);
    text = getValue(text, '\n', 1);
    obj = JSON.parse(text);
    Serial.println(text);
    text = obj["result"][0]["alternative"][0]["transcript"];
    text.toLowerCase();
    Serial.println(text);

    if ((text.indexOf("включи") >= 0 || text.indexOf("Включи") >= 0) && (text.indexOf("свет") >= 0 || text.indexOf("Свет") >= 0)) {
      Serial.println("L");
      Serial2.print("L");
    } else if ((text.indexOf("выключи") >= 0 || text.indexOf("Выключи") >= 0) && (text.indexOf("свет") >= 0 || text.indexOf("Свет") >= 0)) {
      Serial.println("Q");
      Serial2.print("Q");
    } else if ((text.indexOf("Температур") >= 0 || text.indexOf("температур") >= 0)) {
      Serial2.print("T");
    } else if ((text.indexOf("Вперёд") >= 0 || text.indexOf("вперёд") >= 0)) {
      Serial2.print("F");
    } else if ((text.indexOf("Назад") >= 0 || text.indexOf("назад") >= 0)) {
      Serial2.print("B");
    } else if ((text.indexOf("Вправо") >= 0 || text.indexOf("вправо") >= 0)) {
      Serial2.print("r");
    } else if ((text.indexOf("Влево") >= 0 || text.indexOf("влево") >= 0)) {
      Serial2.print("l");
    } else if ((text.indexOf("Сила") >= 0 || text.indexOf("сила") >= 0) && (text.indexOf("robocode") >= 0 || text.indexOf("Robocode") >= 0)) {
      Serial2.print("f");
    } else if ((text.indexOf("Устал") >= 0 || text.indexOf("устал") >= 0)) {
      Serial2.print("t");
    } else if ((text.indexOf("Как") >= 0 || text.indexOf("как") >= 0) && (text.indexOf("Тебя") >= 0 || text.indexOf("тебя") >= 0) && (text.indexOf("Зовут") >= 0 || text.indexOf("зовут") >= 0)) {
      Serial2.print("n");
    } else if ((text.indexOf("Ты") >= 0 || text.indexOf("ты") >= 0) && (text.indexOf("Откуда") >= 0 || text.indexOf("откуда") >= 0)) {
      Serial2.print("w");
    }
  } else {
    if (millis() > lastCheckTime + 5000) {
      String str = "I" + WiFi.softAPIP().toString() + "P";
      Serial.println(str);
      Serial2.print(str);
      lastCheckTime = millis();
    }
  }
}

String recognize() {
  File f = SD.open("/data.wav", FILE_READ);
  http.addHeader("Content-Type", "audio/l16; rate=16000");

  int httpResponseCode = http.sendRequest("POST", &f, f.size());

  String resp = "";
  if (httpResponseCode > 0) {
    resp = http.getString();
  }

  f.close();
  return resp;
}

void record() {
  File f = SD.open("/data.wav", FILE_WRITE);
  CreateWavHeader(header, waveDataSize);
  f.write(header, headerSize);
  for (uint32_t j = 0; j < waveDataSize / numPartWavData; ++j) {
    I2S_Read(communicationData, numCommunicationData);
    for (uint32_t i = 0; i < numCommunicationData / 8; ++i) {
      partWavData[2 * i] = communicationData[8 * i + 2];
      partWavData[2 * i + 1] = communicationData[8 * i + 3];
    }
    f.write((const byte*)partWavData, numPartWavData);
  }
  f.close();
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String processor(const String& var) {
  return String("Hello");
}

bool connectToWiFiWithSavedSettings() {
  File configs = SD.open("/config.txt");
  if (configs) {
    String configJsonStr = "";
    while (configs.available()) {
      configJsonStr += (char)configs.read();
    }

    configs.close();

    JSONVar configJson = JSON.parse(configJsonStr);

    WIFI_SSID = configJson["ssid"];
    WIFI_PASS = configJson["pass"];

    Serial.print(WIFI_SSID);
    Serial.print(" ");
    Serial.println(WIFI_PASS);

    if (strlen(WIFI_SSID) > 0) {
      connectToWiFi();
    }
  }
}

bool connectToWiFi() {
  //WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  long startConnectTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    esp_task_wdt_reset();
    delay(1000);
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    if (millis() > startConnectTime + TIME_FOR_CONNECTING) {
      return false;
    }
  }

  Serial.println(WiFi.localIP());

  return true;
}

void setupServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/script.js", "text/javascript");
  });

  // Route to set GPIO to HIGH
  server.on("/wifiCred",
            HTTP_POST,
  [](AsyncWebServerRequest * request) {},
  NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    String jsonStr = "";
    for (size_t i = 0; i < len; i++) {
      jsonStr += (char)data[i];
    }

    Serial.println(jsonStr);

    JSONVar wifiSettingsJson = JSON.parse(jsonStr);

    WIFI_SSID = wifiSettingsJson["ssid"];
    WIFI_PASS = wifiSettingsJson["pass"];

    Serial.print(WIFI_SSID);
    Serial.print(" : ");
    Serial.println(WIFI_PASS);

    File config = SD.open("/config.txt", "w");
    if (config.print(jsonStr)) {
      request->send(200, "text/plain", "Network credentials saved");
    } else {
      request->send(200, "text/plain", "Something went wrong");
    }

    config.close();
    connectToWiFi();
  });

  server.on("/searchNetworks", HTTP_POST, [](AsyncWebServerRequest * request) {
    String networks = searchNetworks();
    request->send(200, "text/plain", networks);
  });

  server.begin();
}

void createAccessPoint() {
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ESP_SSID, ESP_SSID_PASS);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

String searchNetworks() {
  String networks = "";

  int n = WiFi.scanNetworks();
  if (n == 0) {
    networks = "{\"error\":\"No networks found\"}";
    return networks;
  }
  networks = "{\"networks\":[";
  for (int i = 0; i < n; ++i) {
    networks += "{\"ssid\":\"" + (String)WiFi.SSID(i) + "\",";
    networks += "\"rssi\":\"" + (String)WiFi.RSSI(i) + "\",";
    String temp = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "false" : "true";
    networks += "\"password\":\"" + temp + "\"}";
    if (i == n - 1) {
      networks += "]}";
    } else {
      networks += ",";
    }
  }

  Serial.println(networks);

  return networks;
}
