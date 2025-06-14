#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht11.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT Sensor
dht11 DHT;

// Pin definitions
#define SOIL_MOISTURE_PIN A0   // Soil Moisture Sensor
#define DHT_PIN 2              // DHT11 Sensor
#define BUZZER_PIN 6           // Buzzer

// RGB LED (Common Cathode)
#define RED_LED 3              // Red
#define GREEN_LED 5            // Green
#define BLUE_LED 4             // Blue

void setup() {
  Serial.begin(9600);

  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Initialize LCD Display
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sistema");
  lcd.setCursor(0,1);
  lcd.print("Inicializado");

}

void loop() {
  // Read Raw Soil Moisture Value
  int moistureValue = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("Raw Moisture Value: ");
  Serial.println(moistureValue);

  // Adjust Mapping Based on Real Readings
  moistureValue = constrain(moistureValue, 300, 900); // Limita a 300–900
  int moisturePercentage = map(moistureValue, 900, 300, 0, 100); // 900=0%, 300=100%

  // Read Temperature & Humidity
  DHT.read(DHT_PIN);
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  // Print values in Serial Monitor
  Serial.print("Moisture: "); Serial.print(moisturePercentage); Serial.println("%");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");

  // Display Sensor Data LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hum Suelo:");
  lcd.print(moisturePercentage);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp:");
  lcd.print(round(temperature));
  lcd.print("C");
  lcd.print(" Hum:");
  lcd.print(round(humidity));
  lcd.print("%");

  // LED & Buzzer Alerts
  if (moisturePercentage < 30) { // Suelo muy seco
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (moisturePercentage > 90) { // Suelo demasiado húmedo
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else { // Nivel adecuado
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(2000);  // Refresh every 2 seconds
}
