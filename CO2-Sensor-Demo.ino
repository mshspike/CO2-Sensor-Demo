#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_CCS811.h>

// Initialize objects
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_CCS811 ccs;

// Define pins
const int RELAY_PIN = 4;  // Connect relay to digital pin 4
const int CO2_THRESHOLD = 1000;  // CO2 threshold in ppm to trigger fan

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Initializing...");
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Start with fan off
  
  // Initialize CCS811
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    lcd.clear();
    lcd.print("Sensor Error!");
    while(1);
  }
  
  // Wait for the sensor to be ready
  while(!ccs.available());
  
  lcd.clear();
}

void loop() {
  if(ccs.available()){
    if(!ccs.readData()){
      int co2 = ccs.geteCO2();
      
      // Update LCD display
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CO2: ");
      lcd.print(co2);
      lcd.print(" ppm");
      
      // Control fan based on CO2 levels
      if(co2 > CO2_THRESHOLD){
        digitalWrite(RELAY_PIN, HIGH);  // Turn fan on
      } else {
        digitalWrite(RELAY_PIN, LOW);   // Turn fan off
      }
      
      // Print to Serial for debugging
      Serial.print("CO2: ");
      Serial.print(co2);
      Serial.println("ppm");
    }
  }
  delay(1000);  // Wait for a second before next reading
} 