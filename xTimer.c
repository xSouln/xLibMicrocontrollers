/*
 * Timers.c
 *
 * Created: 12.03.2020 18:33:54
 *  Author: rekuts
 */
//=================================================================================================================================
#include "xTimer.h"
#include "tim.h"
//=================================================================================================================================
TimerT Timer;

TimStm32_T *xTimer1 = (TimStm32_T*)TIM1;
TimStm32_T *xTimer2 = (TimStm32_T*)TIM2;
TimStm32_T *xTimer3 = (TimStm32_T*)TIM3;
TimStm32_T *xTimer4 = (TimStm32_T*)TIM4;
/*
TimStm32_T *xTimer5 = (TimStm32_T*)TIM5;
TimStm32_T *xTimer6 = (TimStm32_T*)TIM6;
TimStm32_T *xTimer7 = (TimStm32_T*)TIM7;
TimStm32_T *xTimer8 = (TimStm32_T*)TIM8;
TimStm32_T *xTimer9 = (TimStm32_T*)TIM9;
TimStm32_T *xTimer10 = (TimStm32_T*)TIM10;
*/
//=================================================================================================================================
inline void xTimerHandler(){
  Timer.Events.Time1ms = true;
  if(++Timer.Counters.Time10ms == 10){ Timer.Counters.Time10ms = 0; Timer.Events.Time10ms = true;
    if(++Timer.Counters.Time100ms == 10){ Timer.Counters.Time100ms = 0; Timer.Events.Time100ms = true;
      if(++Timer.Counters.Time1000ms == 10){ Timer.Counters.Time1000ms = 0; Timer.Events.Time1000ms = true; }
    }
  }
}
//=================================================================================================================================
void xTimerDecrement(xTimerT* timer)
{
  xTimerRequestT *request;
  xListElementT* element = timer->Requests.FirstElement;
  timer->Handler.Decrement = true;
  
  for (uint8_t i = 0; i < timer->Requests.Count; i++)
  {
    request = (xTimerRequestT*)element->Value;//xListGet(&timer->Requests, i);
    element = element->Next;
    if(request->Retention && request->Handler.Enable){ request->Retention--; }
  }
  
  timer->Handler.Decrement = false;
}
//=================================================================================================================================
void xTimer(xTimerT* timer)
{
  xTimerRequestT *request;
  xListElementT* element = timer->Requests.FirstElement;
  timer->Handler.Update = true;
  
  uint8_t i = 0;
  while(i < timer->Requests.Count)
  {
    request = (xTimerRequestT*)element->Value;//xListGet(&timer->Requests, i);
    element = element->Next;
    
    if(request->Action && !request->Retention)
    {
      request->Action(timer, request);
      
      if(request->Period)
      {
        request->Retention = request->Period;
      }
      else
      {
        xListRemoveAt(&timer->Requests, i);
        free(request);
        goto end_while;
      }
    }
    
    i++;
  end_while:;
  }
  
  timer->Handler.Update = false;
}
//=================================================================================================================================
xTimerRequestT* xTimerAdd(xTimerT* timer, xTimerAction action, uint32_t retention, uint32_t period){
  timer->Handler.Add = true;
  
  xTimerRequestT* request = (xTimerRequestT*)calloc(1, sizeof(xTimerRequestT));
  request->Retention = retention;
  request->Period = period;
  request->Action = action;
  xListAdd(&timer->Requests, request);
  
  timer->Handler.Add = false;
  return request;
}
//=================================================================================================================================