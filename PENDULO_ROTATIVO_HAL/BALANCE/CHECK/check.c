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
#include "check.h"
/**************************************************************************
Funcao: display OLED, detecao se o deslocamento angular precisa ser ajustado, funcao de ajuste de deslocamento angular
Parametros de entrada: Nenhum
Valor de retorno: Nenhum
**************************************************************************/
void Tips(void)
{
	static u8 i=0;     //para cronometrar
	u8 key;            //Usado para obter o valor-chave
	static u8 count=0; //Usado para contar o n¨²mero de vezes que o valor ADC correto ¨¦ detectado
	
begin: //Marque a posicao onde a funcao inicia. Caso precise ajustar o sensor de deslocamento angular, pule aqui para iniciar a execucao da funcao ap¨®s a conclusao do ajuste.
	
		if(tips_flag ==1 )  //Na primeira vez tips_flag = 0, a execucao nao ir¨¢ aqui.
		{
			tips_flag=0;                 //Se voc¨º entrou na funcao de deteccao, significa que a interrupcao est¨¢ bloqueada. Voc¨º precisa abrir todas as interrupcoes que precisam ser usadas e atualizar o OLED.
			
		//Restaurar interrupcoes mascaradas ao entrar no modo de configuracao
		 EXTI->IMR |= 1<<5;	
		 EXTI->IMR |= 1<<7;
		 EXTI->IMR |= 1<<11;
		 EXTI->IMR |= 1<<12;	
     TIM1->DIER |= 0x01;			
		 OLED_Clear(); //Limpe a tela e prepare-se para entrar no sistema
		}
		
		//===Quando o programa estiver rodando normalmente, apenas este passo ser¨¢ executado. Caso seja necess¨¢rio entrar no modo de ajuste, outras funcoes serao utilizadas.
		else
		oled_show();  //===A tela est¨¢ ativada		

	  key = Long_Press(); //Detecte o botao, pressione e segure por cerca de 1s para entrar no modo de ajuste
		if(key==1)
		{
			system_start=1;
		}
		
 //O usu¨¢rio pressiona longamente o botao, system_start=1, e entra na funcao de ajuste.
  if(system_start==1)
	 {
		 system_start=0;    //O flag nao entra na funcao de ajuste de deslocamento angular, esta funcao s¨® pode ser executada uma vez ap¨®s a inicializacao.
		 OLED_Clear();     //limpar tela
		 tips_flag = 1;	  //Pacote  tips_flag = 1, Certifique-se de que todas as interrupcoes possam ser restauradas depois que o angulo for ajustado adequadamente e que o OLED possa ser atualizado e exibido uma vez.
		 PWMB=0;         //Desligue o motor para evitar que ele retenha seu valor e continue se movendo.
		 
	 //M¨¢scara de interrupcoes usadas pelo sistema no modo de regulacao
		 EXTI->IMR &= ~(1<<5);	
		 EXTI->IMR &= ~(1<<7);
		 EXTI->IMR &= ~(1<<11);
		 EXTI->IMR &= ~(1<<12);
		 TIM1->DIER &= 0xFE;   //Depois de entrar no modo de depuracao, desligue temporariamente a interrupcao do temporizador 1
		 
	//P¨¢gina de prompt
   while(1)
  {
		  page_tips:	         //P¨¢gina de prompt	
		   key = click();
			 show_Tips();     //Exibir informacoes: Inicie o modo de inspecao de instalacao de deslocamento angular, opere de acordo com as instrucoes. Pressione o botao do usu¨¢rio para confirmar...
		   HAL_Delay(30);   ////Atrasa o debounce para evitar que seja muito f¨¢cil virar duas p¨¢ginas seguidas ao virar a pr¨®xima p¨¢gina
       if(key==5)    //Se a tecla do usu¨¢rio for pressionada, limpa a tela e executa o pr¨®ximo passo ou se o usu¨¢rio pressiona a tecla de retorno ¨¤ p¨¢gina anterior, sai do ajuste
			 {
				 OLED_Clear();  
				 while(1)
				 {
					 page_0:          //P¨¢gina 0
					 HAL_Delay(30);   //Atrasa o debounce para evitar que seja muito f¨¢cil virar duas p¨¢ginas seguidas ao virar a pr¨®xima p¨¢gina
					 key = click(); ////Detecta chave
					 step_0();     //Exibir informacoes: Por favor, fixe o p¨ºndulo invertido para que ele fique verticalmente para baixo e permaneca estacion¨¢rio. Ap¨®s a conclusao da instalacao, pressione o botao da p¨¢gina para continuar...
					 if(key==11) //Pr¨®xima p¨¢gina
					 {
						 OLED_Clear();
						 while(1)  
						 {
			         page_1:	        //Entra na p¨¢gina 1
           		 step_1();     //Exibir informacoes: Afrouxe os dois parafusos que conectam a haste do p¨ºndulo e o sensor de deslocamento angular e, em seguida, ajuste o botao da haste do p¨ºndulo para garantir
							 HAL_Delay(30);   //Atrasa o debounce para evitar que seja muito f¨¢cil virar duas p¨¢ginas seguidas ao virar a pr¨®xima p¨¢gina
							 key = click();  //Detecta chave
							 if(key==11)   //Pr¨®xima p¨¢gina
							 {
								 OLED_Clear();
								 while(1) 
								 {
									 page_2:            //Entra na p¨¢gina 2
									 step_2();        //Exibir informacoes: Prove que o valor ADC est¨¢ entre 1015 e 1025 quando o p¨ºndulo est¨¢ verticalmente para baixo e, em seguida, aperte a conexao entre o p¨ºndulo e a posicao do canto
									 HAL_Delay(30);    //Atrasa o debounce para evitar que seja muito f¨¢cil virar duas p¨¢ginas seguidas ao virar a pr¨®xima p¨¢gina
									 key = click();  //Detecta chave
									 if(key ==11)    //Pr¨®xima p¨¢gina
									 {
						    		 OLED_Clear();
										 while(1)
										 {
											 page_3:        //Entra na p¨¢gina 3
											 Angle_Balance=Get_Adc_Average(3,15);  //Obt¨¦m o valor ADC na p¨¢gina 3 para ajuste
											 step_3();    //Exibe informacoes: dois parafusos no sensor de mudanca. Ap¨®s o ajuste bem-sucedido, pressione o botao do usu¨¢rio para verificar
											 HAL_Delay(50);   //Atraso no debounce + atraso na exibicao para evitar que o valor ADC seja atualizado muito r¨¢pido e dificultando a distincao
											 key = click();   //Detecta chave
											 if(key==12)   //Pressione a tecla da p¨¢gina anterior e volte para a p¨¢gina 2
											 {
												 OLED_Clear();
												 goto page_2;
											 }																		 
											 else if (key==5)  //A tecla do usu¨¢rio ¨¦ pressionada e entra na fase de verificacao e julgamento do sistema
											 {
												 OLED_Clear();
												 i=0;
												 key = 10;//Ap¨®s entrar nesta etapa, use a vari¨¢vel chave como uma contagem regressiva
												 while(1)
												 {
													 Angle_Balance=Get_Adc_Average(3,15); //Obt¨¦m o valor do ADC
													 OLED_ShowNumber(88,16,key,2,16); //Mostra contagem regressiva
													 check_display(); //Exibe informacoes: Verificando se o ajuste est¨¢ correto...10 Por favor, mantenha o p¨ºndulo parado						 
													 count++; //conta o tempo
													 HAL_Delay(30); //Um pequeno atraso aumentar¨¢ o tempo de detecao
													 if(count<=200) //Durante a contagem <= 200, registre o n¨²mero de vezes que o angulo est¨¢ correto
													 {
															 if(Angle_Balance>=1010&&Angle_Balance<=1030) //Quando a contagem ¨¦ menor que 200, verifica se o angulo est¨¢ correto
															 {
																 i++;
																 if(i>150)  //Enquanto a contagem for menor que 200, o n¨²mero de angulos corretos chega a 150 e o angulo ¨¦ considerado correto.
																 {
																	 OLED_Clear();
																	 while(1)
																	 {
																		 count=0;
																		 success_display(); //Solicita informacoes de sucesso e espera o usu¨¢rio pressionar o botao
																		 if(click()==5) goto begin;		//Quando o usu¨¢rio pressiona o botao, toda a funcao de ajuste ir¨¢ saltar	
																	 }
																 }
															 }
															 if(count%20==1)  //Usado para contagem regressiva de cerca de 8~10s
															 {
																 key--;
															 }
													 }
													 
													 else //Se durante a contagem<=200, o n¨²mero de angulos corretos nao atender aos requisitos, ser¨¢ considerado que o ajuste falhou e retornar¨¢ ¨¤ interface de ajuste para continuar o ajuste.
													 {
														 count=0; //Timer limpo, esta ¨¦ uma vari¨¢vel est¨¢tica
														 OLED_Clear();   //Limpa a tela
														 fail_display(); //Exibe informacoes: Falha na verificacao, reajuste
														 HAL_Delay(2000);  //Atraso informacoes do prompt
														 OLED_Clear();
														 goto page_3;    //Volta para a interface de ajuste e reajusta
													 }														
												 }
											 }
										 }
									 }
									 else if (key ==12) {OLED_Clear();goto page_1; } //O botao da p¨¢gina anterior (X1) ¨¦ pressionado, correspondendo ao salto entre a p¨¢gina 2 e a p¨¢gina 1
								 }				 
							 }
						  else if(key==12) {OLED_Clear();goto page_0;}  //A tecla da p¨¢gina anterior ¨¦ pressionada (X1), correspondendo ao salto entre a p¨¢gina 1 e a p¨¢gina 0
						 }
					 }
            else if (key==12) {OLED_Clear();goto page_tips;}	//A tecla da p¨¢gina anterior ¨¦ pressionada (X1), correspondente ¨¤ p¨¢gina 0 para voltar ¨¤ p¨¢gina de prompt
				 }		 			 				 
			 }
			 else if (key==12) {goto begin;}  //corresponde ¨¤ escolha do usu¨¢rio na p¨¢gina inicial
		 }			 	 
	 }
}
