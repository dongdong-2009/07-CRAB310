
/* 头文件包含 ------------------------------------------------------------------*/
#include "Process.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdlib.h>
#ifdef __cplusplus
 }
#endif
 
/* 类的实现--------------------------------------------------------------------*/
 
/**
  * @brief  TIM2定时器中断函数
  * @param  None
  * @retval None
  */
void Process::runOnTime2(void)
{
	getJoyStickValue();													//	获取摇杆数据
	getBatValue();														//	获取电池电压
	getKeyValue();													//	获取AD按键值
	dealKeyValue();														  //	处理按键
	
	if(protocol->com2nd_Receive())										//	接收串口数据
	{
		dealBotData();													//	处理数据		
	}
}

/**
  * @brief  TIM3定时器中断函数,更新电阻值与电源电压
  * @param  None
  * @retval None
  */
void Process::runOnTime3(void)
{	
	
	detectPad();                                // 检测Pad
	detectPowerSwitch();                         //检测电源开关
	dealPadData();													//	处理数据
	
	
	setPowerLed();														//	设置电量显示
	
	setComToPadData();	
	
	protocol->com1st_UpdateSendBuf(com1st_DataToSend);	
	if(protocol->com1st_Receive())										//	接收到pad数据之后再发送
	{
//		testLed->reverseState();
		//dealPadData();													//	处理数据	
	}
	//if(ctrFlag_PadInsert)												//	插入pad才发数据
	{
		protocol->com1st_Send();										//	发送数据	
	}
}

/**
  * @brief  TIM4定时器中断函数，TIM4本程序中未使用
  * @param  None
  * @retval None
  */
void Process::runOnTime4(void)
{
//	if (ctrChannel==botChannel)
//	{
//		setCom2Data();
//		protocol->com2nd_UpdateSendBuf(com2nd_DataToSend);
//		protocol->com2nd_Send();											//	com2nd发送数据
//	}		
//	else//	if (ctrChannel!=botChannel)	//	设置控制器通道
//	{
//		setChannel(botChannel);
//		setChannel(botChannel);
//		ctrChannel=botChannel;
//	}
//	testWithUsart();													//	调试代码，查看AD数值
	setComToRobotData();
	protocol->com2nd_UpdateSendBuf(com2nd_DataToSend);
	protocol->com2nd_Send();											//	com2nd发送数据
	ctrChannel=botChannel;
}


/**
  * @brief  获取控制数据
  * @param  None
  * @retval None	
  */
#define	POWER_OFF_COUNT_DOWN			10							//	关机计时,5s以后自动关机
#define	POWER_OFF_HARD_WARE_COUNT_DOWN	50							//	强制关机计时
void Process::dealKeyValue()
{
	static 	bool	preLeftAdKey[LEFT_KEY_NUM]		={false,false,false,false};

	static 	bool	preRightAdKey[RIGHT_KEY_NUM]		={false,false,false,false};
	static 	bool	finalRightAdKey[RIGHT_KEY_NUM]	={false,false,false,false};	

	static 	uint8_t finalVedioSwitch=0x01;							//	视频切换
	static 	uint8_t finalSpeed=0x01;									//	速度调节
//	static 	uint16_t	trigAutoCount=0;								//	触发自动模式计时


	
//<左按键 处理>
	//	设置远焦/近焦
	ctrBotData1&=~0x03;												//	清零bit1~0							
	if(zoomInKey->FinalState==false)											//	远焦
	{
		ctrBotData1|=0x01;
	}
	else if (zoomOutKey->FinalState==false)										//	近焦
	{
		ctrBotData1|=0x01<<1;
	}

	//	设置速度档位	
	if (preLeftAdKey[LEFT3]!=speedUpKey->FinalState)						//	左3按键-速度+；每按1次速度+1
	{
		preLeftAdKey[LEFT3]=speedUpKey->FinalState;
		if (speedUpKey->FinalState==true && finalSpeed<0x03)				//	速度最大3			
			finalSpeed++;
	}
	if (preLeftAdKey[LEFT4]!=speedDownKey->FinalState)						//	左4按键-速度-；每按1次速度-1
	{
		preLeftAdKey[LEFT4]=speedDownKey->FinalState;
		if (speedDownKey->FinalState==true && finalSpeed>0x00)				//	速度最小0			
			finalSpeed--;
	}
	
//	finalSpeed=2;
	
	ctrBotData1	&=~(0x03<<4);										//	清零bit5~4
	ctrBotData1	|=finalSpeed<<4;									//	设置本体数据
	ctrStatus2	&=~(0x03<<6);										//	清零bit7~6
	ctrStatus2	|=finalSpeed<<6;									//	设置pad数据
	
//</左按键 处理>
	
	//	设置自动模式	【代码调试完毕】
//	if (0X7A>=ctrLeftX||ctrLeftX>=0X85||0X7A>=ctrLeftY||ctrLeftY>=0X85)		//	如果运动摇杆动，则退出自动模式
//	{
//		ctrFlag_Auto=false;
//	}
//	if (botMode==0x02)												//	自动模式完成后才退出自动模式
//	{
//		ctrFlag_Auto=false;										
//	}
//	if (rightAdKey[RIGHT1])											//	右1按键持续按下1s开启自动模式
//	{	
//		trigAutoCount++;
//		if(trigAutoCount>=(1000/TIM3_DELAY_TIME))
//		{
//			ctrFlag_Auto=true;
//			trigAutoCount=0;
//		}
//	}
//	else
//	{
//		trigAutoCount=0;
//	}
	
	
	//<右按键 处理>
	if (!autoModeKey->FinalState)											//	右1按键持续按下1s开启自动模式
	{	
		ctrFlag_Auto=true;
	}
	else
	{
		ctrFlag_Auto=false;
	}
	ctrBotData2&=~(0x03<<4);
	if (ctrFlag_Auto)
		ctrBotData2|=0x01<<4;
		
	//	设置画面切换	
	if (preRightAdKey[RIGHT2] != pictureSwitchKey->FinalState)					//	右2按键-视频切换
	{
		preRightAdKey[RIGHT2]=pictureSwitchKey->FinalState;
		if (pictureSwitchKey->FinalState==false)
			finalVedioSwitch++;
	}
	if(finalVedioSwitch>3)
		finalVedioSwitch=1;
	
	ctrBotData1&=~(0x03<<2);										//	清零bit3~2
	ctrBotData1|=finalVedioSwitch<<2;
	ctrStatus2&=~(0x03<<4);											//	清零bit5~4
	ctrStatus2|=finalVedioSwitch<<4;
	
	//	设置行车灯
	
	if (preRightAdKey[RIGHT3] != vehicleLedKey->FinalState)					//	右3按键-行车灯
	{
		preRightAdKey[RIGHT3]=vehicleLedKey->FinalState;
		if (vehicleLedKey->FinalState==false)
		{
			finalRightAdKey[RIGHT3]=!finalRightAdKey[RIGHT3];   //取反，因为开灯是奇偶操作
			if (finalRightAdKey[RIGHT3])
			{
				ctrBotData1&=~(0x01<<7);										//	清零bit7
				ctrBotData1|=0x01<<7;
			}
			else
			{
				ctrBotData1&=~(0x01<<7);										//	清零bit7
			}
		}
	}
	
	//	设置观测灯
	
	if (preRightAdKey[RIGHT4] != observeLedKey->FinalState)					//	右4按键-观测灯
	{
		preRightAdKey[RIGHT4]= observeLedKey->FinalState;
		if (observeLedKey->FinalState == false)
		{
			finalRightAdKey[RIGHT4]=!finalRightAdKey[RIGHT4];          //取反，因为开灯是奇偶操作
			if (finalRightAdKey[RIGHT4])
			{
				ctrBotData1&=~(0x01<<6);										//	清零bit6
				ctrBotData1|=0x01<<6;
			}
			else
			{
				ctrBotData1&=~(0x01<<6);										//	清零bit6
			}
		}
	}
	
	//	更新状态到pad
	ctrStatus1=0x00;
	ctrStatus1|=botMode<<6;
	ctrStatus1|=botIR<<4;
	ctrStatus1|=ctrChannel;
	//	设置本体通道
	ctrBotData2&=0xF0;							//	清零bit3~0	
	ctrBotData2|=padChannel;
	
	//</右按键 处理>
	
		

}

//检测关机按钮是否关机
void Process::detectPowerSwitch()
{
	static	uint8_t	powerOffHardWareCountDown=0;					
	//	设置关机
	if (AD_Filter[NUM_KEY_POWER]<=0X200)
		ctrFlag_PowerOn=1;
	
	if(AD_Filter[NUM_KEY_POWER]>=0X240 && ctrFlag_PowerOn==1)
	{
		
		ctrData1&=~(0x03<<6);
		ctrData1|=0x01<<6;
		
		if(powerOffHardWareCountDown<POWER_OFF_HARD_WARE_COUNT_DOWN)
			powerOffHardWareCountDown++;
		else
		{
			ctrFlag_PowerLedOff=true;
			setPowerState(false);	//关机
		}
		
	}
}

//检测Pad是否连接
void Process::detectPad()
{

	static	uint8_t	padInsertCount=0;								//	pad插入计时
	padDetector->getValue();
	
		//	检测pad是否插入
	if (padDetector->FinalState && !ctrFlag_PadInsert)	//插入后AD检测到接地
	{
		padInsertCount++;
		if(padInsertCount>=20)
		{
			ctrFlag_PadInsert=true;
			padInsertCount=0;
		}
	}
	else if (!padDetector->FinalState)
	{
		padInsertCount=0;
		ctrFlag_PadInsert=false;
	}
//	if(ctrFlag_PadInsert)
//		vbtCtr->setOn();
//	else
//		vbtCtr->setOff();
	
	blueToothPowCtrl->setOn();
	videoCardPowCtrl->setOn();
	
}

/**
  * @brief  处理Pad数据
  * @param  None
  * @retval None
	* @Modified wuxiyou 1111
  */
void Process::dealPadData(void)
{
	static	uint8_t	padOffDelayCount=0;								//	pad关机延时
	uint8_t	*dataFromPad;
	uint8_t	tempPadOffState=0;
	
	dataFromPad 	= protocol->com1st_GetData();
	tempPadOffState	= dataFromPad[3]&0x03;			//	bit 1~0;
	padChannel		= dataFromPad[4]&0x0F;			//	bit 3~0;
	
	if(tempPadOffState!=0x00)
		padOffState=true;
	
	if(padOffState)
	{
		padOffDelayCount++;
		if(padOffDelayCount>=POWER_OFF_COUNT_DOWN)
		{
			ctrFlag_PowerLedOff=true;
			setPowerState(false);	//关机
		}
	}
	
}

/**
  * @brief  处理本体数据
  * @param  None
  * @retval None
已经废弃不用
  */
void Process::dealBotData(void)
{
	uint8_t	*dataFromBot;
	dataFromBot		= protocol->com2nd_GetData();
	botLeftBatVal	= dataFromBot[3];			//	bit 7~0;
	botRightBatVal	= dataFromBot[4];			//	bit 7~0;
	botAngle		= dataFromBot[5];
	botMode			= (dataFromBot[6]&(0x03<<6))>>6;
	botIR			= (dataFromBot[6]&(0x03<<4))>>4;
	botChannel		= dataFromBot[6]&0x0F;
}
/**
  * @brief  设置com1st需要发送的数据
  * @param  None
  * @retval None
  */
void Process::setComToPadData(void)
{
	com1st_DataToSend[0] = 0x80;
	com1st_DataToSend[1] = ctrData1;
	com1st_DataToSend[2] = ctrBatVal;
	com1st_DataToSend[3] = botLeftBatVal;
	com1st_DataToSend[4] = botRightBatVal;
	com1st_DataToSend[5] = botAngle;
	com1st_DataToSend[6] = ctrStatus1;
	com1st_DataToSend[7] = ctrStatus2;
	com1st_DataToSend[8] = 0x00;
}

/**
  * @brief  设置com2nd需要发送的数据
  * @param  None
  * @retval None
  */
void Process::setComToRobotData(void)
{
	com2nd_DataToSend[0] = 0x80;
	com2nd_DataToSend[1] = ctrLeftX;
	com2nd_DataToSend[2] = ctrLeftY;
	com2nd_DataToSend[3] = ctrRightX;
	com2nd_DataToSend[4] = ctrRightY;
//	com2nd_DataToSend[1] = 0x80;
//	com2nd_DataToSend[2] = 0;
//	com2nd_DataToSend[3] = 0x80;
//	com2nd_DataToSend[4] = 0x80;
	com2nd_DataToSend[5] = ctrBotData1;
	com2nd_DataToSend[6] = ctrBotData2;
	com2nd_DataToSend[7] = 0x00;
	com2nd_DataToSend[8] = 0x00;
}

/**
  * @brief  获取摇杆数据，由于摇杆本身不是轨对轨输出，此处需要做死区处理
  * @param  None
  * @retval None
  */
#define MIN_JOY_VALUE		0x20
#define	MIN_MIDDLE_VALUE	0x7A
#define	MAX_MIDDLE_VALUE	0x85
#define MAX_JOY_VALUE		0xE0
void Process::getJoyStickValue()
{	
	uint16_t tempLX, tempLY, tempRX, tempRY;
	
	tempLX	= (AD_Filter[NUM_ctrLeftX]>>4) *1.32;
	tempLY	= (AD_Filter[NUM_ctrLeftY]>>4) *1.32;
	tempRX	= (AD_Filter[NUM_ctrRightX]>>4) *1.32;
	tempRY	= (AD_Filter[NUM_ctrRightY]>>4) *1.32;
	
	
	//左遥感 X
	
	if (tempLX<=MIN_JOY_VALUE)
	{
		tempLX=0;
	}
	else if(tempLX<=MIN_MIDDLE_VALUE)
	{
		tempLX 	= (tempLX-MIN_JOY_VALUE)*0x80/(MIN_MIDDLE_VALUE-MIN_JOY_VALUE);
	}
	else if (tempLX<=MAX_MIDDLE_VALUE)
	{
		tempLX=0x80;
	}
	else if(tempLX<=MAX_JOY_VALUE)
	{
		tempLX	= (tempLX-MAX_MIDDLE_VALUE)*0x80/(MAX_JOY_VALUE-MAX_MIDDLE_VALUE)+0x7F;
	}
	else
	{
		tempLX = 0xFF;
	}
	
	
		//左遥感 Y
	if (tempLY<=MIN_JOY_VALUE)
	{
		tempLY=0;
	}
	else if(tempLY<=MIN_MIDDLE_VALUE)
	{
		tempLY 	= (tempLY-MIN_JOY_VALUE)*0x80/(MIN_MIDDLE_VALUE-MIN_JOY_VALUE);
	}
	else if (tempLY<=MAX_MIDDLE_VALUE)
	{
		tempLY=0x80;
	}
	else if(tempLY<=MAX_JOY_VALUE)
	{
		tempLY	= (tempLY-MAX_MIDDLE_VALUE)*0x80/(MAX_JOY_VALUE-MAX_MIDDLE_VALUE)+0x7F;
	}
	else
	{
		tempLY = 0xFF;
	}
	
	
		//右遥感 X 重点分析20161024
	
	if (tempRX<=MIN_JOY_VALUE)
	{
		tempRX=0;
	}
	else if(tempRX<=MIN_MIDDLE_VALUE)
	{
		tempRX 	= (tempRX-MIN_JOY_VALUE)*0x80/(MIN_MIDDLE_VALUE-MIN_JOY_VALUE);
	}
	else if (tempRX<=MAX_MIDDLE_VALUE)
	{
		tempRX=0x80;
	}
	else if(tempRX<=MAX_JOY_VALUE)
	{
		tempRX	= (tempRX-MAX_MIDDLE_VALUE)*0x80/(MAX_JOY_VALUE-MAX_MIDDLE_VALUE)+0x7F;
	}
	else
	{
		tempRX = 0xFF;
	}
	
	
		//右遥感 Y
	if (tempRY<=MIN_JOY_VALUE)
	{
		tempRY=0;
	}
	else if(tempRY<=MIN_MIDDLE_VALUE)
	{
		tempRY 	= (tempRY-MIN_JOY_VALUE)*0x80/(MIN_MIDDLE_VALUE-MIN_JOY_VALUE);
	}
	else if (tempRY<=MAX_MIDDLE_VALUE)
	{
		tempRY=0x80;
	}
	else if(tempRY<=MAX_JOY_VALUE)
	{
		tempRY	= (tempRY-MAX_MIDDLE_VALUE)*0x80/(MAX_JOY_VALUE-MAX_MIDDLE_VALUE)+0x7F;
	}
	else
	{
		tempRY = 0xFF;
	}
	
	
	ctrLeftX	= uint8_t(tempLX);
	ctrLeftY	= 255-uint8_t(tempLY);
	
	ctrRightX	= uint8_t(tempRX);
	ctrRightY	= uint8_t(tempRY);
	
	ctrLeftY=255-ctrLeftY;
	ctrRightY=255-ctrRightY;
}


/**
  * @brief  获取AD按键值,已加入软件消抖
  * @param  None
  * @retval None
Modified by wu xin you 1110
  */
/*
#define	AD_KEY_DEAD	3
#define	AD_KEY1_VALUE	0x80
#define	AD_KEY2_VALUE	0x55
#define	AD_KEY3_VALUE	0x40
#define	AD_KEY4_VALUE	0x33
#define	AD_KEY_COUNT	2
void Process::getAdKeyValue(void)
{
	uint8_t	leftAdKeyValue=0;
	uint8_t	rightAdKeyValue=0;
	static	bool tempLeftAdKeyValue[4] = {false,false,false,false};
	static	uint8_t countLeftAdKey[4] = {0,0,0,0};
	static	bool tempRightAdKeyValue[4] = {false,false,false,false};
	static	uint8_t countRightAdKey[4] = {0,0,0,0};
	

//左边 按键值处理
	leftAdKeyValue	= AD_Filter[NUM_KEY_LEFT]>>4;	
	if (leftAdKeyValue<=0x05)									//	左侧没有按键按下,下拉电阻1K
	{
		for(uint8_t i=0 ; i<LEFT_AD_KEY_NUM ; i++ )				//	清零全部按键数据
			tempLeftAdKeyValue[i]=false;
	}
	else if (leftAdKeyValue>(AD_KEY1_VALUE-AD_KEY_DEAD)&&leftAdKeyValue<(AD_KEY1_VALUE+AD_KEY_DEAD))			//	按下左1按键，上拉电阻120R
	{
		for(uint8_t i=0 ; i<LEFT_AD_KEY_NUM ; i++ )				//	清零全部按键数据
		{tempLeftAdKeyValue[i]=false;}		
		tempLeftAdKeyValue[LEFT1]=true;
	}
	else if (leftAdKeyValue>(AD_KEY2_VALUE-AD_KEY_DEAD)&&leftAdKeyValue<(AD_KEY2_VALUE+AD_KEY_DEAD))			//	按下左2按键，上拉电阻330R
	{
		for(uint8_t i=0 ; i<LEFT_AD_KEY_NUM ; i++ )				//	清零全部按键数据
		{tempLeftAdKeyValue[i]=false;}		
		tempLeftAdKeyValue[LEFT3]=true;
	}
	
	else if (leftAdKeyValue>(AD_KEY3_VALUE-AD_KEY_DEAD)&&leftAdKeyValue<(AD_KEY3_VALUE+AD_KEY_DEAD))			//	按下左3按键，上拉电阻1K
	{
		for(uint8_t i=0 ; i<LEFT_AD_KEY_NUM ; i++ )				//	清零全部按键数据
		{tempLeftAdKeyValue[i]=false;}		
		tempLeftAdKeyValue[LEFT2]=true;
	}
	else if (leftAdKeyValue>(AD_KEY4_VALUE-AD_KEY_DEAD)&&leftAdKeyValue<(AD_KEY4_VALUE+AD_KEY_DEAD))			//	按下左4按键，上拉电阻2K
	{
		for(uint8_t i=0 ; i<LEFT_AD_KEY_NUM ; i++ )				//	清零全部按键数据
		{tempLeftAdKeyValue[i]=false;}		
		tempLeftAdKeyValue[LEFT4]=true;
	}

	for(uint8_t i=0; i<4; i++)
	{
		if(leftAdKey[i]!=tempLeftAdKeyValue[i])
		{
			countLeftAdKey[i]++;
			if(countLeftAdKey[i]>AD_KEY_COUNT)
			{
				leftAdKey[i]=tempLeftAdKeyValue[i];
				countLeftAdKey[i]=0;
			}
		}
		else
		{
			countLeftAdKey[i]=0;
		}
	}
	//右边 按键值处理
	rightAdKeyValue	= AD_Filter[NUM_KEY_RIGHT]>>4;
	if (rightAdKeyValue<=0x05)									//	右侧没有按键按下,下拉电阻1K
	{
		for(uint8_t i=0 ; i<RIGHT_AD_KEY_NUM ; i++ )
			tempRightAdKeyValue[i]=false;
	}
	else if (rightAdKeyValue>(AD_KEY1_VALUE-AD_KEY_DEAD)&&rightAdKeyValue<(AD_KEY1_VALUE+AD_KEY_DEAD))		//	按下右1按键，上拉电阻120R
	{
		for(uint8_t i=0 ; i<RIGHT_AD_KEY_NUM ; i++ )			//	清零全部按键数据
			{tempRightAdKeyValue[i]=false;}		
		tempRightAdKeyValue[RIGHT1]=true;
	}
	else if (rightAdKeyValue>(AD_KEY2_VALUE-AD_KEY_DEAD)&&rightAdKeyValue<(AD_KEY2_VALUE+AD_KEY_DEAD))		//	按下右2按键，上拉电阻330R
	{
		for(uint8_t i=0 ; i<RIGHT_AD_KEY_NUM ; i++ )			//	清零全部按键数据
			{tempRightAdKeyValue[i]=false;}		
		tempRightAdKeyValue[RIGHT3]=true;
	}
	else if (rightAdKeyValue>(AD_KEY3_VALUE-AD_KEY_DEAD)&&rightAdKeyValue<(AD_KEY3_VALUE+AD_KEY_DEAD))		//	按下右3按键，上拉电阻1K
	{
		for(uint8_t i=0 ; i<RIGHT_AD_KEY_NUM ; i++ )			//	清零全部按键数据
			{tempRightAdKeyValue[i]=false;}		
		tempRightAdKeyValue[RIGHT2]=true;
	}
	else if (rightAdKeyValue>(AD_KEY4_VALUE-AD_KEY_DEAD)&&rightAdKeyValue<(AD_KEY4_VALUE+AD_KEY_DEAD))		//	按下右4按键，上拉电阻2K
	{
		for(uint8_t i=0 ; i<RIGHT_AD_KEY_NUM ; i++ )			//	清零全部按键数据
			{tempRightAdKeyValue[i]=false;}		
		tempRightAdKeyValue[RIGHT4]=true;
	}	
	
	for(uint8_t i=0; i<4; i++)
	{
		if(rightAdKey[i]!=tempRightAdKeyValue[i])
		{
			countRightAdKey[i]++;
			if(countRightAdKey[i]>AD_KEY_COUNT)
			{
				rightAdKey[i]=tempRightAdKeyValue[i];
				countRightAdKey[i]=0;
			}
		}
		else
		{
			countRightAdKey[i]=0;
		}
	}
	
	if(rightAdKey[0]||rightAdKey[1]||rightAdKey[2]||rightAdKey[3]||leftAdKey[0]||leftAdKey[1]||leftAdKey[2]||leftAdKey[3])
		testLed->setOn();
	else
		testLed->setOff();
}
*/
	
/**
  * @brief  获取8个按键值
  * @param  None
  * @retval None
  * @Modified wu xinyou 1111
  */
void Process::getKeyValue()
{
//	for(uint8_t index=0; index<NUM_KEY; index++) 				// 读取所有按键信息
//			keys[index]->getValue();							// 获取按键信息
	zoomInKey->getValue();
	zoomOutKey->getValue();
	speedUpKey->getValue();
	speedDownKey->getValue();
	
	vehicleLedKey->getValue();
	observeLedKey->getValue();
	autoModeKey->getValue();
	pictureSwitchKey->getValue();
	

	
	
}

/**
  * @brief  使能SWD，关闭JTAG
  * @param  None
  * @retval None
  */
void Process::enable_SWD_disable_Jtag()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//	使能AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//	关闭JTAG，引脚作为GPIO用
}



 Process::Process()
{
	enable_SWD_disable_Jtag();											//	关闭JTAG打开SWD	
	initGPIO();															//	初始化指示灯
	initADs();															//	初始化AD通道
	initUsarts();														//	初始化串口
	initTimers();														//	初始化定时器	
	initAllData();														//	初始化所有数据
	testLed->setOn();													//	
	SysTick_DelayMs(1500);												//	按下电源开关3000ms后再开机，数传需要初始化2s左右
	setPowerState(true);												//	开启系统电源
//	setDataCh(1);														//	每次上电必须设置一次数传
	testLed->setOff();													//	指示灯关闭-系统上电完成
	setPowerLed();
}
	 
/**
  * @brief  开启用户外设
  * @param  None
  * @retval None
  */
void Process::openPeriph()
{
	SysTick_DelayMs(10);												//	延时 10ms
	comToPad->open();    													//	开启通信端口1
	comToRobot->open();														//	开启通信端口2
	t2->start();														//	开启定时器2
	t3->start();														//	开启定时器3
	t4->start();														//	开启定时器4
}

/**
  * @brief  初始化IO
  * @param  None
  * @retval None
  */
void Process::initGPIO()
{	
	testLed		= new IoOut(TESTLED_PORT,TESTLED_PIN);					//	指示灯，低电平点亮
	//8个按键实例化
	zoomInKey    = new Key(KEY2_PORT,KEY2_PIN);    //默认为低电平有效
	zoomOutKey   = new Key(KEY4_PORT,KEY4_PIN); 		//正确
	speedUpKey   = new Key(KEY1_PORT,KEY1_PIN);     //正确
	speedDownKey = new Key(KEY3_PORT,KEY3_PIN);     //正确
	
	vehicleLedKey     = new Key(KEY5_PORT,KEY5_PIN);  //正确
	observeLedKey     = new Key(KEY8_PORT,KEY8_PIN);  //正确
  autoModeKey       = new Key(KEY6_PORT,KEY6_PIN);  //正确
	pictureSwitchKey  = new Key(KEY7_PORT,KEY7_PIN);  //正确
	//Pad 检测
	padDetector    = new Key(PADDET_PORT,PADDET_PIN);
	//3个电源控制开关  高电平有效
	blueToothPowCtrl  =new IoOut(CTRLBTPOW_PORT,CTRLBTPOW_PIN,true);
	videoCardPowCtrl  =new IoOut(CTRLVIDEOPOW_PORT,CTRLVIDEOPOW_PIN,true);
	systemPowCtrl     =new IoOut(CTRLPOW_PORT,CTRLPOW_PIN,true);
	//4个电源指示灯 低电平有效
	powerLed1   = new IoOut(POWLED1_PORT,POWLED1_PIN);
	powerLed2   = new IoOut(POWLED2_PORT,POWLED2_PIN);
	powerLed3   = new IoOut(POWLED3_PORT,POWLED3_PIN);
	powerLed4   = new IoOut(POWLED4_PORT,POWLED4_PIN);
	
	
	
	/*
	
	powerLed1R	= new IoOut(POWER_LED1R_PORT,POWER_LED1R_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed1B	= new IoOut(POWER_LED1B_PORT,POWER_LED1B_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed2R	= new IoOut(POWER_LED2R_PORT,POWER_LED2R_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed2B	= new IoOut(POWER_LED2B_PORT,POWER_LED2B_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed3R	= new IoOut(POWER_LED3R_PORT,POWER_LED3R_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed3B	= new IoOut(POWER_LED3B_PORT,POWER_LED3B_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed4R	= new IoOut(POWER_LED4R_PORT,POWER_LED4R_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	powerLed4B	= new IoOut(POWER_LED4B_PORT,POWER_LED4B_PIN,true);		//	指示灯，1输出高电平，0输出低电平
	
	powerCtr 	= new IoOut(POWER_CTR_PORT,POWER_CTR_PIN,true);			//	电源控制引脚，高电平开启
	
	digSet		= new IoOut(DIG_SET_PORT,DIG_SET_PIN,true);				//	数传配置引脚，高电平开启
	
	vedioCh1	= new IoOut(VEDIO_CH1_PORT,VEDIO_CH1_PIN,true);			//	图传配置通道1，高电平有效
	vedioCh2	= new IoOut(VEDIO_CH2_PORT,VEDIO_CH2_PIN,true);			//	图传配置通道2，高电平有效
	vedioCh3	= new IoOut(VEDIO_CH3_PORT,VEDIO_CH3_PIN,true);			//	图传配置通道3，高电平有效
	vbtCtr		= new IoOut(VBT_CTR_PORT,VBT_CTR_PIN,true);				//	蓝牙/图传电源开关
	
	BtSet		= new IoOut(BT_SET_PORT,BT_SET_PIN,true);				//	usb控制引脚，默认不用此功能
	
	*/
}


/**
  * @brief  初始化AD
  * @param  None
  * @retval None
  */
void Process::initADs()
{
	ADC1_DMA_Init();
}

/**
  * @brief  初始化定时器
  * @param  None
  * @retval None
  */
void Process::initTimers()
{
	t2=new Timer (TIM2);
	t2->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM2_SubPriority);
	t2->setTimeOut(TIM2_DELAY_TIME);		// 修改宏定义以改变延时

	t3=new Timer (TIM3);
	t3->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM3_SubPriority);
	t3->setTimeOut(TIM3_DELAY_TIME);		// 修改宏定义以改变延时
	
	t4=new Timer (TIM4);
	t4->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM4_SubPriority);
	t4->setTimeOut(TIM4_DELAY_TIME);		// 修改宏定义以改变延时
}

/**
  * @brief  初始化usart
  * @param  None
  * @retval None
  */
void Process::initUsarts()
{
	//com to pad
	comToPad = new SerialPort(3,9600);	
	//com to Robot
	comToRobot = new SerialPort(2,9600);	
	protocol = new Protocol(comToPad,comToRobot);
}

/**
  * @brief  初始化全部数据
  * @param  None
  * @retval None
  */	
void Process::initAllData(void)
{
	for(uint8_t i=0; i<COMN1_SEND_LENGTH;i++)
	{
		com1st_DataToSend[i]=0;
	}	
	
	for(uint8_t i=0; i<COMN2_SEND_LENGTH;i++)
	{
		com2nd_DataToSend[i]=0;
	}
	
	//	本体默认数据
	botLeftBatVal=0;	//	左电池电量0
	botRightBatVal=0;	//	右电池电量0
	botAngle=0;			//	本体角度0
	botIR=0;			//	本体红外无信号
	botMode=0;			//	本体手动模式
	
	//	pad默认数据
	padOffState=false;	//	pad工作中
	padChannel=0x00;	//	pad通道0
	
	//	控制器默认数据
	ctrBatVal=0;		//	电池电量0	
	ctrLeftX=128;		//	左摇杆X中心位置
	ctrLeftY=128;		//	左摇杆Y中心位置
	ctrRightX=128;		//	右摇杆X中心位置
	ctrRightY=128;		//	右摇杆Y中心位置
	
	//	控制器默认状态
	ctrData1=0x00;
	ctrStatus1=0x10;
	ctrStatus2=0x50;
	ctrBotData1=0x12;
	ctrBotData2=0x00;	
	ctrFlag_PowerOn=0x00;	
	ctrFlag_PowerLedOff=0x00;
	ctrFlag_Auto=false;
	ctrFlag_InterruptAuto=false;
	ctrFlag_WorkAuto=false;
	ctrFlag_PadInsert=false;
	
	//	设置通道
	padChannel=0x01;
	ctrChannel=0x01;
	botChannel=0x01;
	
	setComToPadData();													//	设置需要发送的数据
	setComToRobotData();													//	设置需要发送的数据
	protocol->com1st_UpdateSendBuf(com1st_DataToSend);				//	更新需要发送的数据
	protocol->com2nd_UpdateSendBuf(com2nd_DataToSend);				//	更新需要发送的数据
}

/**
  * @brief  设置电源指示灯
  * @param  None
  * @retval None
  * modified by :wu xin you 1103
  */
void Process::setPowerLed()
{
	static uint8_t showCount=0;
	uint8_t  tempValue=ctrBatVal;
	
	if(ctrFlag_PowerLedOff==0x00)
	{
		showCount++;
		if(showCount>=50)												//	每隔50次更新一次显示，解决闪灯问题
		{
			showCount=0; 
			//电量小于20
			if(tempValue < 111)
			{
				powerLed1->setOff();
				powerLed2->setOff();
				powerLed3->setOff();
				powerLed4->setOff();

			}
			//电量小于40
			else if(tempValue < 113)
			{	
				powerLed1->setOn();
				powerLed2->setOff();
				powerLed3->setOff();
				powerLed4->setOff();
			}
			//电量 <60
			else if(tempValue <115)
			{
				powerLed1->setOn();
				powerLed2->setOn();
				powerLed3->setOff();
				powerLed4->setOff();
			}
			//电量 <80
			else if(tempValue <= 120)
			{
				powerLed1->setOn();
				powerLed2->setOn();
				powerLed3->setOn();
				powerLed4->setOff();
			}
			//电量 >80
			else 	//	tempValue>120/2
			{
				powerLed1->setOn();
				powerLed2->setOn();
				powerLed3->setOn();
				powerLed4->setOn();
			}			
		}
	}
	else
	{
				powerLed1->setOff();
				powerLed2->setOff();
				powerLed3->setOff();
				powerLed4->setOff();
	}
}

/**
  * @brief  计算控制器电池电压,1unit=0.1V
  * @param  None
  * @retval None
  * Modified by wu xin you 1103
  */
#define	R_UP	10	//	分压电阻10k
#define	R_DOWN	1	//	分压电阻1k
void Process::getBatValue()
{	
	uint16_t tempValue=0;
	
	tempValue = AD_Filter[NUM_POWER];
	
	tempValue = tempValue*33*(R_DOWN+R_UP)/(R_DOWN*4096);
	ctrBatVal = (uint8_t)(tempValue);
}


/**
  * @brief  设置电源状态
  * @param  
			*true	打开控制器电源
			*false	关闭控制器电源
  * @retval None	
  */
void Process::setPowerState(bool state)
{
	if(state)
		systemPowCtrl->setOn();
	else
		systemPowCtrl->setOff();
}

/**
  * @brief  设置图传通道
  * @param  
			*ch		图传通道
  * @retval None	
	* @Modified wu 废弃不用
  */
/*
void Process::setVedioCh(uint8_t ch)
{
	switch (ch)
	{
		case 0:
			vedioCh1->setOff();
			vedioCh2->setOff();
			vedioCh3->setOff();
			break;
		case 1:
			vedioCh1->setOn();
			vedioCh2->setOff();
			vedioCh3->setOff();
			break;
		case 2:
			vedioCh1->setOff();
			vedioCh2->setOn();
			vedioCh3->setOff();
			break;
		case 3:
			vedioCh1->setOn();
			vedioCh2->setOn();
			vedioCh3->setOff();
			break;
		case 4:
			vedioCh1->setOff();
			vedioCh2->setOff();
			vedioCh3->setOn();
			break;
		case 5:
			vedioCh1->setOn();
			vedioCh2->setOff();
			vedioCh3->setOn();
			break;
		case 6:
			vedioCh1->setOff();
			vedioCh2->setOn();
			vedioCh3->setOn();
			break;
		case 7:
			vedioCh1->setOn();
			vedioCh2->setOn();
			vedioCh3->setOn();
			break;
		default :
			vedioCh1->setOff();
			vedioCh2->setOff();
			vedioCh3->setOff();
			break;
	}
}
*/

/**
  * @brief  设置数传通道,通道号从1开始有效
  * @param  
			*ch		数传通道
  * @retval 废弃不用
  */
	
	/*
void Process::setDataCh(uint8_t ch)
{	
	uint8_t DTSet[17];						//	配置数据长度17
	char baudRate=0x03;						//	发射速率&串口波特率
	
	if(ch<1)
		ch=1;
	
	DTSet[0]=0xAA;							//	不能更改
	DTSet[1]=0xFA;							//	不能更改	
	DTSet[2]=0x03;							//	不能更改
	DTSet[3]=ch;							//	信道,信道从1开始，0无效
	DTSet[4]=0x14;							//	工作频段，不能更改，01表示433MHz
	DTSet[5]=baudRate;						//	无线传输速率，03表示9600
	DTSet[6]=0x07;							//	发射功率，不建议更改
	DTSet[7]=baudRate;						//	串口波特率，03表示9600
	DTSet[8]=0x02;							//	串口数据位
	DTSet[9]=0x01;							//	串口停止位
	DTSet[10]=0x01;							//	串口校验位
	DTSet[11]=0x00;							//	NET ID	
	DTSet[12]=0x00;							//	NET ID	
	DTSet[13]=0x00;							//	NET ID	
	DTSet[14]=0x00;							//	NET ID	
	DTSet[15]=0x00;							//	NODE ID	
	DTSet[16]=0xac;							//	NODE ID	

//	delete	com2nd;
//	com2nd = new SerialPort(3,9600);	
	digSet->setOn();						//	开启数传设置模式
	SysTick_DelayMs(5);						//	此处延时最小3ms，小于3ms设置失败
	protocol->com2nd->write(DTSet,17);		//	发送数据
	SysTick_DelayMs(5);						//	此处延时最小3ms，小于3ms设置失败
	digSet->setOff();						//	关闭数传设置模式
	
//	delete	com2nd;
//	com2nd = new SerialPort(3,19200);
}
*/

/**
  * @brief  设置数传/图传通道
  * @param  
			*ch		数传/图传通道
  * @retval None	
  */
	/*
void Process::setChannel(uint8_t ch)
{
	switch(ch)
	{
		case 0:
			setDataCh(1);						//	设置数传信道
			setVedioCh(0);						//	设置图传通道
			break;
		case 1:
			setDataCh(2);						//	设置数传信道
			setVedioCh(1);						//	设置图传通道
			break;
		case 2:
			setDataCh(3);						//	设置数传信道
			setVedioCh(2);						//	设置图传通道
			break;
		case 3:
			setDataCh(4);						//	设置数传信道
			setVedioCh(3);						//	设置图传通道
			break;
		case 4:
			setDataCh(5);						//	设置数传信道
			setVedioCh(4);						//	设置图传通道
			break;
		case 5:
			setDataCh(6);						//	设置数传信道
			setVedioCh(5);						//	设置图传通道
			break;
		case 6:
			setDataCh(7);						//	设置数传信道
			setVedioCh(6);						//	设置图传通道
			break;
		case 7:
			setDataCh(8);						//	设置数传信道
			setVedioCh(7);						//	设置图传通道
			break;
		default :
			break;
	}
}
*/

/**
  * @brief  调试代码，串口输出
  * @param  None
  * @retval None
  */
void Process::testWithUsart(void)
{
	uint8_t aa;
	aa=AD_Filter[1]>>4;													//	调试代码	
	protocol->com2nd->putChar(aa);										//	调试代码	
//	testLed->reverseState();
}

/*--------------------------------- End of Process.cpp -----------------------------*/
