#include <stdio.h>
#include "loopback.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "modbus.h"
#include "ModProt.h"
#include "stm32f4xx_hal.h"
#include "cli.h"


#ifdef _USE_HW_W5500
//extern uint8_t test_1,test_2, test_3;
uint8_t gEthSendBuf[DATA_BUF_SIZE];
uint8_t bSockStatus;
uint8_t echo_Flag;
uint8_t *pEcho;
extern BYTE    bTxTcpFrameSize;
extern BYTE    bTxTcpFrameSend;
extern WORD wEthErrCnt;
extern uint8_t pData[5], UartTx1[200];

#ifdef _USE_HW_CLI
 void cliW5500(cli_args_t *args);
#endif


#if LOOPBACK_MODE == LOOPBACK_MAIN_NOBLCOK
#if 0
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret;
   uint16_t size = 0, sentsize=0;

#ifdef _LOOPBACK_DEBUG_
   uint8_t destip[4];
   uint16_t destport;
#endif

   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)
         {
#ifdef _LOOPBACK_DEBUG_
			getSn_DIPR(sn, destip);
			destport = getSn_DPORT(sn);

			printf("%d:Connected - %d.%d.%d.%d : %d\r\n",sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
			setSn_IR(sn,Sn_IR_CON);
         }
		 if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
         {
			if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
			ret = recv(sn, buf, size);

			if(ret <= 0) return ret;      // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
			size = (uint16_t) ret;
			sentsize = 0;

			while(size != sentsize)
			{
				ret = send(sn, buf+sentsize, size-sentsize);
				if(ret < 0)
				{
					close(sn);
					return ret;
				}
				sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
			}
         }
         break;
      case SOCK_CLOSE_WAIT :
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:CloseWait\r\n",sn);
#endif
         if((ret = disconnect(sn)) != SOCK_OK) return ret;
#ifdef _LOOPBACK_DEBUG_
         printf("%d:Socket Closed\r\n", sn);
#endif
         break;
      case SOCK_INIT :
#ifdef _LOOPBACK_DEBUG_
    	 printf("%d:Listen, TCP server loopback, port [%d]\r\n", sn, port);
#endif
         if( (ret = listen(sn)) != SOCK_OK) return ret;
         break;
      case SOCK_CLOSED:
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:TCP server loopback start\r\n",sn);
#endif
         if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret;
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:Socket opened\r\n",sn);
#endif
         break;
      default:
         break;
   }
   return 1;
}
#endif


int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret;
   uint16_t size = 0, sentsize=0;
	uint16_t buf_idx = 0;
	uint16_t buf_len = 0;
	uint16_t echo_len = 0;

#ifdef _LOOPBACK_DEBUG_
   uint8_t destip[4];
   uint16_t destport;

#endif

   bSockStatus = getSn_SR(sn);


   /*@ : Keep alive packet 을 송신하여 connection 이 유효한지 확인한다.
            만약 상대방이 더 이상 응답이 없어서 connection 이 유효하지 않은 경우 connection 을 종료한다. Timeout Interrupt 가 발생한다.
	*/
   setSn_CR(sn,Sn_CR_SEND_KEEP);

   switch(bSockStatus)
   {
      case SOCK_ESTABLISHED :

    	if(wEthErrCnt >= 10)
    	{ // 3sec동안 통신이 없는 경우
        	wEthErrCnt = 0;
        	socket(sn, Sn_MR_TCP, port, 0x00);  // wiz550io SOCK_CLOSED
    	}

		  if(getSn_IR(sn) & Sn_IR_CON)
		  {
#ifdef _LOOPBACK_DEBUG_
			getSn_DIPR(sn, destip);
			destport = getSn_DPORT(sn);
			//sprintf(UartTx1, "%d:Connected - %d.%d.%d.%d : %d\r\n",sn, destip[0], destip[1], destip[2], destip[3], destport);
		   // Userprintf(UartTx1);

#endif
				setSn_IR(sn,Sn_IR_CON);
		  }

		 if((size = getSn_RX_RSR(sn)) > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
		 {
			 wEthErrCnt = 0;
			 bTxTcpFrameSend = 0;
			 bTxTcpFrameSize = 0;

			if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;

			buf_idx = 0;

			while(size > buf_idx)
			{
				buf_len = size - buf_idx;

				if(buf_len > 20)
				{
					buf_len = 20;
				}

				if(echo_Flag == true) // echo_Flag :1 loopback // 0: protocol //
				{

					ret = recv(sn, buf+buf_idx, buf_len);

					bTxTcpFrameSend = TRUE;
					bTxTcpFrameSize = size;
					echo_len = size;
				}
				else
				{
					ret = recv(sn, buf+buf_idx, buf_len); //TCP/IP
				}

				if(ret <= 0) return ret; // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.
				buf_idx += buf_len;
			}

		}

		 if(echo_Flag == false) // echo_Flag :1 loopback // 0: protocol //
		 {
			 RTUtcpLoop(buf, size, gEthSendBuf);
   	 }


		if((bTxTcpFrameSend == 1) && (bTxTcpFrameSize > 0))
		{
			sentsize = 0;
			size = bTxTcpFrameSize;
			while(size != sentsize)
			{
				buf_len = size - sentsize;

				if(buf_len  > 20) buf_len = 20;

				if(echo_Flag == true) // echo_Flag :1 loopback // 0: protocol //
				{
					ret = send(sn, buf+sentsize, buf_len); //loopback
				}
				else
				{
					ret = send(sn, gEthSendBuf+sentsize, buf_len); //Modbus
				}

				if(ret < 0)
				{
					close(sn);
					return ret;
				}
				sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
			}
			bTxTcpFrameSend = 0;
			bTxTcpFrameSize = 0;
		}

         break;
      case SOCK_CLOSE_WAIT :
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:CloseWait\r\n",sn);


#endif
         if((ret = disconnect(sn)) != SOCK_OK) return ret;
#ifdef _LOOPBACK_DEBUG_
//         printf("%d:Socket Closed\r\n", sn);

#endif
         break;
      case SOCK_INIT :
#ifdef _LOOPBACK_DEBUG_
//    	 printf("%d:Listen, TCP server loopback, port [%d]\r\n", sn, port);


#endif
         if( (ret = listen(sn)) != SOCK_OK) return ret;
         break;
      case SOCK_CLOSED:
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:TCP server loopback start\r\n",sn);

#endif
         if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret;
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:Socket opened\r\n",sn);


#endif
         break;
      default:
         break;
   }
   return 1;
}


int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport)
{
   int32_t ret; // return value for SOCK_ERRORs
   uint16_t size = 0, sentsize=0;

   // Destination (TCP Server) IP info (will be connected)
   // >> loopback_tcpc() function parameter
   // >> Ex)
   //	uint8_t destip[4] = 	{192, 168, 0, 214};
   //	uint16_t destport = 	5000;

   // Port number for TCP client (will be increased)
   static uint16_t any_port = 	50000;

   // Socket Status Transitions
   // Check the W5500 Socket n status register (Sn_SR, The 'Sn_SR' controlled by Sn_CR command or Packet send/recv status)
   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)	// Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
         {
#ifdef _LOOPBACK_DEBUG_
			printf("%d:Connected to - %d.%d.%d.%d : %d\r\n",sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
			setSn_IR(sn, Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
         }

         //////////////////////////////////////////////////////////////////////////////////////////////
         // Data Transaction Parts; Handle the [data receive and send] process
         //////////////////////////////////////////////////////////////////////////////////////////////
		 if((size = getSn_RX_RSR(sn)) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
         {
			if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE; // DATA_BUF_SIZE means user defined buffer size (array)
			ret = recv(sn, buf, size); // Data Receive process (H/W Rx socket buffer -> User's buffer)

			if(ret <= 0) return ret; // If the received data length <= 0, receive failed and process end
			size = (uint16_t) ret;
			sentsize = 0;

			// Data sentsize control
			while(size != sentsize)
			{
				ret = send(sn, buf+sentsize, size-sentsize); // Data send process (User's buffer -> Destination through H/W Tx socket buffer)
				if(ret < 0) // Send Error occurred (sent data length < 0)
				{
					close(sn); // socket close
					return ret;
				}
				sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
			}
         }
		 //////////////////////////////////////////////////////////////////////////////////////////////
         break;

      case SOCK_CLOSE_WAIT :
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:CloseWait\r\n",sn);
#endif

#ifdef _LOOPBACK_DEBUG_
         printf("%d:Socket Closed\r\n", sn);
#endif
         break;

      case SOCK_INIT :
#ifdef _LOOPBACK_DEBUG_
    	 printf("%d:Try to connect to the %d.%d.%d.%d : %d\r\n", sn, destip[0], destip[1], destip[2], destip[3], destport);
#endif
    	 if( (ret = connect(sn, destip, destport)) != SOCK_OK) return ret;	//	Try to TCP connect to the TCP server (destination)
         break;

      case SOCK_CLOSED:
    	  close(sn);
    	  if((ret=socket(sn, Sn_MR_TCP, any_port++, 0x00)) != sn){
         if(any_port == 0xffff) any_port = 50000;
         return ret; // TCP socket open with 'any_port' port number
        } 
#ifdef _LOOPBACK_DEBUG_
    	 //printf("%d:TCP client loopback start\r\n",sn);
         //printf("%d:Socket opened\r\n",sn);
#endif
         break;
      default:
         break;
   }
   return 1;
}


int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t  ret;
   uint16_t size, sentsize;
   uint8_t  destip[4];
   uint16_t destport;

   switch(getSn_SR(sn))
   {
      case SOCK_UDP :
         if((size = getSn_RX_RSR(sn)) > 0)
         {
            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recvfrom(sn, buf, size, destip, (uint16_t*)&destport);
            if(ret <= 0)
            {
#ifdef _LOOPBACK_DEBUG_
               printf("%d: recvfrom error. %ld\r\n",sn,ret);
#endif
               return ret;
            }
            size = (uint16_t) ret;
            sentsize = 0;
            while(sentsize != size)
            {
               ret = sendto(sn, buf+sentsize, size-sentsize, destip, destport);
               if(ret < 0)
               {
#ifdef _LOOPBACK_DEBUG_
                  printf("%d: sendto error. %ld\r\n",sn,ret);
#endif
                  return ret;
               }
               sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
            }
         }
         break;
      case SOCK_CLOSED:
#ifdef _LOOPBACK_DEBUG_
         //printf("%d:UDP loopback start\r\n",sn);
#endif
         if((ret = socket(sn, Sn_MR_UDP, port, 0x00)) != sn)
            return ret;
#ifdef _LOOPBACK_DEBUG_
         printf("%d:Opened, UDP loopback, port [%d]\r\n", sn, port);
#endif
         break;
      default :
         break;
   }
   return 1;
}

#endif


#ifdef _USE_HW_CLI

void cliW5500(cli_args_t *args)
{
  bool ret = false;
  extern wiz_NetInfo gWIZNETINFO;
  uint8_t SocketStatus;
  uint16_t Port_info= 0;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
	  wizchip_getnetinfo(&gWIZNETINFO); // Get W5500 info //
	  Port_info = getSn_PORT(0);  //getSn_PORT

	 // getSn_DPORT
	  cliPrintf("  MAC: 0x%x. 0x%x. 0x%x. 0x%x. 0x%x. 0x%x\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1],gWIZNETINFO.mac[2], gWIZNETINFO.mac[3],gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	  cliPrintf("  IP : %d. %d. %d. %d\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1],gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	  cliPrintf("  SN : %d. %d. %d. %d\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1],gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	  cliPrintf("  GW : %d. %d. %d. %d\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1],gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	  cliPrintf("PORT : %d \n", Port_info);
	  switch(echo_Flag)
	  {
	    case true:
		  cliPrintf("Mode : loopback \n");
		  break;
	    case false:
	      cliPrintf("Mode : TCP/IP \n");
		  break;
	  }

	  ret = true;
  }

  if (args->argc == 2 && args->isStr(1, "Socketstate") == true)
  {
	  SocketStatus =  (uint8_t)args->getData(0);

	  if(SocketStatus > _WIZCHIP_SOCK_NUM_ )
	  {
		  ret = false;

	  }

	     switch(getSn_SR(SocketStatus))
	     {
	   	  case SOCK_CLOSED:
	   		cliPrintf("Sn %d : SOCK_CLOSED\n", SocketStatus);
	   		ret = true;
	   		break;         // closed
	   	  case SOCK_INIT:
	   		cliPrintf("Sn %d : SOCK_INIT\n", SocketStatus);
	   		ret = true;
	   		break; // init state
	   	  case SOCK_LISTEN:
	   		cliPrintf("Sn %d : SOCK_LISTEN\n", SocketStatus);
	   		ret = true;
	   		break; // listen state
	   	  case SOCK_SYNSENT:
	   		cliPrintf("Sn %d : SOCK_SYNSENT\n", SocketStatus);
	   		ret = true;
	   		break; // connection state
	   	  case SOCK_SYNRECV:
	   		cliPrintf("Sn %d : SOCK_SYNRECV\n", SocketStatus);
	   		ret = true;
	   		break; // connection state
	   	  case SOCK_ESTABLISHED:
	   		cliPrintf("Sn %d : SOCK_ESTABLISHED\n", SocketStatus);
	   		ret = true;
	   		break; // success to connect
	   	  case SOCK_FIN_WAIT:
	   		cliPrintf("Sn %d : SOCK_FIN_WAIT\n", SocketStatus);
	   		ret = true;
	   		break; // closing state
	   	  case SOCK_CLOSING:
	   		cliPrintf("Sn %d : SOCK_CLOSING\n", SocketStatus);
	   		ret = true;
	   		break; // closing state
	   	  case SOCK_TIME_WAIT:
	   		cliPrintf("Sn %d : SOCK_TIME_WAIT\n", SocketStatus);
	   		ret = true;
	   		break; // closing state
	   	  case SOCK_CLOSE_WAIT:
	   		cliPrintf("Sn %d : SOCK_CLOSE_WAIT\n", SocketStatus);
	   		ret = true;
	   		break; // closing state
	   	  case SOCK_LAST_ACK:
	   		cliPrintf("Sn %d : SOCK_LAST_ACK\n", SocketStatus);
	   		ret = true;
	   		break;

	      }
  }

  if (args->argc == 1 && args->isStr(0, "reset") == true)
  {
 	 wizchip_sw_reset();
 	 cliPrintf("W5500 Reset OK\n");
 	 ret = true;
  }


  if (args->argc == 2 && args->isStr(0, "test") == true)
  {
  	uint8_t value = 0;

  	value = (uint8_t) args->getData(1);

  	if (value != 0)
  	{
  		echo_Flag = true; //loopback
  		cliPrintf("W5500 looback mode \n");
  	}
  	else
  	{
  		echo_Flag = false; //Modbus protocol
  		cliPrintf("W5500 Modbus mode \n");
  	}

  	ret = true;
  }





  if (ret != true)
  {
	  cliPrintf("W5500 info\n");
	  cliPrintf("W5500 ch[0~7] Socketstate \n");
	  cliPrintf("W5500 reset\n");
	  cliPrintf("W5500 test 0/1");  // loopback test//

	  echo_Flag = false; //For Mode : tcp/ip
  }

// }//Time//

}

#endif

#endif
