/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2025-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "stm32f4_system.h"
#include "port_button.h"
#include "stm32f4_button.h"
#include "port_ultrasound.h"
#include "stm32f4_ultrasound.h"


// Include headers of different port elements:

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. **Increment the System tick counter `msTicks` in 1 count.** To do so, use the function `port_system_get_millis()` and `port_system_set_millis()`.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
    uint32_t msTicks_actual= port_system_get_millis();
    port_system_set_millis(msTicks_actual+1);

}
void EXTI15_10_IRQHandler(void) {

    if (port_button_get_pending_interrupt(PORT_PARKING_BUTTON_ID)) {

        if (port_button_get_value(PORT_PARKING_BUTTON_ID)==true){

            port_button_set_pressed(PORT_PARKING_BUTTON_ID, false);
        }
        else {
            port_button_set_pressed(PORT_PARKING_BUTTON_ID, true);
        }

    }
    port_button_clear_pending_interrupt(PORT_PARKING_BUTTON_ID);
}
/** @brief Interrupt service routine for the TIM5 timer
*
* This timer controls the duration of the measurements of the ultrasound
* sensor. When the interrupt occurs it means that the time of the 
* the measurement has expired and a new mesasurement can be started
*
*/
void TIM2_IRQHandler(void){
    
    

    if (TIM2->SR & TIM_SR_UIF) /*!< Checking if the UIF flag is set*/
    {

        uint32_t current_overflows= port_ultrasound_get_echo_overflows(PORT_REAR_PARKING_SENSOR_ID) +1;
        port_ultrasound_set_echo_overflows(PORT_REAR_PARKING_SENSOR_ID, current_overflows);
        TIM2-> SR &= ~TIM_SR_UIF;
    }

    if ((TIM2->SR & TIM_SR_CC2IF) !=0) /*!< Checking if the UIF flag is set*/
    {
        uint32_t current_tick= TIM2->CCR2;
        if(((port_ultrasound_get_echo_init_tick(PORT_REAR_PARKING_SENSOR_ID))==0) && ((port_ultrasound_get_echo_end_tick(PORT_REAR_PARKING_SENSOR_ID))==0)){
            port_ultrasound_set_echo_init_tick(PORT_REAR_PARKING_SENSOR_ID, current_tick);
            port_ultrasound_get_echo_end_tick (PORT_REAR_PARKING_SENSOR_ID);
            port_ultrasound_set_echo_received(PORT_REAR_PARKING_SENSOR_ID,true);
        }
        }
    
        //

    }
    
    







/** @brief Interrupt service routine for the TIM3 timer
*
* This timer controls the duration of th trigger signal of the ultrasound
* sensor. When the interrupt occurs it means that the time of the 
* the trigger signal has expired and must be lowered
*
*/
void TIM3_IRQHandler(void){
    TIM3->SR &= ~TIM_SR_UIF;/*!<Clearing the interrupt flag UIF in the status register SR*/
    port_ultrasound_set_trigger_end(PORT_REAR_PARKING_SENSOR_ID,true); /*!<Calling the function to set the flag
    that indicates that the time of the trigger signal has expired*/

}

/** @brief Interrupt service routine for the TIM5 timer
*
* This timer controls the duration of the measurements of the ultrasound
* sensor. When the interrupt occurs it means that the time of the 
* the measurement has expired and a new mesasurement can be started
*
*/
void TIM5_IRQHandler(void){
    TIM5->SR &= ~TIM_SR_UIF; /*!Clear the interrupt flag UIF in the status register SR*/
    port_ultrasound_set_trigger_ready(PORT_PARKING_BUTTON_ID,true);/*!<Calling the function to set the 
    flag that indicates that a new measurement can be started*/

}