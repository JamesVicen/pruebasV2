/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file, defines FSM for button states.
 * @author Raúl Ávila Cerezo
 */
#ifndef FSM_BUTTON_H_  
#define FSM_BUTTON_H_

/* Includes -----------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

/* Other includes */
#include "fsm.h"
/* Includes ------------------------------------------------------------------*/
/* Standard C includes */


/* Other includes */


/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_BUTTON {
    BUTTON_RELEASED=0,
    BUTTON_RELEASED_WAIT,
    BUTTON_PRESSED,
    BUTTON_PRESSED_WAIT
};

/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_button_t fsm_button_t;

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Check of the button FSM is active or not.
 * @param p_fsm Pointer to an fsm_button_t struct
 * @returns true
 * @returns false
 */
bool fsm_button_check_activity (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Destroy a button FSM
 * @param p_fsm Pointer to an fsm_button_t struct

 */
void fsm_button_destroy (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Fire the button FSM
 * @param p_fsm Pointer to an fsm_button_t struct

 */
void fsm_button_fire (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Get the debounce time of the button FSM
 * @param p_fsm Pointer to an fsm_button_t struct
*  @returns uint32_t Debounce time in ms
 */
uint32_t fsm_button_get_debounce_time_ms (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Return the duration of the last button press
 * @param p_fsm Pointer to an fsm_button_t struct
 * @returns uint32_t duration of the last button press in ms
 */
uint32_t fsm_button_get_duration (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Get the inner FSM of the button
 * @param p_fsm Pointer to an fsm_button_t struct
 * @returns fsm_t* pointer to the inner FSM
 */
fsm_t* fsm_button_get_inner_fsm (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Get the state of the button FSM
 * @param p_fsm Pointer to an fsm_button_t struct
 * @returns uint32_t Current state of the button FSM
 */
uint32_t fsm_button_get_state (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Create a new button FSM
 * @param debounce_time_ms Debounce time in ms
 * @param button_id Button ID. Must be unique
 * @returns fsm_button_t* pointer to the button fsm
 */
fsm_button_t* fsm_button_new(uint32_t debounce_time_ms, uint32_t button_id);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Reset the duration of the last button press
 * @param p_fsm Pointer to an fsm_button_t struct
 * 
 */
void fsm_button_reset_duration (fsm_button_t * p_fsm);

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief COnfigure the HW specifications of a given button
 * 
 * @param button_id Button ID. This index is used to select the element
 * of the buttons_arr[] array
 * 
 */
void port_button_init (uint32_t button_id);


#endif /* FSM_BUTTON_H_ */