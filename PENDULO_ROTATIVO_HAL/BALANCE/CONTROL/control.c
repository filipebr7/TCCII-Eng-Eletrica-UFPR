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
#include "control.h"		
int Balance_Pwm,Position_Pwm; //angulo alvo PWM, posicao alvo PWM
u8 Position_Target;//Tempo usado para marcar o controle de posicao
u8 step1_flag=0;//A primeira etapa do balanco autom¨¢tico ¨¦ usada para determinar se a primeira etapa do balanco ocorre antes do balanco autom¨¢tico.
u16 step1_target=10000;//A posicao de oscilacao autom¨¢tica ¨¦ usada para determinar a amplitude de oscilacao da barra de oscilacao durante a oscilacao autom¨¢tica.
u16 success_count=0; //Registre o n¨²mero de vezes que o p¨ºndulo est¨¢ na posicao de equilibrio para determinar se ele pode entrar no estado de equilibrio.
u8 hand_start=0,auto_start=0; //Julgamento de oscilacao manual e oscilacao autom¨¢tica, usado para determinar se o estado atual est¨¢ no modo de oscilacao autom¨¢tica ou no modo de oscilacao manual
u8 fail_count=0;//O n¨²mero de falhas de oscilacao autom¨¢tica ¨¦ usado para determinar quando comecar a determinar se a oscilacao foi bem-sucedida ou nao.
u8 hand_flag=0; //Sinalizador de oscilacao manual, usado para determinar se o valor do codificador deve ser inserido como valor alvo para manter o equil¨ªbrio
int Last_Encoder=10000;//Salvar o valor do ¨²ltimo codificador.¨¦ utilizado para salvar o valor do ¨²ltimo codificador para facilitar o c¨¢lculo da diferenca entre os dois valores do codificador.
int abnormal_motor;//A diferenca entre os codificadores dianteiro e traseiro ¨¦ usada para determinar se o motor deve ser desligado.

//Vari¨¢veis relacionadas ao PID
float Last_Bias;    
float Last_Position;
/**************************************************************************
Funcao da funcao: todos os c¨®digos de controle estao aqui
           Interrupcao cronometrada de 5ms controlada por TIM1
**************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{    
	if(htim==(&htim1)) //Verifique se a interrupcao TIM especificada ocorre: Fonte de interrupcao TIM //interrupcao programada de 5ms
	{                                       //===Limpe o bit do sinalizador de interrupcao do temporizador 1                    
	     if(delay_flag==1)
			 {
				 if(++delay_50==10)	 delay_50=0,delay_flag=0;          //===Fornece um atraso preciso de 50 ms para a funcao principal  10*5ms = 50ms
			 }		
    	Encoder=Read_Encoder(4);             	                   //===Atualizar informacoes de localizacao do codificador 
      Angle_Balance=Get_Adc_Average(3,15);                     //===atualizar posicao	
			 
		//A oscilacao autom¨¢tica s¨® pode ser executada uma vez ap¨®s ligar, enquanto a oscilacao manual pode ser executada a qualquer momento.	  
     if(Flag_Stop==0&&auto_start==1&&hand_start==0)
		 {
			 Auto_run(); 	//Balancar automaticamente para cima, s¨® pode balancar automaticamente uma vez depois de ligar a m¨¢quina	 
		 }
		 
		 else if (Flag_Stop==0&&hand_start==1&&auto_start==0)//Modo de balanco manual
		 {
			 if(hand_flag==0) hand_flag=1,Position_Zero=Read_Encoder(4); // Antes de iniciar o swing manualmente, obtenha a posicao atual do codificador 
																																	 // e utilize-o como ponto de refer¨ºncia de equil¨ªbrio para iniciar o swing, 
																																	 // para que voc¨º possa iniciar manualmente o swing em qualquer posicao e a qualquer momento.

			 Balance_Pwm =Balance(Angle_Balance);                                          //===Controle PD de angulo	
			 if(++Position_Target>4)  Position_Pwm=Position(Encoder),Position_Target=0;    //===Controle PD de posicao. Controle de posicao uma vez a cada 25ms
			  
			 if(Turn_Off(Voltage)) Flag_Stop=1;                                             //===Protecao contra baixa tensao e inclinacao excessiva
			 else
			  Xianfu_Pwm(),                         //===A limitacao PWM evita a instabilidade do sistema causada por um ciclo de trabalho de 100%
	    	Moto=Balance_Pwm-Position_Pwm,       //===Calcular o PWM final do motor
		    Set_Pwm(Moto);                      //===Atribuir valor ao registro PWM
		 }
		 
		 else if (Flag_Stop==1)
		 Set_Pwm (0),hand_flag=0;
		
    //Protecao de motor voando, se o motor voando for detectado, defina o sinalizador para 1 e o sistema ir¨¢ parar.		 
		 abnormal_motor=Encoder-Last_Encoder; //Variacao do codificador de registro
		 Last_Encoder=Encoder; //Salve o ¨²ltimo valor do codificador
		 if(myabs(abnormal_motor)>15)  //Se o valor absoluto do desvio for superior a 15, significa que o motor gira muito r¨¢pido neste momento, o que ¨¦ um estado anormal e o motor para.
		 {
			  Flag_Stop=1;
		 }
			 
	  	Led_Flash(100);                       //===LED piscando indica operacao normal do sistema 
	    Voltage=Get_battery_volt();           //===Obtenha a voltagem da bateria	      
			Key();                                //===Digitalizar alteracoes de chave
	}       	  
} 

/**************************************************************************
Funcao: Controle PD de inclinacao
Parametro de entrada: angulo
Valor de retorno: controle de angulo de inclinacao PWM
Autor: Balance Car Home
**************************************************************************/
int Balance(float Angle)
{  
   float Bias;                       //Desvio de inclinacao
	 static float D_Bias;    //Vari¨¢veis relacionadas ao PID
	 int balance;                      //Valor de retorno PWM 
	 Bias=Angle-ZHONGZHI;              //Encontrar o angulo mediano de equil¨ªbrio est¨¢ relacionado ao maquin¨¢rio
	 D_Bias=Bias-Last_Bias;            //Encontre o diferencial do desvio e execute o controle diferencial
	 balance=-Balance_KP*Bias-D_Bias*Balance_KD;   //===C¨¢lculo do controle PWM PD do motor para controle de inclinacao
   Last_Bias=Bias;                   //Mantenha o ¨²ltimo desvio
	 return balance;
}

/**************************************************************************
Funcao: controle de posicao PD
Parametros de entrada: codificador
Valor de retorno: controle de posicao PWM
Autor: Balance Car Home
**************************************************************************/
int Position(int Encoder)
{  
   static float Position_PWM,Position_Bias,Position_Differential;
	 static float Position_Least;
  	Position_Least =Encoder-Position_Zero;             //===
    Position_Bias *=0.8;		   
    Position_Bias += Position_Least*0.2;	             //===Filtro passa-baixo de primeira ordem  
	  Position_Differential=Position_Bias-Last_Position;
	  Last_Position=Position_Bias;
		Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===controle de velocidade		
	  return Position_PWM;
}

/**************************************************************************
Funcao: atribuir valor ao registro PWM
Parametros de entrada: PWM
Valor de retorno: Nenhum
**************************************************************************/
void Set_Pwm(int moto)
{
    	if(moto<0)			BIN2=1,			BIN1=0;
			else 	          BIN2=0,			BIN1=1;
			PWMB=myabs(moto);
}

/**************************************************************************
Funcao: Limitar atribuicao PWM
Parametros de entrada: nenhum
Valor de retorno: Nenhum
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===A faixa completa do PWM ¨¦ 7200 e limitada a 6900
	  if(Moto<-Amplitude) Moto=-Amplitude;	
		if(Moto>Amplitude)  Moto=Amplitude;		
}
/**************************************************************************
Funcao: pressione o botao para modificar a posicao do p¨ºndulo de controle
Parametros de entrada: nenhum
Valor de retorno: Nenhum
**************************************************************************/
void Key(void)
{	
	// O produto utiliza um motor sensor Hall de 13 fios com uma taxa de reducao de 20. O programa usa 4 vezes a frequ¨ºncia. O valor no codificador para uma revolucao do motor ¨¦ 13*4*20 = 1040 
	int position=2080; //Posicao alvo: A posicao original do motor ¨¦ 10.000. Uma revolucao ¨¦ 1.040. Est¨¢ relacionada ¨¤ precisao do codificador. O padrao ¨¦ que o eixo Z do p¨ºndulo gire uma vez e produza 1.040 bordas de transicao.
	static int tmp,flag,count;
	
		
	if(Flag_Stop==1) //Se nao comecar a se equilibrar, faca um teste
	{
		if(Angle_Balance>(ZHONGZHI-500)&&Angle_Balance<(ZHONGZHI+500))  //se estiver em posicao de equil¨ªbrio
	{
		hand_start=1;//Balanco manual
		auto_start=0;//Desligue o sinal de balanco autom¨¢tico
	}
	  else auto_start=1,hand_start=0; //  Se nao estiver na posicao de equil¨ªbrio, comecar¨¢ a balancar automaticamente.
	}

	tmp=click_N_Double(100); 
	if(tmp==1)flag=1;//++
  if(tmp==2)flag=2;//--
	
	if(flag==1) //O p¨ºndulo se move no sentido hor¨¢rio
	{
		Position_Zero++;
		count++;	
		if(count==position) 	flag=0,count=0;   //Ou seja, a posicao alvo +2080
	}	
		if(flag==2) //O p¨ºndulo se move no sentido anti-hor¨¢rio
	{
		Position_Zero--;
		count++;	
		if(count==position) 	flag=0,count=0;//Ou seja, a posicao alvo -2080
	}


}

/**************************************************************************
Funcao: desligue o motor de forma anormal
Parametros de entrada: tensao
Valor de retorno: 1: Anormal 0: Normal
**************************************************************************/
u8 Turn_Off(int voltage)
{
	    u8 temp; 
			if(1==Flag_Stop||Angle_Balance<(ZHONGZHI-800)||Angle_Balance>(ZHONGZHI+800)||voltage<1110) //A tensao da bateria est¨¢ muito baixa, desligue o motor
			{	      
      Flag_Stop=1;				
      temp=1;                                            
			BIN1=0;                                            
			BIN2=0;
      }
			else
      temp=0;
      return temp;			
}
	

/**************************************************************************
Funcao da funcao: funcao de valor absoluto
Parametro de entrada: int
Valor de retorno: unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}


/**************************************************************************
Funcao: controlador PID posicional
Parametros de entrada: informacoes de posicao de medicao do codificador, posicao alvo
Valor de retorno: Motor PWM
De acordo com a f¨®rmula PID discreto posicional
pwm=Kp*e(k)+Ki*¡Æe(k)+Kd[e(k)-e(k-1)]
e(k) representa esse desvio
e(k-1) representa o ¨²ltimo desvio
¡Æe(k) representa a soma cumulativa de e(k) e desvios anteriores, onde k ¨¦ 1, 2,,k;
pwm representa sa¨ªda
**************************************************************************/
int Position_PID (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                   //Calcular desvio
	 Integral_bias+=Bias;	                                  //Encontre a integral do desvio
	 Pwm=Position_KP_1*Bias+0*Integral_bias+Position_KD_1*(Bias-Last_Bias);       //Controlador PID posicional
	 Last_Bias=Bias;                                        //Salve o ¨²ltimo desvio 
	 return Pwm;                                            //sa¨ªda incremental
}


/**************************************************************************
Funcao: balanco autom¨¢tico
Parametros de entrada: nenhum
Valor de retorno: Nenhum
**************************************************************************/
void Auto_run(void)
{
	static u16 try_count=0;  //Usado para ajudar a avaliar se o swing foi bem-sucedido
	static u16 stop_count=0; //Registre o momento em que o motor para
	
	if(step1_flag==0)  //Jogue a haste apenas uma vez
	{
		fail_count++; //Registre o n¨²mero de vezes que a funcao foi inserida. Ela ser¨¢ inserida duas vezes. Se for inserida duas vezes e o angulo nao estiver na posicao de equil¨ªbrio, significa que o balanco falhou.
		Moto=Position_PID(Encoder,step1_target);  //A primeira vez que step1_target=10000 obt¨¦m a posicao na inicializacao e conta como 10000
	  step1_target=10341; //Atribua um valor. Na pr¨®xima vez que voc¨º entrar na funcao, acione a alavanca e registre os valores dispon¨ªveis: 10334 10337
		Xianfu_Pwm();    //recorte PWM
		
		/*  Leia a posicao do codificador para julgamento. Se nao estiver no valor inicial, far¨¢ com que o motor voe.  */
		if(fail_count==1&&(Encoder<9980||Encoder>10020))  //¨¦ necess¨¢rio avaliar o valor do codificador ao balancar a haste pela primeira vez. Se o valor do codificador se desviar muito do valor inicial, o motor voar¨¢. Portanto, ¨¦ proibido comecar a balancar automaticamente quando o desvio ¨¦ muito grande.
		{
			fail_count=0,step1_target=10000,Flag_Stop=1;   //Leia o valor do encoder. Caso nao esteja na posicao adequada, todos os dados modificados retornarao ao valor original. O flag de partida do motor ser¨¢ colocado em 1 e aguardar¨¢ a pr¨®xima oscilacao autom¨¢tica.
		}
	  else Set_Pwm(Moto);	 //balanco
		
	 }

if(++try_count&&(Angle_Balance>ZHONGZHI-350&&Angle_Balance<ZHONGZHI+350))   //  Se o balanco atingir a posicao de equil¨ªbrio
 {
	 try_count=0;//Se voc¨º entrar nesta funcao e falhar novamente, ¨¦ uma situacao de falha de oscilacao 2.
//	 if(success_count==0)  Balance_KP=60,Balance_KD=350,Position_KP=35,Position_KD=750; //Insira os parametros pid necess¨¢rios para oscilacao autom¨¢tica
	 if(success_count==0)  Balance_KP=30,Balance_KD=150,Position_KP=15,Position_KD=500; //Insira os parametros pid necess¨¢rios para oscilacao autom¨¢tica
	 success_count++; //Registre o n¨²mero de vezes na posicao de equil¨ªbrio
	 
	 if(success_count>10) //se mais de 10 vezes
	 {
		 step1_flag=1; //Marque o swing como bem-sucedido e nao entre na funcao de swing novamente.
		 //Assim que o balanco for bem-sucedido, voc¨º poder¨¢ realizar o controle pid na haste do balanco.
		 Balance_Pwm =Balance(Angle_Balance); 
		 if(success_count>109) //Ap¨®s um curto per¨ªodo de tempo ap¨®s o in¨ªcio do balanco, o p¨ºndulo precisa reduzir a frequ¨ºncia do controle de posicao e ajustar o parametro pid de volta ao seu valor est¨¢vel original.
		 {
			 if(success_count==110)  Balance_KP=50,Balance_KD=244,Position_KP=25,Position_KD=600;//Ap¨®s iniciar o swing por um per¨ªodo de tempo, deixe o parametro pid retornar ao valor do swing manual.
			 success_count=115; //Bloqueie o valor Success_count para evitar loops de estouro
       if(++Position_Target>4)  Position_Pwm=Position(Encoder),Position_Target=0; //===Controle PD de posicao: execute o controle de posicao a cada 25ms
		 }
     else 	Position_Zero=step1_target+0, Position_Pwm=Position(Encoder);  //Quando o balanco apenas comeca, o controle de posicao precisa ser adicionado imediatamente para manter o equil¨ªbrio. A posicao alvo do equil¨ªbrio ¨¦ mais distante do que a distancia da haste do balanco, o que contribui para a estabilidade do equil¨ªbrio e para a compensacao de ultrapassagem.

		stop_count=0; //Cronometragem quando o motor para de girar. Se estiver em um estado equilibrado, a cronometragem ser¨¢ redefinida para evitar erros de julgamento devido ao ac¨²mulo de tempo. 
		Xianfu_Pwm(); //recorte pwm
		Moto=Balance_Pwm-Position_Pwm;  //Dados de fusao 
	  Set_Pwm(Moto);  //enviar pwm
	 }
 }
	
 //Existem tr¨ºs situacoes em que o motor para: 
 //1. Ele nao entra na posicao de equil¨ªbrio quando comeca a oscilar. 
 //2. Ele entra na posicao de equil¨ªbrio depois de comecar a oscilar, mas falha novamente. 
 //3. A haste de oscilacao ¨¦ trazida artificialmente para a posicao inicial.
 if((!(Angle_Balance>ZHONGZHI-500&&Angle_Balance<ZHONGZHI+500))&&fail_count>2&&try_count==0) //Caso 2: Entrou na posicao de equil¨ªbrio ap¨®s balancar, mas falhou
 {
	step1_flag=1; //Sinal da haste de balanco, ¨¦ proibido entrar na haste de balanco
	Set_Pwm(0);	 //Motor parado
	 
	 //Depois que o motor parar, atrase um pouco e entao defina o sinalizador de habilitacao do motor para 1. 
	 //O objetivo ¨¦ evitar erros de julgamento e fazer com que o p¨ºndulo que originalmente pode manter o equil¨ªbrio perca o controle do motor.
	 stop_count++; //Comece a cronometrar quando o motor parar
	 if(stop_count>200) stop_count=0,Flag_Stop=1; //Posicao da flag 1
 }
 
 //angulo de inclinacao, protecao de tensao
 if(((!(Angle_Balance>ZHONGZHI-500&&Angle_Balance<ZHONGZHI+500))&&fail_count>2&&try_count>120)||Voltage<1110)  //Casos 1 e 3: A posicao de equil¨ªbrio nao foi alcancada ap¨®s o in¨ªcio do swing e o swing falhou (desde o in¨ªcio do swing at¨¦ a confirmacao do swing correto ou a confirmacao da falha do swing, try_count ++ exceder¨¢ 100)
 {
	try_count=150; //Bloqueie o valor try_count para evitar loops de estouro
	step1_flag=1;  //Sinal da haste de balanco, ¨¦ proibido entrar na haste de balanco
	Set_Pwm(0);	 //Motor parado
	 
	 //Depois que o motor parar, atrase um pouco e entao defina o sinalizador de habilitacao do motor para 1. O objetivo ¨¦ evitar erros de julgamento e fazer com que o p¨ºndulo que originalmente pode manter o equil¨ªbrio perca o controle do motor.
	 stop_count++; //Comece a cronometrar quando o motor parar
	 if(stop_count>200) stop_count=0,Flag_Stop=1; //Posicao da flag 1
 }
 
 
}

