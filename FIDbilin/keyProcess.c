
#include "config.h"

extern 	uint8 fidSel,tempSel,vaccumSel,autoCalibSel,autoCalibFlg,autoCalibOrder,lecChar,ledCode;
extern uint8 keyVlu[4];
extern uint8 digiterBuf[13];
//extern uint16 gasPreesure;
extern uint8 conditionStatus;
extern unsigned char erroStatus;
extern volatile uint8 lightAquirFlg ,lightTim20sDlayFlg,lightLosFlg,lightOnFlg;
extern volatile uint16 lightLosCnt,lightTim2sDlay,lightTim20sDlay;
extern uint16 autoCalibTimCnt;
extern float TempCatalyticNow,TempDetctNow;

extern unsigned char 	nonMethaneTotalHydroCarbonFlg;
extern uint32 nonMethaneTotalHydroCarbonTimCnt;
extern float  methaneVlv;

extern void hct595sendByte(uint8 date);

//unsigned char testAutoCnt = 0;

unsigned char KeyFunProcess(uint8 presskeyVlu)  
{  
	unsigned char cTemp;
	unsigned char cTemp1[11],ladVluBuf[10];
	unsigned int iTemp ;
	unsigned long i,j;
	unsigned long lTemp;
	float fTemp;

	switch(presskeyVlu)
	{
		case 0x01: //����
			P23Lo();
			P24Hi();
			P25Hi();

			conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
			conditionStatus |= 0x01;

			i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);
		
		break;
		case 0x02: //�л��¶�

			if(tempSel == 0)
			{
				ledCode = 0x7b;
			}
			else
			{
				ledCode = 0xbb;
			}
		
			hct595sendByte(ledCode&lecChar);
		
			//do{
				for(i = 0;i < 2000; i++)
				for(j = 0;j < 3000; j++);
				cTemp1[0] = 0xfc;
				cTemp1[1] = 0xfc;
				cTemp1[2] = 0xfc;
				cTemp1[3] = 0xfc;
				cTemp1[4] = 0xfc;
	
				I2C_WriteNByte(0x70, 1, 0x11,cTemp1,4);//����ʾ
				for(i = 0;i < 6; i++)
				{
					cTemp1[i] = 0x00;
				}

				if(tempSel == 0)
				{
					AdProcess(&lTemp,2);
					fTemp = 4700/((4.7/4.8)-(lTemp*3.3/1024)/113.3)-4700;
					fTemp -= 100;
					fTemp /= 0.39;

					//fTemp += 10;
					//sprintf((unsigned char *)&cTemp1[1],"%d",TempDetctNow);
				}
				else
				{
					AdProcess(&lTemp,0);
					fTemp = 4700/((4.7/4.8)-(lTemp*3.3/1024)/113.3)-4700;
					fTemp -= 100;
					fTemp /= 0.39;

					//fTemp += 10;
				    //sprintf((unsigned char *)&cTemp1[1],"%d",TempCatalyticNow);
				}
				memset(ladVluBuf,'\0',10);
				sprintf(ladVluBuf,"%.1f",fTemp);
				i = 1;
				j = 0;
				do{
					if(ladVluBuf[j] >= 0x30)
					{
						cTemp =  ladVluBuf[j]- 0x30;
						cTemp1[i] = digiterBuf[cTemp]; 
					}
					else if	(ladVluBuf[j] == '.')
					{
						i--;
						cTemp1[i] |= 0x01;
					}
					
					i++;
					j++;
				}while(	ladVluBuf[j] != '\0');
				I2C_WriteNByte(0x70, 1, 0x10,cTemp1,5);//��ʾ

				//I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			//}while(keyVlu[0] == presskeyVlu);
		
			tempSel = ~tempSel;
			
			if(fidSel == 0)
				ledCode = 0x77;
			else
				ledCode = 0xb7;
			//ledCode = 0x77;
			hct595sendByte(ledCode&lecChar);
			//fidSel = 0;
			
	
		break;
		case 0x09: //����
			P23Hi();
			P24Lo();
			P25Hi();

			conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
			conditionStatus |= 0x02;
			i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);
	
		break;
		case 0x1a: //������Ǽ��������л� 
		   	/*
			if(fidSel == 0)
			{
				fidSel = 1;	//�����
				ledCode = 0xb7;
				//P27Lo();
				nonMethaneTotalHydroCarbonTimCnt = 0;
				nonMethaneTotalHydroCarbonFlg = 0;
			}
			else 
			{
				fidSel = 0;//������
				ledCode = 0x77;
				P27Hi();
				conditionStatus &= ~0x0010;
				nonMethaneTotalHydroCarbonFlg = 0;
				methaneVlv = 0.0;

			}
			*/
			if(fidSel == 0)
			{
				fidSel = 1;	//�����
				ledCode = 0xb7;
				P27Lo();
				conditionStatus |= 0x0010;
				nonMethaneTotalHydroCarbonFlg = 1;
			}
			else 
			{
				fidSel = 0;//������
				ledCode = 0x77;
				P27Hi();
				conditionStatus &= ~0x0010;
				nonMethaneTotalHydroCarbonFlg = 0;
			}

			hct595sendByte(ledCode&lecChar);
			i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);

	
		break;
		case 0x11: //����
			P23Hi();
			P24Hi();
			P25Lo();
			
			conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
			conditionStatus |= 0x04; 

			i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);
	
		break;
		//case 0x12: //�л����
		case 0x0a: //�л����
			
			if(vaccumSel == 0)
			{
				ledCode = 0x7d;
				P119Lo();	//ѹ��ģ��ͨ���л�
  				//P120Lo();
				P122Lo();
			}
			else if(vaccumSel == 1)
			{
				ledCode = 0xbd;
				P119Hi();	//ѹ��ģ��ͨ���л�
  				//P120Lo();
				P122Lo();
			}
			else
			{
				ledCode = 0xdd;
				P119Lo();	//ѹ��ģ��ͨ���л�
  				//P120Hi();
				P122Hi();
			}
		
			hct595sendByte(ledCode&lecChar);

			
	
			if(vaccumSel < 2)
				vaccumSel++;
			else
				vaccumSel = 0;
			



			//do{
				for(i = 0;i < 2000; i++)
				for(j = 0;j < 3000; j++);

				cTemp1[0] = 0xfc;
				cTemp1[1] = 0xfc;
				cTemp1[2] = 0xfc;
				cTemp1[3] = 0xfc;
				cTemp1[4] = 0xfc;
	
				I2C_WriteNByte(0x70, 1, 0x11,cTemp1,4);//����ʾ
				for(i = 0;i < 6; i++)
				{
					cTemp1[i] = 0x00;
				}

				AdProcess(&lTemp,5);//ѹ��
				fTemp = 2*(3.3*lTemp/1024)/10;
	
				memset(ladVluBuf,'\0',10);
				sprintf(ladVluBuf,"%.2f",fTemp);
				i = 1;
				j = 0;
				do{
					if(ladVluBuf[j] >= 0x30)
					{
						cTemp =  ladVluBuf[j]- 0x30;
						cTemp1[i] = digiterBuf[cTemp]; 
					}
					else if	(ladVluBuf[j] == '.')
					{
						i--;
						cTemp1[i] |= 0x01;
					}
					
					i++;
					j++;
				}while(	ladVluBuf[j] != '\0');
				
				I2C_WriteNByte(0x70, 1, 0x10,cTemp1,5);//��ʾ

				//I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			//}while(keyVlu[0] == presskeyVlu);
		
			if(fidSel == 0)
				ledCode = 0x77;
			else
				ledCode = 0xb7;
			//ledCode = 0x77;
			hct595sendByte(ledCode&lecChar);
			//fidSel = 0;
			
	
		break;
		case 0x19: //���
			P23Hi();
			P24Hi();
			//P28Lo();//��������

			//if((lightOnFlg == 0)&&(lightLosFlg == 0))//δ���
			if((lightOnFlg == 0)&&((erroStatus&0x04) == 0x00))//û�е���������
			{
				if(lightAquirFlg == 0)
				{
					alarmLedOn();
					//P121Hi();//�������
					P123Hi();//�������

					P28Lo();//��������
					//P118Lo();//������

					/*
					P28Lo();//��������

					P118Lo();//������
					//for(j = 0;j < 1000; j++)
					//for(i = 0;i < 1000; i++);
					//P121Hi();//�������
					P123Hi();//�������
					*/
					
					lightTim2sDlay = 0;
					lightLosCnt = 0;
					lightTim20sDlay = 0;
					lightTim20sDlayFlg = 0;	
					lightAquirFlg = 1;
				}
			}

		    //alarmLedOn();

		   	i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);

			alarmLedOff();
			

			break;
		/*
		case 0x12:
			if(testAutoCnt == 0)
			{
				P18Hi();//���̿���
				P19Lo();
				P110Lo();
				P114Lo();
				P116Lo();
			}
			else if(testAutoCnt == 1)
			{
				P18Lo();
				P19Lo();
				P110Hi();
				P114Lo();
				P116Lo();
			}
			else if(testAutoCnt == 2)
			{
				P18Lo();
				P19Lo();
				P110Lo();
				P114Lo();
				P116Hi();
			}
			else if(testAutoCnt == 3)
			{
				P18Lo();
				P19Lo();
				P110Lo();
				P114Hi();
				P116Lo();
			}
			else
			{
				P18Lo();
				P19Hi();
				P110Lo();
				P114Lo();
				P116Lo();	
			}
			if(testAutoCnt == 4)
			{
				testAutoCnt = 0;
			}
			else
				testAutoCnt++;

			i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);
		break;
		*/
		//case 0x0a: //�Զ�У׼
		/*
	    case 0x12: //�Զ�У׼

			if((autoCalibFlg == 0)&&((erroStatus&0x06) == 0)&&(fidSel == 0)) //���ɹ������ڲ�����ʱ�����Զ�У׼
			{
				autoCalibFlg = 1;

				conditionStatus |= 0x08;

				ledCode = 0x7e;
				hct595sendByte(ledCode&lecChar);

				
				autoCalibTimCnt = 0;
				autoCalibOrder = 0;

				P23Lo(); //ͨ����
				P24Hi();
				P25Hi();
				conditionStatus &= 0x78;//�л���ŷ�ͬʱ�˳�standby״̬
				conditionStatus |= 0x01;


				
		   	}
			i = 0;
			do{
				if(i > 1000)
				{
					i = 0;
					break;
				}
				else 
					i++;
				I2C_ReadNByte(0x70, 1, 0x01, keyVlu,3);
			}while(keyVlu[0] == presskeyVlu);

		break;
		*/
		default:
		break;
	}
  return 0;
}