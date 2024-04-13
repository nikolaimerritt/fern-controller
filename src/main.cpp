#include "main.h"

ArduinoLEDMatrix led_matrix;
SHT31 temp_humidity_sensor;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS; 
int keyIndex = 0;

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

float temperature, humidity;
unsigned long last_read_time_ms = 0;

void setup() {
  Serial.begin(9600);   
  led_matrix.begin();

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
    server.begin();       
    Wire.begin();
    Wire.setClock(WIRE_CLOCK);

  temp_humidity_sensor.begin();                   
  print_wifi_status();                        
}


void loop() {
    unsigned long ms_since_arduino_started = millis();
    unsigned long ms_since_last_read = ms_since_arduino_started - last_read_time_ms;
    if (last_read_time_ms == 0 || ms_since_last_read > 5000) {
        read_temperature_humidity(temperature, humidity);
        last_read_time_ms = millis();
    }

    WiFiClient client = server.available();
    if (client) {
        String requestLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.print(c);
                if (c == '\n') {
                    if (requestLine.length() == 0) {
                        respond_to_client(client);
                        break;
                    } else {
                        Serial.flush();
                        requestLine = "";
                    }
                } else if (c != '\r') {
                    requestLine += c;
                }
            }
        }
        client.stop();
    }
}

void print_wifi_status() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void read_temperature_humidity(float& temperature, float& humidity) {
    temp_humidity_sensor.read(false);
    temperature = temp_humidity_sensor.getTemperature();
    humidity = temp_humidity_sensor.getHumidity();
}

void respond_to_client(WiFiClient& client) {
    for (int i = 0; RESPONSE_TEMPLATE[i] != '\0'; i++) {
        if (RESPONSE_TEMPLATE[i] == '$') {
            if (RESPONSE_TEMPLATE[i + 1] == 'T') {
                client.print(temperature);
            } else if (RESPONSE_TEMPLATE[i + 1] == 'H') {
                client.print(humidity);
            }
            i++;
        } else {
            client.print(RESPONSE_TEMPLATE[i]);
        }
    }
}