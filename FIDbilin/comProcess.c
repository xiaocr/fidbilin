
#include "config.h"
extern unsigned char ppmRateSel,ppmRateAutoSelFlg,sorftCtrRatFlg; 
extern unsigned char erroStatus,pintfItemSel;  
extern unsigned int conditionStatus;
extern uint8 fidSel,lecChar,ledCode;
extern uint8 autoCalibFlg,autoCalibOrder;
extern uint16 autoCalibTimCnt;
extern uint16 tempThreshhold;

extern unsigned char ppmRate[5];
extern uint8 adVluBuf[10];
extern uint16 measurVoltageRefBuf[5][2];
extern uint32 measurConcentRefBuf[5];

extern uint16 measurMethaneVoltageRefBuf[5][2];
extern uint32 measurMethaneConcentRefBuf[5];

//extern float ppmConcent;
extern float totalHydroCarbonVlv, methaneVlv;

unsigned char ComCmdPocess()
{  
	unsigned char cTemp,erroStatusTemp;
	unsigned char testBuf[12];

	unsigned int iTemp ;
	unsigned long i,j,k; 
	unsigned long lTemp;
	float fTemp;

	if(Uart0.RecvBuf[2] == 'A')
	{
		if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"AKON",4) == 0)  //请求返回FID测量得到的总烃PPM值
		{
			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x30;
			Uart0.RecvBuf[8] = 0x20;
			/*
			AdProcess(&lTemp,4);//ppm
			ppmConcent = 3.3*lTemp/1024;
			memcpy((unsigned char *)&Uart0.RecvBuf[8],(unsigned  char *)&ppmConcent,4); 
			*/	
			//memcpy((unsigned char *)&Uart0.RecvBuf[8],(unsigned  char *)&ppmConcent,4);
			//sprintf((unsigned char *)&Uart0.RecvBuf[9],"%.1f",ppmConcent);

			sprintf(adVluBuf,"%.2f",totalHydroCarbonVlv);
			iTemp = strlen(adVluBuf);
			for(i = 0; i < iTemp;i++)
			{
				Uart0.RecvBuf[9+i] = adVluBuf[i];
			}

			Uart0.RecvBuf[iTemp+9] = 0x03;
			for(i = 0; i < (iTemp+10); i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}	
				
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"AKO2",4) == 0)    //甲烷
		{
			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x30;
			Uart0.RecvBuf[8] = 0x20;

			sprintf(adVluBuf,"%.2f",methaneVlv);
			iTemp = strlen(adVluBuf);
			for(i = 0; i < iTemp;i++)
			{
				Uart0.RecvBuf[9+i] = adVluBuf[i];
			}

			Uart0.RecvBuf[iTemp+9] = 0x03;
			for(i = 0; i < (iTemp+10); i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"AKO3",4) == 0)    //非甲烷总烃
		{
			fTemp = totalHydroCarbonVlv-methaneVlv;

			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x30;
			Uart0.RecvBuf[8] = 0x20;

			sprintf(adVluBuf,"%.2f",fTemp);
			iTemp = strlen(adVluBuf);
			for(i = 0; i < iTemp;i++)
			{
				Uart0.RecvBuf[9+i] = adVluBuf[i];
			}

			Uart0.RecvBuf[iTemp+9] = 0x03;
			for(i = 0; i < (iTemp+10); i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"AEMB",4) == 0)  //请求返回FID现在选择的量程
		{
			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x30;
			Uart0.RecvBuf[8] = 0x20;
			Uart0.RecvBuf[9] = 'M';
			//if(ppmRateAutoSelFlg == 0)
			/*
			if(ppmRateSel == 0x31)
				Uart0.RecvBuf[10] = 0x32;
			else if(ppmRateSel == 0x32)
				Uart0.RecvBuf[10] = 0x31;
			else
				Uart0.RecvBuf[10] = ppmRateSel;
			*/
			Uart0.RecvBuf[10] = ppmRateSel;

			//else
				//Uart0.RecvBuf[10] = 0x35;
			Uart0.RecvBuf[11] = 0x03;
			for(i = 0; i < 12; i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"AMBE",4) == 0)  //请求返回FID中设置的各量程的量程值
		{
			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x31;

			iTemp = 8;
			Uart0.RecvBuf[iTemp] = 0x20;
			Uart0.RecvBuf[iTemp+1] = 'M';
			Uart0.RecvBuf[iTemp+2] = 0x31;
			Uart0.RecvBuf[iTemp+3] = 0x20;
			Uart0.RecvBuf[iTemp+4] = 0x31;
			Uart0.RecvBuf[iTemp+5] = 0x35;
			//sprintf((unsigned char *)&Uart0.RecvBuf[iTemp+4],"%d", (unsigned char *)&ppmRate[0]);
			//iTemp += strlen((unsigned char *)&Uart0.RecvBuf[iTemp+4]);
			iTemp += 6;
			Uart0.RecvBuf[iTemp] = 0x2E;
			iTemp += 1;

			Uart0.RecvBuf[iTemp] = 0x20;
			Uart0.RecvBuf[iTemp+1] = 'M';
			Uart0.RecvBuf[iTemp+2] = 0x32;
			Uart0.RecvBuf[iTemp+3] = 0x20;
			Uart0.RecvBuf[iTemp+4] = 0x31;
			Uart0.RecvBuf[iTemp+5] = 0x30;
			Uart0.RecvBuf[iTemp+6] = 0x30;
			//sprintf((unsigned char *)Uart0.RecvBuf[iTemp+4],"%d", ppmRate[1]);
			//iTemp += strlen((unsigned char *)&Uart0.RecvBuf[iTemp]);
			iTemp += 7;
			Uart0.RecvBuf[iTemp] = 0x2E;
			iTemp += 1;

			Uart0.RecvBuf[iTemp] = 0x20;
			Uart0.RecvBuf[iTemp+1] = 'M';
			Uart0.RecvBuf[iTemp+2] = 0x33;
			Uart0.RecvBuf[iTemp+3] = 0x20;
			Uart0.RecvBuf[iTemp+4] = 0x31;
			Uart0.RecvBuf[iTemp+5] = 0x30;
			Uart0.RecvBuf[iTemp+6] = 0x30;
			Uart0.RecvBuf[iTemp+7] = 0x30;
			//sprintf((unsigned char *)Uart0.RecvBuf[iTemp+4],"%d", ppmRate[2]);
			//iTemp += strlen((unsigned char *)&Uart0.RecvBuf[iTemp]);
			iTemp += 8;
			Uart0.RecvBuf[iTemp] = 0x2E;
			iTemp += 1;

			Uart0.RecvBuf[iTemp] = 0x20;
			Uart0.RecvBuf[iTemp+1] = 'M';
			Uart0.RecvBuf[iTemp+2] = 0x34;
			Uart0.RecvBuf[iTemp+3] = 0x20;
			Uart0.RecvBuf[iTemp+4] = 0x31;
			Uart0.RecvBuf[iTemp+5] = 0x30;
			Uart0.RecvBuf[iTemp+6] = 0x30;
			Uart0.RecvBuf[iTemp+7] = 0x30;
			Uart0.RecvBuf[iTemp+8] = 0x30;
			//sprintf((unsigned char *)Uart0.RecvBuf[iTemp+4],"%d", ppmRate[3]);
			//iTemp += strlen((unsigned char *)&Uart0.RecvBuf[iTemp]);
			iTemp += 9;
			Uart0.RecvBuf[iTemp] = 0x2E;
			iTemp += 1;

			Uart0.RecvBuf[iTemp] = 0x20;
			Uart0.RecvBuf[iTemp+1] = 'M';
			Uart0.RecvBuf[iTemp+2] = 0x35;
			Uart0.RecvBuf[iTemp+3] = 0x20;
			Uart0.RecvBuf[iTemp+4] = 0x31;
			Uart0.RecvBuf[iTemp+5] = 0x30;
			Uart0.RecvBuf[iTemp+6] = 0x30;
			Uart0.RecvBuf[iTemp+7] = 0x30;
			Uart0.RecvBuf[iTemp+8] = 0x30;
			Uart0.RecvBuf[iTemp+9] = 0x30;
			//sprintf((unsigned char *)Uart0.RecvBuf[iTemp+4],"%d", ppmRate[4]);
			//iTemp += strlen((unsigned char *)&Uart0.RecvBuf[iTemp]);
			iTemp += 10;
			Uart0.RecvBuf[iTemp] = 0x2E;
			iTemp += 1;

			Uart0.RecvBuf[iTemp] = 0x03;
			for(i = 0; i < (iTemp+1); i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ASTF",4) == 0)  //请求返回FID现错误状态代码
		{
			Uart0.RecvBuf[1] = 0x00;
			if(erroStatus == 0)//报警代码
			{
	  		    Uart0.RecvBuf[7] = 0x30;
				Uart0.RecvBuf[8] = 0x20;
				j = 9;
			}
			else 
			{
				erroStatusTemp = erroStatus;
				Uart0.RecvBuf[7] = 0x31;
				Uart0.RecvBuf[8] = 0x20;
				i = 1;
				j = 9;
				for(k = 0; k < 8; k++,i++)
				{
					if((erroStatusTemp&0x01) == 0x01)
					{
						Uart0.RecvBuf[j] = 0x30+i;
						Uart0.RecvBuf[j+1] = 0x20;
						j += 2;
					}
					erroStatusTemp = erroStatusTemp>>1;
				}
			}

			Uart0.RecvBuf[j] = 0x03;
			for(i = 0; i < (j+1); i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ASTZ",4) == 0)  //请求返回FID现仪器状态代码
		{
			Uart0.RecvBuf[1] = 0x00;
			if(conditionStatus != 0)
			{
				Uart0.RecvBuf[7] = 0x31;
				Uart0.RecvBuf[8] = 0x20;
				if((conditionStatus&0x0001) == 0x0001)
				{
					Uart0.RecvBuf[9] = 'S';
					Uart0.RecvBuf[10] = 'N';	
					Uart0.RecvBuf[11] = 'G';
					Uart0.RecvBuf[12] = 'A';
				}
				else if((conditionStatus&0x0002) == 0x0002)
				{
					Uart0.RecvBuf[9] = 'S';
					Uart0.RecvBuf[10] = 'E';	
					Uart0.RecvBuf[11] = 'G';
					Uart0.RecvBuf[12] = 'A';
				}
				else if((conditionStatus&0x0004) == 0x0004)
				{
					Uart0.RecvBuf[9] = 'S';
					Uart0.RecvBuf[10] = 'M';	
					Uart0.RecvBuf[11] = 'G';
					Uart0.RecvBuf[12] = 'A';
				}
				
				else if((conditionStatus&0x0080) == 0x0080)
				{
					Uart0.RecvBuf[9] = 'S';
					Uart0.RecvBuf[10] = 'T';	
					Uart0.RecvBuf[11] = 'B';
					Uart0.RecvBuf[12] = 'Y';
				}
				iTemp = 13;
				if((conditionStatus&0x0100) == 0x0100)
				{
					Uart0.RecvBuf[iTemp] = 0x20;
					Uart0.RecvBuf[iTemp+1] = 'S';
					Uart0.RecvBuf[iTemp+2] = 'A';	
					Uart0.RecvBuf[iTemp+3] = 'R';
					Uart0.RecvBuf[iTemp+4] = 'E';	
				}
				else
				{
					Uart0.RecvBuf[iTemp] = 0x20;
					Uart0.RecvBuf[iTemp+1] = 'S';
					Uart0.RecvBuf[iTemp+2] = 'A';	
					Uart0.RecvBuf[iTemp+3] = 'R';
					Uart0.RecvBuf[iTemp+4] = 'A';	
				}
				iTemp = 18;
				if((conditionStatus&0x0010) == 0x0000)
				{
					Uart0.RecvBuf[iTemp] = 0x20;
					Uart0.RecvBuf[iTemp+1] = 'S';
					Uart0.RecvBuf[iTemp+2] = 'K';	
					Uart0.RecvBuf[iTemp+3] = 'A';
					Uart0.RecvBuf[iTemp+4] = '1';
				}
				else if((conditionStatus&0x0010) == 0x0010)
				{
					Uart0.RecvBuf[iTemp] = 0x20;
					Uart0.RecvBuf[iTemp+1] = 'S';
					Uart0.RecvBuf[iTemp+2] = 'K';	
					Uart0.RecvBuf[iTemp+3] = 'A';
					Uart0.RecvBuf[iTemp+4] = '2';
				}
				iTemp = 23;

				Uart0.RecvBuf[iTemp] = 0x03;
				for(i = 0; i < (iTemp+1); i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}
			}
			else
			{
			    Uart0.RecvBuf[7] = 0x30;
				Uart0.RecvBuf[8] = 0x20;
				Uart0.RecvBuf[9] = 0x03;
				for(i = 0; i < 10; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}
			}

		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ASYZ",4) == 0)  //请求返回FID 设置的时间
		{
			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x31;
			Uart0.RecvBuf[8] = 0x20;

			Uart0.RecvBuf[9] = (RTC_YEAR%100)/10+0x30;
			Uart0.RecvBuf[10] = (RTC_YEAR%100)%10+0x30;
			Uart0.RecvBuf[11] = RTC_MONTH/10+0x30;
			Uart0.RecvBuf[12] = RTC_MONTH%10+0x30;
			Uart0.RecvBuf[13] = RTC_DOM/10+0x30;
			Uart0.RecvBuf[14] = RTC_DOM%10+0x30;

			Uart0.RecvBuf[15] = 0x20;

			Uart0.RecvBuf[16] = RTC_HOUR/10+0x30;
			Uart0.RecvBuf[17] = RTC_HOUR%10+0x30;
			Uart0.RecvBuf[18] = RTC_MIN/10+0x30;
			Uart0.RecvBuf[19] = RTC_MIN%10+0x30;
			Uart0.RecvBuf[20] = RTC_SEC/10+0x30;
			Uart0.RecvBuf[21] = RTC_SEC%10+0x30;

			Uart0.RecvBuf[22] = 0x03;
			for(i = 0; i < 23; i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}	
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ASOL",4) == 0)  //请求返回FID中设置的N通道的温度及低高限
		{
		
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ATEM",4) == 0)  //请求返回FID中现在N通道的温度
		{
			if(Uart0.RecvBuf[10] == 0x31) //检测室温度
			{
				AdProcess(&lTemp,2);
				fTemp = 4700/((4.7/4.8)-(lTemp*3.3/1024)/113.3)-4700;
				fTemp -= 100;
				fTemp /= 0.39;

				sprintf(&Uart0.RecvBuf[9],"%.1f",fTemp);

				Uart0.RecvBuf[1] = 0x00;
			    Uart0.RecvBuf[7] = 0x31;
				Uart0.RecvBuf[8] = 0x20;
				for(i = 0; i < 13; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}

			}
			else if(Uart0.RecvBuf[10] == 0x32)	//催化室温度
			{
				AdProcess(&lTemp,0);
				fTemp = 4700/((4.7/4.8)-(lTemp*3.3/1024)/113.3)-4700;
				fTemp -= 100;
				fTemp /= 0.39;

				sprintf(&Uart0.RecvBuf[9],"%.1f",fTemp);

				Uart0.RecvBuf[1] = 0x00;
			    Uart0.RecvBuf[7] = Uart0.RecvBuf[10];
				Uart0.RecvBuf[8] = 0x20;
				for(i = 0; i < 13; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}

			}
			else
			{
				for(i = 0; i < 12; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}	
			}

		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ADRU",4) == 0)  //请求返回FID各压力值
		{
			if(Uart0.RecvBuf[10] == 0x31)//旁路
			{
				P119Lo();	
  				//P120Lo();
				P122Lo();

				AdProcess(&lTemp,5);//压力
				fTemp = 2*(3.3*lTemp/1024)/10;
	
				sprintf(&Uart0.RecvBuf[9],"%.2f",fTemp);
				Uart0.RecvBuf[1] = 0x00;
			    Uart0.RecvBuf[7] = Uart0.RecvBuf[10];
				Uart0.RecvBuf[8] = 0x20;
				for(i = 0; i < 13; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}
			}
			else if(Uart0.RecvBuf[10] == 0x32)	//出口
			{
				P119Hi();	
  				//P120Lo();
			    P122Lo();

			    AdProcess(&lTemp,5);//压力
				fTemp = 2*(3.3*lTemp/1024)/10;
	
				sprintf(&Uart0.RecvBuf[9],"%.2f",fTemp);
				Uart0.RecvBuf[1] = 0x00;
			    Uart0.RecvBuf[7] = Uart0.RecvBuf[10];
				Uart0.RecvBuf[8] = 0x20;
				for(i = 0; i < 13; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}
			}
			else if(Uart0.RecvBuf[10] == 0x33)	 //氢气
			{
				P119Lo();	
  				//P120Hi();
				P122Hi();

				AdProcess(&lTemp,5);//压力
				fTemp = 2*(3.3*lTemp/1024)/10;
	
				sprintf(&Uart0.RecvBuf[9],"%.2f",fTemp);
				Uart0.RecvBuf[1] = 0x00;
			    Uart0.RecvBuf[7] = Uart0.RecvBuf[10];
				Uart0.RecvBuf[8] = 0x20;
				for(i = 0; i < 13; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}
			}
		    else
			{
				for(i = 0; i < 12; i++)
				{
					U0THR = Uart0.RecvBuf[i];
					while((U0LSR & 0x20) == 0);
				}	
			}
			

		}
		else 
		{
			
		}
		
	}
	else if(Uart0.RecvBuf[2] == 'S')
	{
		if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"STBY",4) == 0) //设置FID为等待状态 
		{
			conditionStatus &= 0xffe8;
			conditionStatus |= 0x0080;	
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SNGA",4) == 0)  //设置FID为通零气状态
		{
			P23Lo();
			P24Hi();
			P25Hi();
			conditionStatus &= 0x78;//切换电磁阀同时退出standby状态
			conditionStatus |= 0x01;
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SEGA",4) == 0)  //设置FID为通标气状态
		{
			P23Hi();
			P24Lo();
			P25Hi();
			conditionStatus &= 0x78;//切换电磁阀同时退出standby状态
			conditionStatus |= 0x02;
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SATK",4) == 0)  //设置FID进入自动标定状态
		{
			if((autoCalibFlg == 0)&&((erroStatus&0x06) == 0)&&(fidSel == 0)) //点火成功，并在测总烃时进入自动校准
			{
				autoCalibFlg = 1;

				conditionStatus |= 0x08;

				ledCode = 0x7e;
				hct595sendByte(ledCode&lecChar);

				
				autoCalibTimCnt = 0;
				autoCalibOrder = 0;

				P23Lo(); //通零气
				P24Hi();
				P25Hi();
				conditionStatus &= 0x78;//切换电磁阀同时退出standby状态
				conditionStatus |= 0x01;
		
		   	}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SEMB",4) == 0)  //设置FID量程
		{
			if(sorftCtrRatFlg ==1)//软件选择量程开启
			{
				if(Uart0.RecvBuf[11] == 0x31)
				{
					//ppmRateSel = 0x32;
					ppmRateSel = Uart0.RecvBuf[11];
					P18Hi();
					P19Lo();
					P110Lo();
					P114Lo();
					P116Lo();
				}
				else if(Uart0.RecvBuf[11] == 0x32)
				{
					//ppmRateSel = 0x31;
					ppmRateSel = Uart0.RecvBuf[11];
					P18Lo();
					P19Lo();
					P110Hi();
					P114Lo();
					P116Lo();
				}
				else if(Uart0.RecvBuf[11] == 0x33)
				{
					ppmRateSel = Uart0.RecvBuf[11];
					P18Lo();
					P19Lo();
					P110Lo();
					P114Lo();
					P116Hi();
				}
				else if(Uart0.RecvBuf[11] == 0x34)
				{
					ppmRateSel = Uart0.RecvBuf[11];
					P18Lo();
					P19Lo();
					P110Lo();
					P114Hi();
					P116Lo();
				}
				else if(Uart0.RecvBuf[11] == 0x35)
				{
					ppmRateSel = Uart0.RecvBuf[11];
					P18Lo();
					P19Hi();
					P110Lo();
					P114Lo();
					P116Lo();
				}
			}
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SMGA",4) == 0)  //设置FID为采样气状态
		{
			P23Hi();
			P24Hi();
			P25Lo();
			conditionStatus &= 0x78;//切换电磁阀同时退出standby状态
			conditionStatus |= 0x04;
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SETH",4) == 0)  //设置火焰温度检测阀值  
		{
			tempThreshhold = (Uart0.RecvBuf[10]-0x30)*100+(Uart0.RecvBuf[11]-0x30)*10+(Uart0.RecvBuf[12]-0x30);
			I2C_WriteNByte(0xa0, 2, 120,(uint8 *)&tempThreshhold,2);
		}
		 
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SARE",4) == 0) //软件选择量程开启
		{
			//ppmRateAutoSelFlg = 1;
			//conditionStatus |= 0x0100;
			if((conditionStatus&0x0100) == 0x0100)
				sorftCtrRatFlg = 1;
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SARA",4) == 0)  //软件选择量程关闭
		{
			//ppmRateAutoSelFlg = 0;
			//conditionStatus &= ~0x0100;
			sorftCtrRatFlg = 0;
		}
		
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SKA2",4) == 0) //测甲烷 
		{
			fidSel = 1;
			ledCode = 0xb7;
			P27Lo();
			hct595sendByte(ledCode&lecChar);
			conditionStatus |= 0x0010;	
		}
		else if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"SKA1",4) == 0) //测总烃 
		{
			fidSel = 0;
			ledCode = 0x77;
			P27Hi();
			hct595sendByte(ledCode&lecChar);
			conditionStatus &= ~0x0010;	
		}
		else 
		{

		}

		Uart0.RecvBuf[1] = 0x00;
	    Uart0.RecvBuf[7] = 0x30;
		Uart0.RecvBuf[8] = 0x20;
		Uart0.RecvBuf[9] = 0x03;
		for(i = 0; i < 10; i++)
		{
			U0THR = Uart0.RecvBuf[i];
			while((U0LSR & 0x20) == 0);
		}
	}
	else if(Uart0.RecvBuf[2] == 'E')
	{
		if(strncmp((unsigned char *)&Uart0.RecvBuf[2],"ESYZ",4) == 0) //设置仪器时间
		{
			RTC_CCR &= ~0x01;
				
			RTC_YEAR = 2000+(Uart0.RecvBuf[10]-0x30)*10+(Uart0.RecvBuf[11]-0x30);
			RTC_MONTH = (Uart0.RecvBuf[12]-0x30)*10+(Uart0.RecvBuf[13]-0x30);
			//RTC_DOW = 1;
			RTC_DOM = (Uart0.RecvBuf[14]-0x30)*10+(Uart0.RecvBuf[15]-0x30);
			RTC_HOUR = (Uart0.RecvBuf[17]-0x30)*10+(Uart0.RecvBuf[18]-0x30);
			RTC_MIN = (Uart0.RecvBuf[19]-0x30)*10+(Uart0.RecvBuf[20]-0x30);
			RTC_SEC = (Uart0.RecvBuf[21]-0x30)*10+(Uart0.RecvBuf[22]-0x30);  	
		  	
		   	RTC_CCR = 0x10;	//选择外部时钟
		    RTC_CCR |= 0x02;//复位
		    for(i = 0; i < 1000; i++);
		    RTC_CCR &= ~0x02;
		    RTC_CCR |= 0x01;//启动RTC
		    
		    PCONP &= ~0x00000200;//PCRTC = 0;	

			Uart0.RecvBuf[1] = 0x00;
		    Uart0.RecvBuf[7] = 0x30;
			Uart0.RecvBuf[8] = 0x20;
			Uart0.RecvBuf[9] = 0x03;
			for(i = 0; i < 10; i++)
			{
				U0THR = Uart0.RecvBuf[i];
				while((U0LSR & 0x20) == 0);
			}
		}	
	}
	else if(Uart0.RecvBuf[2] == 'T')
	{
		if(Uart0.RecvBuf[8] == 0x30 )
		{
			pintfItemSel = 0;
			printu("\r\n点火温度电压（120）：");	
		}
		else if(Uart0.RecvBuf[8] == 0x31 )
		{
			pintfItemSel = 1;
			printu("\r\n检测室温度电压（512）：");	
		}
		else if(Uart0.RecvBuf[8] == 0x32 )
		{
			pintfItemSel = 2;
			printu("\r\n催化室温度电压（760）：");	
		}
		else if(Uart0.RecvBuf[8] == 0x33 )
		{
			pintfItemSel = 3;
			printu("\r\n测量值电压：");	
		}	
		else if(Uart0.RecvBuf[8] == 0x34 )
		{
			pintfItemSel = 4;
			printu("\r\n测量值：");	
		}
		else if(Uart0.RecvBuf[8] == 0x35 )
		{
			pintfItemSel = 5;
			printu("\r\n仪器自检开始");

			for(i = 0 ; i < 6; i++)
			{
				testBuf[i] = 0;	
			}
			I2C_WriteNByte(0x70, 1, 0x10,testBuf,6);//数码管测试初始化
		    cTemp = 0xff;
			hct595sendByte(cTemp);//指示灯测试初始化

			P18Lo();//量程选择切换测试初始化
			P19Lo();
			P110Lo();
			P114Lo();
			P115Lo();
			P116Lo();

			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);

			printu("\r\n数码管测试");

			for(i = 0 ; i < 6; i++)
			{
				testBuf[i] = 0xff;	
			}
			I2C_WriteNByte(0x70, 1, 0x10,testBuf,6);//数码管测试
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			for(i = 0 ; i < 6; i++)
			{
				testBuf[i] = 0;	
			}
			I2C_WriteNByte(0x70, 1, 0x10,testBuf,6);//数码管测试
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			for(i = 0 ; i < 6; i++)
			{
				testBuf[i] = 0xff;	
			}
			I2C_WriteNByte(0x70, 1, 0x10,testBuf,6);//数码管测试
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			for(i = 0 ; i < 6; i++)
			{
				testBuf[i] = 0;	
			}
			I2C_WriteNByte(0x70, 1, 0x10,testBuf,6);//数码管测试
			
			printu("\r\n指示灯测试");
			cTemp = 0x00;
			hct595sendByte(cTemp);//指示灯测试
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			cTemp = 0xff;
			hct595sendByte(cTemp);//指示灯测试
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			cTemp = 0x00;
			hct595sendByte(cTemp);//指示灯测试
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			/*
			cTemp = 0;
			hct595sendByte(cTemp)//指示灯测试
			*/
			hct595sendByte(ledCode&lecChar);//恢复状态

			printu("\r\n量程切换测试");
			P18Hi();//量程选择切换测试
			P19Hi();
			P110Hi();
			P114Hi();
			P115Hi();
			P116Hi();
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			P18Lo();//量程选择切换测试
			P19Lo();
			P110Lo();
			P114Lo();
			P115Lo();
			P116Lo();
		    for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			P18Hi();//量程选择切换测试
			P19Hi();
			P110Hi();
			P114Hi();
			P115Hi();
			P116Hi();
			for(i = 0;i  < 5000; i++)
			for(j = 0;j  < 10000; j++);
			P18Lo();//量程选择切换测试
			P19Lo();
			P110Lo();
			P114Lo();
			P115Lo();
			P116Lo();
			/*
			printu("\r\n存储测试");
		    U0THR = '\r';
			while((U0LSR & 0x20) == 0);
			U0THR = '\n';
			while((U0LSR & 0x20) == 0);
			for(i = 0; i < 5;i++)
				I2C_WriteNByte(0xa0, 2, 200,(uint8 *)&measurVoltageRefBuf[i][0],4);//存储测试

			for(i = 0; i < 5;i++)
				I2C_ReadNByte(0xa0, 2, 200,(uint8 *)&measurVoltageRefBuf[i][0],4);

			for(i = 0; i < 5; i++)
			{
				for(j = 0; j < 2;j++)
				{
					cTemp = measurVoltageRefBuf[i][j]/1000;
					U0THR = cTemp+48;
					while((U0LSR & 0x20) == 0);
					cTemp = (measurVoltageRefBuf[i][j]%1000)/100;
					U0THR = cTemp+48;
					while((U0LSR & 0x20) == 0);
					cTemp = ((measurVoltageRefBuf[i][j]%1000)%100)/10;
					U0THR = cTemp+48;
					while((U0LSR & 0x20) == 0);
					cTemp = ((measurVoltageRefBuf[i][j]%1000)%100)%10;
					U0THR = cTemp+48;
					while((U0LSR & 0x20) == 0);

					U0THR = '\r';
					while((U0LSR & 0x20) == 0);
					U0THR = '\n';
					while((U0LSR & 0x20) == 0);
				}
					 
			}
			*/
			printu("\r\n按键测试");
			U0THR = '\r';
			while((U0LSR & 0x20) == 0);
			U0THR = '\n';
			while((U0LSR & 0x20) == 0);
			while(1)
			{
				FIO2MASK = 0xffffffff &(~keyInt);//按键测试
				if((FIO2PIN & keyInt) == 0)
				{
					I2C_ReadNByte(0x70, 1, 0x01, testBuf,3);
					cTemp = (testBuf[0]>>4)&0x0f;
					if(cTemp < 0x0a)
						cTemp += 48;
					else
						cTemp += 55;
					U0THR = cTemp;
					while((U0LSR & 0x20) == 0);

					cTemp = testBuf[0]&0x0f;
					if(cTemp < 0x0a)
						cTemp += 48;
					else
						cTemp += 55;
					U0THR = cTemp;
					while((U0LSR & 0x20) == 0);

					U0THR = '\r';
					while((U0LSR & 0x20) == 0);
					U0THR = '\n';
					while((U0LSR & 0x20) == 0);
					if(testBuf[0] == 0x1a)
						break;
				}
			}

			printu("\r\n仪器自检结束");	
		}
		else 
		{
			pintfItemSel = 10;//停止打印测试	
		}
		/*
		else if(Uart0.RecvBuf[8] == 0x39 )
		{
			measurVoltageRefBuf[0][0] =	634;
			measurVoltageRefBuf[0][1] =	2069;
			measurVoltageRefBuf[1][0] =	244;
			measurVoltageRefBuf[1][1] =	2460;
			measurVoltageRefBuf[2][0] =	631;
			measurVoltageRefBuf[2][1] =	2237;
			measurVoltageRefBuf[3][0] =	634;
			measurVoltageRefBuf[3][1] =	2380;  
			measurVoltageRefBuf[4][0] =	634;
			measurVoltageRefBuf[4][1] =	2360;

			for(i = 0; i < 5;i++)
				I2C_WriteNByte(0xa0, 2, i*20,(uint8 *)&measurVoltageRefBuf[i][0],4);

			printu("\r\n已恢复参数出厂设置");
		}
		*/
	}	
    //else if(Uart0.RecvBuf[2] == 'P') //PARA  K 1 XX MM 2 XX MM 3 XX MM 4 XX MM 5 XX MM  03  //	02 20 50 41 52 41 20 4B 31 XX MM 32 XX MM 33 XX MM 34 XX MM 35 XX MM 03  //(XX-浓度,MM-电压)
	else if(Uart0.RecvBuf[2] == 'P') //PARA  K H NN XXXX MM  03 //02 20 50 41 52 41 20 4B H NNNN XXXXX MMMM 03 //(H-量程1-5；NNNN-零点电压；XXXXX-标气浓度；MMMM-标气电压)
	{
		measurVoltageRefBuf[Uart0.RecvBuf[8]-0x31][0] = (Uart0.RecvBuf[9]-0x30)*1000+(Uart0.RecvBuf[10]-0x30)*100+(Uart0.RecvBuf[11]-0x30)*10+(Uart0.RecvBuf[12]-0x30);	 
		I2C_WriteNByte(0xa0, 2, (Uart0.RecvBuf[8]-0x31)*20,(uint8 *)&measurVoltageRefBuf[Uart0.RecvBuf[8]-0x31][0],2);

		measurVoltageRefBuf[Uart0.RecvBuf[8]-0x31][1] = (Uart0.RecvBuf[18]-0x30)*1000+(Uart0.RecvBuf[19]-0x30)*100+(Uart0.RecvBuf[20]-0x30)*10+(Uart0.RecvBuf[21]-0x30);
		I2C_WriteNByte(0xa0, 2, ((Uart0.RecvBuf[8]-0x31)*20)+2,(uint8 *)&measurVoltageRefBuf[Uart0.RecvBuf[8]-0x31][1],2);

		measurConcentRefBuf[Uart0.RecvBuf[8]-0x31] = (Uart0.RecvBuf[13]-0x30)*10000+(Uart0.RecvBuf[14]-0x30)*1000+(Uart0.RecvBuf[15]-0x30)*100+(Uart0.RecvBuf[16]-0x30)*10+(Uart0.RecvBuf[17]-0x30);

		I2C_WriteNByte(0xa0, 2, ((Uart0.RecvBuf[8]-0x31)*20)+4,(uint8 *)&measurConcentRefBuf[Uart0.RecvBuf[8]-0x31],4);
		
		printu("\r\n校准参数设置成功");	
	}
	else if(Uart0.RecvBuf[2] == 'M') //MPRA  K H NN XXXX MM  03 //02 20 4D 41 52 41 20 4B H NNNN XXXXX MMMM 03 //(H-量程1-5；NNNN-零点电压；XXXXX-标气浓度；MMMM-标气电压)
	{
		measurMethaneVoltageRefBuf[Uart0.RecvBuf[8]-0x31][0] = (Uart0.RecvBuf[9]-0x30)*1000+(Uart0.RecvBuf[10]-0x30)*100+(Uart0.RecvBuf[11]-0x30)*10+(Uart0.RecvBuf[12]-0x30);	 
		I2C_WriteNByte(0xa0, 2, (Uart0.RecvBuf[8]-0x31)*20+130,(uint8 *)&measurMethaneVoltageRefBuf[Uart0.RecvBuf[8]-0x31][0],2);

		measurMethaneVoltageRefBuf[Uart0.RecvBuf[8]-0x31][1] = (Uart0.RecvBuf[18]-0x30)*1000+(Uart0.RecvBuf[19]-0x30)*100+(Uart0.RecvBuf[20]-0x30)*10+(Uart0.RecvBuf[21]-0x30);
		I2C_WriteNByte(0xa0, 2, ((Uart0.RecvBuf[8]-0x31)*20+130)+2,(uint8 *)&measurMethaneVoltageRefBuf[Uart0.RecvBuf[8]-0x31][1],2);

		measurMethaneConcentRefBuf[Uart0.RecvBuf[8]-0x31] = (Uart0.RecvBuf[13]-0x30)*10000+(Uart0.RecvBuf[14]-0x30)*1000+(Uart0.RecvBuf[15]-0x30)*100+(Uart0.RecvBuf[16]-0x30)*10+(Uart0.RecvBuf[17]-0x30);

		I2C_WriteNByte(0xa0, 2, ((Uart0.RecvBuf[8]-0x31)*20+130)+4,(uint8 *)&measurMethaneConcentRefBuf[Uart0.RecvBuf[8]-0x31],4);
		
		printu("\r\n校准参数设置成功");	
	}
  return 0;
}