#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "math.h"
#include "Adafruit_LIS3DH.h"
#include "Adafruit_ADS1015.h"
#include "float.h"
#include "time.h"
#include "WMsg_encoder.h"
#include "msgQueueConst.h"
#include "LogMgr.h"
#include <string.h>
#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>
#include <cerrno>

#ifdef __cplusplus
extern "C"
{
	#endif
	#include "ssd1306_i2c.h"

#include "wiringPi.h"
#include "wiringPiI2C.h"
#ifdef __cplusplus
}
#endif

using namespace std;

bool connected;

int acceli2c, adci2c, oledi2c, stk_btn, a_btn, b_btn, blue_old, blue_new, input_old, input_new,
	blue_sw, input_sw, GPIO_STK, GPIO_A, GPIO_B, GPIO_BLUE, GPIO_INPUT, GPIO_tmp;

uint8_t check_reg, ctrl1_tmp, check_status;

int16_t signed_axisX, signed_axisY, signed_axisZ, signed_adc0, signed_adc1, signed_adc2;

uint16_t config, reading, raw_axisX, raw_axisY, raw_axisZ;

float g_axisX, g_axisY, g_axisZ, adc0, adc1, adc2, x_msg_adc, y_msg_adc, batt_msg,
	axisX_msg, axisY_msg, axisZ_msg,
	adc_res, stk_max, stk_mid, stk_mid_100, batt_min, batt_max, batt_range, batt_range_100;

clock_t old_time_input, old_time_beat, curr_time, diff_time_input,
		diff_time_beat, update_time, screen_time, diff_time_screen, old_screen_time,
		stk_btn_time, a_btn_time, b_btn_time, beat_time, 
		blue_sw_time, input_sw_time, btn_debounce, btn_diff_time;

char* blue_string;
char* input_string;
char* batt_string;

enum STATE {STK_IDLE, STK_DEBOUNCE, STK_PRIMED, STK_READY, STK_EVENT,
			A_IDLE, A_DEBOUNCE, A_PRIMED, A_READY, A_EVENT,
			B_IDLE, B_DEBOUNCE, B_PRIMED, B_READY, B_EVENT,
			BLUE_VALID, BLUE_INVALID,
			INPUT_VALID, INPUT_INVALID};

STATE STK_STATE, A_STATE, B_STATE, BLUE_STATE, INPUT_STATE;

struct sigevent;
struct mq_attr;

mqd_t sendBox, recvBox;
WMessage msgOut, msgIn;

void initialize();
void i2c_init();
void accel_init();
void adc_init();
void gpio_init();
void box_init();
void screen_init();
void getGPIO();
void debounce();
void getadc();
void getbatt();
void getaccel();
void screenupdate();
void dispBLUE();
void dispINPUT();
void dispBATT();

static void recvBoxOnData(union sigval sv);
	
int main(int argc, char* argv[])
{
	initialize();
	old_time_input = clock();//get a reference time
	old_time_beat = 0;//set a reference time
	old_screen_time = old_time_input;
	blue_old = 1;

	//sleep(3);
	
	while (true) {
		
		//get times
		curr_time = clock();//get the current system time
		diff_time_input = curr_time - old_time_input;//find difference between current time and last
		diff_time_beat = curr_time - old_time_beat;//find difference between current time and last
		diff_time_screen = curr_time - old_screen_time;
		
		//get current button states
		getGPIO();
		
		//debounce GPIO
		debounce();
		
		//check if time to update direction 
		if(diff_time_input > update_time){
			old_time_input = curr_time;
			
			if(input_old){
				//check adc values
				getadc();
			} else {
				//check accleration axes
				getaccel();
			}
		}
		
		//check if time to update battery
		if(diff_time_beat > beat_time){
			old_time_beat = curr_time;
			//get battery status
			getbatt();
		}
		
		//check time to update screen
		if(diff_time_screen > screen_time){
			old_screen_time = curr_time;
			screenupdate();
		}

	}
}

void initialize(){
	
	i2c_init();
	accel_init();
	adc_init();
	gpio_init();
	box_init();
	screen_init();
	
	update_time = 500000; //time between input checks
	btn_debounce = 500; //time of a valid debounce
	beat_time = 50000; //time between heartbeats
	screen_time = 500000; //time between screen updates

}

void i2c_init(){
	
	//initialize wiringPi
	wiringPiSetup();
	
	//set LIS3DH device address
	acceli2c = wiringPiI2CSetup(0x18);
	
	//set ADS1015 address
	adci2c = wiringPiI2CSetup(0x48);
	
	//set SSD1306 address
	oledi2c = wiringPiI2CSetup(0x3c);

	printf("I2C initialized\n"); // I2C init

}

void accel_init(){
	
	//config LIS3DH
	check_reg = 0x00;
	ctrl1_tmp = 0x00;
	ctrl1_tmp = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_CTRL1);
	ctrl1_tmp |= 0x07; //enable 3 axes
	ctrl1_tmp |= LIS3DH_DATARATE_400_HZ << 4; // set data rate to 400 Hz
	
	//update crtl1 with settings
	wiringPiI2CWriteReg8(acceli2c, LIS3DH_REG_CTRL1, ctrl1_tmp);
	
	//check_reg = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_CTRL1);
	//printf("CTRL 1: 0x%X\n", check_reg); //display status reg value
	
	wiringPiI2CWriteReg8(acceli2c, LIS3DH_REG_CTRL4, 0x88); // High res & BDU enabled
	// check_reg = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_CTRL4);
	// printf("CTRL 4: 0x%X\n", check_reg); //display status reg value
	
	//Set inital values for reading axes
	raw_axisX = 0x0000;
	raw_axisY = 0x0000;
	raw_axisZ = 0x0000;
	signed_axisX = 0;
	signed_axisY = 0;
	signed_axisZ = 0;
	g_axisX = 0;
	g_axisY = 0;
	g_axisZ = 0;
	//value to check axis adc conversion readiness
	 check_status = 0x00;
	 
	printf("Accel initialized\n"); // I2C init

}

void adc_init(){
	
	//setup variables
	config = 0x0000;
	reading = 0x0000;
	signed_adc0 = 0;
	signed_adc1 = 0;
	signed_adc2 = 0;
	adc_res = 100;
	stk_max = 1600;
	stk_mid = stk_max/2;
	stk_mid_100 = stk_mid/100;
	batt_max = 2047;
	batt_min = 1600;
	batt_range = batt_max - batt_min;
	batt_range_100 = batt_range / 100;
	
	printf("ADC initialized\n"); // ADC init
}

void gpio_init(){
	
	//gpio variables
	btn_diff_time = 0;
	
	stk_btn = 0;
	a_btn = 2;
	b_btn = 3;
	blue_sw = 1;
	input_sw = 4;
	
	GPIO_STK = 1;
	GPIO_A = 1;
	GPIO_B = 1;
	GPIO_BLUE = 1;
	GPIO_INPUT = 1;
	
	//set inital gpio states
	STK_STATE = STK_IDLE;
	A_STATE = A_IDLE;
	B_STATE = B_IDLE;
	BLUE_STATE = BLUE_VALID;
	blue_old = GPIO_BLUE;
	INPUT_STATE = INPUT_VALID;
	input_old = GPIO_INPUT;

	//setup gpio internal pullups
	pullUpDnControl(stk_btn, PUD_UP);
	pullUpDnControl(a_btn, PUD_UP);
	pullUpDnControl(b_btn, PUD_UP);
	pullUpDnControl(blue_sw, PUD_UP);
	pullUpDnControl(input_sw, PUD_UP);
	printf("GPIO initialized\n"); //confirm I2C init

}

void screen_init(){
	
	diff_time_screen = 0;
	
	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	//ssd1306_invertDisplay(1);
	ssd1306_clearDisplay();
	char* boot_msg = "Controller booting up\n";
	ssd1306_drawString(boot_msg);
	
	ssd1306_display(); //Adafruit logo is visible
	ssd1306_clearDisplay();
	//usleep(50000);
}

void screenupdate(){
	
	ssd1306_clearDisplay();
	
	//display bluetooth status
	dispBLUE();
	
	//display input status
	dispINPUT();

	//display battery status
	dispBATT();
	
	ssd1306_display();

}

void dispBLUE(){
	if(blue_old){
		char* blue_string = "Bluetooth: ON\n";
		ssd1306_drawString(blue_string);
	} else {
		char* blue_string = "Bluetooth: OFF\n";
		ssd1306_drawString(blue_string);
	}
}

void dispINPUT(){
	if(input_old){
		char* input_string = "Input: JOYSTICK\n";
		ssd1306_drawString(input_string);
	} else {
		char* input_string = "Input: ACCELEROMETER\n";
		ssd1306_drawString(input_string);
	}
}

void dispBATT(){
	if(batt_msg > 90){
		char* batt_string = "Battery: |++++++++++|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 80){
		char* batt_string = "Battery: |+++++++++-|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 70){
		char* batt_string = "Battery: |++++++++--|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 60){
		char* batt_string = "Battery: |+++++++---|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 50){
		char* batt_string = "Battery: |++++++----|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 40){
		char* batt_string = "Battery: |+++++-----|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 30){
		char* batt_string = "Battery: |++++------|\n";
		ssd1306_drawString(batt_string);
	} else if (batt_msg > 20){
		char* batt_string = "Battery: |+++-------|\n";
	ssd1306_drawString(batt_string);
	} else if (batt_msg > 10){
		char* batt_string = "Battery: |++--------|\n";
		ssd1306_drawString(batt_string);
	} else {
		char* batt_string = "Battery: |+---------|\n";
		ssd1306_drawString(batt_string);
	}
}

void getGPIO(){
	
	//polls all GPIO states
	GPIO_STK = digitalRead(stk_btn);
	GPIO_A = digitalRead(a_btn);
	GPIO_B = digitalRead(b_btn);
	GPIO_BLUE = digitalRead(blue_sw);
	GPIO_INPUT = digitalRead(input_sw);

}

void debounce(){
	
	//debounce stick
	switch(STK_STATE) {
		case STK_IDLE:
			if(GPIO_STK == 0){
				stk_btn_time = clock();
				printf ("Checking debounce...\n");
				STK_STATE = STK_DEBOUNCE;
			} else {
				STK_STATE = STK_IDLE;
			}
			break;
		case STK_DEBOUNCE:
			btn_diff_time = clock() - stk_btn_time;
			// printf ("Stick button is %d.\n",GPIO_STK);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_STK == 0){
					printf ("Legit press.\n");
					printf ("Primed for depress.\n");
					STK_STATE = STK_PRIMED;
				} else {
					printf ("Invalid press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					STK_STATE = STK_IDLE;
				}
			} else {
				STK_STATE = STK_DEBOUNCE;
			}
			break;
		case STK_PRIMED:
			if(GPIO_STK == 0){
				STK_STATE = STK_PRIMED;
			} else {
				printf ("Button depressed, moving to event.\n");
				STK_STATE = STK_EVENT;
			}
			break;
		case STK_READY:
		// do something in the stop state
			break;
		case STK_EVENT:
			printf ("Stick button went from 1 to 0 back to 1.\n");
			printf ("***************************.\n");
			printf ("***************************.\n");
			
			// if(blue_old){
				// msgOut.type = eMsgTypes::BUTTON;
				// msgOut.button = BTN_STICK;
				// msgOut.btnState = true;
				// mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0);
			// }
			
			STK_STATE = STK_IDLE;
			break;
		default:
			break;
	}	

	//debounce A
	switch(A_STATE) {
		case A_IDLE:
			if(GPIO_A == 0){
				a_btn_time = clock();
				printf ("A button went from 1 to 0.\n");
				printf ("Debouncing...\n");
				A_STATE = A_DEBOUNCE;
			} else {
				A_STATE = A_IDLE;
			}
			break;
		case A_DEBOUNCE:
			btn_diff_time = clock() - a_btn_time;
			// printf ("A button is %d.\n",GPIO_A);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_A == 0){
					printf ("Legit press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					A_STATE = A_PRIMED;
				} else {
					printf ("Invalid press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					A_STATE = A_IDLE;
				}
			} else {
				A_STATE = A_DEBOUNCE;
			}
			break;
		case A_PRIMED:
			if(GPIO_A == 0){
				A_STATE = A_PRIMED;
			} else {
				printf ("Button depressed, moving to event.\n");
				A_STATE = A_EVENT;
			}
			break;
		case A_READY:
		// do something in the stop state
			break;
		case A_EVENT:
			printf ("A button went from 1 to 0 back to 1.\n");
			printf ("***************************.\n");
			printf ("***************************.\n");
			
			 if(blue_old){
				 msgOut.type = eMsgTypes::BUTTON;
				 msgOut.button = BTN_A;
				 msgOut.btnState = true;
				 mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0);
			 }
			
			A_STATE = A_IDLE;
			break;
		default:
			break;
	}

	//debounce B
	switch(B_STATE) {
		case B_IDLE:
			if(GPIO_B == 0){
				b_btn_time = clock();
				printf ("B button went from 1 to 0.\n");
				printf ("Debouncing...\n");
				B_STATE = B_DEBOUNCE;
			} else {
				B_STATE = B_IDLE;
			}
			break;
		case B_DEBOUNCE:
			btn_diff_time = clock() - b_btn_time;
			// printf ("B button is %d.\n",GPIO_B);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_B == 0){
					printf ("Legit press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					B_STATE = B_PRIMED;
				} else {
					printf ("Invalid press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					B_STATE = B_IDLE;
				}
			} else {
				B_STATE = B_DEBOUNCE;
			}
			break;
		case B_PRIMED:
			if(GPIO_B == 0){
				B_STATE = B_PRIMED;
			} else {
				printf ("Button depressed, moving to event.\n");
				B_STATE = B_EVENT;
			}
			break;
		case B_READY:
		// do something in the stop state
			break;
		case B_EVENT:
			printf ("B button went from 1 to 0 back to 1.\n");
			printf ("***************************.\n");
			printf ("***************************.\n");
			
			 if(blue_old){
				 msgOut.type = eMsgTypes::BUTTON;
				 msgOut.button = BTN_B;
				 msgOut.btnState = true;
				 mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0);
			 }
			B_STATE = B_IDLE;
			break;
		default:
			break;
	}	

	//debounce BLUE
	switch(BLUE_STATE) {
		case BLUE_VALID:
			if(GPIO_BLUE != blue_old){
				blue_sw_time = clock();
				printf ("BLUE switch changed from %d.\n", blue_old);
				printf ("Debouncing...\n");
				blue_new = GPIO_BLUE;
				BLUE_STATE = BLUE_INVALID;
			} else {
				BLUE_STATE = BLUE_VALID;
			}
			break;
		case BLUE_INVALID:
			btn_diff_time = clock() - blue_sw_time;
			// printf ("BLUE button is %d.\n",GPIO_BLUE);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_BLUE == blue_new){
					printf ("Legit toggle.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("BLUE switch changed to %d.\n", blue_new);
					blue_old = GPIO_BLUE;
					BLUE_STATE = BLUE_VALID;
				} else {
					printf ("Invalid toggle.\n");
					blue_old = GPIO_BLUE;
					BLUE_STATE = BLUE_VALID;
				}
			} else {
				BLUE_STATE = BLUE_INVALID;
			}
			break;
				default:
			break;
	}	

	//debounce INPUT
	switch(INPUT_STATE) {
		case INPUT_VALID:
			if(GPIO_INPUT != input_old){
				input_sw_time = clock();
				printf ("INPUT switch changed from %d.\n", input_old);
				printf ("Debouncing...\n");
				input_new = GPIO_INPUT;
				INPUT_STATE = INPUT_INVALID;
			} else {
				INPUT_STATE = INPUT_VALID;
			}
			break;
		case INPUT_INVALID:
			btn_diff_time = clock() - input_sw_time;
			// printf ("INPUT button is %d.\n",GPIO_INPUT);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_INPUT == input_new){
					printf ("Legit toggle.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("INPUT switch changed to %d.\n", input_new);
					input_old = GPIO_INPUT;
					INPUT_STATE = INPUT_VALID;
				} else {
					printf ("Invalid toggle.\n");
					input_old = GPIO_INPUT;
					INPUT_STATE = INPUT_VALID;
				}
			} else {
				INPUT_STATE = INPUT_INVALID;
			}
			break;
		default:
			break;
	}	
	
}

void getadc(){
	//ADC 0 (X stick)
	config = 0x0000;
	reading = 0x0000;
	config = ADS1015_REG_CONFIG_PGA_4_096V | // +/-4.096V range = Gain 1
			ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_3300SPS   | // 3300 samples per second
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
	config |= ADS1015_REG_CONFIG_MUX_SINGLE_0; //set config to read adc 0
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
	wiringPiI2CWriteReg16(adci2c, ADS1015_REG_POINTER_CONFIG, (config>>8) | (config & 0xFF));
	// Wait for the conversion to complete
	usleep(10000);
	// Read the conversion results
	wiringPiI2CWrite(adci2c, ADS1015_REG_POINTER_CONVERT);
	reading = wiringPiI2CReadReg16(adci2c, ADS1015_REG_POINTER_CONVERT);
	signed_adc0 = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
	//signed_adc0 &= 0x0000FFFF; //parse out an extra byte from shifting
	// Shift 12-bit results right 4 bits for the ADS1015
	signed_adc0 = signed_adc0 >> 4; 
	adc0 = (signed_adc0 * 0.002) + 0.031; //conversion to voltage with hardware offset
	
	//ADC 1 (Y stick)
	config = 0x0000;
	reading = 0x0000;
	config = ADS1015_REG_CONFIG_PGA_4_096V | // +/-4.096V range = Gain 1
			ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_3300SPS   | // 3300 samples per second
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
	config |= ADS1015_REG_CONFIG_MUX_SINGLE_1; //set config to read adc 1
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
	wiringPiI2CWriteReg16(adci2c, ADS1015_REG_POINTER_CONFIG, (config>>8) | (config & 0xFF));
	// Wait for the conversion to complete
	usleep(10000);
	// Read the conversion results
	wiringPiI2CWrite(adci2c, ADS1015_REG_POINTER_CONVERT);
	reading = wiringPiI2CReadReg16(adci2c, ADS1015_REG_POINTER_CONVERT);
	signed_adc1 = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
	//signed_adc1 &= 0x0000FFFF; //parse out an extra byte from shifting
	// Shift 12-bit results right 4 bits for the ADS1015
	signed_adc1 = signed_adc1 >> 4; 
	adc1 = (signed_adc1 * 0.002) + 0.031; //conversion to voltage with hardware offset

	// printf("ADC 0: %9.6f\n", adc0); //display value as voltage float
	// printf("ADC 0: 0x%04X\n", (uint16_t)signed_adc0); //display value as hex from 16bit register
	if(signed_adc0 > stk_max){
		signed_adc0 = stk_max;
	} else if (signed_adc0 < 0){
		signed_adc0 = 0;
	}
	
	if(signed_adc0 > stk_mid){
		x_msg_adc = (signed_adc0 - stk_mid) / stk_mid_100;
		if(x_msg_adc > 100){
			x_msg_adc = 100;
		}
	} else {
		x_msg_adc = (signed_adc0 / stk_mid_100) - 100;
	}
	//printf("X stick msg: %9.6f\n", x_msg_adc); //display value as hex from 16bit register
	
	// printf("ADC 1: %9.6f\n", adc1); //display value as voltage float
	// printf("ADC 1: 0x%04X\n", (uint16_t)signed_adc1); //display value as hex from 16bit register
	if(signed_adc1 > stk_max){
		signed_adc1 = stk_max;
	} else if (signed_adc1 < 0){
		signed_adc1 = 0;
	}
	
	if(signed_adc1 > stk_mid){
		y_msg_adc = (signed_adc1 - stk_mid) / stk_mid_100;
		if(y_msg_adc > 100){
			y_msg_adc = 100;
		}
	} else {
		y_msg_adc = (signed_adc1 / stk_mid_100) - 100;
	}
	//printf("Y stick msg: %9.6f\n", y_msg_adc); //display value as hex from 16bit register
	
	// if(blue_old){
		// msgOut.type = eMsgTypes::DIRECTION;
		// msgOut.x_dir = x_msg_adc;
		// msgOut.y_dir = y_msg_adc;
		// mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0);
	//}
}

void getbatt(){
	//ADC 2 (battery)
	reading = 0x0000;
	config = 0x0000;
	config = ADS1015_REG_CONFIG_PGA_4_096V | // +/-4.096V range = Gain 1
			ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_3300SPS   | // 3300 samples per second
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)
	config |= ADS1015_REG_CONFIG_MUX_SINGLE_2; //set config to read adc 2
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
	wiringPiI2CWriteReg16(adci2c, ADS1015_REG_POINTER_CONFIG, (config>>8) | (config & 0xFF));
	// Wait for the conversion to complete
	usleep(10000);
	// Read the conversion results
	wiringPiI2CWrite(adci2c, ADS1015_REG_POINTER_CONVERT);
	reading = wiringPiI2CReadReg16(adci2c, ADS1015_REG_POINTER_CONVERT);
	reading = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
	//signed_adc2 &= 0x0000FFFF; //parse out an extra byte from shifting
	// Shift 12-bit results right 4 bits for the ADS1015
	signed_adc2 = reading >> 4; 
	adc2 = signed_adc2 * 0.002; //conversion to voltage 

	// printf("ADC 2: %9.6f\n", adc2); //display value as voltage float
	// printf("ADC 2: 0x%04X\n", (uint16_t)signed_adc2); //display value as hex from 16bit register
	
	batt_msg = (signed_adc2 - batt_min) / batt_range_100;
	if(batt_msg > 100.0){
		batt_msg = 100.0;
	} else if (batt_msg < 0) {
		batt_msg = 0;
	}

	//printf("Batt msg: %9.6f\n", batt_msg); //display value as hex from 16bit register

	 if(blue_old){
		 msgOut.type = eMsgTypes::STATUS;
		 msgOut.battery = batt_msg;
		 mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0);
	 }
}

void getaccel(){
	//check for accel conversion status
	check_status = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_STATUS2);
	
	if(check_status) {
		//old_time = curr_time;
		//system("clear");
		//printf ("got into update. %d clicks (%f seconds).\n", (int)diff_time,((float)diff_time)/CLOCKS_PER_SEC);
		
		//x axis
		raw_axisX = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_X_L);
		// printf("raw X axis L: 0x%X\n", raw_axisX); //display value as hex from 16bit register
		raw_axisX |= wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_X_H) << 8;
		// printf("raw X axis with H: 0x%X\n", raw_axisX); //display value as hex from 16bit register
		// Shift 12-bit results right 4 bits for the LIS3DH
		signed_axisX = raw_axisX >> 4; 
		g_axisX = signed_axisX * 0.00095; //conversion to g's 
		//signed_axisX &= 0x0000FFFF; //parse out an extra byte from shifting
		//usleep(10);

		//y axis
		raw_axisY = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Y_L);
		// printf("raw Y axis L: 0x%X\n", raw_axisY); //display value as hex from 16bit register
		raw_axisY |= wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Y_H) << 8;
		// printf("raw Y axis with H: 0x%X\n", raw_axisY); //display value as hex from 16bit register
		// Shift 12-bit results right 4 bits for the LIS3DH
		signed_axisY = raw_axisY; 
		// printf("signed hex Y axis from raw: 0x%X\n", signed_axisY); //display value as hex from 16bit register
		// printf("signed int Y axis from raw: %d\n", signed_axisY); //display value as hex from 16bit register
		signed_axisY = signed_axisY >> 4; 
		// printf("shifted signed int Y axis: %d\n", signed_axisY); //display value as hex from 16bit register
		g_axisY = signed_axisY * 0.00095; //conversion to g's 
		//signed_axisY &= 0x0000FFFF; //parse out an extra byte from shifting
		//usleep(10);
		
		//z axis
		raw_axisZ = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Z_L);
		// printf("raw Z axis L: 0x%X\n", raw_axisZ); //display value as hex from 16bit register
		raw_axisZ |= wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Z_H) << 8;
		// printf("raw Z axis with H: 0x%X\n", raw_axisZ); //display value as hex from 16bit register
		// Shift 12-bit results right 4 bits for the LIS3DH
		raw_axisZ = raw_axisZ >> 4; 
		signed_axisZ = raw_axisZ; 
		g_axisZ = signed_axisZ * 0.00095; //conversion to g's 
		//signed_axisZ &= 0x0000FFFF; //parse out an extra byte from shifting

		//printf("X axis: %9.6f\n", g_axisX); //display value in g's as float
		//printf("X axis: 0x%X\n", signed_axisX); //display value as hex from 16bit register
		if(g_axisX > 1.0) {
			axisX_msg = 100;
		} else if (g_axisX < -1.0) {
			axisX_msg = -100;
		} else {
			axisX_msg = g_axisX * 100;
		}

		// printf("Y axis: %9.6f\n", g_axisY); //display value in g's as float
		// printf("Y axis: 0x%X\n", signed_axisY); //display value as hex from 16bit register
		if(g_axisY > 1.0) {
			axisY_msg = 100;
		} else if (g_axisY < -1.0) {
			axisY_msg = -100;
		} else {
			axisY_msg = g_axisY * 100;
		}
		
		// printf("Z axis: %9.6f\n", g_axisZ); //display value in g's as float
		// printf("Z axis: 0x%X\n", signed_axisZ); //display value as hex from 16bit register
		if(g_axisZ > 1.0) {
			axisZ_msg = 100;
		} else if (g_axisZ < -1.0) {
			axisZ_msg = -100;
		} else {
			axisZ_msg = g_axisZ * 100;
		}
		// printf("msg X accel: %9.6f\n", axisX_msg); //display value in g's as float
		// printf("msg Y accel: %9.6f\n", axisY_msg); //display value in g's as float
		// printf("msg Z accel: %9.6f\n", axisZ_msg); //display value in g's as float
		
		// if(blue_old){
			// msgOut.type = eMsgTypes::DIRECTION;
			// msgOut.x_dir = axisX_msg;
			// msgOut.y_dir = axisY_msg;
			// mq_send(sendBox, (const char *) &msgOut, sizeof(msgOut), 0);
		// }
	}
}

static void recvBoxOnData(union sigval sv)
{
    struct mq_attr MQStat;
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    printf("On Entering MQStat.mq_curmsgs: %ld\n", MQStat.mq_curmsgs);
    
    WMessage msgIn;
    while (errno != EAGAIN)
    {
        mq_receive(recvBox, (char *) &msgIn, 8192 , NULL);
		if(msgIn.type == eMsgTypes::STATUS){
			connected = true;
		}
    }
    
    if(mq_getattr(recvBox, &MQStat) == ERROR)
    {
        perror("mq_getattr");
        return;
    }
    
    printf("On Exiting MQStat.mq_curmsgs: %ld\n", MQStat.mq_curmsgs);

    struct sigevent signal;
    signal.sigev_notify = SIGEV_THREAD;
    signal.sigev_notify_function = recvBoxOnData;
    signal.sigev_notify_attributes = NULL;
    if(mq_notify(recvBox, &signal) == ERROR)
    {
        perror("mq_notify");
        return;
    }

    return;
}

void box_init(){
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;
    // mailbox of messages to be sent over bluetooth
    sendBox = mq_open(sendQueueName, O_RDWR, QUEUE_PERMISSIONS, attr);
    // mailbox of messges received over bluetooth
    recvBox = mq_open(recvQueueName, O_RDWR|O_NONBLOCK, QUEUE_PERMISSIONS, attr);
		
	connected = false;

}
