  /**************************************************************************
Autor: Balance Car Home
Minha loja Taobao: http://shop114407458.taobao.com/
**************************************************************************/

#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
 
 
extern unsigned char DataScope_OutPut_Buffer[42];	   //Buffer de dados do quadro a ser enviado


void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // Grave os dados do canal na ¨¢rea do buffer de dados do quadro a ser enviado

unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // Enviar funcao de geracao de dados de quadro
 
 
#endif 



