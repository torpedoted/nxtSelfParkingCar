/* nxtcolorsensortest.c */
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"


DeclareCounter(SysTimerCnt);
DeclareAlarm(AlarmCheckEvents);
DeclareEvent(EventTouchOn);
DeclareEvent(EventTouchOff);
//DeclareEvent(EventCheckDistance);
DeclareEvent(EventCheckEvents);
DeclareTask(TouchOn);
DeclareTask(TouchOff);
//DeclareTask(CheckDistance);
DeclareTask(CheckEvents);

void ecrobot_device_initialize(void)
{
	
}
void ecrobot_device_terminate(void)
{	

}
void user_1ms_isr_type2(void)
{	
	(void)SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
}
/* Alarm executed Task2 */
TASK(TouchOn)
{
	while(1)
	{
		WaitEvent(EventTouchOn);
   	ClearEvent(EventTouchOn);
		nxt_motor_set_speed(NXT_PORT_B, -100, 0);
		nxt_motor_set_speed(NXT_PORT_C, -100, 0);
		display_clear(0);
		display_goto_xy(0, 0);
		
		display_string("Button ON");
			
		display_update();
		
	}
}

TASK(TouchOff)
{
	while(1)
	{
		WaitEvent(EventTouchOff);
      ClearEvent(EventTouchOff);
      
      nxt_motor_set_speed(NXT_PORT_B, 0, 1);
      nxt_motor_set_speed(NXT_PORT_C, 0, 1);
		display_clear(0);
		display_goto_xy(0, 0);
		
		display_string("Button OFF");
			
		display_update();
		
	}
}

//TASK(CheckDistance)
//{
	
//}

TASK(CheckEvents)
{
	SetRelAlarm(AlarmCheckEvents, 1, 100); //Not sure where to put
	U8 motorOn = 0;
	U8 Button = 0;
	display_clear(0);
	display_goto_xy(0, 0);
	display_string("Button OFF");	
	display_update();
	
	nxt_motor_set_speed(NXT_PORT_B, 0, 1);
	nxt_motor_set_speed(NXT_PORT_C, 0, 1);
	
	
	while(1)
	{
		WaitEvent(EventCheckEvents);
      ClearEvent(EventCheckEvents);
   
		
		Button = ecrobot_get_touch_sensor(NXT_PORT_S4);
		if(Button == 1 && motorOn == 0)
		{
			Button = 0;
			motorOn = 1;
			SetEvent(TouchOn, EventTouchOn);
		}
		
		//if(Button == 1 && motorOn == 1)
		//{
		//	Button = 0;
		//	motorOn = 0;
		//	SetEvent(TouchOff, EventTouchOff);
		//}
		
		
	}
	
	TerminateTask();

}







