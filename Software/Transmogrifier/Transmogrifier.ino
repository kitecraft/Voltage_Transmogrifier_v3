/*
 Name:		Transmogrifier.ino
 Created:	9/11/2022 9:06:54 AM
 Author:	Kitecraft
*/
#include "src/Transmogrifier_Config.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_PCD8544.h>
#include "src/fonts/Ramabhadra_Regular8pt7b.h"
#include "src/fonts/Ramabhadra_Regular6pt7b.h"

Adafruit_PCD8544 g_Nokia = Adafruit_PCD8544(NOKIA_DC_PIN, NOKIA_CS_PIN, NOKIA_RS_PIN);

void setup() {
	g_Nokia.begin();
	g_Nokia.setRotation(3);
	g_Nokia.clearDisplay();
	g_Nokia.display();

	g_Nokia.setTextColor(BLACK);
	g_Nokia.drawFastHLine(0, DISPLAY_HEIGHT / 2, DISPLAY_WIDTH, BLACK);

	g_Nokia.display();
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	Update33Values();
	Update50Values();
	delay(40);
}

float GetVoltage(int pin) 
{
	int sample_count = 0;
	int sum = 0;
	while (sample_count < NUM_SAMPLES) {
		sum += analogRead(pin);
		sample_count++;
		delay(10);
	}
	float voltage = (((float)sum / (float)NUM_SAMPLES * VD_VREF) / 1024.0);
	return (voltage * VD_MULTIPLIER);
}


float GetCurrent(int pin)
{
	float acsValue = 0.0, samples = 0.0, avgAcs = 0.0, acsValueF = 0.0;

	for (int x = 0; x < NUM_SAMPLES; x++) {
		samples += analogRead(pin);;
	}
	avgAcs = (samples / NUM_SAMPLES);
	avgAcs = avgAcs * (VD_VREF / 1023.0);
	if (avgAcs > OFFSET_VOLTAGE) {
		avgAcs = OFFSET_VOLTAGE;
	}
	
	float current = (OFFSET_VOLTAGE - avgAcs) / CURRENT_MULTIPLIER;
	return current;
}

void Update33Values()
{
	g_Nokia.setFont(&Ramabhadra_Regular8pt7b);
	float value = GetVoltage(VOLTAGE_PIN_33);
	g_Nokia.fillRect(0, 0, DISPLAY_WIDTH, 11, WHITE);
	g_Nokia.display();

	g_Nokia.setCursor(0, VOLTAGE_VALUE_33_Y);
	g_Nokia.print(value);
	g_Nokia.print(" V");


	g_Nokia.display();
	g_Nokia.setFont(&Ramabhadra_Regular6pt7b);
	value = GetCurrent(CURRENT_PIN_33);
	g_Nokia.fillRect(0, 18, DISPLAY_WIDTH, 11, WHITE);
	g_Nokia.setCursor(0, CURRENT_VALUE_33_Y);
	if (value < 1) {
		g_Nokia.print(int(value * 1000));
		g_Nokia.print(" mA");
	}
	else {
		g_Nokia.print(value);
		g_Nokia.print(" A");
	}

}

void Update50Values()
{
	g_Nokia.setFont(&Ramabhadra_Regular8pt7b);
	float value = GetVoltage(VOLTAGE_PIN_50);
	g_Nokia.fillRect(0, (DISPLAY_HEIGHT / 2) + 2, DISPLAY_WIDTH, 11, WHITE);
	g_Nokia.display();

	g_Nokia.setCursor(0, VOLTAGE_VALUE_50_Y);
	g_Nokia.print(value);
	g_Nokia.print(" V");
	g_Nokia.display();

	g_Nokia.setFont(&Ramabhadra_Regular6pt7b);
	g_Nokia.setCursor(0, CURRENT_VALUE_50_Y);
	value = GetCurrent(CURRENT_PIN_50);
	g_Nokia.fillRect(0, (DISPLAY_HEIGHT / 2) + 18, DISPLAY_WIDTH, 11, WHITE);
	if (value < 1) {
		g_Nokia.print(int(value * 1000));
		g_Nokia.print(" mA");
	}
	else {
		g_Nokia.print(value);
		g_Nokia.print(" A");
	}

	g_Nokia.display();
}