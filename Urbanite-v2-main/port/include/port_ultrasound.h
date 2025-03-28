/**
 * @file port_ultrasound.h
 * @brief Header for the portable functions to interact with the HW of the ultrasound sensors. The functions must be implemented in the platform-specific code.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_ULTRASOUND_H_
#define PORT_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
/* Defines and enums ----------------------------------------------------------*/
#define PORT_REAR_PARKING_SENSOR_ID 0 //Rear parking sensor identifier
#define PORT_PARKING_SENSOR_TRIGGER_UP_US 10.0 //Duration in microsecons of the trigger signal
#define PORT_PARKING_SENSOR_TIMEOUT_MS 100.0 //Time in ms wait for the next measurement
#define PORT_PARKING_SENSOR_ECHO_US 1 //Duration in microsecons of echo time
#define SPEED_OF_SOUND_MS 343 //Speed of sound in air in m/s
/* Function prototypes and explanation -------------------------------------------------*/


/**
 * @brief Get the time tick when the end of echo signal was received
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @returns uint32_t
 */
uint32_t port_ultrasound_get_echo_end_tick(uint32_t ultrasound_id);

/**
 * @brief Get the time tick when the init of echo signal was received
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @returns uint32_t
 */
uint32_t port_ultrasound_get_echo_init_tick(uint32_t ultrasound_id);

/**
 * @brief Get the number of overflows of the echo signal timer
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @returns uint32_t
 */
uint32_t port_ultrasound_get_echo_overflows(uint32_t ultrasound_id);


/**
 * @brief Set the number of overflows of the echo signal timer
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @param echo_overflows Number of overflows of the echo signal timer
 * @returns uint32_t
 */
void port_ultrasound_set_echo_overflows(uint32_t ultrasound_id, uint32_t echo_overflows);




/**
 * @brief Get the status of the echo signal. 
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @returns  true
 * @returns false
 */
bool port_ultrasound_get_echo_received(uint32_t ultrasound_id);

/**
 * @brief Get the status of the trigger signal
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @returns true 
 * @returns false
 */
bool port_ultrasound_get_trigger_end(uint32_t ultrasound_id);

/**
 * @brief Get the readiness of the trigger signal
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @returns true 
 * @returns false
 */
bool port_ultrasound_get_trigger_ready(uint32_t ultrasound_id);

/**
 * @brief Configure the HW specifications of a given ultrasound sensor
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 
 */
void port_ultrasound_init(uint32_t ultrasound_id);

/**
 * @brief Reset the time ticks of the echo signal
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 
 */
void port_ultrasound_reset_echo_ticks(uint32_t ultrasound_id);


/**
 * @brief Set the time when the end of echo signal was received
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @param echo_end_tick Time tick when the end of echo signal was received
 */
void port_ultrasound_set_echo_end_tick(uint32_t ultrasound_id, uint32_t echo_end_tick);

/**
 * @brief Set the time when the init of echo signal was received
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @param echo_init_tick Time tick when the init of echo signal was received
 */
void port_ultrasound_set_echo_init_tick(uint32_t ultrasound_id, uint32_t echo_init_tick);



/**
 * @brief Set the status of the echo signal
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @param echo_received status of the echo signal
 */
void port_ultrasound_set_echo_received(uint32_t ultrasound_id, bool echo_received);

/**
 * @brief Set the status of the trigger signal
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @param trigger_end Status of the trigger signal
 */
void port_ultrasound_set_trigger_end(uint32_t ultrasound_id, bool trigger_end);

/**
 * @brief Set the readiness of the trigger signal
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array
 * @param trigger_ready status of the trigger signal
 */
void port_ultrasound_set_trigger_ready(uint32_t ultrasound_id, bool trigger_ready);

/**
 * @brief Start a new measurement of the ultrasound sensor
 *
 * @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array

 */
void port_ultrasound_start_measurement(uint32_t ultrasound_id);

/**
 * @brief Start the timer that controls the new measurement
 *
 */
uint32_t port_ultrasound_start_new_measurement_timer(void);

/**
 * @brief Stop the timer that controls the echo signal
 *  @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array

 */
void port_ultrasound_stop_echo_timer(uint32_t ultrasound_id);

/**
 * @brief Stop the timer that controls the new measurement
 *
 */
void port_ultrasound_stop_new_measurement_timer(void);


/**
 * @brief Stop the timer that controls the trigger signal
 *  @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array

 */
void port_ultrasound_stop_trigger_timer(uint32_t ultrasound_id);

/**
 * @brief Stop all the timers of the ultrasound sensor and reset the echo ticks
 *  @param ultrasound_id Ultrasound ID. This index is used to select the element of the ultrasound_arr[] array

 */
void port_ultrasound_stop_ultrasound(uint32_t ultrasound_id);


#endif /* PORT_ULTRASOUND_H_*/

