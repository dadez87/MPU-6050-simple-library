// Created by Davide Cavaliere
// E-mail: dadez87@gmail.com
// Website: www.monocilindro.com
// 25 February 2016
// This is an example on how to use the "MPU6050mgr" library
// You should connect MPU-6050 to your Arduino using I2C
// In case "AUTOMATIC_PRINT = 1", ASCII data (acceleration, gyroscope, temperature) are sent via Serial once every 1 second
// In case "AUTOMATIC_PRINT = 0", binary raw data (acceleration, gyroscope, temperature) are sent via Serial when you poll the message using "m" character
// For more info, please read the source files "MPU6050mgr.cpp" and "MPU6050mgr.h"

#include <MPU6050mgr.h> // IMU module (if present)

#define MIN_TIME_BETWEEN_PRINTS 1000 // time between prints, in ms
#define AUTOMATIC_PRINT 1 // Enables automatic printing (=1). In case =0, polling printing is active

unsigned long time_last_print = 0; // last time that the MPU6050 was sent to Serial

void setup() {
  Serial.begin(57600);
  MPU6050mgr.begin(); // Initializes IMU module 
}

void loop() {
  unsigned long time_now_tmp = millis(); // Time entering the loop 
  MPU6050mgr.manager(time_now_tmp); // IMU communication manager

#if AUTOMATIC_PRINT == 1
  if ((time_now_tmp - time_last_print) >= MIN_TIME_BETWEEN_PRINTS){
    MPU6050mgr.send_ASCII_data();
    time_last_print=time_now_tmp;
  }
#else if
  if (Serial.available()){ // if any data available on Serial
    char tmp_char = Serial.read(); // read character received on Serial
    if (tmp_char == 'm'){ // if the character read is "m", as "mpu6050"
      uint8_t buffer_temporary[MPU6050_BUFFER_SD_WRITE_SIZE]; // create temporary writing buffer
      if (MPU6050mgr.prepare_SD_packet(buffer_temporary)){ // if there is any packet available on MPU6050 buffer
        Serial.write(buffer_temporary, MPU6050_BUFFER_SD_WRITE_SIZE); // Send IMU data to serial
      }
    }
  }
#endif
}
