#include <stdio.h>

#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include <iot_pwm.h>
#include <iot_gpio.h>

#include <unistd.h>
#include <hi_types_base.h>
#include <hi_early_debug.h>

#include <hi_pwm.h>
#include <hi_gpio.h>
#include <hi_io.h>
#include "car_test.h"

//IO
#define GPIO0 0
#define GPIO1 1
#define GPIO9 9
#define GPIO10 10
#define GPIOFUNC 0
#define PWM_FREQ_FREQUENCY  (60000)
//
//
int state = 0;
int table_state=-1;
int pos_state=109;
int show_state=0;
int step_count = 20;
struct car_sys_info car_info;
void car_info_init(void)
{
	car_info.go_status = CAR_STATUS_STOP;
	car_info.cur_status = CAR_STATUS_STOP;

	car_info.mode = CAR_MODE_STEP;

	car_info.step_count = step_count;
}

void step_count_update(void)
{
	car_info.step_count = step_count;
}

void set_car_status(CarStatus status)
{
	if(status != car_info.cur_status)
	{
		car_info.status_change = 1;
	}
	car_info.go_status = status;

	step_count_update();
}


//设置行驶模式
void set_car_mode(CarMode mode)
{
	car_info.mode = mode;
}


void pwm_init(void)
{
	IoTGpioInit(IO_NAME_GPIO_0);
	IoTGpioInit(IO_NAME_GPIO_1);
	IoTGpioInit(IO_NAME_GPIO_9);
	IoTGpioInit(IO_NAME_GPIO_10);

	//引脚复用
	hi_io_set_func(IO_NAME_GPIO_0, IO_FUNC_GPIO_0_PWM3_OUT); 
    hi_io_set_func(IO_NAME_GPIO_1, IO_FUNC_GPIO_1_PWM4_OUT); 	
	hi_io_set_func(IO_NAME_GPIO_9, IO_FUNC_GPIO_9_PWM0_OUT); 
    hi_io_set_func(IO_NAME_GPIO_10, IO_FUNC_GPIO_10_PWM1_OUT); 

	//初始化pwm
	// IoTPwmInit(PWM_PORT_PWM3);
    // IoTPwmInit(PWM_PORT_PWM4);
	// IoTPwmInit(PWM_PORT_PWM0);
    // IoTPwmInit(PWM_PORT_PWM1);

	hi_pwm_init(PWM_PORT_PWM3);
	hi_pwm_init(PWM_PORT_PWM0);
	hi_pwm_init(PWM_PORT_PWM4);
	hi_pwm_init(PWM_PORT_PWM1);

	hi_pwm_set_clock(PWM_CLK_160M);
}

//停止
void pwm_stop(void)
{
	//先停止PWM
	// IoTPwmStop(PWM_PORT_PWM3);
    // IoTPwmStop(PWM_PORT_PWM4);
	// IoTPwmStop(PWM_PORT_PWM0);
    // IoTPwmStop(PWM_PORT_PWM1);
	hi_pwm_stop(PWM_PORT_PWM3);
	hi_pwm_stop(PWM_PORT_PWM4);
	hi_pwm_stop(PWM_PORT_PWM0);
	hi_pwm_stop(PWM_PORT_PWM1);
}
void car_stop(void)
{
	car_info.cur_status = car_info.go_status;

	printf("pwm_stop \r\n");

	pwm_stop();
}

//前进
void pwm_forward(void)
{
	hi_pwm_stop(PWM_PORT_PWM3);
	hi_pwm_stop(PWM_PORT_PWM4);
	hi_pwm_stop(PWM_PORT_PWM0);
	hi_pwm_stop(PWM_PORT_PWM1);
    hi_pwm_start(PWM_PORT_PWM3,1350,1500); /* duty: 750 freq:1500 */
	hi_pwm_start(PWM_PORT_PWM0, 1000, 1500); /* duty: 750 freq:1500 *///右前
	printf("pwm_forward_end\n");
}
void car_forward(void)
{
	step_count = 200;
	if(car_info.go_status != CAR_STATUS_FORWARD)
	{
		//直接退出
		printf("out out out out \n");
		return ;
	}
	// if(car_info.cur_status == car_info.go_status)
	// {
	// 	//状态没有变化，直接推出
	// 	return;
	// }

	car_info.cur_status = car_info.go_status;

	printf("pwm_forward \r\n");

	pwm_forward();

	step_count_update();
}

//后退
void pwm_backward(void)
{
	hi_pwm_stop(PWM_PORT_PWM3);
	hi_pwm_stop(PWM_PORT_PWM4);
	hi_pwm_stop(PWM_PORT_PWM0);
	hi_pwm_stop(PWM_PORT_PWM1);
    hi_pwm_start(PWM_PORT_PWM4, 1350, 1500); /* duty: 750 freq:1500 */
	hi_pwm_start(PWM_PORT_PWM1, 1000, 1500); /* duty: 750 freq:1500 */
	printf("pwm_backward_end\n");
}
void car_backward(void)
{
	step_count = 200;
	if(car_info.go_status != CAR_STATUS_BACKWARD)
	{
		//直接退出
		return ;
	}
	// if(car_info.cur_status == car_info.go_status)
	// {
	// 	//状态没有变化，直接推出
	// 	return;
	// }

	car_info.cur_status = car_info.go_status;

	printf("pwm_backward \r\n");

	pwm_backward();

	step_count_update();
}

//左转
void pwm_left(void)
{
	hi_pwm_stop(PWM_PORT_PWM3);
	hi_pwm_stop(PWM_PORT_PWM4);
	hi_pwm_stop(PWM_PORT_PWM0);
	hi_pwm_stop(PWM_PORT_PWM1);
	hi_pwm_start(PWM_PORT_PWM4, 1350, 1500);
    hi_pwm_start(PWM_PORT_PWM0, 1000, 1500); /* duty: 750 freq:1500 */
	printf("pwm_left_end\n");
	
}
void car_left(void)
{
	step_count = 60;
	if(car_info.go_status != CAR_STATUS_LEFT)
	{
		//直接退出
		return ;
	}
	// if(car_info.cur_status == car_info.go_status)
	// {
	// 	//状态没有变化，直接推出
	// 	return;
	// }

	car_info.cur_status = car_info.go_status;

	printf("pwm_left \r\n");

	pwm_left();

	step_count_update();
}

//右转
void pwm_right(void)
{
	hi_pwm_stop(PWM_PORT_PWM3);
	hi_pwm_stop(PWM_PORT_PWM4);
	hi_pwm_stop(PWM_PORT_PWM0);
	hi_pwm_stop(PWM_PORT_PWM1);
    hi_pwm_start(PWM_PORT_PWM3, 1350, 1500); /* duty: 750 freq:1500 */
	hi_pwm_start(PWM_PORT_PWM1, 1000, 1500);
	printf("pwm_right_end\n");
}
void car_right(void)
{
	step_count = 60;
	if(car_info.go_status != CAR_STATUS_RIGHT)
	{
		//直接退出
		return ;
	}
	// if(car_info.cur_status == car_info.go_status)
	// {
	// 	//状态没有变化，直接推出
	// 	return;
	// }

	car_info.cur_status = car_info.go_status;

	printf("pwm_right \r\n");

	pwm_right();

	step_count_update();
}


// CarStatus carstatus = CAR_STATUS_STOP;
// CarMode carmode = CAR_MODE_STEP;

extern void start_udp_thread(void);

void car_test(void)
{
	start_udp_thread();
    pwm_init();
	car_info_init();
	// set_car_status(CAR_STATUS_FORWARD);
	// set_car_mode(CAR_MODE_ALWAY);
	
	while(1)
	{
		if(car_info.status_change)
		{
			car_info.status_change = 0;
			printf("state = %d\n",state);
			switch(car_info.go_status)
			{
				case CAR_STATUS_STOP:
					state = 1;
					car_stop();
					break;
				
				case CAR_STATUS_FORWARD:
					state = 2;
					step_count = 200;
					//printf("car_test_forward\n");
					car_forward();
					break;

				case CAR_STATUS_BACKWARD:
					state = 3;
					step_count = 200;
					car_backward();
					break;

				case CAR_STATUS_LEFT:
					state = 4;
					step_count = 60;
					car_left();
					break;

				case CAR_STATUS_RIGHT:
					state = 5;
					step_count = 60;
					car_right();
					break;

				default:

					break;
			}
		}
		if(car_info.mode == CAR_MODE_STEP)
		{
			if(car_info.go_status != CAR_STATUS_STOP)
			{
				if(car_info.step_count > 0)
				{
					car_info.step_count --;
				}else{
					printf("stop... \r\n");
					set_car_status(CAR_STATUS_STOP);
				}
			}
		}

		usleep(1000);
	}
	
}

void car_run(void)
{
	//start_udp_thread();
    pwm_init();
	// car_info_init();
	//set_car_status(CAR_STATUS_FORWARD);
	//set_car_mode(CAR_MODE_ALWAY);
	printf("in in in in\n");
	switch(car_info.go_status)
	{
		case CAR_STATUS_STOP:
			state = 1;
			car_stop();
			break;
		
		case CAR_STATUS_FORWARD:
			state = 2;
			step_count = 200;
			//printf("car_run_forward\n");
			car_forward();
			break;

		case CAR_STATUS_BACKWARD:
			state = 3;
			step_count = 200;
			//printf("car_run_backward\n");
			car_backward();
			break;

		case CAR_STATUS_LEFT:
			state = 4;
			step_count = 60;
			car_left();
			break;

		case CAR_STATUS_RIGHT:
			state = 5;
			step_count = 60;
			car_right();
			break;

		default:

			break;
	}
	usleep(1000);
}

