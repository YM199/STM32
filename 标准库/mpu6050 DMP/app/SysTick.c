#include "SysTick.h"
#include "key.h" 
#include "multi_button.h"
#include "soft_timer.h"

void SysTick_Callback(void)
{
#if Key_Scan_OPEN==1
		if(xitong_haomiao%Key_Scan_Time==0)
			Key=KEY_Scan(0);
#endif		
#if Multi_Button_OPEN==1
		if(xitong_haomiao%TICKS_INTERVAL==0)
			Button_Ticks();
#endif		
#if SOFT_TIMER_OPEN==1
	  if(xitong_haomiao%TIMER_CYCEL==0)
			SoftTimer_Update();
#endif		
}