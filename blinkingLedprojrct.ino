#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// WiFi credentials
#define WIFI_SSID "STARK NETWORKS"        // 🔹 Change this
#define WIFI_PASSWORD "Vinod630211"       // 🔹 Change this

// Firebase configuration
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Pin to control LED / Motor
#define LED_PIN 2   // 🔹 Built-in LED on NodeMCU (D4, GPIO2)

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // OFF initially (built-in LED is active LOW)

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Connected to Wi-Fi");

  // Firebase setup
  config.api_key = "AIzaSyATnIhcs_-UMqpOIgQsCkSfp9cG2-Rybzw";
  config.database_url = "https://remote-motor-zihqk-default-rtdb.firebaseio.com/";

  // Anonymous login
  auth.user.email = "";
  auth.user.password = "";

  // Start Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("✅ Firebase initialized");
}

void loop() {
  if (Firebase.ready()) {
    if (Firebase.getInt(fbdo, "/farm1/motor")) {   // Read motor state from DB
      int motorState = fbdo.intData();
      Serial.printf("Motor state from Firebase: %d\n", motorState);

      if (motorState == 1) {
        digitalWrite(LED_PIN, LOW);   // ON (built-in LED is active LOW)
        Serial.println("💡 LED ON");
      } else {
        digitalWrite(LED_PIN, HIGH);  // OFF
        Serial.println("💡 LED OFF");
      }
    } else {
      Serial.printf("❌ Firebase read failed: %s\n", fbdo.errorReason().c_str());
    }
  }
  delay(2000);  // check every 2 sec
}
