#include "config.h"

/*********************************************************************************************************

** Function name:			kbfft

** Descriptions:			���ٸ���Ҷ�任����任
** parameters describe:

pr-----˫����ʵ��һά���飬����Ϊn����l=0ʱ�����n�����������ʵ��������ʱ�����ɢ����Ҷ�任��ģ����l��1 ʱ����Ÿ���Ҷ�任��n��ʵ��������ʱ����渵��Ҷ�任��ģ��

pi-----˫����ʵ��һά���飬����Ϊn����l=0ʱ�����n������������鲿������ʱ�����ɢ����Ҷ�任�ķ��ǣ���l�� 1ʱ����Ÿ���Ҷ�任��n���鲿������ʱ����渵��Ҷ�任�ķ��ǡ����з��ǵĵ�λΪ�ȡ�

n------���α���������ĵ�����

k------���α���������n=2~k��2��k�η�����

fr-----˫����ʵ��һά���飬����Ϊn����l=0ʱ�����ظ���Ҷ�任��ʵ������l��1ʱ�������渵��Ҷ�任��ʵ����

fi-----˫����ʵ��һά���飬����Ϊn����l=0ʱ�����ظ���Ҷ�任���鲿����l��1ʱ�������渵��Ҷ�任���鲿��

l------���α�������l��0����ʾҪ�󱾺������㸵��Ҷ�任����l��1����ʾҪ�󱾺��������渵��Ҷ�任��

il-----���α�������il��0����ʾ��Ҫ�󱾺������㸵��Ҷ�任���渵��Ҷ�任��ģ����ǣ���il��1����ʾҪ�󱾺��� ���㸵��Ҷ�任���渵��Ҷ�任��ģ����ǡ�
*********************************************************************************************************/

//void kbfft(double * pr,double * pi,int n,int k,double * fr,double * fi,int l,int il)
/*
void kbfft(float * pr,float * pi,int n,int k,float * fr,float * fi,int l,int il)
{

	int it,m,is,i,j,nv,l0;
	//double p,q,s,vr,vi,poddr,poddi;
	float p,q,s,vr,vi,poddr,poddi;

	for(it=0;it<=n-1;it++)
	{
		m=it;is=0;
		for(i=0;i<=k-1;i++)
		{
			j=m/2;is=2*is+(m-2*j);m=j;
		}
		fr[it]=pr[is];
		fi[it]=pi[is];
	}

	pr[0]=1.0;
	pi[0]=0.0;
	p=6.283185306/(1.0*n);
	pr[1]=cos(p);
	pi[1]=-sin(p);

	if(l!=0)
	{
		pi[1]=-pi[1];
	}

	for(i=2;i<n-1;i++)
	{
		p=pr[i-1]*pr[1];q=pi[i-1]*pi[1];
		s=(pr[i-1]+pi[i-1])*(pr[1]+pi[1]);
		pr[i]=p-q;pi[i]=s-p-q;
	}

	for(it=0;it<=n-2;it=it+2)
	{
		vr=fr[it];vi=fi[it];
		fr[it]=vr+fr[it+1];fi[it]=vi+fi[it+1];
		fr[it+1]=vr-fr[it+1];fi[it+1]=vi-fi[it+1];
	} 

	m=n/2;nv=2;

	for(l0=k-2;l0>=0;l0--)
	{
		m=m/2;
		nv=2*nv;
		for(it=0;it<=(m-1)*nv;it=it+nv)
		{
			for(j=0;j<=(nv/2)-1;j++)
			{
				p=pr[m*j]*fr[it+j+nv/2];
				q=pi[m*j]*fi[it+j+nv/2];
				s=pr[m*j]+pi[m*j];
				s=s*(fr[it+j+nv/2]+fi[it+j+nv/2]);
				poddr=p-q;
				poddi=s-p-q;
				fr[it+j+nv/2]=fr[it+j]-poddr;
				fi[it+j+nv/2]=fi[it+j]-poddi;
				fr[it+j]=fr[it+j]+poddr;
				fi[it+j]=fi[it+j]+poddi;
			}
		}
	}

	if(l!=0)
	{
		for(i=0;i<=n-1;i++)
		{
			fr[i]=fr[i]/(1.0*n);
			fi[i]=fi[i]/(1.0*n);
		}
	}

	if(il!=0)
	{
		for(i=0;i<=n-1;i++)
		{
			pr[i]=sqrt(fr[i]*fr[i]+fi[i]*fi[i]);
			if(fabs(fr[i])<0.000001*fabs(fi[i]))
			{
				if((fi[i]*fr[i])>0)
					pi[i]=90.0;
				else
					pi[i]=-90.0;
			}
			else
				pi[i]=atan(fi[i]/fr[i])*360.0/6.283185306;
		}
	}

	return;
} 
*/
/*********************************************************************************************************
** Function name:			AdProcess
**
** Descriptions:			��A/Dת��ֵ
**
** input parameters:		uint16 * val---A/D���ݻ�����
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/04/16
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void AdProcess(uint32 * val,uint8 adChannel) 
{ 
	uint32 regVal;
	
	
	U0IER = 0x00;
	T0IR = 0x01;
	T0TCR = 0x00;
	


    AD0CR &= 0xFFFFFF00;
    AD0CR |= (1 << 24) | (1 << adChannel);	

    while ( 1 )
    {
	    regVal = *(uint32 *)(AD0_BASE_ADDR 
			+ 0x10 + 4 * adChannel);
	    if ( regVal & 0x80000000 )
	    {
		    break;
	    }
    }	
        
    AD0CR &= 0xF8FFFFFF;
    if ( regVal & 0x40000000 )
    {
	    *val = 0;
    }
    else
        *val = ( regVal >> 6 ) & 0x3FF;

	//PCONP &= ~(1 << 12);
	
	
	U0IER = 0x05;
	T0IR = 0x01;
	T0TCR = 0x03;
    T0TCR = 0x01;
	
		
	/*	
	AD0CR = 0; 
	AD0DR = 0; 

	AD0CR = 0x00200200; //CLKDIV= 2
	AD0CR |= (1<< adChannel);

	AD0CR |= 0x01000000;
	
	do 
	{ 
		*val = AD0DR;  
	}while ((*val & 0x80000000) == 0); 
	*val = ((*val >> 6) & 0x03FF);
	
	AD0CR &= ~0x01000000; 
	*/
	
	return; 
} 

/*********************************************************************************************************
** Function name:			itoa
**
** Descriptions:			�޷�������ת�����ַ���
**
** input parameters:		int32 num---��ת������, uint8 *str---�ַ���, int8 radix---����
** Returned value:			�ַ���
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/04/11
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
/*
uint8 *itoa(uint32 num, uint8 *str, int8 radix) 
{ 
	uint8 string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	
	int8 temp;
	int8 i; 
	int8 j; 
	uint8* ptr = str; 
	
	if(num == 0)
	{
		*ptr = '0';
		ptr++;
		*ptr = '0';
		ptr++;
		*ptr = '\0';
	}
	else
	{
		while (num) 
		{ 
			*ptr = string[num % radix]; 
			num /= radix; 
			ptr++;
			if (num < radix) 
			{ 
				*ptr = string[num]; 
				ptr++;
				*ptr = '\0'; 
				break; 
			} 
		} 
	}
	j = ptr - str - 1; 

	for (i = 0; i < (ptr - str) / 2; i++ ) 
	{ 

		temp = str[i]; 
		str[i] = str[j]; 
		str[j--] = temp; 
	} 

	return str; 
}
*/
/*********************************************************************************************************
** Function name:			strtod
**
** Descriptions:			�ַ���ת���ɸ�����
							
						   �������������ַ����еĿո��ַ��Ͳ���'+'��'-'��'.'��

						   ���ֵ��ַ�������ַ����ǿյĻ��߶����ɿո���ɣ�������

						   ���κ�ת���������ǰ��ַ����Ľ�����ַ����endptr�������

						   �����Ϸ����������ת���������ַ�������NULL�ĵ�ַ��

						   endptr���������ʹ��endptr��������ô��һ��NULLֵ��

   �����ˡ�
**
** input parameters:		const char* s---��ת���ַ���, char** endptr---������ַ
** Returned value:			������IEEE 754��׼
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/04/11
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

double strtod(const char* s, char** endptr) 

{

   	register const char*  p     = s;
    register long double  value = 0.L;
    int                   sign  = 0;
    long double           factor;
    unsigned int          expo;


    while ( *p == ' ' )//����ǰ��Ŀո�
      p++;

    if(*p == '-' || *p == '+')
      sign = *p++;//�ѷ��Ÿ����ַ�sign��ָ����ơ�

   

   	//���������ַ�
    while ( (unsigned int)(*p - '0') < 10u )//ת����������
      value = value*10 + (*p++ - '0');

   //����������ı�ʾ��ʽ���磺1234.5678��
   if ( *p == '.' ) 
   {
        factor = 1.;
        p++;
        while ( (unsigned int)(*p - '0') < 10u ) 
      	{
         	factor *= 0.1;
            value  += (*p++ - '0') * factor;
        }
    }
    
   //�����IEEE754��׼�ĸ�ʽ���磺1.23456E+3��
    if ( (*p | 32) == 'e' ) 
   	{
      	expo   = 0;
        factor = 10.L;
        switch (*++p)
      	{ 
        case '-': 
        	factor = 0.1;
        case '+': 
           p++;
           break;
        case '0': 
        case '1': 
        case '2':
        case '3':
        case '4': 
        case '5': 
        case '6': 
        case '7': 
        case '8': 
        case '9':
           break;
        default : 
           value = 0.L;
           p     = s;
           goto done;
        }
        while ( (unsigned int)(*p - '0') < 10u )
            expo = 10 * expo + (*p++ - '0');

        while ( 1 )
      	{
        if ( expo & 1 )
           	value *= factor;
            if ( (expo >>= 1) == 0 )
                break;
            factor *= factor;

        }

    }

done:
    if ( endptr != 0 )
        *endptr = (char*)p;
        
    return (sign == '-' ? -value : value);

}
/*********************************************************************************************************
** Function name:			atof
**
** Descriptions:			�ַ���ת���ɸ�����
**
** input parameters:		char *str---��ת���ַ���
** Returned value:			������IEEE 754��׼
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/04/11
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
/*
float atof(char *str)
{
   return strtod(str,0);

}
*/

uint32 adFilter(uint8 adChannel,uint8 adIndex)
{
   uint8 count;
   uint16 i,j;
   uint32 temp,lTemp,value_buf[ADCNT];
   uint32  sum=0;

    //S0SPCR &= 0xef;
	//for(i = 0; i < 30; i++);

   adIndex = 2;

   for(count = 0;count < ADCNT;count++)
   {
		P016_0();
		
      	//for(i = 0; i < 300; i++); //FCCLKƵ�ʼӿ����ʱ�ӳ�
      	S0SPDR =0;
		//for(j = 0; j < 30; j++);//FCCLKƵ�ʼӿ����ʱ�ӳ�

		while(!(S0SPSR &0x80));
  	  	lTemp = S0SPDR;
   	  	lTemp = (lTemp << 8) & 0x0000ff00;
   	  	S0SPDR =0;
		//for(j = 0; j < 30; j++);//FCCLKƵ�ʼӿ����ʱ�ӳ�
		while(!(S0SPSR &0x80));
   	  	lTemp |= S0SPDR;
   	  	lTemp = (lTemp >> 4) & 0x00000fff;
   	  	P016_1();
   	  	S0SPDR =0;
		//for(j = 0; j < 30; j++);//FCCLKƵ�ʼӿ����ʱ�ӳ�
		while(!(S0SPSR &0x80));
		

      value_buf[count] = lTemp;

	  for(j = 0; j < 5; j++)
	  {
	  	for(i = 0; i < 3000; i++);
	  }
   }
   //S0SPCR |= 0x18;

   for (j = 0;j < ADCNT-1;j++)
   {
      for (i = 0;i < ADCNT-j;i++)
      {
         if ( value_buf[i] > value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
            value_buf[i+1] = temp;
         }
      }
   }
   for(count = adIndex;count < (ADCNT-adIndex);count++)
      sum += value_buf[count];
      
   count = ADCNT-2*adIndex; 
     
   return (sum/count);
}

/*********************************************************************************************************
** Function name:			adFilter
**
** Descriptions:			��λֵƽ���˲�
**
** input parameters:		uint8 adChannel---A/Dͨ����;uint8 adIndex--ADCNT-2*adIndexΪʵ��ȡ����
** Returned value:			A/Dƽ��ֵ
**         
** Used global variables:	N---��������
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/04/11
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
 /*
uint32 adFilter(uint8 adChannel,uint8 adIndex)
{
   uint8 count;
   uint16 i,j;
   uint32 temp,lTemp,value_buf[ADCNT];
   uint32  sum=0;

	for(i = 0; i < 30; i++);

   for(count = 0;count < ADCNT;count++)
   {
	  AdProcess(&lTemp,adChannel);
      value_buf[count] = lTemp;
   }

   for (j = 0;j < ADCNT-1;j++)
   {
      for (i = 0;i < ADCNT-j-1;i++)
      {
         if ( value_buf[i] > value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
            value_buf[i+1] = temp;
         }
      }
   }
   for(count = adIndex;count < (ADCNT-adIndex);count++)
      sum += value_buf[count];
      
   count = ADCNT-2*adIndex; 
     
   return (sum/count);
}
 */
/*********************************************************************************************************
** Function name:			filter
**
** Descriptions:			��λֵ�˲�--ð���㷨
**
** input parameters:		uint8 adChannel---A/Dͨ����
** Returned value:			A/Dƽ��ֵ
**         
** Used global variables:	N---��������
** Calling modules:			None
**
** Created by:				XIAOCR
** Created Date:			2008/04/11
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
/*
uint32 adFilter(uint8 adChannel)
{
   uint32 temp,lTemp,value_buf[ADCNT];
   uint8 count,i,j;
   
   for ( count = 0;count < ADCNT;count++)
   {
   	  AdProcess(&lTemp,adChannel);
      value_buf[count] = lTemp;
   }
   for (j = 0;j < ADCNT-1;j++)
   {
      for (i = 0;i < ADCNT-j;i++)
      {
         if (value_buf[i] > value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
            value_buf[i+1] = temp;
         }
      }
   }
   return value_buf[(ADCNT-1)/2];
}    
*/

