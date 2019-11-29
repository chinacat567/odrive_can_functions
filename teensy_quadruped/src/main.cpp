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
FlexCAN Can0(1000000);
static CAN_message_t tx_msg;



unsigned int txTimer;
int txCount;

const uint32_t nodeid_0 = 7;
const uint32_t nodeid_1 = 8;
const uint32_t get_motor_error = 3; /*implemented in request_msg*/
const uint32_t get_encoder_error = 4;/*implemented in request_msg*/
const uint32_t get_encoder_estimate = 9; /*implemented in request_msg*/
const uint32_t get_encoder_counts = 10; /*implemented in request_msg*/
const uint32_t get_iq_values = 20; /*implemented in request_msg*/
const uint32_t get_vbus_voltage = 23; /*implemented in request_msg*/


// -----------------------------------request functions-----------------------------//
void request_motor_error(uint32_t node_id)
{
	tx_msg.id = (get_motor_error | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write motor error request");
  else Serial.println(tx_msg.id);


}
void request_encoder_error(uint32_t node_id)
{
	tx_msg.id = (get_encoder_error | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write encoder error request");

}
void request_encoder_estimate(uint32_t node_id)
{
	tx_msg.id = (get_encoder_estimate | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write encoder estimate request");
  else Serial.println(tx_msg.id);

}
void request_encoder_counts(uint32_t node_id)
{
	tx_msg.id = (get_encoder_counts | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write encoder counts request");

}
void request_iq_values(uint32_t node_id)
{
	tx_msg.id = (get_iq_values | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write request_iq_values request");

}void request_vbus_voltage(uint32_t node_id)
{
	tx_msg.id = (get_vbus_voltage | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write vbus voltage request");

}


// ------------------------setup-----------------------------------//
void setup(void)
{
  Can0.begin();
  while(!Serial);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  delay(1000);
  Serial.println(F("Hello Teensy 3.1 CAN Test."));

  sysTimer.reset();
  tx_msg.rtr = 1;
  tx_msg.len = 8;
}


// ----------------------loop------------------------------------//
void loop(void)
{
  // service software timers based on Metro tick
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
  }

  // insert a time delay between transmissions
  if ( !txTimer ) {
    txTimer = 100;//milliseconds
   
    // send 6 at a time to force tx buffering
    txCount = 6;
    digitalWrite(led, 1);
    Serial.println(".");
    request_encoder_estimate(nodeid_0);
    request_encoder_estimate(nodeid_1);
    // request_motor_error(nodeid_0);
    // request_motor_error(nodeid_1);
    // request_encoder_error(uint32_t node_id);
    // request_encoder_counts(uint32_t node_id);
    // request_iq_values(uint32_t node_id);
    // request_vbus_voltage(uint32_t node_id);
    digitalWrite(led, 0);
  }

}
