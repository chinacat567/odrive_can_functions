#include<odrive_can_functions.hpp>

#define FRONT_LEFT_LEG   
#define FRONT_RIGHT_LEG
#define BACK_LEFT_LEG
#define BACK_RIGHT_LEG

#define NO_OF_LIMB_MOTORS 12
#define NO_OF_LIMB 4 
#define NO_OF_STANCE 4

using namespace std;
//define command ids

class motion{

public:
	motion();

	void initializeBot();
	void retrievedata();
	void controlLoop();
	void sendData();

	//control modes
	void positionMode();
	void velocityMode();
	void torqueMode();

	//interface and dianogistics
	void enter_menu_state();
	void calibrate();
	void print_encoder();	


	void storebuffer();



	//auxillary math for conversion to CAN format if needed
	//independent of architecture 
	u_int8_t* floatToByteArray(float f);
	float byteToFloatArray(u_int8_t *c) ;
	//dependent on architecture, Endian-ness must be same
	void loat2Bytes(float float_variable,u_int8_t bytes_temp[4]);
	void bytes2Float(u_int8_t bytes_temp[4],float* float_variable);





private:
	int32_t stance0[12]; //sitting neutral stance
	int32_t stance1[12]; //standing neutral stance
	int32_t stance2[12]; //leg lifted pose
	int32_t stance3[12]; //back to ground, legs slightly forward
	char sStanceDataReady = 0;
	char sLearningStance = 0;

	int i;
	controller pc104;
	FILE *fptr;

};
