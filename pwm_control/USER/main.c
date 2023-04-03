#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
extern int Steering_Speedflag;	
uint8_t TIM1_Enable   = ENABLE;
 int main(void)
 { 
	int SpeedDecrease  = 5;
	int CeleOrDeceFlag = 0;
	int PWMChangeFlag  = 0;
	int InitPWMFlag    = 0;
  vu8 key            = 0;	
  u16 delay_Val      = 100;	 
 	u16 led0pwmval     = 195;
	u16 TimeForPWM     = 2000;
	u8 dir=1;	
	delay_init();	 
	KEY_Init();    
  TIM1_Int_Init(TimeForPWM, 7199, TIM1_Enable);	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
 	TIM3_PWM_Init(199,7199);	 //不分频。PWM频率=72000000/900=80Khz
  while(1)
	{
     key=KEY_Scan(0);	//得到键值
		 if(key)
		 { 
			 printf("key is %d\n",key);
			 switch(key)
			 {
				case WKUP_PRES:	//直接切换舵机旋转方向
          TimeForPWM +=400;
				  TIM1_Int_Init(TimeForPWM, 7199, TIM1_Enable);	
					break;
				case KEY2_PRES:	//直接切换舵机PWM值到0或最高
					InitPWMFlag =!InitPWMFlag;
          if(InitPWMFlag) led0pwmval = 170;
          else led0pwmval = 197;
				  printf("led0pwmval is %d\n",led0pwmval);
					break;
				case KEY1_PRES:	//舵机减速程度
					PWMChangeFlag =!PWMChangeFlag;
          if(PWMChangeFlag) SpeedDecrease = 5;
          else SpeedDecrease = 1;
				  printf("SpeedDecrease is %d\n",SpeedDecrease);
					break;
				case KEY0_PRES:	//定时器来切换舵机旋转频度 
          TimeForPWM -=400;
				  if(TimeForPWM <200) TimeForPWM = 200;
				  TIM1_Int_Init(TimeForPWM, 7199, TIM1_Enable);	 
				  printf("ConstantOrChangeSpeed is %d\n",TimeForPWM);
					break;
			 }
		 }
		 
		 
		 switch(Steering_Speedflag)
        {
        case 1: // for timer 1
          if(led0pwmval <= 170) CeleOrDeceFlag = 0;
          else if(led0pwmval >= 195) CeleOrDeceFlag = 1;
          switch(CeleOrDeceFlag)
            {
            case 0:
							//printf("led0pwmval is %d\n",led0pwmval);
              led0pwmval +=SpeedDecrease; // decelation
              printf("led0pwmval is %d\n",led0pwmval);
              break;
            case 1:
              led0pwmval -=SpeedDecrease; // celelation
						  printf("led0pwmval is %d\n",led0pwmval);
              break;
            default:
              break;
            }
          Steering_Speedflag = 0;
          break;

        default:
          break;
        }
		 TIM_SetCompare2(TIM3,led0pwmval);
		 //delay_ms(delay_Val);
	}	 
 }
 
 
 
 //        case 2: // for CAN message coming
//          switch(Rx1_DATA0)
//            {
//            case 1://
//              SpeedDecrease = 6;
//              led0pwmval -= SpeedDecrease;
//              LED0 = !LED0;
//              printf("bl %d\n", Rx1_DATA4);
//              break;
//            case 2:
//							LED1 = !LED1;
//              printf("de %d\n", Rx1_DATA4);
//              break;
//            case 3:
//							LEDA1 = !LEDA1;
//              printf("ce %d\n", Rx1_DATA4);
//              break;
//            case 4:
//							LEDA4 = !LEDA4;
//              printf("con %d\n", Rx1_DATA4);
//              break;
//            default:
//              break;
//            }
//          Steering_Speedflag = 0;
//          break;
//				 case 3: // for timer 4
//							tv =TSpeed(Tem_NumHigh2Freq);
//						//printf("tv:%.3f\n",tv);
//						Steering_Speedflag = 0;
//						break;



//		 TIM_SetCompare2(TIM3,195);		
//     delay_ms(100);		
////		 TIM_SetCompare2(TIM3,190);		
////     delay_ms(100);	
////		 TIM_SetCompare2(TIM3,185);		
////     delay_ms(100);	
//		 TIM_SetCompare2(TIM3,180);		
//     delay_ms(100);	
////				 TIM_SetCompare2(TIM3,175);		
////     delay_ms(100);	
////				 TIM_SetCompare2(TIM3,170);		
////     delay_ms(100);	
//				 TIM_SetCompare2(TIM3,165);		
//     delay_ms(100);	
////		 		 TIM_SetCompare2(TIM3,160);		
////     delay_ms(100);	