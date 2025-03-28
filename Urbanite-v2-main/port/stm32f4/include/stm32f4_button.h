/**
 * @file stm32f4_button.h
 * @brief Header for stm32f4_button.c file
 * @author Raúl Ávila Cerezo
 */
#ifndef STM32F4_BUTTON_H
#define STM32F4_BUTTON_H

/*Includes ------------------------------*/
/*Standard C includes*/
#include <stdint.h>


/* HW dependent includes */
#include "stm32f4xx.h"
/*Defines and enums-----------------------*/
/*Defines*/


#define STM32F4_PARKING_BUTTON_GPIO GPIOC /**  GPIO to which the user button on the board is connected*/
#define STM32F4_PARKING_BUTTON_PIN 13 /**Button GPIO pin*/


/* Functions prototypes and explanation -------------------*/
/**
 * @brief Auxiliary function to change the GPIO and pin of a button. 
 * @details This function is used for testing purposes mainly although it can be used in the final
 * implementation if needed.alignas
 * @param button_id Id of the button to change
 * @param p_port New GPIO port for the button
 * @param pin New GPIO pin for the button
 * 
 */
void stm32f4_button_set_new_gpio (uint32_t button_id, GPIO_TypeDef *p_port, uint8_t pin);



#endif /* STM32F4_BUTTON_H_ */




