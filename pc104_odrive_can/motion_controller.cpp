/****************************************************************
Motion Controller

Objective: 
Provide base class for high level controller to interface 
with low level controller and implement the control function of
gait planning and force control.

Requirements v1.0:s
1) Force Control: 
	Provide efficient way to 
	a)obtain values from low level leg controller
	b)store values
	c)process values
	b)set values to low level leg controller
	e)handshake to check integrity of motor
2) Gait planning:
	Create state machine operation 
	a)define states: sit, stand, prance, walk(???)
	b)enabling tracking of current state
	c)polling for event triggers initiating transition to the next state
	d)maintain motor interlock to ensure no more than 2 legs in the air
3) provide a means of logging, error handling and processing for debugging purpses
4) initiatise must set defaults, clear all error

Ver 1.0 - CHLOW - base 




****************************************************************/

	void motion::motion(){

		//initialize controller
		initializeBot()


		//start separate thread to loop and read CAN messages
		pc104.start_internal_thread();


		for (int i=0;i<NO_OF_LIMB; i++){
			pc104.legs[i].axis_error = 0;
		    pc104.legs[i].axis_current_state = 0;
		    pc104.legs[i].motor_error = 0;
		    pc104.legs[i].encoder_error = 0;
		    pc104.legs[i].sensorless_error = 0;
		    pc104.legs[i].axis_requested_state = 0;
		    pc104.legs[i].encoder_pos_estimate = 0;
		    pc104.legs[i].encoder_vel_estimate = 0;
		    pc104.legs[i].encoder_shadow_count = 0;
		    pc104.legs[i].encoder_count_in_cpr = 0;
		    pc104.legs[i].iq_setpoint = 0;
		    pc104.legs[i].iq_measured = 0;
		    pc104.legs[i].vbus_voltage = 0; 
		    

		    pc104.legs[i].sensorless_pos_estimate = 0;
		    pc104.legs[i].sensorless_vel_estimate = 0;
		    pc104.legs[i].velocity_p_gain = 0; 
		    pc104.legs[i].velocity_i_gain = 0; 
		    pc104.legs[i].goal_position = 0;
       	 	pc104.legs[i].vel_limit = 0;
		    pc104.legs[i].traj_vel_limit = 0;
		    pc104.legs[i].traj_accel_limit = 0;
		    pc104.legs[i].decel_limit = 0;
		    pc104.legs[i].decel_limit = 0;
		    pc104.legs[i].traj_a_per_css = 0;

		}
	
	}

	void motion::initializeBot(uint32_t node_id, uint32_t axis_requested_state){
		//perform calibration, 
		pc104.set_axis_requested_state(node_id, axis_requested_state); 

		//set position_velocity_current PID gains....


		//load stance position data
		if (loadPosition()){
			char sStanceDataReady = 1; //signify successfully data loaded
		}

	}

	int motion::loadPosition()
	{
		int i;
		char temp[25];

		fp = fopen("/home/l/positionDATA.txt","r");
		//check for text file with previously saved position data . If text file does not exist, exit and request for postion data
		if (fp = NULL)
			return 0;

		//get stance0 data
		while(fgets(temp, 25, fp) != NULL) {
			if((strstr(temp, "sitting_neutral")) != NULL) {
			printf("%s",temp);
			break;
			}   
		}
		for (i=0;i<NO_OF_LIMB_MOTORS;i++){
		fscanf(fp, "%f", &stance0[i]);
	  	}

	  	//get stance1 data
	  	fseek(fp, 0, SEEK_SET);
		while(fgets(temp, 25, fp) != NULL) {
			if((strstr(temp, "standing_neutral")) != NULL) {
			printf("%s",temp);
			break;
			}   
		}
		for (i=0;i<NO_OF_LIMB_MOTORS;i++){
		fscanf(fp, "%f", &stance1[i]);
	  	}

	  	//get stance2 data
	  	fseek(fp, 0, SEEK_SET);
		while(fgets(temp, 25, fp) != NULL) {
			if((strstr(temp, "leg_lifted")) != NULL) {
			printf("%s",temp);
			break;
			}   
		}
		for (i=0;i<NO_OF_LIMB_MOTORS;i++){
		fscanf(fp, "%f", &stance2[i]);
	  	}

	  	//get stance3 data
	  	fseek(fp, 0, SEEK_SET);
		while(fgets(temp, 25, fp) != NULL) {
			if((strstr(temp, "legs_slightly_forward")) != NULL) {
			printf("%s",temp);
			break;
			}   
		}
		for (i=0;i<NO_OF_LIMB_MOTORS;i++){
		fscanf(fp, "%f", &stance3[i]);
	  	}

	  	fclose(fp);
		return 1;
	} 

	void motion::retrievedata(){
		//continuously loop this code to obtain values from CAN bus
		
		//call message handler via CAN
		
		//stitch back data
		
		//Separate into respective location
		

	}


	
	void motion::controlLoop(){
		/******************************************************
		for simiplicity, doing proportional P control
		******************************************************/
		
		//retrieve data from buffer

		//compute

		//write output to corresponding motor

		

	}

	
	void motion::sendData(){
		
		//get data from control loop

		//break down data

		//separate into respective location

		//send via can
		

	}

	//control modes
	void motion::positionMode(){
		

	}
	void motion::velocityMode(){


	}
	void motion::torqueMode(){
		
	}

	void motion::enter_menu_state(void){

		//UX for setting up
		//in case need to calibrate before running
		
		char input;
		printf("\n\r\n\r\n\r");
	    printf(" Commands:\n\r");
	    wait_us(10);
	    printf(" m - Motor Mode\n\r");
	    wait_us(10);
	    printf(" c - Calibrate Encoder\n\r");
	    wait_us(10);
	    printf(" e - Display Encoder\n\r");
	    wait_us(10);
	    printf(" l - Learn Positions\n\r");
	    wait_us(10);
	   	printf(" g - Go to Positions\n\r");
	    wait_us(10);
	    printf(" q - Exit Menu and continue\n\r");
	    wait_us(10);
	    getchar(" %c", &input);

	    //react according to input
	    switch(input){
	    	case m:
	    		break;
	    	case c:
	    		initializeBot();
	    		break;
	    	case e:
	    		print_encoder();
	    		break;
	    	case l:
	    		learnPosition();
	    		break;
	    	case g:
	    		gotoPosition();
	    		break;
	    	case q:
	    		return;


	    }

	}

	void motion::print_encoder(void){


	}
	void motion::savePosition(float * storeThis, FILE * fp, char * positionName){
		int i;

		fp = fopen("/home/l/positionDATA.txt","a+");
  		fprintf(fp,"#%s:\n",positionName);
    	for (i=0;i<NO_OF_LIMB_MOTORS;i++){
      		fprintf(fp,"%f ",*storeThis++);
	    }
	    fprintf(fp,"\n" );
	    fclose(fp);
	}

	void motion::learnPosition(void)
	{
		//function for pre-experiment calibration
		//memorize postions for position control

		//standby to receive postion data
		char input;
		
		//For safety, only begin learning position from Sit position
		//If no sit position data, save current values. 
		//During this learning "state", manually move doggy to desired location
		//and press the respective learning buttons to save position data
		if (sSit())
		{
		    printf(" s - Print Positions\n\r");
		    wait_us(10);
		    printf(" 0 - Learn Sit\n\r");
		    wait_us(10);
		    printf(" 1 - Learn Standr\n\r");
		    wait_us(10);
		    printf(" 2 - Learn Leg Lift\n\r");
		    wait_us(10);
		    printf(" 3 - Learn Leg Forward\n\r");
		    wait_us(10);
		    printf(" q - Exit Menu and continue\n\r");
		    wait_us(10);
		    getchar(" %c", &input);

			//get input from encoder
			switch (input)
			{
				case 0:
				for (i = 0; i<NO_OF_LIMB; i++){
					stance0[i]= pc104.legs[i].encoder_pos_estimate; //hip 
					stance0[i+1]= pc104.legs[i].encoder_pos_estimate; //knee
					stance0[i+2]= pc104.legs[i].encoder_pos_estimate; //adduction
					printf("Position saved! Hip %f Knees %f Adduction %f\n", stance1[i], stance1[i+1], stance1[i+2]);
					break;
				}
				case 1:
				for (i = 0; i<NO_OF_LIMB; i++){
					stance1[i]= pc104.legs[i].encoder_pos_estimate; //hip 
					stance1[i+1]= pc104.legs[i].encoder_pos_estimate; //knee
					stance1[i+2]= pc104.legs[i].encoder_pos_estimate; //adduction
					printf("Position saved! Hip %f Knees %f Adduction %f\n", stance1[i], stance1[i+1], stance1[i+2]);
					break;
				}
				case 1:
				for (i = 0; i<NO_OF_LIMB; i++){
					stance2[i]= pc104.legs[i].encoder_pos_estimate; //hip 
					stance2[i+1]= pc104.legs[i].encoder_pos_estimate; //knee
					stance2[i+2]= pc104.legs[i].encoder_pos_estimate; //adduction
					printf("Position saved! Hip %f Knees %f Adduction %f\n", stance1[i], stance1[i+1], stance1[i+2]);
					break;
				}
				case 3:
				for (i = 0; i<NO_OF_LIMB; i++){
					stance3[i]= pc104.legs[i].encoder_pos_estimate; //hip 
					stance3[i+1]= pc104.legs[i].encoder_pos_estimate; //knee
					stance3[i+2]= pc104.legs[i].encoder_pos_estimate; //adduction
					printf("Position saved! Hip %f Knees %f Adduction %f\n", stance1[i], stance1[i+1], stance1[i+2]);
					break;
				}
				case s:
					//save to text file
					savePosition(stance0,fptr,"sitting_neutral")
					savePosition(stance1,fptr,"standing_neutral")
					savePosition(stance2,fptr,"leg_lifted")
					savePosition(stance3,fptr,"legs_slightly_forward")
					break;
			}
		} 
	}

	void motion::gotoPosition(void){
		//check if position data available
		
		if (sStanceDataReady == 0 ){
			printf("Position data not available\n\r");
			return;
		}

		char input;

			printf(" 1 - Go to Sit\n\r");
		    wait_us(10);
		    printf(" 2 - Go to Standr\n\r");
		    wait_us(10);
		    printf(" 3 - Go to Prance\n\r");
		    wait_us(10);
		    printf(" 4 - Go to Walk\n\r");
		    wait_us(10);
		    printf(" q - Exit Menu and continue\n\r");
		    wait_us(10);
		    getchar(" %c", &input);

	    //react according to input
	    switch(input){
	    	case 1:
	    		sSit();
	    		break;
	    	case 2:
	    		sStand();
	    		break;
	    	case 3:
	    		sPrance();
	    		break;
	    	case 4:
	    		sWalk();
	    		break;
	    	case q:
	    		return;
	    }
	}

	int motion::sSit(void){
		//return 1 if successful. Else, call error handler
		
		/**************************************
		ASSUMPTION: stupid dog is booted up in sitting position
		**************************************/
		if (sStanceDataReady == 0){
			//on first boot, when preload not ready. get input from encoder
			
			for (i = 0; i<NO_OF_LIMB; i++){
				stance0[i]= pc104.legs[i].encoder_pos_estimate; //hip 
				stance0[i+1]= pc104.legs[i].encoder_pos_estimate; //knee
				stance0[i+2]= pc104.legs[i].encoder_pos_estimate; //adduction
				printf("Sitting! Hip %f Knees %f Adduction %f\n", stance1[i], stance1[i+1], stance1[i+2]);
			}
			return 1;
		}
		else {
			//lower body to ground
			for (i = 0; i<NO_OF_LIMB; i++){
				pc104.legs[i].set_pos_setpoint(stance0[i]); //hip 
				pc104.legs[i].set_pos_setpoint(stance0[i+1]); //knee
				pc104.legs[i].set_pos_setpoint(stance0[i+2]); //adduction
				printf("Position saved! Hip %f Knees %f Adduction %f\n", stance0[i], stance0[i+1], stance0[i+2]);
			}
		}
		
		if (sLearningStance == 1){
			//disengage motor for position adjustment (velocity/current control to 0? AXIS_STATE_IDLE?) 
		
			return 1;
		}
		
		if (	/*take too long*/	){
			errorHandler();
			return 0;
		} 
		else{
			return 1; fclose(fp);

		}
	}

	void motion::sStand(void){
		for (i = 0; i<NO_OF_LIMB; i++)
		{
			pc104.legs[i].set_pos_setpoint(stance0[i]); //hip 
			pc104.legs[i].set_pos_setpoint(stance0[i+1]); //knee
			pc104.legs[i].set_pos_setpoint(stance0[i+2]); //adduction
			printf("Position saved! Hip %f Knees %f Adduction %f\n", stance0[i], stance0[i+1], stance0[i+2]);
		}
		if (	/*take too long*/	){
			errorHandler();
			return 0;
		} 
		else{
			return 1; fclose(fp);

		}
	}

	void motion::sPrance(void){
		//cycle between standing neutral pose and leg lifted pose at a specific freqeuncy
		for (i = 0; i<NO_OF_LIMB; i++)
		{
			pc104.legs[i].set_pos_setpoint(stance0[i]); //hip 
			pc104.legs[i].set_pos_setpoint(stance0[i+1]); //knee
			pc104.legs[i].set_pos_setpoint(stance0[i+2]); //adduction
			printf("Position saved! Hip %f Knees %f Adduction %f\n", stance0[i], stance0[i+1], stance0[i+2]);
		}
		if (	/*take too long*/	){
			errorHandler();
			return 0;
		} 
		else{
			return 1; fclose(fp);

		}

	}

	void motion::sWalk(void){
		//set sequence
		//(Current) Loop through a sequence of 3 different walking "stance"

		//check interlock

	}

	void motion::walkInterlock(void){
		//motion interlock for walk
		//prevents other legs from lifting when 2 legs are lifted 
		//prevents "walk" from being called when dog is sitting

	}

	void motion::errorHandler(void){
		//to be called if position not reached after position command is set. ie. SetPosition times out

		//prompt for possible errors. 
		//if know error returns, save results and reboot
		this->msgreq.get_motor_error(nodeID);
		this->msgreq.get_encoder_error(nodeID);

		pc104.reboot_odrive();
		sleep(10);
		initializeBot();

		//else, wait


	}

	//independent of architecture 
	u_int8_t* motion::floatToByteArray(float f) { 
	    u_int8_t* ret = malloc(4 * sizeof(u_int8_t));
	    int asInt = *((int*)&f);

	    int i;
	    for (i = 0; i < 4; i++) {
	        ret[i] = (asInt >> 8 * i) & 0xFF;
	    }

	    return ret;
	}

	//independent of architecture 
	float motion::byteToFloatArray(u_int8_t *c) { 

	    printf ("byteArray is at %02x\n", &c);
	    printf ("value in byteArray is %02x\n", *c);
	    float asFloat = *((float*)c);

	    return asFloat;
	}

	//architecture dependent float to byte converter (Endian-ness)
	void motion::float2Bytes(float float_variable,u_int8_t bytes_temp[4]){ 
	  union {
	    float a;
	    u_int8_t bytes[4];
	  } thing;
	  thing.a = float_variable;
	  memcpy(bytes_temp, thing.bytes, 4);
	}

	//architecture dependent byte to float converter (Endian-ness)
	void motion::bytes2Float(u_int8_t bytes_temp[4],float* float_variable){ 
	  union {
	    float a;
	    u_int8_t bytes[4];
	  } thing;
	  //swap around for different endian
	  thing.bytes[0] = bytes_temp[0];
	  thing.bytes[1] = bytes_temp[1];
	  thing.bytes[2] = bytes_temp[2];
	  thing.bytes[3] = bytes_temp[3];
	  *(float_variable) = thing.a;
	}
