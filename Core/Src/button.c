/*
 * button.c
 *
 *  Created on: Jan 12, 2024
 *      Author: krzychu
 */

#include "main.h"
#include "button.h"

//button init func
void ButtonInitKey(TButton* Key, GPIO_TypeDef* GpioPort, uint16_t GpioPin,
		uint16_t TimerDebounce, uint32_t TimerLongpress, uint32_t TimerRepeat)
{
	Key->State = IDLE;
	Key->GpioPort = GpioPort;
	Key->GpioPin = GpioPin;
	Key->TimerDebounce = TimerDebounce;
	Key->TimerRepeat = TimerRepeat;
	Key->TimerLongPress = TimerLongpress;
}
//time setting function
void ButtonSetDebounceTime(TButton* Key, uint32_t milliseconds)
{
	Key->TimerDebounce = milliseconds;
}
void ButtonSetRepeatTime(TButton* Key, uint32_t milliseconds)
{
	Key->TimerDebounce = milliseconds;
}
void ButtonSetLongPressTime(TButton* Key, uint32_t milliseconds)
{
	Key->TimerDebounce = milliseconds;
}
//register callback

void ButtonRegisterPressedCallback(TButton* Key, void *Callback)
{
	Key->ButtonPressed = Callback;
}
void ButtonRegisterLongPressCallback(TButton* Key, void *Callback)
{
	Key->ButtonLongPress = Callback;
}
void ButtonRegisterRepeatCallback(TButton* Key, void *Callback)
{
	Key->ButtonRepeat = Callback;
}

//states routine (whatever)
void ButtonIDLERoutine(TButton* Key)
{
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort,Key->GpioPin))
	{
		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}
}
void ButtonDebounceRoutine(TButton* Key)
{
	if((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce)
	{
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort,Key->GpioPin))
		{
			Key->State =PRESSED;
			if(Key->ButtonPressed != NULL)
			{
			Key->ButtonPressed();
			}
		}
		else
		{
			Key->State = IDLE;
		}
	}
}
void ButtonPressedRoutine(TButton* Key)
{
	// Check if button was released
		if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin))
		{
			// If released - go back to IDLE state
			Key->State = IDLE;
		}
		if((HAL_GetTick() - Key->LastTick) > Key->TimerLongPress)
			{
				Key->State = REPEAT;
				Key->LastTick = HAL_GetTick();
				if(Key->ButtonLongPress != NULL)
							{
							Key->ButtonLongPress();
							}
			}		//shit look prietier than this code

}

void ButtonRepeatRoutine(TButton* Key)
{
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort,Key->GpioPin))
		{
			Key->State = DEBOUNCE;
			Key->LastTick = HAL_GetTick();
		}else
		{
			if((HAL_GetTick() - Key->LastTick) > Key->TimerRepeat)
			{
				Key->LastTick = HAL_GetTick();
				if(Key->ButtonRepeat != NULL)
				{
					Key->ButtonRepeat;
				}
			}
		}
}

//state machine itself
void ButtonTask(TButton* Key)
{
	switch(Key->State)
	{
		case 	IDLE:
		//do idle
			ButtonIDLERoutine(Key);
		break;
		case	DEBOUNCE:
		//do Debounce
			ButtonDebounceRoutine(Key);
		break;
		case 	PRESSED:
		//do pressed
			ButtonPressedRoutine(Key);
		break;
		case 	REPEAT:
				//do pressed
			ButtonRepeatRoutine(Key);
		break;
	}
}
