/**
 * @file port_button.h
 * @brief Header for port_system.c file
 * @author Raúl Ávila Cerezo
 */
#ifndef PORT_BUTTON_H
#define PORT_BUTTON_H
/*Includes -----------------------------------*/
/*Standard C includes*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

 /* Defines and enums*/
 /*Defines*/
#define PORT_PARKING_BUTTON_ID 0
#define PORT_PARKING_BUTTON_DEBOUNCE_TIME_MS

/*Function prototypes ------------------------------*/

/**
 * @brief COnfigure the hw specifications of a given button
 * @param button_id Button ID. This index is used to select the 
 * element of the buttons_arr[] array

 */
void port_button_init (uint32_t button_id); 

/**
 * @brief Return the status of the button (pressed or not)
 *@param button_id Button_ID. This index is ued to get the correct button
 status struct
 * @returns true if the button has been pressed
 * @returns false if the button has not been pressed
 * 
 */
bool port_button_get_pressed (uint32_t button_id); 


/**
 * @brief Get the value of the GPIO connected to the button
 *@param button_id Button_ID. This index is ued to select the element
 of the buttons_arr[] array
 * @returns true
 * @returns false 
 * 
 */
bool port_button_get_value (uint32_t button_id); 


/**
 * @brief Set the status of the button (pressed or not)
 * @param button_id Button ID. This index is used to get the correct
 * button struct of the buttons_arr[] array
 * @param pressed  Status of the button
 *  
 * @note 
 * @note 
 */
void port_button_set_pressed (uint32_t button_id, bool pressed); 
/**
 * @brief Get the status of the interrupt line connected to the button
 * @param button_id Button ID. This index is used to get the correct
 * button struct of the buttons_arr[] array
 *
 * @returns true
 * @returns false
 */
bool port_button_get_pending_interrupt (uint32_t button_id);
/**
 * @brief Clear the pending interrupt of the button
  * @param button_id Button ID. This index is used to get the correct
 * button struct of the buttons_arr[] array
 
 */
void port_button_clear_pending_interrupt (uint32_t button_id); 
/**
 * @brief Disable the interrupts of the button
 * @param p_port Pointer to the port of the GPIO
 * @param button_id Button ID. This index is used to get the correct
 * button struct of the buttons_arr[] array
 */
void port_button_disable_interrupts (uint32_t button_id); 





#endif /* PORT_BUTTON_H*/
 