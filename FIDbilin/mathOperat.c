#include "config.h"

/*********************************************************************************************************

** Function name:			kbfft

** Descriptions:			快速傅立叶变换和逆变换
** parameters describe:

pr-----双精度实型一维数组，长度为n。当l=0时，存放n个采样输入的实部，返回时存放离散傅立叶变换的模；当l＝1 时，存放傅立叶变换的n个实部，返回时存放逆傅立叶变换的模。

pi-----双精度实型一维数组，长度为n。当l=0时，存放n个采样输入的虚部，返回时存放离散傅立叶变换的幅角；当l＝ 1时，存放傅立叶变换的n个虚部，返回时存放逆傅立叶变换的幅角。其中幅角的单位为度。

n------整形变量。输入的点数。

k------整形变量。满足n=2~k（2的k次方）。

fr-----双精度实型一维数组，长度为n。当l=0时，返回傅立叶变换的实部；当l＝1时，返回逆傅立叶变换的实部。

fi-----双精度实型一维数组，长度为n。当l=0时，返回傅立叶变换的虚部；当l＝1时，返回逆傅立叶变换的虚部。

l------整形变量。若l＝0，表示要求本函数计算傅立叶变换；若l＝1，表示要求本函数计算逆傅立叶变换。

il-----整形变量。若il＝0，表示不要求本函数计算傅立叶变换或逆傅立叶变换的模与幅角；若il＝1，表示要求本函数 计算傅立叶变换或逆傅立叶变换的模与幅角。
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
** Descriptions:			读A/D转换值
**
** input parameters:		uint16 * val---A/D数据缓冲区
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
** Descriptions:			无符号整型转换成字符串
**
** input parameters:		int32 num---待转换数据, uint8 *str---字符串, int8 radix---进制
** Returned value:			字符串
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
** Descriptions:			字符串转换成浮点数
							
						   函数将会跳过字符串中的空格字符和不是'+'、'-'、'.'、

						   数字的字符。如果字符串是空的或者都是由空格组成，将不会

						   做任何转换，仅仅是把字符串的结束地址赋给endptr。如果字

						   符串合法，将会进行转换，并把字符串最后的NULL的地址给

						   endptr。如果你想使用endptr参数，那么赋一个NULL值就

   可以了。
**
** input parameters:		const char* s---待转换字符串, char** endptr---结束地址
** Returned value:			浮点数IEEE 754标准
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


    while ( *p == ' ' )//跳过前面的空格
      p++;

    if(*p == '-' || *p == '+')
      sign = *p++;//把符号赋给字符sign，指针后移。

   

   	//处理数字字符
    while ( (unsigned int)(*p - '0') < 10u )//转换整数部分
      value = value*10 + (*p++ - '0');

   //如果是正常的表示方式（如：1234.5678）
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
    
   //如果是IEEE754标准的格式（如：1.23456E+3）
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
** Descriptions:			字符串转换成浮点数
**
** input parameters:		char *str---待转换字符串
** Returned value:			浮点数IEEE 754标准
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
		
      	//for(i = 0; i < 300; i++); //FCCLK频率加快后，延时加长
      	S0SPDR =0;
		//for(j = 0; j < 30; j++);//FCCLK频率加快后，延时加长

		while(!(S0SPSR &0x80));
  	  	lTemp = S0SPDR;
   	  	lTemp = (lTemp << 8) & 0x0000ff00;
   	  	S0SPDR =0;
		//for(j = 0; j < 30; j++);//FCCLK频率加快后，延时加长
		while(!(S0SPSR &0x80));
   	  	lTemp |= S0SPDR;
   	  	lTemp = (lTemp >> 4) & 0x00000fff;
   	  	P016_1();
   	  	S0SPDR =0;
		//for(j = 0; j < 30; j++);//FCCLK频率加快后，延时加长
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
** Descriptions:			中位值平均滤波
**
** input parameters:		uint8 adChannel---A/D通道号;uint8 adIndex--ADCNT-2*adIndex为实际取样数
** Returned value:			A/D平均值
**         
** Used global variables:	N---采样次数
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
** Descriptions:			中位值滤波--冒泡算法
**
** input parameters:		uint8 adChannel---A/D通道号
** Returned value:			A/D平均值
**         
** Used global variables:	N---采样次数
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

