
#include "oled.h"
#include "Driver_I2C.h"
#include "oledfont.h"  	

I2C_HandleTypeDef hi2c1;
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   Start_I2C();
   Wr_I2C(0x78);      //Slave address,SA0=0
   Wr_I2C(0x00);			//write command
   Wr_I2C(IIC_Command); 
   Stop_I2C();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   Start_I2C();
   Wr_I2C(0x78);			//D/C#=0; R/W#=0
   Wr_I2C(0x40);			//write data
   Wr_I2C(IIC_Data); 
   Stop_I2C();
}
/**********************************************
// Write OLED
**********************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd) Write_IIC_Data(dat);
  else    Write_IIC_Command(dat);	
}

/**********************************************
// Write OLED
**********************************************/
void OLED_WR_MutiByte(unsigned char *Pdata,unsigned char Num)
{
    ;//HAL_I2C_Mem_Write(&hi2c1,0x78,0x40,I2C_MEMADD_SIZE_8BIT,Pdata,Num,100);
	for (u32 i = 0; i < Num; i++)
	{
		Write_IIC_Data(Pdata[i]);
	}
	
	
}

/********************************************************************
* OLED初始化
* 为OLED正常工作做准备
********************************************************************/			    
void OLED_Init(void)
{
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

/********************************************
* 开启OLED
* OLED DISPLAY ON
********************************************/   
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/********************************************
* 关闭OLED
* OLED DISPLAY OFF
********************************************/ 
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

/********************************************
* OLED 清屏，无任何显示
********************************************/  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);//设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);  //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);  //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
/********************************************
* OLED点亮，全部显示
********************************************/  
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);//设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);  //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);  //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}

/********************************************
* 设置坐标
********************************************/  
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}
/********************************************************************
* 0.96 OLED 128*64像素，即:128列 x 64行
* 64行被均分成了8页，最开始8行为第0页，最后8行为第7页。
* 
* 函数功能为:从x列，y页开始显示一个字符,Char_Size为字体大小。
*
* x:0-127(列)
* y:0-7  (页)
* Char_Size: 16(字体大小为：8列 x 16行)，其他(6列 x 8行)
* flag,反白显示，默认为0，正常显示。1：反白显示。
********************************************************************/	
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 flag)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			{
				if(flag == 0) OLED_WR_Byte( F8X16[c*16+i],OLED_DATA);
				else          OLED_WR_Byte(~F8X16[c*16+i],OLED_DATA);				
			}			
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			{
				if(flag == 0) OLED_WR_Byte( F8X16[c*16+i+8],OLED_DATA);
				else          OLED_WR_Byte(~F8X16[c*16+i+8],OLED_DATA);				
			}
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			{
				if(flag == 0) OLED_WR_Byte( F6x8[c*6+i],OLED_DATA);
				else          OLED_WR_Byte(~F6x8[c*6+i],OLED_DATA);					
			}		
		}
}
/********************************************************************
* 
* 函数功能为:m的n次方
* 
********************************************************************/	
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/********************************************************************
* 0.96 OLED 128*64像素，即:128列 x 64行
* 64行被均分成了8页，最开始8行为第0页，最后8行为第7页。
* 
* 函数功能为:从x列，y页开始显示数字。
*
* x:0-127(列)
* y:0-7  (页)
* len:数字长度
* Char_Size: 16(字体大小为：8列 x 16行)，其他(6列 x 8行)
*
********************************************************************/	
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Char_Size,u8 flag)
{         	
	u8 t,temp,RowChar,CharSize;
	u8 enshow=0;	
	
	if(Char_Size==16)//判断字体大小
	{
		RowChar = 8;
		CharSize= 16;
	}
	else
	{
		RowChar = 6;
		CharSize= 8;		
	}
	
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+RowChar*t,y,' ',CharSize,flag);//第一个非0数字之前用空格替代
				continue;
			}
			else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+RowChar*t,y,temp+'0',CharSize,flag); 
	}
} 
/********************************************************************
* 0.96 OLED 128*64像素，即:128列 x 64行
* 64行被均分成了8页，最开始8行为第0页，最后8行为第7页。
* 
* 函数功能为:从x列，y页开始显示字符串。
*
* x:0-127(列)
* y:0-7  (页)
* *chr:字符串首地址
* Char_Size: 16(字体大小为：8列 x 16行)，其他(6列 x 8行)
*
********************************************************************/	
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 flag)
{
	u8 CharSize,RowChar,j=0;
	
	if(Char_Size==16)
	{
		RowChar = 8;
		CharSize= 16;
	}
	else
	{
		RowChar = 6;
		CharSize= 8;		
	}	
	
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],CharSize,flag);
		x+=RowChar;
		if(x>(120-RowChar))//行尾不足一个字，换行
		{
			x=0;
			y+=2;
		}
			j++;
	}
}
/********************************************************************
* 0.96 OLED 128*64像素，即:128列 x 64行
* 64行被均分成了8页，最开始8行为第0页，最后8行为第7页。
* 
* 函数功能为:从x列，y页开始显示汉字，汉字大小16x16
*
* x:0-127(列)
* y:0-7  (页)
* *chr:字符串首地址
* Num:第Num个汉字，汉字定义在oledfont.h的数组HzK[]中。
*
********************************************************************/	
void OLED_ShowCHinese(u8 x,u8 y,u8 Num,u8 flag)
{      			    
	u8 t,adder=0;
	
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				if(flag == 0) OLED_WR_Byte( Hzk[2*Num][t],OLED_DATA);
				else 					OLED_WR_Byte(~Hzk[2*Num][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				if(flag == 0) OLED_WR_Byte( Hzk[2*Num+1][t],OLED_DATA);
				else 					OLED_WR_Byte(~Hzk[2*Num+1][t],OLED_DATA);
				adder+=1;
      }					
}
/********************************************************************
* 0.96 OLED 128*64像素，即:128列 x 64行
* 64行被均分成了8页，最开始8行为第0页，最后8行为第7页。
* 
* 函数功能为:从x0列，y0页开始显示图片，坐标
*
* x:0-127(列)
* y:0-7  (页)
* *chr:字符串首地址
* BMP[]:图片字模数组
*
********************************************************************/	
void OLED_DrawBMP(u8 x0,u8 y0,u8 BMP[],u8 flag)
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
	for(y=y0;y<8;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<128;x++)
	    {      
	    	if(flag == 0) OLED_WR_Byte( BMP[j++],OLED_DATA);	
				else					OLED_WR_Byte(~BMP[j++],OLED_DATA);			
	    }
	}
}

/********************************************************************
* 0.96 OLED 128*64像素，即:128列 x 64行
* 64行被均分成了8页，最开始8行为第0页，最后8行为第7页。
*
* 函数功能为:从x0列，y0页开始显示图片，坐标
*
* x:0-127(列)
* y:0-7  (页)
* *chr:字符串首地址
* BMP[]:图片字模数组
*
********************************************************************/

//OLED的显存
__attribute__((unused)) u8 OLED_GRAM[8][128];

unsigned char  User_OLED_GRAM[8*128] ={
        0x00,0x80,0x00,0xC0,0x00,0xE0,0x00,0xF0,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x82,0x82,0x82,0x82,0xFE,0x38,0x38,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x10,0x10,0x10,0x10,0x30,0x30,0x20,0xE0,0xC0,0x00,0x00,0x38,0x30,0xC0,0x00,0x00,
        0x00,0x00,0x80,0xC0,0x38,0x00,0x00,0x00,0x00,0x00,0x38,0xC0,0x80,0x00,0x00,0x00,
        0x00,0x00,0x80,0xC0,0x70,0xF8,0x00,0x00,0x00,0x00,0xC0,0xC0,0x40,0x30,0x30,0x18,
        0x18,0x18,0x38,0x30,0x30,0x20,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
        0x00,0x00,0x00,0x00,0x3C,0x7C,0x84,0x83,0x03,0x00,0x00,0x00,0x00,0x03,0x03,0xBC,
        0xBC,0xFC,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x01,0x03,0x0C,0x1C,0x30,
        0x1C,0x02,0x03,0x01,0x00,0xFF,0x00,0x00,0x00,0x3F,0x7F,0x61,0x80,0x80,0x80,0x00,
        0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x7F,0x80,0x80,0x00,0x00,0x00,0x00,
        0x00,0x80,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
        0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,
        0x03,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x03,0x01,
        0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//更新显存到LCD
//User_OLED_GRAM[8][128];
//BackGround_OLED_GRAM[8][128];
void OLED_Refresh_Gram(void)
{
    //合并用户图像、背景图像
    for(u8 j = 0; j < 8; ++j)
    {
        for(u8 k = 0; k < 128; ++k) OLED_GRAM[j][k] =  OLED_GRAM[j][k]|User_OLED_GRAM[j*128+k];
    }
    //写入OLED
    for(u8 i = 0; i < 8; i++)
    {
        //确定起始位置
        OLED_WR_Byte(0xb0 + i, OLED_CMD);  //设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);      //设置显示位置,列低地址
        OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置,列高地址

        OLED_WR_MutiByte(&OLED_GRAM[i][0],128);
    }
}
//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
//显示器位左上角为（0,0）
void OLED_DrawPoint(u8 x,u8 y,u8 t)//点亮屏幕上的某一点
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//超出范围了.
    pos=7-y/8;	//第几页
    bx=y%8;
    temp=1<<(7-bx);//每页中的位置，移到每页中要点亮的相应位置
    if(t)OLED_GRAM[pos][x]|=temp;//在x列pos页（一页8位）,一个字节中相应位写入1
    else OLED_GRAM[pos][x]&=~temp;//在相应位写入	0
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  //点亮或熄灭屏幕上的某一个区域
{
    u8 x,y;
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }
   // OLED_Refresh_Gram();//更新显示
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_ClearGram(void)
{
    u8 i,n;
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[i][n]=0X00;
    //OLED_Refresh_Gram();//更新显示
}
