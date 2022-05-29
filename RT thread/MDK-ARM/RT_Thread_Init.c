#include "RT_Thread_Init.h"
#include <rtthread.h>
#include "main.h"





//rt_thread_handle
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t led2_thread = RT_NULL;
static rt_thread_t led3_thread = RT_NULL;

//rt_timer_handle
static rt_timer_t SW_timer = RT_NULL;

//rt_semaphore_handle
static rt_sem_t sem_close_led3 = RT_NULL;


//rt_thread_function
static void led1_thread_entry(void *parameter);
static void led2_thread_entry(void *parameter);
static void led3_thread_entry(void *parameter);

//rt_timer_function
static void SW_timer_callback(void *parameter);

int Start_thread(void)
{
	//semaphore
	sem_close_led3 = rt_sem_create("sem_close_led3",0,RT_IPC_FLAG_FIFO);
	if(sem_close_led3 != RT_NULL)
	{
		rt_kprintf("sem is created\n");
	}
	//thread_one
	led1_thread = rt_thread_create("led1",led1_thread_entry,RT_NULL,512,1,20);
	if (led1_thread != RT_NULL )
	{
		rt_thread_startup(led1_thread);
	}else{
		return -1;
	}
	//thread_two
	led2_thread = rt_thread_create("led2",led2_thread_entry,RT_NULL,512,2,20);
	if (led2_thread != RT_NULL)
	{
		rt_thread_startup(led2_thread);
	}else{
		return -1;
	}
	//thread_three
	led3_thread = rt_thread_create("led3",led3_thread_entry,RT_NULL,512,3,20);
	if (led3_thread != RT_NULL)
	{
		rt_thread_startup(led3_thread);
	}else{
		return -1;
	}
	
	//SW timer
	SW_timer =  rt_timer_create("SW_timer",SW_timer_callback,0,2000,RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
	if(SW_timer != RT_NULL)
	{
		rt_timer_start(SW_timer);
	}else{
		return -1;
	}
	


	
	return 0;
}

void led1_thread_entry(void *parameter)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_3);
		rt_thread_delay(500);
		
	}
}

void led2_thread_entry(void *parameter)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_4);
		rt_thread_delay(1000);
	}
}

void led3_thread_entry(void *parameter)
{
	while(1)
	{
		rt_sem_take(sem_close_led3,RT_WAITING_FOREVER);
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_9);
		rt_thread_delay(2000);
	}
}

void SW_timer_callback(void *parameter)
{
	rt_kprintf("SW timer timeout\n");
	rt_thread_delete(led1_thread);
	rt_sem_release(sem_close_led3);
}