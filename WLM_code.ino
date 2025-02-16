// Blynk template details - you need these to communicate with the Blynk app
#define BLYNK_TEMPLATE_ID "TMPL3BFN2oENm"
#define BLYNK_TEMPLATE_NAME "ESP32 WaterLevel"
#define BLYNK_AUTH_TOKEN "7w4fj76RmqQgt6OsMU0hlFvS5mu1ykih"

// Include necessary libraries for Blynk, display, and sensor operations
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <WiFi.h>

// Define virtual pin for Blynk button (used for distance reading)
#define VPIN_BUTTON_1 V1

// WiFi credentials for ESP32
const char* ssid = "Device_name"; // Replace with your Wi-Fi name
const char* password = "Wifi-password"; // Replace with your Wi-Fi password
char auth[] = "7w4fj76RmqQgt6OsMU0hlFvS5mu1ykih"; // Blynk authentication token

// OLED display settings
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Initialize display

// Ultrasonic sensor pins for measuring distance
const int trigPin = 5;   // Trigger pin of ultrasonic sensor
const int echoPin = 18;  // Echo pin of ultrasonic sensor

// Constants for sound speed and conversion to inches
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;        // Variable to store pulse duration
int distanceCm;       // Variable to store distance in cm
int distanceInch;     // Variable to store distance in inches

// Buzzer pin configuration
const int buzzerPin = 4;  // Pin connected to buzzer

void setup() {
    Serial.begin(115200); // Start serial communication for debugging
    delay(1000); // Initial delay before setting up Blynk

    Blynk.begin(auth, ssid, password); // Initialize Blynk with credentials

    pinMode(trigPin, OUTPUT); // Set trigger pin as output
    pinMode(echoPin, INPUT);  // Set echo pin as input
    pinMode(buzzerPin, OUTPUT);  // Set buzzer pin as output

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;) ; // Halt the program if display initialization fails
    }
    delay(500); // Small delay before clearing the display
    display.clearDisplay();  // Clear the display screen
    display.setTextSize(2);  // Set text size for display
    display.setTextColor(WHITE);  // Set text color to white
}

void loop() {
    Blynk.run(); // Handle all Blynk communication (sending/receiving data)

    // Trigger ultrasonic sensor to send a pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);  // Wait for a short period
    digitalWrite(trigPin, HIGH);  // Send pulse
    delayMicroseconds(10);  // Wait for the pulse to be sent
    digitalWrite(trigPin, LOW);  // End the pulse

    // Measure the duration of the pulse echo
    duration = pulseIn(echoPin, HIGH);  // Read the echo pin
    distanceCm = duration * SOUND_SPEED / 2;  // Calculate distance in cm
    distanceInch = distanceCm * CM_TO_INCH;  // Convert distance to inches

    // Print the measured distances in the serial monitor for debugging
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    Serial.print("Distance (inch): ");
    Serial.println(distanceInch);

    // Send the distance (in cm) to the Blynk app using virtual pin 1
    Blynk.virtualWrite(VPIN_BUTTON_1, distanceCm);

    // Update OLED display with the distance value
    display.clearDisplay();  // Clear previous display content
    display.setCursor(0, 25);  // Set text cursor to position (x, y)
    display.print(distanceCm); // Print the distance in cm on display
    display.print(" cm");  // Print " cm" after the distance value
    display.display();  // Update the display with the new content

    // Check if the distance is more than 21 cm
    if (distanceCm > 21) {
        // If the distance is more than 21 cm, turn on the buzzer
        digitalWrite(buzzerPin, HIGH);
    } else {
        // If the distance is less than or equal to 21 cm, turn off the buzzer
        digitalWrite(buzzerPin, LOW);
    }

    delay(500);  // Wait for half a second before the next iteration
}
