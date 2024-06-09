
//#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL36iLdRBFE"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "4L6EECmODPBbT0UL4M1ahMnrnpYscQ_W"
#define PIR D5
#define soil A0
#define RELAY_PIN_1 D3

#include<ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(D4, DHT11);


char auth[] = "4L6EECmODPBbT0UL4M1ahMnrnpYscQ_W";
char ssid[] = "CM_Delhi";
char pass[] = "123456789";


void setup() {
	Serial.begin(9600);
	Blynk.begin(auth, ssid, pass);
	Blynk.run();
	dht.begin();
	lcd.init();
	lcd.clear();
	lcd.backlight();
	pinMode(PIR, INPUT);
	pinMode(RELAY_PIN_1, OUTPUT);
	digitalWrite(RELAY_PIN_1, HIGH);
	dht.begin();
	lcd.setCursor(0, 0);
	lcd.print("  Initializing  ");
	delay(2000);
	lcd.clear();
	lcd.setCursor(11, 1);
	lcd.print("W:OFF");
}

//TO READ FROM VIRTUAL PINS'
int cnt = 0 ;
BLYNK_WRITE(V3) {
	int x = param.asInt();
	if (x) {
		digitalWrite(RELAY_PIN_1, LOW);
	}
	else {
		digitalWrite(RELAY_PIN_1, HIGH);
	}
}
BLYNK_WRITE(V4) {
	int x = param.asInt();
	if (x) {
		cnt= 0 ;
    Blynk.virtualWrite(V4,0);
	}
}


// TO WRITE ON VIRTUAL PINS

//  Blynk.virtualWrite(2, LOW);

void loop() {
	float h = dht.readHumidity();
	float t = dht.readTemperature();
	int val = analogRead(soil);
	int soilm = map(val,493, 1023, 100, 1);
	if (soilm > 50 and cnt < 1) {
    cnt++;
		digitalWrite(RELAY_PIN_1, HIGH);
	}
	if (soilm < 10 and cnt < 1) {
		digitalWrite(RELAY_PIN_1, LOW);
	}
	if (isnan(h) || isnan(t)) {
		Serial.println("Failed to read from DHT sensor!");
	}
	else {
		Serial.print("Temp:");
		Serial.print(t);
		Serial.print(" C, ");
		Serial.print("Hum: ");
		Serial.print(h);
		Serial.print("% SM:");
		Serial.print(soilm);
    Serial.print("  ");
    Serial.print(digitalRead(RELAY_PIN_1));
		Serial.println("");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("T:");
		lcd.print(t);
		lcd.print("C, ");
		lcd.print("H:");
		lcd.print(h);
		lcd.print("%");
		lcd.setCursor(1, 1);
		lcd.print("SM:");
		lcd.print(soilm);
		lcd.print("% ");
	}
	Blynk.virtualWrite(V0, t);
	Blynk.virtualWrite(V1, h);
	Blynk.virtualWrite(V2, soilm);
	delay(2000);
}
