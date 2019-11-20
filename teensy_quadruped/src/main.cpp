// -------------------------------------------------------------
// CANtest for Teensy 3.1
// by teachop
//
// This test is talking to a single other echo-node on the bus.
// 6 frames are transmitted and rx frames are counted.
// Tx and rx are done in a way to force some driver buffering.
// Serial is used to print the ongoing status.
//

#include <Metro.h>
#include <FlexCAN.h>

Metro sysTimer = Metro(1);// milliseconds

int led = 13;
FlexCAN CANbus(1000000);
static CAN_message_t tx_msg;

unsigned int txTimer;

const uint32_t rtr = 1;
const uint32_t get_motor_error = 3; /*implemented in request_msg*/
const uint32_t get_encoder_error = 4;/*implemented in request_msg*/
const uint32_t get_encoder_estimate = 9; /*implemented in request_msg*/
const uint32_t get_encoder_counts = 10; /*implemented in request_msg*/
const uint32_t get_iq_values = 20; /*implemented in request_msg*/
const uint32_t get_vbus_voltage = 23; /*implemented in request_msg*/


// -------------------------------------------------------------
void request_motor_error(uint32_t node_id)
{
	tx_msg.id = (get_motor_error | node_id << 5 | rtr<<30);
  CANbus.write(tx_msg);
  Serial.println("Wrote motor error request");

}
void request_encoder_error(uint32_t node_id)
{
	tx_msg.id = (get_encoder_error | node_id << 5 | rtr<<30);
  CANbus.write(tx_msg);

}
void request_encoder_estimate(uint32_t node_id)
{
	tx_msg.id = (get_encoder_estimate | node_id << 5 | rtr<<30);
  CANbus.write(tx_msg);
  Serial.println("Wrote encoder estimate request");

}
void request_encoder_counts(uint32_t node_id)
{
	tx_msg.id = (get_encoder_counts | node_id << 5 | rtr<<30);
  CANbus.write(tx_msg);

}
void request_iq_values(uint32_t node_id)
{
	tx_msg.id = (get_iq_values | node_id << 5 | rtr<<30);
  CANbus.write(tx_msg);

}void request_vbus_voltage(uint32_t node_id)
{
	tx_msg.id = (get_vbus_voltage | node_id << 5 | rtr<<30);
  CANbus.write(tx_msg);

}

void setup(void)
{
  CANbus.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(9600); 
  while(!Serial);
  Serial.println(F("Teensy writing on CANBUS"));
  sysTimer.reset();

   for (int i = 0; i < 8; i++)
  {
    tx_msg.buf[i] = 0;
  }
}


// -------------------------------------------------------------
void loop(void)
{
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
  }
 
  // insert a time delay between transmissions
  if ( !txTimer ) {
    
   
    txTimer = 100;//milliseconds
    tx_msg.len = 8;
   
      for (int i = 1; i < 13; i++)
      {
        request_motor_error(i);
        // request_encoder_error(i);
        request_encoder_estimate(i);
        // request_encoder_counts(i);
        // request_iq_values(i);
        // request_vbus_voltage(i);
      }
      
    digitalWrite(led, 0);
   
  }







}
