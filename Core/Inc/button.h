/*
 * button.h
 *
 *  Created on: Jan 12, 2024
 *      Author: krzychu
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

//states for state machine
typedef enum
{
	IDLE = 0,
	DEBOUNCE,
	PRESSED,
	REPEAT
}BUTTON_STATE ;


//struct button

typedef struct
{
	BUTTON_STATE 	State;
	GPIO_TypeDef* 	GpioPort;
	uint16_t		GpioPin;
	uint32_t		TimerDebounce;  //stala?
	uint32_t		LastTick;
	uint32_t		TimerLongPress;
	uint32_t		TimerRepeat;
	void(*ButtonPressed)(void);
	void(*ButtonLongPress)(void);
	void(*ButtonRepeat)(void);
}TButton;

//public functions
void ButtonSetDebounceTime(TButton* Key, uint32_t milliseconds);
void ButtonTask(TButton* Key);
void ButtonRegisterPressedCallback(TButton* Key, void *Callback);
void ButtonInitKey(TButton* Key, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint16_t TimerDebounce, uint32_t TimerLongpress, uint32_t TimerRepeat);

#endif /* INC_BUTTON_H_ */
