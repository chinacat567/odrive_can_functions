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
int position;
int velocity;
bool flag = true;
FlexCAN Can0(1000000);
static CAN_message_t tx_msg;


const uint32_t BIT_MASK_0 = 0xFF;
const uint32_t BIT_MASK_1 = 0xFF00;
const uint32_t BIT_MASK_2 = 0xFF0000;
const uint32_t BIT_MASK_3 = 0xFF000000;
const uint32_t BIT_MASK_4 = 0xFF00000000;
const uint32_t BIT_MASK_5 = 0xFF0000000000;
const uint32_t BIT_MASK_6 = 0xFF000000000000;
unsigned int txTimer;
int txCount;

const uint32_t set_pos_cmd = 12;
const uint32_t set_vel_cmd = 13;
const uint32_t set_cur_cmd = 14;



// -----------------------------------some test functions-----------------------------//
void mov_to_pos(uint32_t node_id, int32_t pos)
{
	tx_msg.id =(node_id <<5 | set_pos_cmd);
  tx_msg.buf[0] = (pos & BIT_MASK_0); 
  tx_msg.buf[1] = (pos & BIT_MASK_1) >> 8;
  tx_msg.buf[2] = (pos & BIT_MASK_2) >> 16;
  tx_msg.buf[3] = (pos & BIT_MASK_3) >> 24;
  Can0.write(tx_msg);

}

void set_vel_setpoint(uint32_t node_id, int32_t vel_setpoint , int16_t current_ff)
{
	tx_msg.id =(node_id <<5 | set_vel_cmd);

	tx_msg.buf[0] = ((vel_setpoint*100) & BIT_MASK_0);
	tx_msg.buf[1] = ((vel_setpoint*100) & BIT_MASK_1) >> 8;
	tx_msg.buf[2] = ((vel_setpoint*100) & BIT_MASK_2) >> 16;
	tx_msg.buf[3] = ((vel_setpoint*100) & BIT_MASK_3) >> 24;

	tx_msg.buf[4] = ((current_ff*100) & BIT_MASK_0) ;
	tx_msg.buf[5] = ((current_ff*100) & BIT_MASK_1) << 8;

  Can0.write(tx_msg);	
	
}


// ------------------------setup-----------------------------------//
void setup(void)
{
  Can0.begin();
  // while(!Serial);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  delay(1000);
  Serial.println(F("Hello Teensy 3.1 CAN Test."));

  sysTimer.reset();
  tx_msg.rtr = 0;
  tx_msg.len = 8;
}


// ----------------------loop------------------------------------//
void loop(void)
{ 
  if(flag)
  {
    flag = false;
    velocity = 10000;
  } 
  else
  {
    flag = true;
    velocity = 30000;
  }
  
  // service software timers based on Metro tick
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
  }

  // insert a time delay between transmissions
  if ( !txTimer ) {
    txTimer = 10;//milliseconds
   
    // send 6 at a time to force tx buffering
    txCount = 6;
    digitalWrite(led, 1);
    Serial.println(".");
    set_vel_setpoint(17, velocity, 0);
    digitalWrite(led, 0);
  }

}
