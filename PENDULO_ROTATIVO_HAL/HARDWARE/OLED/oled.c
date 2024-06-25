/***********************************************
Empresa: Lunqu Technology (Dongguan) Co., Ltd.
Marca: WHEELTEC
Site oficial: wheeltec.net
Loja Taobao: shop114407458.taobao.com
AliExpress: https://minibalance.aliexpress.com/store/4455017
Versao: 1.0
Tempo de modificacao: 2021-12-09

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:1.0
Update£º2021-12-09

All rights reserved
***********************************************/
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 

u8 OLED_GRAM[128][8];	 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //Configurando o endereco da pagina£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //Definir posicao de exibicao ¨C endereco inferior da coluna
		OLED_WR_Byte (0x10,OLED_CMD);      //Definir posicao de exibicao ¨C endereco inferior da coluna 
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//Escreva um byte no OLED.
//dat:Dados/comando para escrever
//cmd:Sinalizador de dados/comando 0, indica comando; 1, indica dados;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_RS_Set();   	  
} 

	  	  
//Ative a tela OLED    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC - Comando DCDC
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//Desligue a tela OLED     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC - Comando DCDC
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//Funcao de limpar tela, depois de limpar a tela, a tela inteira ficara preta! eh o mesmo que nao acender!!!  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//Atualizar exibicao
}
//desenhar pontos 
//x:0~127
//y:0~63
//t:1 preencher 0, limpar			   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//Esta fora do escopo.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}


//Exibir um caractere na posicao especificada, incluindo alguns caracteres
//x:0~127
//y:0~63
//mode:0, display branco reverso; 1, display normal 
//size:Selecione a fonte 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//Obtenha o valor de deslocamento				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //acenda fonte 1206
		else temp=oled_asc2_1608[chr][t];		 //acenda fonte 1608                   
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n - funcao
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//Mostra 2 numeros
//x,y: coordenadas do ponto inicial
//len: o nrmero de digitos do numero
//tamanho: tamanho da fonte
//modo: modo 0, modo de preenchimento; 1, modo de sobreposicao
//num: valor numerico (0~4294967295);	 		  
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 

//exibe a string
//x,y: coordenadas do ponto inicial
//*p: endereco inicial da string
//Use 16 fontes
void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,12,1);	 
        x+=8;
        p++;
    }  
}	   

//Exibir 16*16 caracteres chineses do estilo da musica
void OLED_ShowChinese(u8 x,u8 y,u16 chr,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
  for(t=0;t<32;t++)
	{
		temp=asc2_Chinese[chr][t];
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}	 		
	}	        
}


void OLED_ShowChinese_12(u8 x,u8 y,u16 chr,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
  for(t=0;t<28;t++)
	{
		temp=asc2_Chinese_12[chr][t];
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}	 		
	}	        
}

/**************************************************************************
Function: Initialize the OLED
Input   : none
Output  : none
**************************************************************************/					    
void OLED_Init(void)
{ 	
	OLED_RST_Clr();
	HAL_Delay(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //Desligar a tela
	OLED_WR_Byte(0xD5,OLED_CMD); //Defina o fator de divisao do clock e a frequencia de oscilacao
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],Fator de divisao de frequencia; [7:4], frequencia de oscilacao
	OLED_WR_Byte(0xA8,OLED_CMD); //Defina o numero de canais de drive
	OLED_WR_Byte(0X3F,OLED_CMD); //Padrao 0X3F (1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //Definir deslocamento de exibicao
	OLED_WR_Byte(0X00,OLED_CMD); //O padrao e 0

	OLED_WR_Byte(0x40,OLED_CMD); //Defina a linha inicial do display [5:0], o numero de linhas.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //µçºÉ±ÃÉèÖÃ - Configuracoes da bomba de carga
	OLED_WR_Byte(0x14,OLED_CMD); //bit2£¬Ligado/desligado
	OLED_WR_Byte(0x20,OLED_CMD); //Definir modo de endereco de memoria
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00£¬Modo de endereco de coluna; 01, modo de endereco de linha; 10, modo de endereco de pagina; padrao 10;
	OLED_WR_Byte(0xA1,OLED_CMD); //Configuracoes de redefinicao de segmento,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //Defina a direcao de varredura COM; bit3:0, modo normal; 1, modo de redefinicao COM[N-1]->COM0; N: numero de canais de unidade
	OLED_WR_Byte(0xDA,OLED_CMD); //Definir configuracao de pinos de hardware COM
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]Configuracao
		 
	OLED_WR_Byte(0x81,OLED_CMD); //Configuracoes de contraste
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Padrao 0X7F (configuracao de brilho, quanto maior, mais brilhante)
	OLED_WR_Byte(0xD9,OLED_CMD); //Definir ciclo de pre-carga
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //Definir ampliacao de tensao VCOMH
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //A exibicao global esta ativada; bit0:1, ativado; 0, desativado; (tela branca/tela preta)
	OLED_WR_Byte(0xA6,OLED_CMD); //Defina o modo de exibicao; bit0:1, exibicao reversa; 0, exibicao normal 						   
	OLED_WR_Byte(0xAF,OLED_CMD); //Ativar exibicao
	OLED_Clear();
}  




