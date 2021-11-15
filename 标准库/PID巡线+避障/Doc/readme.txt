TIM_SetCompare2(ADVANCE_TIM,800);//小车右后轮
TIM_SetCompare3(ADVANCE_TIM,800);//小车左后轮


中断优先级:
超声波输入捕获：NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:0位抢占优先级，0位响应优先级

串口2接收中断：设置NVIC中断分组2:0位抢占优先级，2位响应优先级


定时器中断：设置NVIC中断分组2:0位抢占优先级，1位响应优先级