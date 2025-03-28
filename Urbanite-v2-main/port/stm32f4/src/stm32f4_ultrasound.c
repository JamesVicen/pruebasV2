/**
 * @file stm32f4_ultrasound.c
 * @brief Portable functions to interact with the ultrasound FSM library. All portable functions must be implemented in this file.
 * @author Raúl ávila Cerezo
 * @author alumno2
 * @date date
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_ultrasound.h"
#include "stm32f4_system.h"
#include "stm32f4_ultrasound.h"

/* Microcontroller dependent includes */
/*Defines ----------------------------------------------------------------*/

#define frequency 40000.0
#define Arrmax 65535.0

#define tick 1.0e+6
/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
    GPIO_TypeDef *p_trigger_port; /*!<GPIO where the trigger signal is connected*/
    GPIO_TypeDef *p_echo_port;    /*!<GPIO where the echo signal is connect*/
    uint8_t trigger_pin;          /*!<Pin/line where the trigger signal is connected*/
    uint8_t echo_pin;             /*!<Pin/line where the echo signal is connected*/
    uint8_t echo_alt_fun;         /*!< Alternate function for the echo signal*/
    bool trigger_ready;           /*!< Flag to indicate that a new measurement can be started*/
    bool trigger_end;             /*!<Flag to indicate that the trigger signal has been sent*/
    bool echo_received;           /*!<Flag to indicate that the echo signal has been received*/
    uint32_t echo_init_tick;      /*!<Tick time when the echo signal was received*/
    uint32_t echo_end_tick;       /*!<Tick time  when the echo signal was received*/
    uint32_t echo_overflows;      /*!<Number of overflows of the timer during the echo signal*/

} stm32f4_ultrasound_hw_t;

// Error porque hace falta asociar los pines y gpios
/* Global variables */
static stm32f4_ultrasound_hw_t ultrasound_arr[] = {[PORT_REAR_PARKING_SENSOR_ID] = {.p_echo_port = STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO, .echo_pin = STM32F4_REAR_PARKING_SENSOR_ECHO_PIN, .p_trigger_port = STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO, .trigger_pin = STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN,  .trigger_ready = false, .trigger_end = false, .echo_received = false, .echo_init_tick = 0, .echo_end_tick = 0, .echo_overflows = 0}};
/* Private functions ----------------------------------------------------------*/

/**
 * @brief Configure the timer that controls the duration of the trigger signal
 *
 *
 */
/**
 * @brief Get the button status struct with the given ID
 * @param button_id Button ID.
 * @return POinter to the button state struct
 * @return NULL if the button ID is not valid
 */

 static stm32f4_ultrasound_hw_t *_stm32f4_ultrasound_get(uint32_t button_id)
 {
     if (button_id < sizeof(ultrasound_arr) / sizeof(ultrasound_arr[0]))
     {
         return &ultrasound_arr[button_id];
     }
 
     else
     {
         return NULL;
     }
 }

static void _timer_trigger_setup()
{
    // Importante no poner los valores de registro a mano.

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; /*<!Enable the clock of the timer
   that controls the trigger signal*/

    TIM3->CR1 &= ~TIM_CR1_CEN; /*<! Disable the counter of the timer*/

    TIM3->CR1 |= TIM_CR1_ARPE; /*<!Enable the autoreload preload to
    enable the autoreload register*/

    TIM3->CNT = 0; /*<! Counter of the timer to 0*/

    //double double_SCC= (double) SystemCoreClock; /*<! Casting the SCC variable*/


    double psc_value = 0.0; /*<! Initial PSC value*/

    double arr_value = round(((16.0 * 10.0) / (psc_value + 1.0)) - 1.0); /*!< Re-computing the value of ARR register with the new PSC value */

    while (arr_value > 65535.0) {
    psc_value += 1.0; /*<! Update the PSC value by a smaller increment */
    arr_value = round(((16.0 * 10.0) / (psc_value + 1.0)) - 1.0); /*<! Re-computing the value of ARR register with the new PSC value */
    }
    

    TIM3->PSC = round(psc_value); /*!<value of PSC*/

    TIM3->ARR = arr_value; /*!< value of PSC*/
    TIM3->EGR = TIM_EGR_UG;/*!<Update event*/
    TIM3->SR &= ~TIM_SR_UIF;    /*!<Clearing the update interrupt flag*/
    


    TIM3->DIER |= TIM_DIER_UIE; /*!<Enable tthe interrupts of the timer
     by setting the UIE bit of the DIER register*/

    NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 4, 0)); /*!<
      Setting the priority of the time interrupt in the NVIC*/
}


/**
 * @brief Configure the timer that controls the duration of the echo signaal
 *
 * @param ultrasound_id Ultrasound ID. This ID is used to configure
 * the timer that controls the echo signal of he ultrasound sensor
 */

 static void _timer_echo_setup(uint32_t ultrasound_id)
 {
     if ((_stm32f4_ultrasound_get(ultrasound_id)) != NULL)
     {
         RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /*<!Enable the clock of the timer
        that controls the trigger signal*/
        /*!<Disable the counter of the timer*/
                    /*!<Clock source: intern*/
        TIM2->CR1 &= ~TIM_CR1_CEN;          
        TIM2->CNT = 0;              /*!< Initializate counter to 0*/
 
        //double double_SCC = (double)SystemCoreClock; /*<! Casting the SCC variable*/
 
         double psc_value = round(((16.0 * 1.0) / (0.0 + 1.0)) - 1.0);    /*<! Computing the PSC value*/
                                                           
         double arr_value = Arrmax; /*!<Re-computing the value of ARR register with the new PSC value*/
                                                              /*!<Initial value of ARR*/
         
         TIM2->ARR= arr_value;
         TIM2->PSC= psc_value;

         TIM2->CR1 |= TIM_CR1_ARPE; /*<!Enable the autoreload preload to
         enable the autoreload register*/
         TIM2->EGR |= TIM_EGR_UG;   /*!<Update event*/
         /*Input capture. Channel 2*/
         TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;                                                       /*Cleaning*/
         
         TIM2->CCMR1 |= (0x1 << TIM_CCMR1_CC2S_Pos);   
                                                 /*!< 01= input*/
         TIM2->CCMR1 &= ~TIM_CCMR1_IC2F;  /*<! Disable digital filter*/                                                     /*!<Disable digital filtering */
         
         TIM2->CCER |= (1 << TIM_CCER_CC2P_Pos | 1 << TIM_CCER_CC2NP_Pos);                     /*!<Rising edge*/
         
         TIM2->CCMR1 &= ~(TIM_CCMR1_IC2PSC);

         TIM2->CCER |= TIM_CCER_CC2E;                                                          /*!<Enable input capture*/
         
         TIM2->DIER |= TIM_DIER_CC2IE;                                                         /*!<Capture interruption*/
         
         TIM2->DIER |= TIM_DIER_UIE;                                                           /*!< Update interruption*/
         
         NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); /*!< Priority 3, sub-priority 0*/
         
         NVIC_EnableIRQ(TIM2_IRQn);

         

         
                                                                 /*!< Enable interruption*/ 
                                                                   /*!<Enable timer to start the counter*/
     }
 }
/**
 * @brief COnfigure the timer that controls the duration of the new measurement
 *
 * This function configures the timer to generate an internal
 * interrupt to control the duration of measurement
 *
 *
 */

 void _timer_new_measurement_setup()
 {
     // Se siguen los mismos pasos que en el timer trigger setup
     // pero para el timer que controla la duración de la nueva
     // medida TIM5
 
     RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;          /*<!Enable the clock of the timer
             that controls the trigger signal*/
     TIM5->CR1 &= ~TIM_CR1_CEN;                   /*<! Disable the counter of the timer*/
     TIM5->CR1 |= TIM_CR1_ARPE;                   /*<!Enable the autoreload preload to
                        enable the autoreload register*/
     TIM5->CNT = 0;                       /*<! Counter of the timer to 0*/
     
    double psc_value = 0.0; /*<! Initial PSC value*/

    double arr_value = round(((16.0 * 100.0* 1.0e3) / (psc_value + 1.0)) - 1.0); /*!< Re-computing the value of ARR register with the new PSC value */
    
    while (arr_value > 65535.0) {
    psc_value += 1.0; /*<! Update the PSC value by a smaller increment */
    arr_value = round(((16.0 * 100.0 * 1.0e3) / (psc_value + 1.0)) - 1.0); /*<! Re-computing the value of ARR register with the new PSC value */
    }
    
 
    TIM5->PSC = round(psc_value); /*!<value of PSC*/

    TIM5->ARR = arr_value; /*!< value of PSC*/
    TIM5->EGR = TIM_EGR_UG;/*!<Update event*/
    TIM5->SR &= ~TIM_SR_UIF;    /*!<Clearing the update interrupt flag*/
    


    TIM5->DIER |= TIM_DIER_UIE; /*!<Enable tthe interrupts of the timer
     by setting the UIE bit of the DIER register*/

    NVIC_SetPriority(TIM5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0)); /*!<
      Setting the priority of the time interrupt in the NVIC*/
}
 

/* Public functions -----------------------------------------------------------*/
void port_ultrasound_init(uint32_t ultrasound_id)
{
    /* Get the ultrasound sensor */
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    
    p_ultrasound->echo_end_tick = 0;     /*!< Tick to 0 */
    p_ultrasound->echo_init_tick = 0;    /*!< Tick to 0 */
    p_ultrasound->trigger_end = false;   /*!< Flag to false */
    p_ultrasound->echo_received = false; /*!< Flag to false*/
    p_ultrasound->trigger_ready = true;  /*!< Flag to true*/
    stm32f4_system_gpio_config(p_ultrasound->p_trigger_port,p_ultrasound->trigger_pin,STM32F4_GPIO_MODE_OUT, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config(p_ultrasound->p_echo_port,p_ultrasound->echo_pin,STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_ultrasound->p_echo_port, p_ultrasound->echo_pin,1);
    _timer_trigger_setup();
    _timer_echo_setup(ultrasound_id);
    _timer_new_measurement_setup();
    p_ultrasound ->echo_overflows=0; 
    
}

// Getters and setters functions

// Util
void stm32f4_ultrasound_set_new_trigger_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_trigger_port = p_port;
    p_ultrasound->trigger_pin = pin;
}

void stm32f4_ultrasound_set_new_echo_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_echo_port = p_port;
    p_ultrasound->echo_pin = pin;
}




void port_ultrasound_start_measurement(uint32_t ultrasound_id)
{

    if (_stm32f4_ultrasound_get(ultrasound_id) != NULL)
    {

        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->trigger_ready = false;
        TIM3->CNT = 0;  /*!<Reset the counter CNT of the trigger timer*/
        TIM2->CNT = 0;  /*!<Reset the counter CNT of the echo timer*/
        TIM5->CNT = 0;  /*!<Reset the counter CNT of the new measurement time*/
        GPIOB->BSRR = (1 << 0); /*!< Set the trigger pin to high*/
        /*!< Enable the timers interrupts*/
        NVIC_EnableIRQ(TIM2_IRQn);
        NVIC_EnableIRQ(TIM3_IRQn);
        NVIC_EnableIRQ(TIM5_IRQn);
        /*!<Enable the timers*/
        // MIRAR...

        TIM3->CR1 |= (1 << 0);
        TIM2->CR1 |= (1 << 0);
        TIM5->CR1 |= (1 << 0);
    }
}

void port_ultrasound_reset_echo_ticks(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->echo_init_tick = 0;
    p_ultrasound->echo_end_tick = 0;
    p_ultrasound->echo_overflows=0;
    p_ultrasound->echo_received = false;
}

void port_ultrasound_stop_echo_timer(uint32_t ultrasound_id)
{
    if (_stm32f4_ultrasound_get(ultrasound_id) != NULL)
    {
        TIM2->CR1 &= ~TIM_CR1_CEN;
    }
}

void port_ultrasound_stop_new_measurement_timer()
{

        TIM5->CR1 = (0 << 1);
    
}

void port_ultrasound_stop_trigger_timer(uint32_t ultrasound_id)
{
    if (_stm32f4_ultrasound_get(ultrasound_id) != NULL)
    {
    GPIOB->BSRR = (0 << 1);
    TIM3->CR1 = (0 << 1);}
}
void port_ultrasound_stop_ultrasound(uint32_t ultrasound_id)
{
    if (_stm32f4_ultrasound_get(ultrasound_id) != NULL)
    {
        port_ultrasound_stop_trigger_timer(ultrasound_id);
        port_ultrasound_stop_new_measurement_timer();
        port_ultrasound_stop_echo_timer(ultrasound_id);
        port_ultrasound_reset_echo_ticks(ultrasound_id);
    }
}

/*Getters and setters functions-------------------------------------**/

bool port_ultrasound_get_trigger_ready(uint32_t ultrasound_id)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        return p_ultrasound->trigger_ready;
   

    
}

bool port_ultrasound_get_trigger_end(uint32_t ultrasound_id)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        return p_ultrasound->trigger_end;
    
}
bool port_ultrasound_get_echo_received(uint32_t ultrasound_id)
{
   
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        return p_ultrasound->echo_received;
    
}
uint32_t port_ultrasound_get_echo_overflows(uint32_t ultrasound_id)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        return p_ultrasound->echo_overflows;
   
}
uint32_t port_ultrasound_get_echo_init_tick(uint32_t ultrasound_id)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        return p_ultrasound->echo_init_tick;
    
}

uint32_t port_ultrasound_get_echo_end_tick(uint32_t ultrasound_id)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        return p_ultrasound->echo_end_tick;
    
}

void port_ultrasound_set_echo_end_tick(uint32_t ultrasound_id, uint32_t echo_end_tick)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->echo_end_tick = echo_end_tick;
    
}
void port_ultrasound_set_echo_init_tick(uint32_t ultrasound_id, uint32_t echo_init_tick)
{
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->echo_init_tick = echo_init_tick;
    
}
void port_ultrasound_set_echo_received(uint32_t ultrasound_id, bool echo_received)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->echo_received = echo_received;
    
}

void port_ultrasound_set_trigger_ready(uint32_t ultrasound_id, bool trigger_ready)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->trigger_ready = trigger_ready;
    
}

void port_ultrasound_set_trigger_end(uint32_t ultrasound_id, bool trigger_end)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->trigger_end = trigger_end;
    
}

void port_ultrasound_set_echo_overflows(uint32_t ultrasound_id, uint32_t echo_overflows)
{
    
        stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
        p_ultrasound->echo_overflows = echo_overflows;
    
}