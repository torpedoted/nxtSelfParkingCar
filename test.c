/* nxtcolorsensortest.c */
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <math.h>


DeclareCounter(SysTimerCnt);
DeclareAlarm(AlarmCheckEvents);
DeclareEvent(EventForward);
DeclareEvent(EventReverse);
DeclareEvent(EventStop);
DeclareEvent(EventCheckDistance);
DeclareEvent(EventCheckEvents);
DeclareEvent(EventTurnLeft);
DeclareEvent(EventTurnRight);
DeclareEvent(EventForwardStep);
DeclareTask(ForwardStep);
DeclareTask(TurnRight);
DeclareTask(TurnLeft);
DeclareTask(Forward);
DeclareTask(Reverse);
DeclareTask(Stop);
DeclareTask(CheckDistance);
DeclareTask(CheckEvents);

int gapDistance = 0;
U8 parked = 0;
int forwardDistance = 0;
int reverseDistance = 0;

void ecrobot_device_initialize(void)
{
	ecrobot_init_sonar_sensor(NXT_PORT_S2);
}
void ecrobot_device_terminate(void)
{	
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
}
void user_1ms_isr_type2(void)
{	
	(void)SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
}
/* Alarm executed Task2 */
TASK(Forward)
{
	while(1)
	{
		WaitEvent(EventForward);
   	ClearEvent(EventForward);
		nxt_motor_set_speed(NXT_PORT_B, -100, 0);
		nxt_motor_set_speed(NXT_PORT_C, -100, 0);
		display_clear(0);
		display_goto_xy(0, 0);
		
		display_string("Button ON");
			
		display_update();
		
		SetEvent(CheckDistance, EventCheckDistance);
	}
}

TASK(ForwardStep)
{
	int degree = 0;
	while(1)
	{
		WaitEvent(EventForwardStep);
   	ClearEvent(EventForwardStep);
		nxt_motor_set_speed(NXT_PORT_B, -100, 0);
		nxt_motor_set_speed(NXT_PORT_C, -100, 0);
		display_clear(0);
		display_goto_xy(0, 0);
		
		display_string("ForwardStep");
			
		display_update();
		
		nxt_motor_set_count(NXT_PORT_B, 0);
      
      while(degree > forwardDistance)
      {
      	systick_wait_ms(50);
      	degree = nxt_motor_get_count(NXT_PORT_B);
      }
      
      degree = 0;
		
	}
}

TASK(Reverse)
{
	int degree = 0;
	while(1)
	{
		WaitEvent(EventReverse);
   	ClearEvent(EventReverse);
		nxt_motor_set_speed(NXT_PORT_B, 100, 0);
		nxt_motor_set_speed(NXT_PORT_C, 100, 0);
		display_clear(0);
		display_goto_xy(0, 0);
		
		display_string("Reverse");
			
		display_update();
		
		nxt_motor_set_count(NXT_PORT_B, 0);
      
      while(degree < -reverseDistance)
      {
      	systick_wait_ms(100);
      	degree = nxt_motor_get_count(NXT_PORT_B);
      }
      
      degree = 0;
		
	}
}

TASK(Stop)
{
	while(1)
	{
		WaitEvent(EventStop);
      ClearEvent(EventStop);
      
      nxt_motor_set_speed(NXT_PORT_B, 0, 1);
      nxt_motor_set_speed(NXT_PORT_C, 0, 1);
		display_clear(0);
		display_goto_xy(0, 0);
		
		display_string("Button OFF");
			
		display_update();
		
	}
}

TASK(TurnRight)
{
int degree = 0;
	while(1)
	{
		WaitEvent(EventTurnRight);
      ClearEvent(EventTurnRight);
      nxt_motor_set_count(NXT_PORT_A, 0);
      nxt_motor_set_speed(NXT_PORT_A, 70, 0);
      degree = nxt_motor_get_count(NXT_PORT_A);
      
      display_clear(0);
		display_goto_xy(0, 0);
		
		display_int(degree, 0);
			
		display_update();
		
      while(degree < 55)
      {
      	systick_wait_ms(10);
      	degree = nxt_motor_get_count(NXT_PORT_A);
      	display_clear(0);
			display_goto_xy(0, 0);
			display_int(degree, 0);
			display_update();
      	
      }
      	degree = 0;
		nxt_motor_set_speed(NXT_PORT_A, 0, 1);
	}
}

TASK(TurnLeft)
{
int degree = 0;
	while(1)
	{
		WaitEvent(EventTurnLeft);
      ClearEvent(EventTurnLeft);
      nxt_motor_set_count(NXT_PORT_A, 0);
      nxt_motor_set_speed(NXT_PORT_A, -70, 0);
      degree = nxt_motor_get_count(NXT_PORT_A);
      
      display_clear(0);
		display_goto_xy(0, 0);
		
		display_int(degree, 0);
			
		display_update();
		
      while(degree > -55)
      {
      	systick_wait_ms(10);
      	degree = nxt_motor_get_count(NXT_PORT_A);
      	display_clear(0);
			display_goto_xy(0, 0);
			display_int(degree, 0);
			display_update();
      	
      }
      	degree = 0;
		nxt_motor_set_speed(NXT_PORT_A, 0, 1);
		
		
		
	}
}

TASK(CheckDistance)
{
	int sonar;
	
	while(1)
	{
		WaitEvent(EventCheckDistance);
      ClearEvent(EventCheckDistance);
      
      while(gapDistance > -3000)
      {
      gapDistance = 0;
      //Calculate distance a few times to get accurate
      for(int i = 0; i < 10; i++)
      {
      systick_wait_ms(50);
      sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);
      }
      
      while(sonar > 20)
      {
      	systick_wait_ms(100);
      	sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);
      	display_clear(0);
			display_goto_xy(0, 0);
			display_int(sonar, 0);
			display_update();
      }
      
      for(int i = 0; i < 10; i++)
      {
      systick_wait_ms(50);
      sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);
      }
      
      while(sonar < 30)
      {
      	systick_wait_ms(100);
      	sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);
      	display_clear(0);
			display_goto_xy(0, 0);
			display_int(sonar, 0);
			display_update();
      }
      
      nxt_motor_set_count(NXT_PORT_B, 0);
      
      while(sonar > 20)
      {
      	systick_wait_ms(100);
      	sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);
      	gapDistance = nxt_motor_get_count(NXT_PORT_B);
      	display_clear(0);
			display_goto_xy(0, 0);
			display_int(gapDistance, 0);
			display_update();
      }
      
      }
	
		SetEvent(Stop, EventStop);
		
		reverseDistance = gapDistance/2.4;
		forwardDistance = -1000;
		SetEvent(ForwardStep, EventForwardStep);
		SetEvent(Stop, EventStop);
		
		SetEvent(TurnLeft, EventTurnLeft);
		
		SetEvent(Reverse, EventReverse);
		SetEvent(Stop, EventStop);
		
		SetEvent(TurnRight, EventTurnRight);
		SetEvent(TurnRight, EventTurnRight);
		
		SetEvent(Reverse, EventReverse);
		SetEvent(Stop, EventStop);
		
		SetEvent(TurnLeft, EventTurnLeft);
		
		forwardDistance = -500;
		SetEvent(ForwardStep, EventForwardStep);
		SetEvent(Stop, EventStop);
		
		parked = 1;	
		
	}
}

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
			SetEvent(Forward, EventForward);
		}
		
		if(Button == 1 && parked == 1)
		{
			Button = 0;
			parked = 0;
			reverseDistance = -500;
			forwardDistance = (gapDistance/2);
			SetEvent(Reverse, EventReverse);
			SetEvent(Stop, EventStop);
			SetEvent(TurnRight, EventTurnRight);
			SetEvent(ForwardStep, EventForwardStep);
			SetEvent(Stop, EventStop);
			SetEvent(TurnLeft, EventTurnLeft);
			SetEvent(TurnLeft, EventTurnLeft);
			SetEvent(ForwardStep, EventForwardStep);
			SetEvent(Stop, EventStop);
			SetEvent(TurnRight, EventTurnRight);
			motorOn = 0;
			gapDistance = 0;
		}	
	}
	
	TerminateTask();
}
