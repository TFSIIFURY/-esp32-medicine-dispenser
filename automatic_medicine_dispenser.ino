#include <WiFi.h> // WiFi library
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //LCD library
#include <WebServer.h> 
#include <time.h> // library for getting real time
#include <ESP32Servo.h>  // library for servo

#define BUZZER 25
#define IR_SENSOR 34

int medicine1Count = 5; // number of medicine 1
int medicine2Count = 5; // number of medicine 2

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // initialising the lcd

// Wi-Fi Credentials
const char* ssid = "ASHOK HOUSE"; // wifi name 
const char* password = "Ankush#10"; // wifi password

// NTP Config
const char* ntp_server = "pool.ntp.org"; 
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

// Web Server
WebServer server(80);

// Target times
int med1Hour = -1, med1Min = -1;
int med2Hour = -1, med2Min = -1;

// Servos
Servo servo1;  // medicine 1
Servo servo2;  // medicine 2

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(IR_SENSOR, INPUT);

  Wire.begin(21, 22);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  // WiFi Setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  // NTP time config
  configTime(gmtOffset_sec, daylightOffset_sec, ntp_server);

  // Attach servos to GPIO
  servo1.attach(18);
  servo2.attach(19);

  // Web page handlers
  server.on("/", handleRoot);
  server.on("/schedule", handleSchedule);
  server.begin();
  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
  checkAndDispense();
}

// Web Page
void handleRoot() {
  String html = "<html><body><h2>Set Schedule</h2>";
  html += "<form action='/schedule' method='GET'>";
  html += "<h3>Medicine 1:</h3>";
  html += "Hour (1-12):<input type='number' name='h1' min='1' max='12'><br>";
  html += "Minute (0-59):<input type='number' name='m1' min='0' max='59'><br>";
  html += "AM/PM:<select name='p1'><option value='AM'>AM</option><option value='PM'>PM</option></select><br><br>";
  html += "<h3>Medicine 2:</h3>";
  html += "Hour (1-12):<input type='number' name='h2' min='1' max='12'><br>";
  html += "Minute (0-59):<input type='number' name='m2' min='0' max='59'><br>";
  html += "AM/PM:<select name='p2'><option value='AM'>AM</option><option value='PM'>PM</option></select><br><br>";
  html += "<input type='submit' value='Set Schedule'>";
  html += "</form></body></html>";

  server.send(200, "text/html", html);
}

void handleSchedule() {
  int h1 = server.arg("h1").toInt();
  int m1 = server.arg("m1").toInt();
  String p1 = server.arg("p1");

  int h2 = server.arg("h2").toInt();
  int m2 = server.arg("m2").toInt();
  String p2 = server.arg("p2");

  if (p1 == "PM" && h1 != 12) h1 += 12;
  if (p1 == "AM" && h1 == 12) h1 = 0;

  if (p2 == "PM" && h2 != 12) h2 += 12;
  if (p2 == "AM" && h2 == 12) h2 = 0;

  med1Hour = h1;
  med1Min = m1;
  med2Hour = h2;
  med2Min = m2;

  String response = "<html><body><h3>Schedule Set!</h3>";
  response += "<p>Medicine 1: " + String(h1) + ":" + (m1 < 10 ? "0" : "") + String(m1) + "</p>";
  response += "<p>Medicine 2: " + String(h2) + ":" + (m2 < 10 ? "0" : "") + String(m2) + "</p>";
  response += "<a href='/'>Back</a></body></html>";
  server.send(200, "text/html", response);
}

void checkAndDispense() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int currentHour = timeinfo.tm_hour;
  int currentMin = timeinfo.tm_min;
  static int lastMin = -1;

  if (currentMin != lastMin) {
    lastMin = currentMin;

    if (currentHour == med1Hour && currentMin == med1Min) {
      dispense_medicine1();
      check_medicine_taken(15000);
    }

    if (currentHour == med2Hour && currentMin == med2Min) {
      dispense_medicine2();
      check_medicine_taken(15000);
    }
  }
}

void dispense_medicine1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Medicine 1...");
  digitalWrite(BUZZER, HIGH);
  delay(500);
  servo1.writeMicroseconds(1710);  // rotate CW
  delay(9900);                     // 1 full turn (tune if needed)
  servo1.writeMicroseconds(1500); // stop
  digitalWrite(BUZZER, LOW);

  if (medicine1Count > 0) medicine1Count--;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Med 1 dispensed");
  lcd.setCursor(0, 1);
  lcd.print("Left: ");
  lcd.print(medicine1Count);
  delay(2000);
}

void dispense_medicine2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Medicine 2...");
  digitalWrite(BUZZER, HIGH);
  delay(500);
  servo2.writeMicroseconds(1300); // rotate CCW
  delay(9200);                    // 1 full turn
  servo2.writeMicroseconds(1500); // stop
  digitalWrite(BUZZER, LOW);

  if (medicine2Count > 0) medicine2Count--;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Med 2 dispensed");
  lcd.setCursor(0, 1);
  lcd.print("Left: ");
  lcd.print(medicine2Count);
  delay(2000);
}

bool check_medicine_taken(unsigned long timeoutMs) {
  unsigned long startTime = millis();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Take your med...");

  while (millis() - startTime < timeoutMs) {
    if (digitalRead(IR_SENSOR) == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Medicine taken");
      delay(1000);
      return true;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please take med!");

  while (true) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);

    if (digitalRead(IR_SENSOR) == LOW) {
      digitalWrite(BUZZER, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Taken after buzz");
      delay(1000);
      return true;
    }
  }
}

