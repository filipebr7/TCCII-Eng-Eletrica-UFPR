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
#include "gpio.h"

/**************************************************************************
Funcao: digitalizacao de teclas
Parametros de entrada: clique duas vezes no tempo de espera
Valor de retorno: Status da chave 0: Nenhuma acao 1: Clique unico 2: Clique duplo
**************************************************************************/
u8 click_N_Double (u8 time)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(KEY2==0)  Forever_count++;   //O sinalizador de pressao longa nao esta definido como 1
     else        Forever_count=0;
		if(0==KEY2&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2;//Clique duas vezes no comando para executar
				}
		}
		if(1==KEY2)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//Clique para executar o comando
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}

/**************************************************************************
Funcao: detecao de pressao longa
Parametros de entrada: Nenhum
Valor de retorno: Status da tecla 0: Nenhuma acao 1: Pressao longa por 2s
**************************************************************************/
u8 Long_Press(void)
{
			static u16 Long_Press_count,Long_Press=0;
	    if(Long_Press==0&&KEY7==0)  Long_Press_count++;   //O sinalizador de pressao longa nao esta definido como 1
      else                       Long_Press_count=0; 
		  if(Long_Press_count>20)		
			{
				Long_Press=1;	
				Long_Press_count=0;
				return 1;
			}				
			 if(Long_Press==1)     //Pressione longamente a posicao 1 da marca
			{
				  Long_Press=0;
			}
			return 0;
}

u8 Long_Press_KEY2(void)
{
			static u16 Long_Press_count,Long_Press=0;
	    if(Long_Press==0&&KEY2==0)  Long_Press_count++;   //O sinalizador de pressao longa nao esta definido como 1
      else                       Long_Press_count=0; 
		  if(Long_Press_count>200)		
			{
				Long_Press=1;	
				Long_Press_count=0;
				return 1;
			}				
			 if(Long_Press==1)     //Pressione longamente a posicao 1 da marca
			{
				  Long_Press=0;
			}
			return 0;
}

///**************************************************************************
//Funcao: varredura de teclas
//Parametros de entrada: nenhum
//Valor de retorno: status do botao 0: nenhuma acao 1: clique
//**************************************************************************/
u8 click(void)
{
			static u8 flag_key=1;//sinal de comunicado de pressao de botao
			if(flag_key&&(KEY5==0||KEY11==0||KEY12==0))
			{
			flag_key=0;
			HAL_Delay(50);
			if(KEY5==0)return 5;	
      else if (KEY11==0) return 11;
      else if (KEY12==0)	return 12;		
			}
			else	flag_key=1;
			return 0;//Nenhum botao pressionado
}
