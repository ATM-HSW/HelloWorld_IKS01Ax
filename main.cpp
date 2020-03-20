/**
 ******************************************************************************
 * @file    main.cpp
 * @author  SRA
 * @version V1.0.0
 * @date    5-March-2019
 * @brief   Simple Example application for using the X_NUCLEO_IKS01A3
 *          MEMS Inertial & Environmental Sensor Nucleo expansion board.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/

/* Includes */
#include "mbed.h"
#include "rtos.h"

//#define IKS01A1
//#define IKS01A2
#define IKS01A3

#ifdef IKS01A1
#include "x_nucleo_iks01a1.h"

#define BOARD_NAME "X_NUCLEO_IKS01A1"

/* Instantiate the expansion board */
static X_NUCLEO_IKS01A1 *mems_expansion_board = X_NUCLEO_IKS01A1::Instance(D14, D15, D4);

/* Retrieve the composing elements of the expansion board */
static HTS221Sensor *hum_temp       = mems_expansion_board->ht_sensor;
static LPS25H *press_temp           = mems_expansion_board->pt_sensor;
static TempSensor *temp             = NULL;
static MagneticSensor *magnetometer = mems_expansion_board->magnetometer;
static LSM6DSOSensor *acc_gyro      = mems_expansion_board->acc_gyro;
static MotionSensor *accelerometer  = NULL; //mems_expansion_board->GetAccelerometer();
#endif

#ifdef IKS01A2
#include "XNucleoIKS01A2.h"

#define BOARD_NAME "X_NUCLEO_IKS01A2"

/* Instantiate the expansion board */
static XNucleoIKS01A2 *mems_expansion_board = XNucleoIKS01A2::instance(D14, D15, D4, D5);

/* Retrieve the composing elements of the expansion board */
static HTS221Sensor            *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HBSensor         *press_temp = mems_expansion_board->pt_sensor;
static TempSensor                  *temp = NULL;
static LSM303AGRMagSensor  *magnetometer = mems_expansion_board->magnetometer;
static LSM303AGRAccSensor *accelerometer = mems_expansion_board->accelerometer;
static LSM6DSLSensor           *acc_gyro = mems_expansion_board->acc_gyro;
#endif

#ifdef IKS01A3 
#include "XNucleoIKS01A3.h"

#define BOARD_NAME "X_NUCLEO_IKS01A3"

/* Instantiate the expansion board */
static XNucleoIKS01A3 *mems_expansion_board = XNucleoIKS01A3::instance(D14, D15, D4, D5, A3, D6, A4);

/* Retrieve the composing elements of the expansion board */
static HTS221Sensor *hum_temp        = mems_expansion_board->ht_sensor;
static LPS22HHSensor *press_temp     = mems_expansion_board->pt_sensor;
static STTS751Sensor *temp           = mems_expansion_board->t_sensor;
static LIS2MDLSensor *magnetometer   = mems_expansion_board->magnetometer;
static LSM6DSOSensor *acc_gyro       = mems_expansion_board->acc_gyro;
static LIS2DW12Sensor *accelerometer = mems_expansion_board->accelerometer;
#endif


/* Helper function for printing floats & doubles */
static char *print_double(char *str, double v, int decimalDigits = 2) {
  int i = 1;
  int intPart, fractPart;
  int len;
  char *ptr;

  /* prepare decimal digits multiplicator */
  for (; decimalDigits != 0; i *= 10, decimalDigits--);

  /* calculate integer & fractinal parts */
  intPart = (int)v;
  fractPart = (int)((v - (double)(int)v) * i);

  /* fill in integer part */
  sprintf(str, "%i.", intPart);

  /* prepare fill in of fractional part */
  len = strlen(str);
  ptr = &str[len];

  /* fill in leading fractional zeros */
  for (i /= 10; i > 1; i /= 10, ptr++) {
    if (fractPart >= i) {
      break;
    }
    *ptr = '0';
  }

  /* fill in (rest of) fractional part */
  sprintf(ptr, "%i", fractPart);

  return str;
}

/* Simple main function */
int main() {
  uint8_t id, isLSM303AGR=false;
  float value1, value2;
  char buffer1[32], buffer2[32];
  int32_t axes[3];
  char *acc_gyro_name;
  char *press_temp_name;
  char *hum_temp_name;
  char *magnetometer_name;
	char *accelerometer_name;
	char *temp_name;

  /* Enable all sensors */
  hum_temp->enable();
  press_temp->enable();
  if(temp!=NULL) temp->enable();
  magnetometer->enable();
  if(accelerometer!=NULL) accelerometer->enable();
  acc_gyro->enable();

  printf("\r\n--- Starting new run ---\r\n");
	printf("--- %s ---\r\n", BOARD_NAME);

  hum_temp->read_id(&id);
  switch(id) {
    case 0xbc:
    hum_temp_name = (char*)"HTS221";
    break;
    default:
    hum_temp_name = (char*)"unknown";
    break;
  }
  printf("humidity & temperature       = 0x%X(%s)\r\n", id, hum_temp_name);

  press_temp->read_id(&id);
  switch(id) {
    case 0xbd:
      press_temp_name = (char*)"LPS25H";
      break;
    case 0xb3:
      press_temp_name = (char*)"LPS22HH";
      break;
    case 0xb1:
      press_temp_name = (char*)"LPS22HB";
      break;
    default:
      press_temp_name = (char*)"unknown";
      break;
  }
  printf("pressure & temperature       = 0x%X(%s)\r\n", id, press_temp_name);

  if(temp!=NULL) {
		temp->read_id(&id);
		switch(id) {
			case 0x53:
				temp_name = (char*)"STTS751";
				break;
			default:
				temp_name = (char*)"unknown";
				break;
		}
    printf("temperature                  = 0x%X(%s)\r\n", id, temp_name);
	}

  if(accelerometer!=NULL) {
		accelerometer->read_id(&id);
		switch(id) {
			case 0x44:
				accelerometer_name = (char*)"LIS2DW12";
				break;
			case 0x33:
				accelerometer_name = (char*)"LSM303AGR";
			  isLSM303AGR = true;
				break;
			default:
				accelerometer_name = (char*)"unknown";
				break;
		}
    printf("accelerometer                = 0x%X(%s)\r\n", id, accelerometer_name);
	}

  magnetometer->read_id(&id);
  switch(id) {
    case 0x3d:
      magnetometer_name = (char*)"LIS3MDL";
      break;
    case 0x40:
			if(!isLSM303AGR)
				magnetometer_name = (char*)"LIS2MDL";
			else
			  magnetometer_name = (char*)"LSM303AGR"; // LSM303AGR has 2 i2c addressen and two WhoAmI registers 0x0f (for accelerometer) and 0x4f (for magnetometer)
			                                          // the magnetometer id is identical to LIS2MDL
      break;
    default:
      magnetometer_name = (char*)"unknown";
      break;
  }
  printf("magnetometer                 = 0x%X(%s)\r\n", id, magnetometer_name);

  acc_gyro->read_id(&id);
  switch(id) {
    case 0x68:
			// the chip id correspondes to LSM9DS1 and not LSM6DSO but the chip doesn't behaves like an LSM9DS1
      acc_gyro_name = (char*)"LSM6DSO ?? LSM9DS1";
      break;
    case 0x6c:
      acc_gyro_name = (char*)"LSM6DSO";
      break;
    case 0x6a:
      acc_gyro_name = (char*)"LSM6DSL";
      break;
    default:
      acc_gyro_name = (char*)"unknown";
      break;
  }
  printf("accelerometer & gyroscope id = 0x%X(%s)\r\n", id, acc_gyro_name);

  while (1) {
    printf("\r\n");

    hum_temp->get_temperature(&value1);
    hum_temp->get_humidity(&value2);
    printf("%s [temp]: %7s C,   [hum] %s%%\r\n", hum_temp_name, print_double(buffer1, value1), print_double(buffer2, value2));

    press_temp->get_temperature(&value1);
    press_temp->get_pressure(&value2);
    printf("%s [temp]: %7s C, [press] %s mbar\r\n", press_temp_name, print_double(buffer1, value1), print_double(buffer2, value2));

    if(temp!=NULL) {
			temp->get_temperature(&value1);
      printf("%s: [temp] %7s C\r\n", temp_name, print_double(buffer1, value1));
		}

    printf("---\r\n");

    magnetometer->get_m_axes(axes);
    printf("%s [mag/mgauss]:  %6d, %6d, %6d\r\n", magnetometer_name, axes[0], axes[1], axes[2]);

    if(accelerometer!=NULL) {
			accelerometer->get_x_axes(axes);
		  printf("%s [acc/mg]:  %6d, %6d, %6d\r\n", accelerometer_name, axes[0], axes[1], axes[2]);
		}

    acc_gyro->get_x_axes(axes);
    printf("%s [acc/mg]:      %6d, %6d, %6d\r\n", acc_gyro_name, axes[0], axes[1], axes[2]);

    acc_gyro->get_g_axes(axes);
    printf("%s [gyro/mdps]:   %6d, %6d, %6d\r\n", acc_gyro_name, axes[0], axes[1], axes[2]);

    ThisThread::sleep_for(10000);
  }
}
