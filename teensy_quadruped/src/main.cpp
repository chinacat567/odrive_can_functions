/*  Sumantra Sharma, Jan 14th 2019 */
#include <Metro.h>
#include <FlexCAN.h>

#define NODEID_0 = 0;
#define NODEID_1 = 1;
#define ODRIVE_HEARTBEAT        1
#define ESTOP                   2
#define GET_MOTOR_ERROR         3/*IMPLEMENTED IN REQUEST_MSG*/
#define GET_ENCODER_ERROR       4/*IMPLEMENTED IN REQUEST_MSG*/
#define SET_AXIS_NODE_ID        6
#define SET_AXIS_REQUESTED_STATE  7
#define GET_ENCODER_ESTIMATE    9 /*IMPLEMENTED IN REQUEST_MSG*/
#define GET_ENCODER_COUNTS      10 /*IMPLEMENTED IN REQUEST_MSG*/
#define MOVE_TO_POS             11
#define SET_POS_SETPOINT        12
#define SET_VEL_SETPOINT        13
#define SET_CUR_SETPOINT        14
#define SET_VEL_LIMIT           15
#define START_ANTI_COGGING      16
#define SET_TRAJ_VEL_LIMIT      17
#define SET_TRAJ_ACCEL_LIMIT    18
#define SET_TRAJ_A_PER_CSS      19
#define GET_IQ_VALUES           20 /*IMPLEMENTED IN REQUEST_MSG*/
#define REBOOT_ODRIVE           22
#define GET_VBUS_VOLTAGE        23 /*IMPLEMENTED IN REQUEST_MSG*/
#define SET_VEL_PI_GAIN         24
#define BIT_MASK_0 0xFF
#define BIT_MASK_1 0xFF00
#define BIT_MASK_2 0xFF0000
#define BIT_MASK_3 0xFF000000

#define K_P 0.00125

Metro sysTimer = Metro(1);
IntervalTimer myTimer;

int led = 13;
FlexCAN Can0(1000000);
static CAN_message_t tx_msg;
static CAN_message_t rx_msg;

static int32_t node0_count;
static int32_t node1_count;
/*define filters for receving only specific RTR responses*/

CAN_filter_t filter1;
CAN_filter_t filter2;

/*  -----------------------------------odrive functions-----------------------------   */
void request_motor_error(uint32_t node_id)
{
	tx_msg.id = (GET_MOTOR_ERROR | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write motor error request");
  Serial.println(tx_msg.id);


}
void request_encoder_error(uint32_t node_id)
{
	tx_msg.id = (GET_ENCODER_ERROR | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write encoder error request");

}
void request_encoder_estimate(uint32_t node_id)
{
	tx_msg.id = (GET_ENCODER_ESTIMATE | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write encoder estimate request");
    Serial.println(tx_msg.id);


}
void request_encoder_counts(uint32_t node_id)
{
	tx_msg.id = (GET_ENCODER_COUNTS | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write encoder counts request");

}
void request_iq_values(uint32_t node_id)
{
	tx_msg.id = (GET_IQ_VALUES | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write request_iq_values request");

}

void request_vbus_voltage(uint32_t node_id)
{
	tx_msg.id = (GET_VBUS_VOLTAGE | node_id << 5);
  if(!Can0.write(tx_msg)) Serial.println("Failed to write vbus voltage request");

}

void set_cur_setpoint(uint32_t node_id, int32_t cur_setpoint)
{
  tx_msg.id = (SET_CUR_SETPOINT | node_id << 5);
  tx_msg.buf[0] = ((cur_setpoint*100) & BIT_MASK_0);
  tx_msg.buf[1] = ((cur_setpoint*100) & BIT_MASK_1) >> 8;
  tx_msg.buf[2] = ((cur_setpoint*100) & BIT_MASK_2) >> 16;
  tx_msg.buf[3] = ((cur_setpoint*100) & BIT_MASK_3) >> 24;
  if(!Can0.write(tx_msg)) Serial.println("Failed to write current setpoint");

  
}



/* ------------------------setup----------------------------------- */

void setup(void)
{
  Can0.begin();
  // while(!Serial);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  delay(1000);
  Serial.println(F("Hello Teensy 3.1 CAN Test."));

  sysTimer.reset();
  tx_msg.rtr = 1;
  tx_msg.len = 8;
  myTimer.begin(rt_loop, 1000); // run this function every 1ms

  // initialize the filters
  filter1.ext = 0;
  filter1.id = (GET_ENCODER_COUNTS | NODEID_0 << 5);
  filter1.rtr = 0;

  filter2.ext = 0;
  filter2.id = (GET_ENCODER_COUNTS | NODEID_1 << 5);
  filter2.rtr = 0;
}


/*  ----------------------loop------------------------------------ */


void rt_loop()
{

  digitalWrite(led, 1);
  Serial.println(".");
  uint8_t a,b,c,d,e,f,g,h; //temp storage variale for the CAN data packets


  /*rtr request msgs*/
  request_encoder_counts(nodeid_1);
  request_encoder_counts(nodeid_0);

  /* read CAN bus data */  
  while(Can0.read(rx_msg))
  {
    
    a = rx_msg.buf[0];  
    b = rx_msg.buf[1];
    c = rx_msg.buf[2];
    d = rx_msg.buf[3];

    e = rx_msg.buf[4];
    f = rx_msg.buf[5];
    g = rx_msg.buf[6];
    h = rx_msg.buf[7];

    if((rx_msg.id & NODE_ID_MASK) >> 5 == NODEID_0)
    {
      node0_count =(e | f << 8 | g << 16 | h << 24);  //SIGNED INT 
    }
    else if((rx_msg.id & NODE_ID_MASK) >> 5 == NODEID_1)
    {
      node1_count =(e | f << 8 | g << 16 | h << 24);  //SIGNED INT 
    }
    else
    {
      Serial.println("Invalid CAN frame , exiting rt_loop");
      return;
    }


  }


  digitalWrite(led, 0);

}


