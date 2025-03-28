/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author Raúl Ávila Cerezo
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
/* HW dependent includes */
#include "port_button.h"
#include "port_system.h"
#include "fsm_button.h"
#include "stm32f4xx.h"

/* Project includes */
/*Struct defines-------------------------------*/

struct fsm_button_t {
    fsm_t f;/*!< Button FSM*/
    uint32_t debounce_time_ms;/*!< Button debounce time in ms*/
    uint32_t next_timeout; /*!<Next timeout fo the anti-debounce in ms */
    uint32_t tick_pressed; /*!<Number of ticks when the button was pressed*/
    uint32_t duration; /*How much time the button has been pressed*/
    uint32_t button_id; /*Button ID. It is unique*/
};


/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Check if the button has been released
 * @param p_this Pointer to an fsm_t struct than contains and fsm_button_t
*  @returns true
*  @returns false
 */


 static bool check_button_released (fsm_t *p_this){
    fsm_button_t *p_fsm= (fsm_button_t *)(p_this);
    
    if (port_button_get_pressed(p_fsm->button_id)==true){

        return false;

    }
    else { 
        return true;
    
    } 
}




/*Private functions--------------------------------------------------------------*/
/* State machine input or transition functions */
/**
 * @brief Check if the button has been pressed
 * @param p_this Pointer to an fsm_t struct than contains and fsm_button_t
*  @returns true
*  @returns false
 */
static bool check_button_pressed (fsm_t * p_this){
    return !check_button_released(p_this); /*!<Checking the status of the button */
    
    }


/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Check if the debounce-time has passed
 * @param p_this Pointer to an fsm_t struct than contains and fsm_button_t
*  @returns true
*  @returns false
 */

 static bool check_timeout (fsm_t * p_this) {
    fsm_button_t *p_fsm= (fsm_button_t *)(p_this);
    uint32_t now= port_system_get_millis();
    return now > p_fsm ->next_timeout;
 }


/* State machine output or action functions */

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Store the system tick when the button was pressed
 * @param p_this Pointer to an fsm_t struct than contains and fsm_button_t

 */


static void do_store_tick_pressed (fsm_t *p_this){
    fsm_button_t *p_fsm= (fsm_button_t *)(p_this);
    p_fsm->tick_pressed=port_system_get_millis();
    p_fsm->next_timeout= port_system_get_millis()+p_fsm->debounce_time_ms;
    

}


/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Store the duration of the button press
 * @param p_this Pointer to an fsm_t struct than contains and fsm_button_t

 */



static void do_set_duration (fsm_t * p_this){
    fsm_button_t *p_fsm= (fsm_button_t *)(p_this);
    p_fsm->duration= port_system_get_millis()-p_fsm->tick_pressed; 
    p_fsm->next_timeout= port_system_get_millis()+p_fsm->debounce_time_ms;
}
/* Variables global statics*/
static fsm_trans_t fsm_trans_button[] = {{BUTTON_RELEASED, check_button_pressed,BUTTON_PRESSED_WAIT,do_store_tick_pressed},{BUTTON_PRESSED_WAIT, check_timeout, BUTTON_PRESSED,NULL},{BUTTON_PRESSED, check_button_released, BUTTON_RELEASED_WAIT,do_set_duration},{BUTTON_RELEASED_WAIT, check_timeout, BUTTON_RELEASED, NULL},{-1,NULL,-1,NULL}}; /*!<Array representing the transitions table of the FSM button*/

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Initialize a button FSM
 * @param p_fsm_button Pointer to the button FSM
 * @param Anti-debounce time in ms
 * @param button_id Unique button identifier number
 * @returns fms_button_t* Pointer to the button FSM
 */


static void fsm_button_init (fsm_button_t * p_fsm_button, uint32_t debounce_time, uint32_t button_id) {
    fsm_init(&p_fsm_button->f, fsm_trans_button);
    p_fsm_button->debounce_time_ms=debounce_time;
    p_fsm_button->button_id=button_id;
    p_fsm_button->tick_pressed=0;
    p_fsm_button->duration=0;
    port_button_init(p_fsm_button->button_id);
    


}


/* Public functions -----------------------------------------------------------*/


fsm_button_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id)
{
    fsm_button_t *p_fsm_button = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm_button, debounce_time, button_id);   /* Initialize the FSM */
    return p_fsm_button;                                       /* Composite pattern: return the fsm_t pointer as a fsm_button_t pointer */
}

/* FSM-interface functions. These functions are used to interact with the FSM */
void fsm_button_fire(fsm_button_t *p_fsm)
{
    fsm_fire(&p_fsm->f); // Is it also possible to it in this way: fsm_fire((fsm_t *)p_fsm);
}

void fsm_button_destroy(fsm_button_t *p_fsm)
{
    free(p_fsm);
}

fsm_t *fsm_button_get_inner_fsm(fsm_button_t *p_fsm)
{   
    return &p_fsm->f;
}

uint32_t fsm_button_get_state(fsm_button_t *p_fsm)
{
    return p_fsm->f.current_state;
}


bool fsm_button_check_activity (fsm_button_t * p_fsm){

    if (fsm_button_get_state(p_fsm)==BUTTON_RELEASED)
    
    {

        return false;

    }
else {
    return true; 


}}

uint32_t fsm_button_get_debounce_time_ms(fsm_button_t * p_fsm) {
    return p_fsm->debounce_time_ms; /*!< Return the debounce time of the button FSM*/
}


uint32_t fsm_button_get_duration(fsm_button_t* p_fsm){
    return p_fsm->duration; /*!< Return the duration of the last button press*/
}

void fsm_button_reset_duration (fsm_button_t *p_fsm){
    p_fsm->duration=0; /*!< Reset to 0 of the last button press*/
}
