#include <stdio.h>
#include <wiringPi.h>
char LED = 26;
int main()
{
	printf("Hello this is the wiringPi testing code.%d\n",__LINE__);
	printf("Build data is %s--%s\n",__DATE__,__TIME__);
	if (wiringPiSetup() < 0)
	{
		printf("wiringPiSetup error!!!\n");
		return 1;
	}
	wiringPiSetupGpio();	//This is very important.
	printf("Init LED\n");
	pinMode(LED, OUTPUT);
	printf("Have set output\n");
	while(1)
	{
		digitalWrite(LED, 1);
		delay(5);	//Delay 5ms
		digitalWrite(LED, 0);
		delay(5);	//Delay 5ms
	}

}
