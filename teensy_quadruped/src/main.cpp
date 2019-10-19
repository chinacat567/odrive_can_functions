#include <Arduino.h>
#include<ODriveArduino.h>

#define HWSERIAL Serial1
bool firmware_check = false;


//CAN from teensy to main control PC104



//ODrive object
ODriveArduino odrive(HWSERIAL);

void setup() {
  
  Serial.begin(9600);
  while(!Serial);
  HWSERIAL.begin(2000000); //ODrive baud rate is set to 500,000
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  char* device_info_string ;
  
   //make sure custom firmware is installed on the ODrive
   long tic = micros();
  device_info_string = odrive.PrintInfo().c_str();
  long toc = micros();
  Serial.println("Out-in latency: ");
  Serial.print(toc-tic);
  if(!firmware_check)
  {
    const char *parsed_string;
    if(device_info_string[0] == 'F')
    {
      parsed_string = strtok(device_info_string,":");
      parsed_string = strtok(NULL,":");
      if(strcmp(parsed_string," custom firmware"))
      {
        firmware_check = true;
        Serial.println("Firmware check successful");
      } 
      else
      {
        Serial.println("Firmware check failed");
      }
      
    } 
  }
}




       




  
  
  
  


 
  
 
