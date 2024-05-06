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
    led_matrix.textFont(Font_4x6);

    while (status != WL_CONNECTED) {
        status = WiFi.begin(ssid, pass);
        delay(10000);
    }
    server.begin();       
    Wire.begin();
    Wire.setClock(WIRE_CLOCK);

    temp_humidity_sensor.begin();                   
    print_server_status();         
    matrix_show_ip_address();               
}


void loop() {
    unsigned long ms_since_last_read = millis() - last_read_time_ms;
    if (last_read_time_ms == 0 || ms_since_last_read > REFRESH_READINGS_MS) {
        read_temperature_humidity(temperature, humidity);
        last_read_time_ms = millis();
    }

    WiFiClient client = server.available();
    if (client) {
        Serial.print("Connected to client!");
        String request_line = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.print(c);
                if (c == '\n') {
                    if (request_line.length() == 0) {
                        respond_to_client(client);
                        break;
                    } else {
                        Serial.flush();
                        request_line = "";
                    }
                } else if (c != '\r') {
                    request_line += c;
                }
            }
        }
        client.stop();
    }
}

void print_server_status() {
    Serial.print("Server can be reached on WiFi ");
    Serial.print(WiFi.SSID());

    IPAddress const ip = WiFi.localIP();
    Serial.print(" over HTTP at IP ");
    Serial.println(ip);
}

void matrix_show_ip_address() {
    const IPAddress ip = WiFi.localIP();
    char last_octet_str[16] = {};
    sprintf(last_octet_str, "%d", ip[3]);
    matrix_print(last_octet_str);
}

void matrix_print(const char line[]) {
    led_matrix.beginText(0, 1, 0xFFFFFF);
    led_matrix.println(line);
    led_matrix.endText();
}

void read_temperature_humidity(float& temperature, float& humidity) {
    temp_humidity_sensor.read(false);
    temperature = temp_humidity_sensor.getTemperature();
    humidity = temp_humidity_sensor.getHumidity();
}

void respond_to_client(WiFiClient& client) {
    for (int i = 0; RESPONSE_TEMPLATE[i] != '\0'; i++) {
        if (RESPONSE_TEMPLATE[i] == '$') {
            switch (RESPONSE_TEMPLATE[i + 1]) {
                case 'T':
                    client.print(temperature);
                    break;
                case 'H':
                    client.print(humidity);
                    break;
            }
            i++;
        } else {
            client.print(RESPONSE_TEMPLATE[i]);
        }
    }
}