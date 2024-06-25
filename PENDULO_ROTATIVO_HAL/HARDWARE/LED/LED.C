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
Funcao: LED piscando
Parametros de entrada: frequencia intermitente
Valor de retorno: Nenhum
**************************************************************************/
void Led_Flash(u16 time)
{
	 static int temp;
	 if(0==time) LED=0;
	 else 	if(++temp==time)	LED=~LED,temp=0;
}
