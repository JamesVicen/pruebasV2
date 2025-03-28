/**
 * @file fsm_ultrasound.h
 * @brief Header for fsm_ultrasound.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

 #ifndef FSM_ULTRASOUND_H_
 #define FSM_ULTRASOUND_H_
 
 /* Includes ------------------------------------------------------------------*/
 /* Standard C includes */
 #include <stdint.h>
#include <stdbool.h>
/*Others includes*/
#include "stm32f4xx.h"
#include "fsm.h"
 /* Defines and enums ----------------------------------------------------------*/
 
 #define FSM_ULTRASOUND_NUM_MEASUREMENTS 5 //Number of measures

enum FSM_ULTRASOUND{
    WAIT_START=0, /*!<Starting state*/
    TRIGGER_START, /*!<State to send the trigger pulse to the ultrasound sensor*/
    WAIT_ECHO_START, /*!<State to wait for the echo signal*/
    WAIT_ECHO_END, /*!State to wait for the echo signal*/
    SET_DISTANCE /*State to compute the distance from the echo signal*/

};

 /* Typedefs --------------------------------------------------------------------*/
 typedef struct fsm_ultrasound_t fsm_ultrasound_t;
 /* Function prototypes and explanation -------------------------------------------------*/
 
 
 


 /**
  * @brief Check if the ultrasound sensor is doing a distance measuremente
  *
  * The ultrasound sensor is always inactive because all the transistions are 
  * due to HW interrupts
  *
  * 
  * 
  * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
  * @returns true
  * @returns false
  */
 bool fsm_ultrasound_check_activity(fsm_ultrasound_t* p_fsm);
 

 /**
  * @brief Destroy an ultrasound FSM
  *
  * This function destroys an ultrasound transceiver FSM and frees the memory
  *
  * 
  * 
  * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
  * 
  */
 void fsm_ultrasound_destroy(fsm_ultrasound_t *p_fsm);
 

 /**
  * @brief Fire the ultrasound FSM
  *
  * This function is used to fire the ultrasound FSM. It is used to check the 
  * transitions and execute the actions of the ultrasound FSM
  *
  * 
  * 
  * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
  * 
  */
 void fsm_ultrasound_fire(fsm_ultrasound_t *p_fsm);
 
/**
  * @brief Return the distance of the last object detected by the ultrasound sensor.a64l
  *
  * This function also resets the field new_measurement to indicate that 
  * the distance has been read
  *
  * 
  * 
  * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
  * @returns Distance measured by the ultrasound sensor in cm
  */
 uint32_t fsm_ultrasound_get_distance (fsm_ultrasound_t * fsm);

/**
  * @brief Get the inner FSM of the ultrasound
  *
  * This function returns the inner FSM of the ultrasound
  * 
  * 
  * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
  * @returns Pointer to the inner FSM
  */
fsm_t* fsm_ultrasound_get_inner_fsm(fsm_ultrasound_t *p_fsm);

/**
  * @brief Return the flag that indicates if a new measurement is ready
  * @param p_fsm Pointer to the ultrasound struct
  * @returns True 
  * @returns false
  */
bool fsm_ultrasound_get_new_measurement_ready(fsm_ultrasound_t *p_fsm);

/**
  * @brief Get the ready flag of the trigger signal in the ultrasound HW 
  * 
  * This function returns the ready flag of the trigger signal in the ultrasound HW
  * @param p_fsm Pointer to the ultrasound struct
  * @returns True If the port indicates that the trigger signal is read to start
  * a new measurement
  * @returns false If the port indicates that the trigger signal is not ready
  * to start a new measurement
  */
bool fsm_ultrasound_get_ready(fsm_ultrasound_t *p_fsm);

/**
  * @brief Get the state of the ultraosund FSM
  * 
  * This function returns the current state of the ultrasound FSM
  * @param p_fsm Pointer to the ultrasound struct
  * @returns Current state of the ultrasound FSM
  */
 uint32_t fsm_ultrasound_get_state(fsm_ultrasound_t *p_fsm);

/**
  * @brief Get the state of the ultraosund FSM
  * 
  * This function returns the current state of the ultrasound FSM
  * @param p_fsm Pointer to the ultrasound struct
  * @returns True if the ultrasound system has been indicated to be active
  * @returns false if the ultrasound system has been indicated to be paused
  */
bool fsm_ultrasound_get_status(fsm_ultrasound_t *p_fsm);

/**
  * @brief Create a new ultrasound FSM
  * 
  * This function creates a new ultraosund transceiver FSM with the given ultrasound ID
  * @param ultrasound_id Ultrasound ID must be unique
  * @returns Pointer to the ultrasound FSM
  */
fsm_ultrasound_t* fsm_ultrasound_new(uint32_t ultrasound_id);


 /**
  * @brief Set the state of the ultrasound FSM
  * 
  * This functionn sets the current state of the ultrasound FSM
  * @param p_fsm Pointer to an fsm_ultrasound_t struct 
  * @param state New state of the ultrasound FSM
  */
void fsm_ultrasound_set_state(fsm_ultrasound_t* p_fsm, int8_t state);

 /**
  * @brief Set the status of the ultrasound sensor. 
  * 
  * true means that the ultrasound sensor is active and a distancia measuremente
  * must be performed. false means that the ultrasund sensor is inactive
  * 
  * This function sets the current state of the ultrasound FSM
  * @param p_fsm Pointer to an fsm_ultrasound_t struct 
  * @param status New state of the ultrasound FSM
  */
 void fsm_ultrasound_set_status(fsm_ultrasound_t* p_fsm, bool status);


/**
  * @brief Start the ultrasound sensor
  * This function starts the ultrasound sensor by indicating to the port to
  * start the ultrasound sensor and to set the status of the ultrasound sensor
  * to active
  
  * @param p_fsm Pointer to an fsm_ultrasound_t struct 
  *
  */
 void fsm_ultrasound_start(fsm_ultrasound_t* p_fsm);

/**
  * @brief Stop the ultrasound sensor
  * This function stops the ultrasound sensor by indicating to the port to
  * stop the ultrasound sensor and to set the status of the ultrasound sensor
  * to inactive
  
  * @param p_fsm Pointer to an fsm_ultrasound_t struct 
  *
  */
 void fsm_ultrasound_stop(fsm_ultrasound_t* p_fsm);

/**
  * @brief Return the flag that indicates if a new measurement is ready
  
  
  * @param p_fsm Pointer to an fsm_ultrasound_t struct 
  * @return true if a new measurement is ready
  * @return false other case
  */
bool fsm_ultrasound_get_new_measurement_ready(fsm_ultrasound_t* p_fsm);

/**
 * @brief Set the status of the ultrasound sensor.
 * @note True means that the ultrasound sensor is active and a distance measurement must be permoed. false means that the ultrasound sensor is inactive
 * @param p_fsm Pointer to an fsm_ultrasound_t struct
 * @param status Status of the ultrasound sensor
 */
void port_ultrasound_set_status(fsm_ultrasound_t* p_fsm, bool status );





 #endif /* FSM_ULTRASOUND_H_ */