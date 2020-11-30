#include "buttons.h"
#include "data.h"
#include "eeprom_backup.h"

/* typedef enum {
  HIGH,
  LOW
} t_button_states; */

#ifdef ARDUINO_NANO_EVERY
	#define BUTTON_1_PIN A0
	#define BUTTON_2_PIN A1
	#define BUTTON_3_PIN A2
#else
	#define BUTTON_1_PIN PB3
	#define BUTTON_2_PIN PB4
	#define BUTTON_3_PIN PB5
#endif

#define DEBOUNCE_DELAY 25
#define LONG_PRESS 2000
#define IGNORE_PRESS 10000

bool current_state = HIGH; // button not pressed, active-low
bool change_flag = false;
unsigned long button1_falling_time;
unsigned long button1_rising_time;
volatile bool button1_falling_flag = false;
volatile bool button1_rising_flag = false;
unsigned long button3_falling_time;
unsigned long button3_rising_time;
volatile bool button3_falling_flag = false;
volatile bool button3_rising_flag = false;

bool check_button1(t_data* data){
	if (button1_rising_flag){
			Serial.println("Button 1 was pressed for:  " + String(button1_rising_time - button1_falling_time) + " ms");
			button1_falling_flag = false;
			button1_rising_flag = false;
	}

	if (button1_falling_flag == true){
		if ((millis() - button1_falling_time) > LONG_PRESS){
			Serial.println("Button 1 was pressed for:  " + String(millis() - button1_falling_time) + " ms");
			// Reset session and also write this to the EEPROM
			data->session->trip_meters = 0;
			data->session->max_speed_kph = 0;
			data->session->millis_elapsed = 0;
			data->session->millis_riding = 0;
			// session_data.min_voltage = data.voltage;
			// eeprom_write_session_data(*data->session);
			Serial.println("Session reset");
			button1_falling_flag = false;
			return true;
		}
	}
	return false;
}

bool check_button3(t_data* data){

	if (button3_rising_flag){
		if (button3_falling_flag == true){
			if(button3_rising_time - button3_falling_time < DEBOUNCE_DELAY){
				Serial.println("But3->No action:  " + String(button3_rising_time - button3_falling_time) + " ms");
			}
			else{
				Serial.println("But3->Press:  " + String(button3_rising_time - button3_falling_time) + " ms");
			}
			button3_falling_flag = false;
		}
		button3_rising_flag = false;
	}

	if (button3_falling_flag == true){
		if ((millis() - button3_falling_time) > LONG_PRESS){
			Serial.println("But3->Long press:  " + String(millis() - button3_falling_time) + " ms");
			// Reset session and also write this to the EEPROM
			data->session->trip_meters = 0;
			data->session->max_speed_kph = 0;
			data->session->millis_elapsed = 0;
			data->session->millis_riding = 0;
			// session_data.min_voltage = data.voltage;
			eeprom_write_session_data(*data->session);
			Serial.println("Session reset");
			button3_falling_flag = false;
			return true;
		}
	}
	return false;
}

// ISR routines for button presses
void button1_changed(){
	if(digitalRead(BUTTON_1_PIN) == LOW){
		if((millis()-button1_falling_time) < DEBOUNCE_DELAY){
			return;
		}
		else{
			button1_falling_time = millis();
			button1_falling_flag = true;
			Serial.println("Button 1 falling time: " + String(button1_falling_time));
			return;
		}
	}
	else if((millis()-button1_falling_time) < DEBOUNCE_DELAY)
	{
		return;
	}
	button1_rising_flag = true;
	button1_rising_time = millis();
	Serial.println("Button 1 rising time: " + String(button1_rising_time));
}

void button2_pressed(){
	Serial.println("Button 2 pressed");
}

void button3_changed(){
	// unsigned long change_time = millis();

	if(digitalRead(BUTTON_3_PIN) == LOW){
		button3_falling_time = millis();
		button3_falling_flag = true;
		//Serial.println("Button 3 falling time: " + String(button3_falling_time));
	}
	else{
		button3_rising_flag = true;
		button3_rising_time = millis();
		// Serial.println("Button 3 rising time: " + String(button3_rising_time));
	}
}