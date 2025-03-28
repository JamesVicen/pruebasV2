
/**
 * @file stm32f4_button.c
 * @brief Portable functions to interact with the button FSM library. All
 * portable functions must be implemented in this file.
 * @author Raúl Ávila Cerezo
 * 
 * @date fecha
 */
/*Includes-----------------------------*/
/*Standard C includes*/
#include <stdio.h>
#include "port_button.h"
#include "port_system.h"
#include "stm32f4_button.h"
#include "stm32f4_system.h"

/* HW dependent includes */


/*Microcontroller dependent includes*/
//Include the necessary files to interect with the GPIOs



/*Typedefs ------------------------------------------------------*/
typedef struct 
{
    /* data */
    GPIO_TypeDef *p_port; //Puede ser que 
    uint8_t pin;
    uint8_t pupd_mode;
    bool flag_pressed;
}stm32f4_button_hw_t;

/*Global variables -------------------------------*/
static stm32f4_button_hw_t buttons_arr[] ={
    [PORT_PARKING_BUTTON_ID]={.p_port= STM32F4_PARKING_BUTTON_GPIO, .pin=STM32F4_PARKING_BUTTON_PIN, .pupd_mode=STM32F4_GPIO_PUPDR_NOPULL}
}; 
/*Private functions--------------------------------*/

/**
 * @brief Get the button status struct with the given ID
 * @param button_id Button ID.
 * @return POinter to the button state struct
 * @return NULL if the button ID is not valid
 */


stm32f4_button_hw_t* _stm32f4_button_get(uint32_t button_id){
    if (button_id<sizeof(buttons_arr)/sizeof(buttons_arr[0]))
    {
            return &buttons_arr[button_id];
    }

    else {
            return NULL;
    }
}


/*Public functions -------------------------*/

void port_button_init(uint32_t button_id){
    //Retrieve the button structu using the private function and the button id
    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id);/** Retrieve the button config struct */
    stm32f4_system_gpio_config(p_button->p_port, p_button->pin, STM32F4_GPIO_MODE_IN, p_button->pupd_mode ); /*Configure the button as input and no pull up neither pull down connection*/
    stm32f4_system_gpio_config_exti(p_button->p_port, p_button->pin,STM32F4_TRIGGER_RISING_EDGE | STM32F4_TRIGGER_FALLING_EDGE | STM32F4_TRIGGER_ENABLE_INTERR_REQ ); /**To configure interruption mode in both rising and falling edges, and enable the interrupt request */
    stm32f4_system_gpio_exti_enable(p_button->pin, 1,0); /*To enable the interrupt line and set the priority level to 1 and sub. level to 0*/
}

bool port_button_get_pressed (uint32_t button_id){
    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id);

    return p_button->flag_pressed; /** Return the status of the button */

} 



bool port_button_get_value (uint32_t button_id){

    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id);/** Retrieve the button config struct */

    return stm32f4_system_gpio_read(p_button->p_port, p_button->pin); /**Reading the digital value of a GPIO */


} 

void port_button_set_pressed (uint32_t button_id, bool pressed){

    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id);/** Retrieve the button config struct */

    p_button->flag_pressed=pressed; /**Set the value of the field flag_pressed */
}

bool port_button_get_pending_interrupt (uint32_t button_id){
    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id);/** Retrieve the button config struct */
    uint8_t pin_GPIO= p_button->pin;
    if (p_button==NULL) /**Checking if the button ID is in the array */
    {
            return false; /**If not, returns false*/
    }
    if (pin_GPIO >= 32) /**Checking if the pin is greater than 32, which is not possible*/
    {
        return false; /**If true, return false*/
    }
    
    
    return (EXTI->PR & (1<< p_button->pin))!=0; /** Return if the PR register is set*/
}

void port_button_clear_pending_interrupt(uint32_t button_id) {
    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id); /** Retrieve the button config struct */
    uint8_t pin_GPIO= p_button->pin;
    
    EXTI->PR |= (1<<pin_GPIO);  /**Clearing PR register associated with the button pin (1) */
    
}

   

void port_button_disable_interrupts (uint32_t button_id) {
    stm32f4_button_hw_t *p_button= _stm32f4_button_get(button_id);/** Retrieve the button config struct */
    uint8_t pin_GPIO= p_button->pin;
    stm32f4_system_gpio_exti_disable(pin_GPIO); /**Disable the EXTI line */


}


void stm32f4_button_set_new_gpio (uint32_t button_id, GPIO_TypeDef *p_port, uint8_t pin){
    stm32f4_button_hw_t *p_button=_stm32f4_button_get(button_id);/** Retrieve the button config struct */
    
    p_button->p_port=p_port; /**Setting new port */
    p_button->pin=pin; /** Setting new pin */
}










