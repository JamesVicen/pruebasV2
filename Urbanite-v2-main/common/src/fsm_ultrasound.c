/**
 * @file fsm_ultrasound.c
 * @brief Ultrasound sensor FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <string.h>

/* HW dependent includes */

/* Project includes */
#include "fsm_ultrasound.h"
#include "port_ultrasound.h"
#include "port_system.h"
#include "fsm.h"

/* Typedefs --------------------------------------------------------------------*/
/*Global variables---------------------------------------------------------------------------------------*/
static fsm_trans_t fsm_trans_ultrasound[] ={{WAIT_START, check_on, TRIGGER_START, do_start_measurement},{TRIGGER_START, check_trigger_end, WAIT_ECHO_START, check_echo_init}, {WAIT_ECHO_START, check_echo_init, WAIT_ECHO_END, NULL}, {WAIT_ECHO_END, check_echo_received, SET_DISTANCE, do_set_distance}, {SET_DISTANCE, check_new_measurement, TRIGGER_START, do_start_measurement}, {SET_DISTANCE, check_off, WAIT_START, do_stop_measurement}, {-1,NULL,-1,NULL}};
/*Structs---------------------------------------------------------------------------------*/
typedef struct
{
    fsm_t f; //Ultrasound FSM
    uint32_t distance_cm; //How much time the ultrasound has been pressed
    bool status; //Indicate if the ultrasound sensor is active or not
    bool new_measurement; //Flag to indicate if a new measuremente has been completed
    uint32_t ultrasound_id; //Ultrasound ID. Must be unique
    uint32_t distance_arr [FSM_ULTRASOUND_NUM_MEASUREMENTS]; //Array to store the last distance measurements
    uint32_t distance_idx;
} fsm_ultrasound_t;


/* Private functions -----------------------------------------------------------*/
// Comparison function for qsort
int _compare(const void *a, const void *b)
{
    return (*(uint32_t *)a - *(uint32_t *)b);
}

/* State machine input or transition functions */
/**
 * @brief Check if the ultrasound sensor is active and ready to start a new measuremnt
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 * @return true
 * @return false
 */

static bool check_on (fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    bool status_trigger_signal=port_ultrasound_get_trigger_ready(p_fsm->status);
    if (status_trigger_signal){
        return true;
    }
    else {
        return false;
    }
}
/* State machine input or transition functions */
/**
 * @brief Check if the ultrasound sensor has been set to be inactive off
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 * @return true
 * @return false
 */

 static bool check_off (fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    return p_fsm->status; 
}




/* State machine input or transition functions */
/**
 * @brief Check if the ultrasound sensor has finished the trigger signal
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 * @return true
 * @return false
 */

 static bool check_trigger_end (fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    bool status_trigger_signal=port_ultrasound_get_trigger_ready(p_fsm->status);
    if (status_trigger_signal){
        return false;
    }
    else {
        return true;
    }
}





/* State machine input or transition functions */
/**
 * @brief Check if the ultrasound sensor has received the init of the echo signal
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 * @return true if the time tick is higher than 0
 * @return false Otherwise
 */

 static bool check_echo_init(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    uint32_t time_tick_echo_signal=port_ultrasound_get_echo_init_tick(p_fsm->ultrasound_id);/*Retrieve the time tick of the echo signal*/
    if (time_tick_echo_signal>0){
        return true;
    }
    else {
        return false;
    }
}


/* State machine input or transition functions */
/**
 * @brief Check if the ultrasound sensor has received the end of the echo signal
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 * @return true if both the init and end ticks have been received
 * @return false
 */

 static bool check_echo_received(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    bool status_echo_signal=port_ultrasound_get_echo_received(p_fsm->ultrasound_id);
    return status_echo_signal;
}
/* State machine input or transition functions */
/**
 * @brief Check if a new measurement is ready
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 * @return true means that the ultrasound sensor is ready to start a new measurement
 * @return false
 */

 static bool check_new_measurement(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    bool status_trigger_signal=port_ultrasound_get_trigger_ready(p_fsm->ultrasound_id); /*Retrieve and return the status of the trigger signal*/
    return status_trigger_signal; 
}



/* State machine output or action functions */

/**
 * @brief Start a measurement of the ultrasound transceiver for the firts time after the FSM is started
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 
 */

 static void do_start_measurement(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    port_ultrasound_start_measurement(p_fsm->ultrasound_id);
}

/**
 * @brief Start a new measurement of the ultrasound transceiver
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 
 */

 static void do_start_new_measurement(fsm_t * p_this){
    
    do_start_measurement(p_this); /*Starting a new measurement*/
}

/**
 * @brief Set distance measured by the ultrasound sensor
 * @note This function is called when the ultrasound sensor has received the echo signal. 
 * It calculates the distance in cm and stores it in the array of distances
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 
 */


 static void do_set_distance(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    uint32_t e_i_t= port_ultrasound_get_echo_init_tick(p_fsm->ultrasound_id); //Retrieve echo init tick
    uint32_t e_e_t= port_ultrasound_get_echo_end_tick(p_fsm->ultrasound_id);//Retrieve echo end tick
    uint32_t e_o= port_ultrasound_get_echo_overflows(p_fsm->ultrasound_id);//Retrieve echo overflows
    uint32_t time_echo=port_ultrasound_get_echo_init_tick(p_fsm->ultrasound_id)-port_ultrasound_get_echo_end_tick(p_fsm->ultrasound_id);
    uint32_t distance= (time_echo*SPEED_OF_SOUND_MS)/(2*10000); //Calculate the distance in cm taking into account in the position of the index
    if (p_fsm->distance_idx >= FSM_ULTRASOUND_NUM_MEASUREMENTS){

        qsort(p_fsm->distance_arr, FSM_ULTRASOUND_NUM_MEASUREMENTS, sizeof(uint32_t), _compare);
        
        uint32_t median; //Initialize the variable median

        if (FSM_ULTRASOUND_NUM_MEASUREMENTS %2 ==1) //Odd
        {
            median = p_fsm->distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS/2];

        }

        else  //Even
        {
            median = (p_fsm->distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS/2 -1] + p_fsm->distance_arr[FSM_ULTRASOUND_NUM_MEASUREMENTS/2])/2;

        }
    
    p_fsm->distance_cm=median; // Storing the value of the median in the field distance_cm
    
    p_fsm->new_measurement=true; // New measurement is ready
    
    p_fsm->distance_idx = (p_fsm->distance_idx ++) % FSM_ULTRASOUND_NUM_MEASUREMENTS; //Rset the index if is equal or higher than 5

    }  
    p_fsm->distance_arr[p_fsm->distance_idx]=distance; //Store the distance in the array




    port_ultrasound_stop_echo_timer(p_fsm->ultrasound_id);

    port_ultrasound_reset_echo_ticks(p_fsm->ultrasound_id);




}

/**
 * @brief Stop the ultrasound sensor
 * @note This function is called when the ultrasound sensor is stopped. It stops the ultrasound sensor and resets the echo ticks
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 
 */

 static void do_stop_measurement(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    port_ultrasound_stop_ultrasound(p_fsm->ultrasound_id);
    
}

/**
 * @brief Stop the trigger signal of the ultrasound sensor
 * @note THis function is called when the time to trigger the ultrasound sensor has finished. It stops the trigger signal and the trigger timer
 * @param p_this Pointer to an fsm_t struct that contains an fsm_ultrasound_t
 
 */

 static void do_stop_trigger(fsm_t * p_this){
    fsm_ultrasound_t *p_fsm= (fsm_ultrasound_t *)(p_this);
    port_ultrasound_stop_trigger_timer(p_fsm->ultrasound_id); /* Stopping the timer that controls the trigger signal*/
    port_ultrasound_set_trigger_end(p_fsm->ultrasound_id, false); /*Setting the trigger signal to low*/
}




/* Other auxiliary functions */
/**
 * @brief Initialize a ultrasound FSM
 * @note This function initializes the default values of the FSM struct and calls to the port to initialize the associated HW given the ID
 * @param p_fsm_ultrasound Pointer to the ultrasound FSM
 * @param ultrasound_id Unique ultrasound identifier number
 
 */

static void fsm_ultrasound_init(fsm_ultrasound_t *p_fsm_ultrasound, uint32_t ultrasound_id)
{
    // Initialize the FSM
    fsm_init(&p_fsm_ultrasound->f, fsm_trans_ultrasound); 
    p_fsm_ultrasound->distance_cm=0;
    p_fsm_ultrasound->distance_idx=0;
    for (int i=0; i<FSM_ULTRASOUND_NUM_MEASUREMENTS; i++){
        p_fsm_ultrasound->distance_arr[i]=0;
    }
    port_ultrasound_init(ultrasound_id);
 
}


/* Public functions -----------------------------------------------------------*/
fsm_ultrasound_t *fsm_ultrasound_new(uint32_t ultrasound_id)
{
    fsm_ultrasound_t *p_fsm_ultrasound = malloc(sizeof(fsm_ultrasound_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_ultrasound_init(p_fsm_ultrasound, ultrasound_id);                  /* Initialize the FSM */
    return p_fsm_ultrasound;
}
void fsm_ultrasound_fire(fsm_ultrasound_t * p_fsm){
        
        fsm_fire(p_fsm);
}
void fsm_ultrasound_destroy(fsm_ultrasound_t * p_fsm){
        
    free(p_fsm);// Destroy an ultrasound FSM
}
fsm_t* fsm_ultrasound_get_inner_fsm (fsm_ultrasound_t * p_fsm){
    return &p_fsm->f; //Get the inner FSM of the ultrasound
}
uint32_t fsm_ultrasound_get_state(fsm_ultrasound_t * p_fsm){

    return p_fsm->f.current_state; //Get the state of the ultrasound FSM
}

uint32_t fsm_ultrasound_get_distance(fsm_ultrasound_t * p_fsm){

    return p_fsm->distance_cm; //Return the field distance_cm
    
    p_fsm->new_measurement=false; //Reset the field new_measurement
}

void fsm_ultrasound_stop(fsm_ultrasound_t * p_fsm){
    p_fsm->status=false; //Reset the field status

    port_ultrasound_stop_ultrasound(p_fsm->ultrasound_id); //Stopping the ultrasound sensor
}

void fsm_ultrasound_start(fsm_ultrasound_t * p_fsm){

    p_fsm->status=true; //Set the field status true

    p_fsm->distance_idx=0;// Reset the field distance_idx

    p_fsm->distance_cm=0; //Reset the field distance_cm

    port_ultrasound_reset_echo_ticks(p_fsm->ultrasound_id);

    port_ultrasound_set_trigger_ready(p_fsm->ultrasound_id, true); //Ultrasensor is ready to start a new measurement

    port_ultrasound_start_new_measurement_timer(); //Forcing the new measurement timer to start to provoke the first interrupt

}

bool fsm_ultrasound_get_status(fsm_ultrasound_t * p_fsm){

    return p_fsm->status; //Return the field status


}

void fsm_ultrasound_set_status(fsm_ultrasound_t * p_fsm, bool status){

    p_fsm->status=status; //Update the field status with the received value


}

bool fsm_ultrasound_get_ready(fsm_ultrasound_t * p_fsm)

{

    return port_ultrasound_get_trigger_ready(p_fsm->ultrasound_id); // Trigger

    
}

bool fsm_ultrasound_get_new_measurement_ready(fsm_ultrasound_t * p_fsm){

    return p_fsm->new_measurement;


}


// Other auxiliary functions
void fsm_ultrasound_set_state(fsm_ultrasound_t *p_fsm, int8_t state)
{
    p_fsm->f.current_state = state;
}

