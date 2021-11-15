#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_beep.h"



/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucId, ucLen;
	uint8_t ucLed1Status = 0, ucLed2Status = 0, ucLed3Status = 0, ucBuzzerStatus = 0;

	char cStr [ 100 ] = { 0 }, cCh;

  char * pCh, * pCh1;

	DHT11_Data_TypeDef DHT11_Data;
	

  printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	//测试AT启动
	ESP8266_AT_Test ();
	//模式选择
	ESP8266_Net_Mode_Choose ( AP );
	//设置ESP8266 SoftAP的IP地址
  while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );
  //设置WIFI名字 加密方式 密码
  while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd, macUser_ESP8266_BulitApEcn ) );	
	//设置为多连接模式  想切换为单连接，必须关闭服务器? (AT+CIPSERVER=0)，服务器仅支持多连接
	ESP8266_Enable_MultipleId ( ENABLE );
	//开启服务器 并设置端口
	while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );
	//读取本地IP地址
	ESP8266_Inquire_ApIp ( cStr, 20 );
	printf ( "\r\n本模块WIFI为%s，密码开放\r\nAP IP 为：%s，开启的端口为：%s\r\n手机网络助手连接该 IP 和端口，最多可连接5个客户端\r\n",
           macUser_ESP8266_BulitApSsid, cStr, macUser_ESP8266_TcpServer_Port );
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( 1 )
	{		
		if ( strEsp8266_Fram_Record .InfBit .FramFinishFlag )
		{
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';//中断接收时预留的一个位置给结束字符
			//printf ( "\r\n%s\r\n", strEsp8266_Fram_Record .Data_RX_BUF );//
			if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CMD_LED_" ) ) != 0 ) 
			{
				/*strstr返回的是字符串首地址*/
				//printf("\r\n%s\r\n",pCh);
				cCh = * ( pCh + 8 ); 
				//printf("\r\n%x\r\n",cCh);
				switch ( cCh )
				{
					//'1'对应的ASSIC码为49  十六进制为31
					case '1':
						cCh = * ( pCh + 10 );
					  switch ( cCh )
					  {
							case '0':
								macLED1_OFF ();
							  ucLed1Status = 0;
						    break;
							case '1':
								macLED1_ON ();
							  ucLed1Status = 1;
						    break;
							default :
								break;
						}
						break;
						
					case '2':
						cCh = * ( pCh + 10 );
					  switch ( cCh )
					  {
							case '0':
								macLED2_OFF ();
							  ucLed2Status = 0;
						    break;
							case '1':
								macLED2_ON ();
							  ucLed2Status = 1;
						    break;
							default :
								break;
						}
						break;

					case '3':
						cCh = * ( pCh + 10 );
					  switch ( cCh )
					  {
							case '0':
								macLED3_OFF ();
							  ucLed3Status = 0;
						    break;
							case '1':
								macLED3_ON ();
							  ucLed3Status = 1;
						    break;
							default :
								break;
						}
						break;
						
				  default :
            break;					
						
				}
				
				sprintf ( cStr, "CMD_LED_%d_%d_%d_ENDLED_END", ucLed1Status, ucLed2Status, ucLed3Status );
				//printf("\r\n%s\r\n",cStr);
			}
			
			else if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CMD_BUZZER_" ) ) != 0 ) 
			{
				cCh = * ( pCh + 11 );
				
				switch ( cCh )
				{
					case '0':
						macBEEP_OFF ();
					  ucBuzzerStatus = 0;
						break;
					case '1':
						macBEEP_ON ();
					  ucBuzzerStatus = 1;
						break;
					default:
						break;
				}
				
				sprintf ( cStr, "CMD_BUZZER_%d_ENDBUZZER_END", ucBuzzerStatus );
				
			}
				
			else if ( ( ( pCh  = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CMD_UART_" ) ) != 0 ) && 
				        ( ( pCh1 = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "_ENDUART_END" ) )  != 0 ) ) 
			{
//				printf("\r\nData_RX_BUF：%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);
//				printf("\r\npCh：%s\r\n",pCh);
//				printf("\r\npCh1：%s\r\n",pCh1);
				if ( pCh < pCh1)
				{
					/*pCh1 pCh指向每个字符的首地址相减 
					得到两者之间的地址差 字符串右边是高地址
					char类型一个地址存放一个字节
					*/
					ucLen = pCh1 - pCh + 12;
//					printf("\r\npCh1：%d\r\n",pCh1);
//					printf("\r\npCh：%d\r\n",pCh);
//					printf("\r\npCh1 - pCh：%d\r\n",pCh1 - pCh);
//					printf("\r\nucLen：%d\r\n",ucLen);
					memcpy ( cStr, pCh, ucLen );
					//printf("\r\n%s\r\n",cStr);
					cStr [ ucLen ] = '\0';
					//printf("\r\ncStr：%s\r\n",cStr);
				}
			}

			else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CMD_READ_ALL_END" ) != 0 ) 
			{
				printf("\r\nData_RX_BUF：%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);
				DHT11_Read_TempAndHumidity ( & DHT11_Data );
				sprintf ( cStr, "CMD_LED_%d_%d_%d_ENDLED_DHT11_%d.%d_%d.%d_ENDDHT11_BUZZER_%d_ENDBUZZER_END", 
									ucLed1Status, ucLed2Status, ucLed3Status, DHT11_Data .temp_int, 
			            DHT11_Data .temp_deci, DHT11_Data .humi_int, DHT11_Data .humi_deci,
                  ucBuzzerStatus );
			}
			
				
			if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) 
			{
				//ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				printf("\r\nucId：%d\r\n",ucId);
				ESP8266_SendString ( DISABLE, cStr, strlen ( cStr ), ( ENUM_ID_NO_TypeDef ) ucId );
				//printf("\r\nucId：%d\r\n",ucId);
			}
			
		  strEsp8266_Fram_Record .InfBit .FramLength = 0;
	    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
			
		}
		
	}
	
		
}


