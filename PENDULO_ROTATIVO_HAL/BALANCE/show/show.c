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
Update：2021-12-09

All rights reserved
***********************************************/
#include "show.h"
unsigned char i,temp;          //Variavel de contagem
unsigned char Send_Count; //O numero de dados que precisam ser enviados pela porta serial
float Vol;
/**************************************************************************
Funcao: display OLED
Parametros de entrada: Nenhum
Valor de retorno: Nenhum
**************************************************************************/
void oled_show(void)
{
		//=============A linha 1 exibe o parametro P do controle PD do angulo Position_KP=======================//	
		OLED_ShowString(00,00,"B-KP");                   
		OLED_ShowNumber(40,00,Balance_KP,3,12);
		OLED_ShowString(57,00,"."),  
		OLED_ShowNumber(61,00,(int)(Balance_KP*10)%10,1,12);

		OLED_ShowString(95,00,"A:");	  
		OLED_ShowNumber(108,00,Amplitude1,2,12);

		//=============A linha 2 exibe o parametro D de controle PD do angulo=======================//	
		OLED_ShowString(00,10,"B-KD");
		OLED_ShowNumber(40,10,Balance_KD,3,12);
		OLED_ShowString(57,10,"."),  
		OLED_ShowNumber(61,10,(int)(Balance_KD*10)%10,1,12);

		OLED_ShowString(95,10,"A:");	  
		OLED_ShowNumber(108,10,Amplitude2,2,12);
		//=============A linha 3 mostra o codificador Position_KP=======================//	
		OLED_ShowString(00,20,"P-KP");
		OLED_ShowNumber(40,20,Position_KP,3,12);
		OLED_ShowString(57,20,"."),  
		OLED_ShowNumber(61,20,(int)(Position_KP*10)%10,1,12);
		
		OLED_ShowString(95,20,"A:");	  
		OLED_ShowNumber(108,20,Amplitude3,2,12);
		//=============A linha 4 mostra o codificador 1=======================//	
		OLED_ShowString(00,30,"P-KD");
		OLED_ShowNumber(40,30,Position_KD,3,12);
		OLED_ShowString(57,30,"."),  
		OLED_ShowNumber(61,30,(int)(Position_KD*10)%10,1,12);
		
		OLED_ShowString(95,30,"A:");	  
		OLED_ShowNumber(108,30,Amplitude4,2,12);
		//======Este é o menu de rolagem. Selecione o parametro PD que precisa ser ajustado.											
		  if(Menu==1)
	   	{
			 OLED_ShowChar(75,00,'Y',12,1);   
			 OLED_ShowChar(75,10,'N',12,1);
			 OLED_ShowChar(75,20,'N',12,1);
			 OLED_ShowChar(75,30,'N',12,1);
	  	}
		  else	if(Menu==2)
	   	{
			 OLED_ShowChar(75,00,'N',12,1);
			 OLED_ShowChar(75,10,'Y',12,1);
			 OLED_ShowChar(75,20,'N',12,1);
			 OLED_ShowChar(75,30,'N',12,1);
			}		
      else if(Menu==3)
	   	{			
			 OLED_ShowChar(75,00,'N',12,1);
			 OLED_ShowChar(75,10,'N',12,1);
			 OLED_ShowChar(75,20,'Y',12,1);
			 OLED_ShowChar(75,30,'N',12,1);
			}		
      else if(Menu==4)
	   	{				
			 OLED_ShowChar(75,00,'N',12,1);
			 OLED_ShowChar(75,10,'N',12,1);
			 OLED_ShowChar(75,20,'N',12,1);
			 OLED_ShowChar(75,30,'Y',12,1);
	 	  } 
			
	//=============A quinta linha mostra a tensao e a posicao do alvo=======================//			
			OLED_ShowString(80,40,"T:");	  
			OLED_ShowNumber(95,40,Position_Zero,5,12) ; 
			OLED_ShowString(00,40,"VOL:");
			OLED_ShowString(41,40,".");
			OLED_ShowString(63,40,"V");
			OLED_ShowNumber(28,40,Voltage/100,2,12);
			OLED_ShowNumber(51,40,Voltage%100,2,12);
		 if(Voltage%100<10) 	OLED_ShowNumber(45,40,0,2,12);
		//=============A sexta linha exibe o sensor de deslocamento angular e os dados do codificador=======================//
		OLED_ShowString(80,50,"P:");    OLED_ShowNumber(95,50,Encoder,5,12); 
		OLED_ShowString(0,50,"ADC:");  OLED_ShowNumber(30,50,Angle_Balance,4,12);
		//=============atualizar=======================//
		OLED_Refresh_Gram();	
	}

/**************************************************************************
Funcao: O osciloscópio virtual envia dados para o computador host e fecha a tela
Parametros de entrada: Nenhum
Valor de retorno: Nenhum
Autor: Balance Car Home
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data( Angle_Balance, 1 );      
		DataScope_Get_Channel_Data( Encoder, 2 );         
		DataScope_Get_Channel_Data( Moto, 3 );              
//		DataScope_Get_Channel_Data( 0 , 4 );   
//		DataScope_Get_Channel_Data(0, 5 ); //Basta substituir o 0 pelos dados que você deseja exibir
//		DataScope_Get_Channel_Data(0 , 6 );//Basta substituir o 0 pelos dados que você deseja exibir
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(3);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		//HAL_UART_Transmit(&huart1,Angle_Balance,sizeof(Angle_Balance),50);
		}
}	

//Página de prompt: Foi detectado que a barra oscilante precisa ser ajustada. Ajuste a barra oscilante de acordo com as instrucoes e pressione o botao na página para iniciar o ajuste.
void show_Tips(void)
{
 //Inicie o modo de inspecao de instalacao de deslocamento angular, siga as instrucoes. Pressione o botao do usuário para confirmar...
  OLED_ShowChinese_12(28,0,0,1);
	OLED_ShowChinese_12(42,0,1,1);
	OLED_ShowChinese_12(56,0,2,1);
	OLED_ShowChinese_12(70,0,3,1);
	OLED_ShowChinese_12(84,0,4,1);
	OLED_ShowChinese_12(98,0,5,1);
	OLED_ShowChinese_12(112,0,6,1);
	OLED_ShowChinese_12(0,16,7,1);
	OLED_ShowChinese_12(14,16,8,1);
	OLED_ShowChinese_12(28,16,9,1);
	OLED_ShowChinese_12(42,16,10,1);
	OLED_ShowChinese_12(56,16,11,1);
	OLED_ShowChinese_12(70,16,12,1);
	OLED_ShowChinese_12(84,16,13,1);
	OLED_ShowChinese_12(98,16,14,1);
	OLED_ShowChinese_12(112,16,15,1);
	OLED_ShowChinese_12(0,32,16,1);
	OLED_ShowChinese_12(14,32,17,1);
	OLED_ShowChinese_12(28,32,18,1);
	OLED_ShowChinese_12(42,32,19,1);
	OLED_ShowChinese_12(56,32,20,1);
	OLED_ShowChinese_12(70,32,21,1);
	OLED_ShowChinese_12(84,32,22,1);
	OLED_ShowChinese_12(98,32,23,1);
	OLED_ShowChinese_12(112,32,24,1);
	OLED_ShowChinese_12(0,48,25,1);
	OLED_ShowChinese_12(14,48,26,1);
	OLED_ShowChinese_12(28,48,27,1);
	OLED_ShowChinese_12(42,48,28,1);
	OLED_ShowChinese_12(46,48,28,1);
	OLED_ShowChinese_12(50,48,28,1);
	
	OLED_Refresh_Gram();
}

//Página 0: Por favor, fixe o pêndulo invertido de forma que ele fique verticalmente para baixo e permaneca estacionário. Após a conclusao da instalacao, pressione o botao da página para continuar.
void step_0(void)
{
  OLED_ShowChinese_12(0,0,12,1);
	OLED_ShowChinese_12(14,0,29,1);
	OLED_ShowChinese_12(28,0,30,1);
	OLED_ShowChinese_12(42,0,31,1);
	OLED_ShowChinese_12(56,0,32,1);
	OLED_ShowChinese_12(70,0,33,1);
	OLED_ShowChinese_12(84,0,34,1);
	OLED_ShowChinese_12(98,0,11,1);
	OLED_ShowChinese_12(112,0,35,1);
	OLED_ShowChinese_12(0,16,36,1);
	OLED_ShowChinese_12(14,16,37,1);
	OLED_ShowChinese_12(28,16,38,1);
	OLED_ShowChinese_12(42,16,39,1);
	OLED_ShowChinese_12(56,16,40,1);
	OLED_ShowChinese_12(70,16,41,1);
	OLED_ShowChinese_12(84,16,42,1);
	OLED_ShowChinese_12(98,16,43,1);
	OLED_ShowChinese_12(112,16,44,1);
	OLED_ShowChinese_12(0,32,45,1);
	OLED_ShowChinese_12(14,32,46,1);
	OLED_ShowChinese_12(28,32,19,1);
	OLED_ShowChinese_12(42,32,47,1);
	OLED_ShowChinese_12(56,32,48,1);
	OLED_ShowChinese_12(70,32,49,1);
	OLED_ShowChinese_12(84,32,50,1);
	OLED_ShowChinese_12(98,32,51,1);
	OLED_ShowChinese_12(112,32,24,1);
	OLED_ShowChinese_12(0,48,41,1);
	OLED_ShowChinese_12(14,48,52,1);
	OLED_ShowChinese_12(28,48,53,1);
	OLED_ShowChinese_12(42,48,24,1);
	OLED_ShowChinese_12(56,48,25,1);
	OLED_ShowChinese_12(70,48,54,1);
	OLED_ShowChinese_12(84,48,55,1);
	OLED_ShowChinese_12(98,48,28,1);
	OLED_ShowChinese_12(102,48,28,1);
	OLED_ShowChinese_12(106,48,28,1);
	OLED_Refresh_Gram();
}

//Página 1: Afrouxe os dois parafusos que conectam a haste do pêndulo e o sensor de deslocamento angular e, em seguida, ajuste o botao da haste do pêndulo para garantir
void step_1(void)
{
	OLED_ShowChinese(0,0,60,1);
	OLED_ShowChinese(16,0,61,1);
	OLED_ShowChinese(32,0,62,1);
	OLED_ShowChinese(48,0,7,1);
	OLED_ShowChinese(64,0,8,1);
	OLED_ShowChinese(80,0,63,1);
	OLED_ShowChinese(96,0,64,1);
	OLED_ShowChinese(112,0,65,1);
	OLED_ShowChinese(0,16,66,1);
	OLED_ShowChinese(16,16,67,1);
	OLED_ShowChinese(32,16,68,1);
	OLED_ShowChinese(48,16,69,1);
	OLED_ShowChinese(64,16,70,1);
	OLED_ShowChinese(80,16,71,1);
	OLED_ShowChinese(96,16,72,1);
	OLED_ShowChinese(112,16,73,1);
	OLED_ShowChinese(0,32,74,1);
	OLED_ShowChinese(16,32,75,1);
	OLED_ShowChinese(32,32,19,1);
	OLED_ShowChinese(48,32,76,1);
	OLED_ShowChinese(64,32,17,1);
	OLED_ShowChinese(80,32,77,1);
	OLED_ShowChinese(96,32,48,1);
	OLED_ShowChinese(112,32,5,1);
	OLED_ShowChinese(0,48,6,1);
	OLED_ShowChinese(16,48,7,1);
	OLED_ShowChinese(32,48,8,1);
	OLED_ShowChinese(48,48,27,1);
	OLED_ShowChinese(64,48,28,1);
	OLED_ShowChinese(80,48,29,1);
	OLED_ShowChinese(96,48,17,1);
	OLED_ShowChinese(112,48,41,1);	
	OLED_Refresh_Gram();
}

//Página 2: Prove que o valor ADC está entre 1010 e 1030 quando o pêndulo está verticalmente para baixo e, em seguida, aperte a conexao entre o pêndulo e a posicao do canto
void step_2(void)
{
	OLED_ShowChinese(0,0,78,1);
	OLED_ShowChinese(16,0,7,1);
	OLED_ShowChinese(32,0,8,1);
	OLED_ShowChinese(48,0,42,1);
	OLED_ShowChinese(64,0,43,1);
	OLED_ShowChinese(80,0,44,1);
	OLED_ShowChinese(96,0,45,1);
	OLED_ShowChinese(112,0,79,1);
	OLED_ShowChar(0,16,'A',16,1);
	OLED_ShowChar(8,16,'D',16,1);
	OLED_ShowChar(16,16,'C',16,1);
	OLED_ShowChinese(24,16,71,1);
	OLED_ShowChinese(40,16,31,1);
	OLED_ShowChinese(56,16,80,1);
	OLED_ShowNumber(72,16,1010,4,16);
	OLED_ShowChinese(104,16,33,1);//chegar
	OLED_ShowNumber(0,32,1030,4,16);
	OLED_ShowChinese(32,32,34,1);
	OLED_ShowChinese(48,32,35,1); //entre
	OLED_ShowChinese(64,32,17,1);
	OLED_ShowChinese(80,32,77,1);
	OLED_ShowChinese(96,32,48,1);
	OLED_ShowChinese(112,32,19,1);
	OLED_ShowChinese(0,48,81,1);
	OLED_ShowChinese(16,48,61,1);
	OLED_ShowChinese(32,48,62,1);
	OLED_ShowChinese(48,48,7,1);
	OLED_ShowChinese(64,48,8,1);
	OLED_ShowChinese(80,48,63,1);
	OLED_ShowChinese(96,48,64,1);
	OLED_ShowChinese(112,48,65,1);
	OLED_Refresh_Gram();
}


//Página 3: Dois parafusos no sensor de mudanca. Após o ajuste bem-sucedido, pressione o botao do usuário para verificar
void step_3(void)
{
	OLED_ShowChinese(0,0,66,1);
	OLED_ShowChinese(16,0,67,1);
	OLED_ShowChinese(32,0,68,1);
	OLED_ShowChinese(48,0,69,1);
	OLED_ShowChinese(64,0,70,1);
	OLED_ShowChinese(80,0,71,1);
	OLED_ShowChinese(96,0,72,1);
	OLED_ShowChinese(112,0,73,1);
	OLED_ShowChinese(0,16,74,1);
	OLED_ShowChinese(16,16,75,1);
	OLED_ShowChinese(32,16,18,1); // 。
	OLED_ShowChinese(48,16,5,1);//Após a conclusao do ajuste, pressione o botao do usuário para verificar
	OLED_ShowChinese(64,16,6,1);
	OLED_ShowChinese(80,16,46,1);
	OLED_ShowChinese(96,16,47,1);
	OLED_ShowChinese(112,16,48,1);
	OLED_ShowChinese(0,32,9,1);
	OLED_ShowChinese(16,32,10,1);
	OLED_ShowChinese(32,32,11,1);
	OLED_ShowChinese(48,32,12,1);
	OLED_ShowChinese(64,32,13,1);
	OLED_ShowChinese(80,32,14,1);
	OLED_ShowChinese(96,32,56,1);
	OLED_ShowChinese(112,32,94,1);
	OLED_ShowChar(24,48,'A',16,1);
	OLED_ShowChar(32,48,'D',16,1);
	OLED_ShowChar(40,48,'C',16,1);
  OLED_ShowNumber(78,48,Angle_Balance,4,16);
	OLED_Refresh_Gram();
}

//Página de verificacao do sistema: Verificando se o ajuste está correto...10 Mantenha o pêndulo parado
void check_display(void)
{
	OLED_ShowChinese_12(21,0,56,1);
	OLED_ShowChinese_12(35,0,57,1);
	OLED_ShowChinese_12(49,0,58,1);
	OLED_ShowChinese_12(63,0,59,1);
	OLED_ShowChinese_12(77,0,60,1);
	OLED_ShowChinese_12(91,0,61,1);
	OLED_ShowChinese_12(105,0,62,1);
	OLED_ShowChinese_12(0,16,63,1);
	OLED_ShowChinese_12(14,16,64,1);
	OLED_ShowChinese_12(28,16,65,1);
	OLED_ShowChinese_12(46,16,28,1);
	OLED_ShowChinese_12(53,16,28,1);
	OLED_ShowChinese_12(60,16,28,1);
	OLED_ShowChinese_12(67,16,28,1);
	OLED_ShowChinese_12(74,16,28,1);
	OLED_ShowChinese_12(81,16,28,1);
//Contagem regressiva
//Por favor, mantenha o pêndulo imóvel
	OLED_ShowChinese_12(14,32,12,1);
	OLED_ShowChinese_12(28,32,43,1);
	OLED_ShowChinese_12(42,32,44,1);
	OLED_ShowChinese_12(56,32,36,1);
	OLED_ShowChinese_12(70,32,37,1);
	OLED_ShowChinese_12(84,32,45,1);
	OLED_ShowChinese_12(98,32,46,1);
	OLED_ShowChar(24,48,'A',16,1);
	OLED_ShowChar(32,48,'D',16,1);
	OLED_ShowChar(40,48,'C',16,1);
  OLED_ShowNumber(78,48,Angle_Balance,4,16);
	OLED_Refresh_Gram();
}

//Página de sucesso do ajuste: Ajuste bem-sucedido. Pressione o bot?o do usuário para entrar no sistema.
void success_display(void)
{
	OLED_ShowChinese(32,4,5,1);      /*调*/
	OLED_ShowChinese(48,4,6,1);      /*节*/
	OLED_ShowChinese(64,4,47,1);     /*成*/
	OLED_ShowChinese(80,4,87,1);     /*功*/
	OLED_ShowChinese(24,24,9,1);     /*按*/
	OLED_ShowChinese(40,24,10,1);    /*下*/
	OLED_ShowChinese(56,24,11,1);    /*用*/
	OLED_ShowChinese(72,24,12,1);    /*户*/
	OLED_ShowChinese(88,24,13,1);    /*按*/
	OLED_ShowChinese(104,24,14,1);   /*键*/
	OLED_ShowChinese(0,44,88,1);     /*进*/
	OLED_ShowChinese(16,44,89,1);    /*入*/
	OLED_ShowChinese(32,44,90,1);    /*系*/
	OLED_ShowChinese(48,44,91,1);	   /*统*/
	OLED_Refresh_Gram();
}

//Página de falha de ajuste: falha na verificacao. Ajuste novamente.
void fail_display(void)
{
	OLED_ShowChinese(24,16,56,1);
	OLED_ShowChinese(40,16,94,1);
	OLED_ShowChinese(56,16,100,1);
	OLED_ShowChinese(72,16,101,1);
	OLED_ShowChinese(88,16,102,1);
	OLED_ShowChinese(24,32,0,1);
	OLED_ShowChinese(40,32,103,1);
	OLED_ShowChinese(56,32,104,1);
	OLED_ShowChinese(72,32,5,1);
	OLED_ShowChinese(88,32,6,1);
	OLED_Refresh_Gram();
}
