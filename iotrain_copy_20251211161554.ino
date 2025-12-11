#include <WiFi.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>

// ================= WIFI =================
#define WIFI_SSID "Arusbawah"
#define WIFI_PASSWORD "samono33"

// ================= FIREBASE =================
#define API_KEY "AIzaSyDe9W577rTNSZpL8v3rMI9iNGWehULcKz8"
#define DATABASE_URL "https://iotesp-e752f-default-rtdb.asia-southeast1.firebasedatabase.app/"

// ================= PIN =================
#define RAIN_PIN   27
#define BUZZER_PIN 26

// ================= KONSTANTA =================
const float MM_PER_TIP = 0.2;

// ================= FIREBASE OBJECT =================
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ================= WEB SERVER =================
WebServer server(80);

// ================= VARIABEL =================
volatile uint32_t tipCount = 0;
volatile bool rainEvent = false;

unsigned long lastInterrupt = 0;
unsigned long buzzerStart = 0;
bool buzzerOn = false;
unsigned long lastFirebaseSend = 0;

// ================= ISR =================
void IRAM_ATTR rainISR() {
  unsigned long now = millis();
  if (now - lastInterrupt > 100) {
    tipCount++;
    rainEvent = true;
    lastInterrupt = now;
  }
}

// ================= WEB =================
void handleRoot() {
  noInterrupts();
  uint32_t tips = tipCount;
  interrupts();

  float rainfall = tips * MM_PER_TIP;

  String html = "<html><head><meta http-equiv='refresh' content='3'></head><body>";
  html += "<h1>RAIN MONITOR</h1>";
  html += "<h2>Tipping: " + String(tips) + "</h2>";
  html += "<h2>Curah Hujan: " + String(rainfall, 2) + " mm</h2>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(RAIN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), rainISR, FALLING);

  // ===== WIFI =====
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");

  // ===== FIREBASE =====
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.signUp(&config, &auth, "", "");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("✅ Firebase Ready");

  // ===== WEB =====
  server.on("/", handleRoot);
  server.begin();
  Serial.println("✅ Web Server Active");
  Serial.println(WiFi.localIP());
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  // ===== BUZZER =====
  if (rainEvent) {
    rainEvent = false;
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerOn = true;
    buzzerStart = millis();
  }

  if (buzzerOn && millis() - buzzerStart > 300) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerOn = false;
  }

  // ===== KIRIM KE FIREBASE SETIAP 3 DETIK =====
  if (millis() - lastFirebaseSend > 3000) {
    lastFirebaseSend = millis();

    noInterrupts();
    uint32_t tips = tipCount;
    interrupts();

    float rainfall = tips * MM_PER_TIP;

    Firebase.RTDB.setInt(&fbdo, "/rain/tipping", tips);
    Firebase.RTDB.setFloat(&fbdo, "/rain/mm", rainfall);

    Serial.println("📡 Data hujan dikirim ke Firebase");
  }
}
