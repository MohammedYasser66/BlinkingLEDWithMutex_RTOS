#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "StackMacros.h"



// 2. Declarations Section
//   Global Variables
unsigned long SW1,SW2;  // input from PF4,PF0
unsigned long Out;      // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void PortF_Init(void);
void Delay(void);
void Blink(int x);

#define mainLED_FLASH_PRIORITY			( tskIDLE_PRIORITY + ( unsigned portBASE_TYPE ) 1 )

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );

SemaphoreHandle_t MyMutex; 

int main(void){    
	
	MyMutex= xSemaphoreCreateMutex();	 
	PortF_Init();        
	
	xTaskCreate( vTask1, "Task1", 240, NULL, 2, NULL );
	xTaskCreate( vTask2, "Task2", 240, NULL, 2, NULL );

	/* Start the scheduler. */
	vTaskStartScheduler();
}

/*-----------------------------------------------------------*/

static void vTask1( void *pvParameters )
{
	for( ;; )
	{
		xSemaphoreTake(MyMutex ,portMAX_DELAY);
		
		GPIO_PORTF_DATA_R = 4;
		vTaskDelay(330); 	//Delay 1 second
		
		xSemaphoreGive(MyMutex);	
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/

static void vTask2( void *pvParameters )
{
	for( ;; )
	{
		xSemaphoreTake(MyMutex ,portMAX_DELAY);
		
		GPIO_PORTF_DATA_R = 8;
		vTaskDelay(330); 	//Delay 1 second
		
		xSemaphoreGive(MyMutex);	
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/


void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
	
	
	
    //

    while(1)
    {
    }
}





void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06


