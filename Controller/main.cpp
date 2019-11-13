#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "math.h"
#include "Adafruit_LIS3DH.h"
#include "Adafruit_ADS1015.h"
#include "float.h"
#include "time.h"

#ifdef __cplusplus
extern "C"
{
	#endif
#include "wiringPi.h"
#include "wiringPiI2C.h"
#ifdef __cplusplus
}
#endif

using namespace std;

int acceli2c, adci2c, signed_axis, stk_btn, a_btn, b_btn,
	blue_sw, input_sw, GPIO_STK, GPIO_A, GPIO_B, GPIO_BLUE, GPIO_INPUT;

uint8_t check_reg, ctrl1_tmp, check_status;

int16_t raw_axis, signed_adc0, signed_adc1, signed_adc2, config, reading;

float g_axis, adc0, adc1, adc2;

clock_t old_time, curr_time, diff_time, update_time,
		stk_btn_time, a_btn_time, b_btn_time,
		blue_btn_time, input_btn_time, btn_debounce, btn_diff_time;

enum STATE {STK_IDLE, STK_DEBOUNCE, STK_PRIMED, STK_READY, STK_EVENT,
			A_IDLE, A_DEBOUNCE, A_PRIMED, A_READY, A_EVENT,
			B_IDLE, B_DEBOUNCE, B_PRIMED, B_READY, B_EVENT,
			BLUE_IDLE, BLUE_DEBOUNCE, BLUE_PRIMED, BLUE_READY, BLUE_EVENT,
			INPUT_IDLE, INPUT_DEBOUNCE, INPUT_PRIMED, INPUT_READY, INPUT_EVENT};

	STATE STK_STATE, A_STATE, B_STATE, BLUE_STATE, INPUT_STATE;

void initialize();
void i2c_init();
void accel_init();
void adc_init();
void gpio_init();
void getGPIO();
void debounce();
void getadc();
void getaccel();
//void makescreen();
//void checkRx();
//void sendTx();

int main(int argc, char* argv[])
{
	initialize();
	
	while (true) {
		
		//get times
		curr_time = clock();//get the current system time
		diff_time = curr_time - old_time;//find difference between current time and last
		//printf ("%d clicks (%f seconds).\n",diff_time,((float)diff_time)/CLOCKS_PER_SEC);
		
		//get current button states
		getGPIO();
		
		//debounce GPIO
		debounce();
		
		//check adc values
		//getadc();
		
		//check accleration axes
		getaccel();
		
	}
}

void initialize(){
	
	i2c_init();
	accel_init();
	adc_init();
	gpio_init();

}

void i2c_init(){
	
	//initialize wiringPi
	wiringPiSetup();
	
	//set LIS3DH device address
	acceli2c = wiringPiI2CSetup(0x18);
	
	//set ADS1015 address
	adci2c = wiringPiI2CSetup(0x48);
	
	printf("I2C initialized"); //confirm I2C init

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
	//check_reg = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_CTRL4);
	//printf("CTRL 4: 0x%X\n", check_reg); //display status reg value
	
	//Set inital values for reading axes
	raw_axis = 0x0000;
	signed_axis = 0;
	
	//value to check axis adc conversion readiness
	 check_status = 0x00;
}

void adc_init(){
	
	//setup variables
	config = 0x0000;
	reading = 0x0000;
	signed_adc0 = 0;
	signed_adc1 = 0;
	signed_adc2 = 0;
}

void gpio_init(){
	
	//gpio variables
	old_time = clock();//get a current time
	update_time = 50000;
	btn_debounce = 500;
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
	BLUE_STATE = BLUE_IDLE;
	INPUT_STATE = INPUT_IDLE;

	//setup gpio internal pullups
	pullUpDnControl(stk_btn, PUD_UP);
	pullUpDnControl(a_btn, PUD_UP);
	pullUpDnControl(b_btn, PUD_UP);
	pullUpDnControl(blue_sw, PUD_UP);
	pullUpDnControl(input_sw, PUD_UP);

}

void getGPIO(){
	
	GPIO_STK = digitalRead(stk_btn);
	GPIO_A = digitalRead(stk_btn);
	GPIO_B = digitalRead(stk_btn);
	GPIO_BLUE = digitalRead(stk_btn);
	GPIO_INPUT = digitalRead(stk_btn);
}

void debounce(){
	
	//debounce stick
	switch(STK_STATE) {
		case STK_IDLE:
			if(GPIO_STK == 0){
				stk_btn_time = clock();
				printf ("Stick button went from 1 to 0.\n");
				printf ("Debouncing...\n");
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
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					STK_STATE = STK_PRIMED;
				} else {
					printf ("Invalid press.\n");
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
			B_STATE = B_IDLE;
			break;
		default:
			break;
	}	

	//debounce BLUE
	switch(BLUE_STATE) {
		case BLUE_IDLE:
			if(GPIO_BLUE == 0){
				blue_btn_time = clock();
				printf ("BLUE button went from 1 to 0.\n");
				printf ("Debouncing...\n");
				BLUE_STATE = BLUE_DEBOUNCE;
			} else {
				BLUE_STATE = BLUE_IDLE;
			}
			break;
		case BLUE_DEBOUNCE:
			btn_diff_time = clock() - blue_btn_time;
			// printf ("BLUE button is %d.\n",GPIO_BLUE);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_BLUE == 0){
					printf ("Legit press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					BLUE_STATE = BLUE_PRIMED;
				} else {
					printf ("Invalid press.\n");
					BLUE_STATE = BLUE_IDLE;
				}
			} else {
				BLUE_STATE = BLUE_DEBOUNCE;
			}
			break;
		case BLUE_PRIMED:
			if(GPIO_BLUE == 0){
				BLUE_STATE = BLUE_PRIMED;
			} else {
				printf ("Button depressed, moving to event.\n");
				BLUE_STATE = BLUE_EVENT;
			}
			break;
		case BLUE_READY:
		// do something in the stop state
			break;
		case BLUE_EVENT:
			printf ("BLUE button went from 1 to 0 back to 1.\n");
			printf ("***************************.\n");
			printf ("***************************.\n");
			BLUE_STATE = BLUE_IDLE;
			break;
		default:
			break;
	}	

	//debounce INPUT
	switch(INPUT_STATE) {
		case INPUT_IDLE:
			if(GPIO_INPUT == 0){
				input_btn_time = clock();
				printf ("INPUT switch went from 1 to 0.\n");
				printf ("Debouncing...\n");
				INPUT_STATE = INPUT_DEBOUNCE;
			} else {
				INPUT_STATE = INPUT_IDLE;
			}
			break;
		case INPUT_DEBOUNCE:
			btn_diff_time = clock() - input_btn_time;
			// printf ("INPUT switch is %d.\n",GPIO_INPUT);
			if(btn_diff_time > btn_debounce) {
				if(GPIO_INPUT == 0){
					printf ("Legit press.\n");
					printf ("%d clocks passed.\n", (int)btn_diff_time);
					printf ("%f seconds passed.\n", ((float)btn_diff_time)/CLOCKS_PER_SEC);
					printf ("Primed for depress.\n");
					INPUT_STATE = INPUT_PRIMED;
				} else {
					printf ("Invalid press.\n");
					INPUT_STATE = INPUT_IDLE;
				}
			} else {
				INPUT_STATE = INPUT_DEBOUNCE;
			}
			break;
		case INPUT_PRIMED:
			if(GPIO_INPUT == 0){
				INPUT_STATE = INPUT_PRIMED;
			} else {
				printf ("Button depressed, moving to event.\n");
				INPUT_STATE = INPUT_EVENT;
			}
			break;
		case INPUT_READY:
		// do something in the stop state
			break;
		case INPUT_EVENT:
			printf ("INPUT switch went from 1 to 0 back to 1.\n");
			printf ("***************************.\n");
			printf ("***************************.\n");
			INPUT_STATE = INPUT_IDLE;
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
		usleep(100000);
		// Read the conversion results
		wiringPiI2CWrite(adci2c, ADS1015_REG_POINTER_CONVERT);
		reading = wiringPiI2CReadReg16(adci2c, ADS1015_REG_POINTER_CONVERT);
		signed_adc0 = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
		signed_adc0 &= 0x0000FFFF; //parse out an extra byte from shifting
		// Shift 12-bit results right 4 bits for the ADS1015
		signed_adc0 = signed_adc0 >> 4; 
		adc0 = signed_adc0 * 0.002; //conversion to voltage 

		printf("ADC 0: %9.6f\n", adc0); //display value as voltage float
		printf("ADC 0: 0x%04X\n", (uint16_t)signed_adc0); //display value as hex from 16bit register
		
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
		usleep(100000);
		// Read the conversion results
		wiringPiI2CWrite(adci2c, ADS1015_REG_POINTER_CONVERT);
		reading = wiringPiI2CReadReg16(adci2c, ADS1015_REG_POINTER_CONVERT);
		signed_adc1 = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
		signed_adc1 &= 0x0000FFFF; //parse out an extra byte from shifting
		// Shift 12-bit results right 4 bits for the ADS1015
		signed_adc1 = signed_adc1 >> 4; 
		adc1 = signed_adc1 * 0.002; //conversion to voltage 

		printf("ADC 1: %9.6f\n", adc1); //display value as voltage float
		printf("ADC 1: 0x%04X\n", (uint16_t)signed_adc1); //display value as hex from 16bit register

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
		usleep(100000);
		// Read the conversion results
		wiringPiI2CWrite(adci2c, ADS1015_REG_POINTER_CONVERT);
		reading = wiringPiI2CReadReg16(adci2c, ADS1015_REG_POINTER_CONVERT);
		reading = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
		//signed_adc &= 0x0000FFFF; //parse out an extra byte from shifting
		// Shift 12-bit results right 4 bits for the ADS1015
		signed_adc2 = reading >> 4; 
		adc2 = signed_adc2 * 0.002; //conversion to voltage 

		printf("ADC 2: %9.6f\n", adc2); //display value as voltage float
		printf("ADC 2: 0x%04X\n", (uint16_t)signed_adc2); //display value as hex from 16bit register

		usleep(500000);
}

void getaccel(){
	//check for accel conversion status
	check_status = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_STATUS2);
	
	if(check_status && (diff_time > update_time)) {
		old_time = curr_time;
		//system("clear");
		printf ("got into update. %d clicks (%f seconds).\n", (int)diff_time,((float)diff_time)/CLOCKS_PER_SEC);
		
		//x axis
		raw_axis = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_X_L);
		raw_axis |= wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_X_H) << 8;
		// Shift 12-bit results right 4 bits for the LIS3DH
		signed_axis = raw_axis >> 4; 
		g_axis = signed_axis * 0.00095; //conversion to g's 
		signed_axis &= 0x0000FFFF; //parse out an extra byte from shifting

		// printf("X axis: %9.6f\n", g_axis); //display value in g's as float
		//printf("X axis: 0x%X\n", signed_axis); //display value as hex from 16bit register
		//usleep(10);

		//y axis
		raw_axis = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Y_L);
		raw_axis |= wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Y_H) << 8;
		// Shift 12-bit results right 4 bits for the LIS3DH
		signed_axis = raw_axis >> 4; 
		g_axis = signed_axis * 0.00095; //conversion to g's 
		signed_axis &= 0x0000FFFF; //parse out an extra byte from shifting

		// printf("Y axis: %9.6f\n", g_axis); //display value in g's as float
		//printf("Y axis: 0x%X\n", signed_axis); //display value as hex from 16bit register
		//usleep(10);
		
		//z axis
		raw_axis = wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Z_L);
		raw_axis |= wiringPiI2CReadReg8(acceli2c, LIS3DH_REG_OUT_Z_H) << 8;
		// Shift 12-bit results right 4 bits for the LIS3DH
		signed_axis = raw_axis >> 4; 
		g_axis = signed_axis * 0.00095; //conversion to g's 
		signed_axis &= 0x0000FFFF; //parse out an extra byte from shifting

		// printf("Z axis: %9.6f\n", g_axis); //display value in g's as float
		//printf("Z axis: 0x%X\n", signed_axis); //display value as hex from 16bit register
		//usleep(10);
	}
}

void makescreen(){
	
}

void checkRx(){
	
}

void sendTx(){
	
}


