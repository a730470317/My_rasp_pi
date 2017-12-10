#include <wiringPi.h>  
#include <stdio.h>
#define PWMPin 1    //定义PWM引脚  

void setup();  

int main (void)  
{  
		printf("Build date is %s - %s \n",__DATE__,__TIME__);
		setup();  
		int val = 0;  
		int step = 100;  
		while(1)  
		{ 
			   	printf("Current val is : %d\n", val);	
				if(val>1000)  
				{  
						step = -step;      
						val = 1000;  
				}  
				else if(val<0)  
				{  
						step = -step;  
						val = 0;  
				}  

				pwmWrite(PWMPin,val);  
				val+=step;  
				delay(1000);  
		}  
		return 0 ;  
}   
/*初始化配置*/  
void setup()  
{  
		wiringPiSetup ();         //wiringPi库初始化  
		pinMode (PWMPin, PWM_OUTPUT);   //设置1为PWM输出      
		pwmSetMode(PWM_MODE_MS);
		pwmToneWrite(1,250);
		pwmSetClock(192/20); // The frequency of wiringPi is 19.2Mhz
		pwmSetRange(1000);
}  
