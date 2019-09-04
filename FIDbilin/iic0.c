/*
---------------------------------------------------------------------------------------------------------
*********************************************************************************************************
** �ļ����� ��	I2CINT.c
** ����˵�� ��	LPC2000Ӳ��I2C�жϷ�ʽ��������
** ʹ��˵�� ��  ������Ҫ���ú�I2C���߽ӿ�(I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ)
*********************************************************************************************************
---------------------------------------------------------------------------------------------------------
*/
#include  "config.h" 


extern void StatLoop(void);

/* �������ں�I2C�жϴ�����Ϣ��ȫ�ֱ��� */
volatile uint8 	I2C_sla;			/* I2C�����ӵ�ַ 					*/
volatile uint32	I2C_suba;			/* I2C�����ڲ��ӵ�ַ 				*/
volatile uint8 	I2C_suba_num;		/* I2C�ӵ�ַ�ֽ���					*/
volatile uint8 	*I2C_buf;        	/* ���ݻ�����ָ��  					*/
volatile uint32 I2C_num;			/* Ҫ��ȡ/д������ݸ��� 			*/
volatile uint8 	I2C_end;			/* I2C���߽�����־��������������1 	*/
volatile uint8 	I2C_suba_en;		/* 	�ӵ�ַ���ơ�
										0--�ӵ�ַ�Ѿ��������߲���Ҫ�ӵ�ַ
										1--��ȡ����
										2--д����
									*/
/*
********************************************************************************************************
** �������� ��I2c1Init()
** �������� ��I2C��ʼ��
** ��ڲ��� ��Fi2c	I2C����Ƶ��(���400K)
** ���ڲ��� ����
********************************************************************************************************
*/

void I2c0Init(uint32 Fi2c)
{
	if (Fi2c > 400000)
		Fi2c = 400000;
		
	
	//PINSEL0 = (PINSEL0 & (~0xF0)) | 0x50;
	I2C0SCLH = (Fpclk/Fi2c + 1) / 2;						// �趨I2Cʱ�� 						
	I2C0SCLL = (Fpclk/Fi2c)/2;
	I2C0CONCLR = 0x2C;
	I2C0CONSET = 0x40;									// ʹ����I2C 						
	

	/*
	VICIntSelect = 0x00000000;							// ��������ͨ��ΪIRQ�ж� 			
	VICVectCntl0 = (0x20 | 0x13);						// I2Cͨ�����䵽IRQ slot0��������ȼ� 
	VICVectAddr0 = (int32)I2c1_Handler;					// ����I2C�ж����� 					
	//VICVectAddr0 = (int32)IRQ_I2C;
	VICIntEnable |= (1 << 19);							// ʹ��I2C�ж� 						
	*/
}

/*
**********************************************************************************************************
** �������ƣ�ISendByte()
** �������ܣ������ӵ�ַ��������1�ֽ����ݡ�
** ��ڲ�����sla		������ַ
**           dat		Ҫ���͵�����
** ���ڲ���������ֵΪ0ʱ��ʾ������Ϊ1ʱ��ʾ������ȷ��
** ˵����	ʹ��ǰҪ��ʼ����I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ
*********************************************************************************************************
*/
uint8  ISendByte(uint8 sla, uint8 dat)
{  /* �������� */
   I2C_sla     = sla;		// д������������ַ
   I2C_buf     = &dat;		// �����͵�����
   I2C_num     = 1;			// ����1�ֽ�����
   I2C_suba_en = 0;		 	// ���ӵ�ַ
   I2C_end     = 0;
   
   I2C0CONCLR = 0x2C;
   I2C0CONSET = 0x60;             // ����Ϊ����������������
   
   while(0==I2C_end);
   if(1==I2C_end) return(1);
     else return(0);
}

/*
*********************************************************************************************************
** �������ƣ�IRcvByte()
** �������ܣ������ӵ�ַ������ȡ1�ֽ����ݡ�
** ��ڲ�����sla		������ַ
**           dat		�������ݵı���ָ��
** ���ڲ���������ֵΪ0ʱ��ʾ����������Ϊ1ʱ��ʾ������ȷ��
** ˵����ʹ��ǰҪ��ʼ����I2C���Ź��ܺ�I2C�жϣ�����ʹ��I2C��ģʽ
*********************************************************************************************************
*/
uint8  IRcvByte(uint8 sla, uint8 *dat)
{  /* �������� */
   I2C_sla     = sla+1;		// ��������������ַ
   I2C_buf     = dat;
   I2C_num     = 1;
   I2C_suba_en = 0;			// ���ӵ�ַ
   I2C_end     = 0;
   
   I2C0CONCLR = 0x2C;
   I2C0CONSET = 0x60;         // ����Ϊ����������������
   
   while(0==I2C_end);
   if(1==I2C_end) return(1);
     else return(0);
}


/*
*********************************************************************************************************
** �������� ��I2C_ReadNByte()
** �������� �������ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** ��ڲ��� ��	sla			�����ӵ�ַ
**				suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	2��8+X�ṹ	2��˫�ֽڵ�ַ
**				suba		�����ӵ�ַ
**				s			���ݽ��ջ�����ָ��
**				num			��ȡ�ĸ���
** ���ڲ��� ��	TRUE		�����ɹ�
**				FALSE		����ʧ��
*********************************************************************************************************
*/
uint8 I2C_ReadNByte (uint8 sla, uint32 suba_type, uint32 suba, uint8 *s, uint32 num)
{
	//VICIntEnClr |= 1 << 15;//���δ������ж�
		
	if (num > 0)	/* �ж�num�����ĺϷ��� */
	{	/* �������� */
		if (suba_type == 1)
		{	/* �ӵ�ַΪ���ֽ� */
			I2C_sla     	= sla + 1;							/* �����Ĵӵ�ַ��R=1 	*/
			I2C_suba    	= suba;								/* �����ӵ�ַ 			*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ1�ֽ� 	*/
		}
		if (suba_type == 2)
		{	/* �ӵ�ַΪ2�ֽ� */
			I2C_sla     	= sla + 1;							/* �����Ĵӵ�ַ��R=1 	*/
			I2C_suba   	 	= suba;								/* �����ӵ�ַ 			*/
			I2C_suba_num	= 2;								/* �����ӵ�ַΪ2�ֽ� 	*/
		}
		if (suba_type == 3)
		{	/* �ӵ�ַ�ṹΪ8+X*/
			I2C_sla			= sla + ((suba >> 7 )& 0x0e) + 1;	/* �����Ĵӵ�ַ��R=1	*/
			I2C_suba		= suba & 0x0ff;						/* �����ӵ�ַ	 		*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ8+x	 	*/
		}
		I2C_buf     = s;										/* ���ݽ��ջ�����ָ�� 	*/
		I2C_num     = num;										/* Ҫ��ȡ�ĸ��� 		*/
		I2C_suba_en = 1;										/* ���ӵ�ַ�� 			*/
		I2C_end     = 0;
		
		/* ���STA,SI,AA��־λ */
		I2C0CONCLR = 	(1 << 2)|	/* AA 		*/
					(1 << 3)|	/* SI 		*/
					(1 << 5);	/* STA 		*/
		
		/* ��λSTA,����I2C���� */
		I2C0CONSET = 	(1 << 5)|	/* STA 		*/
					(1 << 6);	/* I2CEN 	*/
		
		/* �ȴ�I2C������� */
		while (I2C_end == 0)
		{
			StatLoop();		
		}

		//VICIntEnable |= 1 << 15;

		if (I2C_end == 1)
			return (TRUE);
		else
			return (FALSE);			
	}

	//VICIntEnable |= 1 << 15;

	return (FALSE);
}

/*
*********************************************************************************************************
** �������� ��I2C_WriteNByte()
** �������� �������ӵ�ַ����д��N�ֽ�����
** ��ڲ��� ��	sla			�����ӵ�ַ
**				suba_type	�ӵ�ַ�ṹ	1�����ֽڵ�ַ	3��8+X�ṹ	2��˫�ֽڵ�ַ
**			  	suba		�����ڲ�������ַ
**			  	*s			��Ҫд������ݵ�ָ��
**			  	num			��Ҫд������ݵĸ���
** ���ڲ��� ��	TRUE		�����ɹ�
**			  	FALSE		����ʧ��
*********************************************************************************************************
*/
uint8 I2C_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num)
{
	uint32 i = 0,j = 0;

	 //VICIntEnClr |= 1 << 15;//���δ������ж�
	//if(sla == 0xa0)
		//P121Clr();	 //EEPROMд����

	if (num > 0)/* �����ȡ�ĸ���Ϊ0���򷵻ش��� */
	{	/* ���ò��� */	
		if (suba_type == 1)
		{	/* �ӵ�ַΪ���ֽ� */
			I2C_sla     	= sla;								/* �����Ĵӵ�ַ	 	*/
			I2C_suba    	= suba;								/* �����ӵ�ַ 			*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ1�ֽ� 	*/
		}
		if (suba_type == 2)
		{	/* �ӵ�ַΪ2�ֽ� */
			I2C_sla     	= sla;								/* �����Ĵӵ�ַ 		*/
			I2C_suba   	 	= suba;								/* �����ӵ�ַ 			*/
			I2C_suba_num	= 2;								/* �����ӵ�ַΪ2�ֽ� 	*/
		}
		if (suba_type == 3)
		{	/* �ӵ�ַ�ṹΪ8+X */
			I2C_sla			= sla + ((suba >> 7 )& 0x0e);		/* �����Ĵӵ�ַ		*/
			I2C_suba		= suba & 0x0ff;						/* �����ӵ�ַ			*/
			I2C_suba_num	= 1;								/* �����ӵ�ַΪ8+X	 	*/
		}

		I2C_buf     = s;										/* ���� 				*/
		I2C_num     = num;										/* ���ݸ��� 			*/
		I2C_suba_en = 2;										/* ���ӵ�ַ��д���� 	*/
		I2C_end     = 0;
		
		/* ���STA,SI,AA��־λ */
		I2C0CONCLR = 	(1 << 2)|	/* AA 	*/
					(1 << 3)|	/* SI 	*/
					(1 << 5);	/* STA 	*/
		
		/* ��λSTA,����I2C���� */
		I2C0CONSET = 	(1 << 5)|	/* STA 	*/
					(1 << 6);	/* I2CEN*/
		
		/* �ȴ�I2C������� */
		while (I2C_end == 0) 
		{
			StatLoop();	
		}

		//VICIntEnable |= 1 << 15;

		if (I2C_end == 1)
		{
			for(i = 0; i < 1000; i++)
			{
				for(j = 0; j < 100; j++);
			}
			return (TRUE);
		}
		else
			return (FALSE);	
	}

	//if(sla == 0xa0)
	//	P121Set();	 //EEPROMд����

	//VICIntEnable |= 1 << 15;

	return (FALSE);
}

/*
*********************************************************************************************************
** �������� ��__irq IRQ_I2C()
** �������� ��Ӳ��I2C�жϷ������ 
** ��ڲ��� ����
** ���ڲ��� ����
** ˵��     ��ע�⴦���ӵ�ַΪ2�ֽڵ������ 
*********************************************************************************************************
*/
//void __irq IRQ_I2C(void)
void StatLoop(void)
{	/* ��ȡI2C״̬�Ĵ���I2C0DAT */
	/* ����ȫ�ֱ��������ý��в���������������־ */
	/* ����ж��߼�,�жϷ��� */
	
	switch (I2C0STAT & 0xF8)
	{	/* ����״̬�������Ӧ�Ĵ��� */
		case 0x08:	/* �ѷ�����ʼ���� */				/* �����ͺ������ն��� 		*/
			/* װ��SLA+W����SLA+R */
		 	if(I2C_suba_en == 1)/* SLA+R */				/* ָ���ӵ�ַ�� 			*/
		 	{	I2C0DAT = I2C_sla & 0xFE; 				/* ��д���ַ 				*/
		 	}
            else	/* SLA+W */
            {  	I2C0DAT = I2C_sla;        				/* ����ֱ�ӷ��ʹӻ���ַ 	*/
            }
            /* ����SIλ */
            I2C0CONCLR =	(1 << 3)|						/* SI 						*/
            			(1 << 5);						/* STA 						*/
            break;
            
       	case 0x10:	/*�ѷ����ظ���ʼ���� */ 			/* �����ͺ������ն��� 		*/
       		/* װ��SLA+W����SLA+R */
       		I2C0DAT = I2C_sla;							/* �������ߺ��ط��ӵ�ַ 	*/
       		I2C0CONCLR = 0x28;							/* ����SI,STA */
       		break;

		case 0x18:
       	case 0x28:	/* �ѷ���I2C0DAT�е����ݣ��ѽ���ACK */
       		if (I2C_suba_en == 0)
       		{
	       		if (I2C_num > 0)
	       		{	I2C0DAT = *I2C_buf++;
	       			I2C0CONCLR = 0x28;					/* ����SI,STA 				*/
	       			I2C_num--;
	       		}
	       		else	/* û�����ݷ����� */
	       		{		/* ֹͣ���� */
	       		  	I2C0CONSET = (1 << 4);				/* STO 						*/
	       			I2C0CONCLR = 0x28;					/* ����SI,STA 				*/
	       		  	I2C_end = 1;						/* �����Ѿ�ֹͣ 			*/
	       		}
       		}
       		
            if(I2C_suba_en == 1)	/* ����ָ����ַ������������������ 				*/
            { 
            	if (I2C_suba_num == 2)
            	{	I2C0DAT = ((I2C_suba >> 8) & 0xff);
	       			I2C0CONCLR = 0x28;					/* ����SI,STA 				*/
	       			I2C_suba_num--;
	       			break;	
	       		} 
	       		
	       		if(I2C_suba_num == 1)
	       		{	I2C0DAT = (I2C_suba & 0xff);
	       			I2C0CONCLR = 0x28;					/* ����SI,STA 				*/
	       			I2C_suba_num--;
	       			break;	
	       		}
	       		
            	if (I2C_suba_num == 0)
            	{	I2C0CONSET = 0x20;
               		I2C0CONCLR = 0x08;
               		I2C_suba_en = 0;     				/* �ӵ�ַ������ 			*/
               		break;
               	}
            }
            
            if (I2C_suba_en == 2)/* ָ���ӵ�ַд,�ӵ�ַ��δָ��,�����ӵ�ַ 		*/
       		{
       		 	if (I2C_suba_num > 0)
            	{	if (I2C_suba_num == 2)
            		{	I2C0DAT = ((I2C_suba >> 8) & 0xff);
            			I2C0CONCLR = 0x28;
            			I2C_suba_num--;
            			break;
            		}
            		if (I2C_suba_num == 1)
            		{	I2C0DAT    = (I2C_suba & 0xff);
               			I2C0CONCLR = 0x28;
               			I2C_suba_num--;
               			I2C_suba_en  = 0;
               			break;
               		}
               	}
             }
       		break;
       		  
       case 0x40:	/* �ѷ���SLA+R,�ѽ���ACK */
       		if (I2C_num <= 1)	/* ��������һ���ֽ� */			
       		{	I2C0CONCLR = 1 << 2;      				/* �´η��ͷ�Ӧ���ź� 		*/
       		}
       		else
       		{ 	I2C0CONSET = 1 << 2;						/* �´η���Ӧ���ź� 		*/
       		}
       		I2C0CONCLR = 0x28;							/* ����SI,STA 				*/
       		break;

       	case 0x20:	/* �ѷ���SLA+W,�ѽ��շ�Ӧ��              */
       	case 0x30:	/* �ѷ���I2C0DAT�е����ݣ��ѽ��շ�Ӧ��     */
       	case 0x38:	/* ��SLA+R/W�������ֽ��ж�ʧ�ٲ�         */
   		case 0x48:	/* �ѷ���SLA+R,�ѽ��շ�Ӧ��              */
         	I2C0CONCLR = 0x28;
            I2C_end = 0xFF;
     		break;   				
	
		case 0x50:	/* �ѽ��������ֽڣ��ѷ���ACK */
			*I2C_buf++ = I2C0DAT;
			I2C_num--;
			if (I2C_num == 1)/* �������һ���ֽ� */
			{  	I2C0CONCLR = 0x2C;						/* STA,SI,AA = 0 			*/
			}
			else
			{  	I2C0CONSET = 0x04;						/* AA=1 					*/
			  	I2C0CONCLR = 0x28;
			}
			break;
		
		case 0x58:	/* �ѽ��������ֽڣ��ѷ��ط�Ӧ�� */
			*I2C_buf++ = I2C0DAT;     					/* ��ȡ���һ�ֽ����� 		*/
            I2C0CONSET = 0x10;        					/* �������� 				*/
            I2C0CONCLR = 0x28;
            I2C_end = 1; 
            break;
            
      	default:
      		break;
	}
   //VICVectAddr = 0x00;              					/* �жϴ������� 			*/
}
