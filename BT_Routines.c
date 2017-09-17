//  Configure BlueTooth-Click module

#include <stdint.h>
#include <stdbool.h>

#include "BT.h"

#include "mcc_generated_files/mcc.h"



uint8_t txt[16];
bool DataReady;
bool CMD_mode;
uint8_t BT_state;
bool response_rcvd;
enum BT_RESPONSES_T responseID, response = 0;

void BT_Process (uint8_t ch)
{
   if (CMD_mode == true)
   {
      /* The responses expected from the EasyBT module:
      CMD
      AOK
      AOK
      AOK
      AOK
      AOK
      END
      SlaveCONNECTmikroE
      EasyBlueTooth
      mikroE ...
      EasyBlueTooth*/

      // Process reception through state machine
      // We are parsing CMD<cr><lf>, AOK<cr><lf>, CONN<cr> and END<cr><lf> responses
      switch (BT_state)
      {
         case 0:
         {
            response = 0; // Clear response
            if (ch == 'C') // We have 'C', it could be CMD<cr><lf>  or CONN
            {
               BT_state = 1; // Expecting 'M' or 'N'
            }
            if (ch == 'A') // We have 'A', it could be AOK<cr><lf>
            {
               BT_state = 11; // expecting 'O'
            }
            if (ch == 'E') // We have 'E', it could be END<cr><lf>
            {
               BT_state = 31; // expecting 'N'
            }
            break; // ...
         }

         case 1:
         {
            if (ch == 'M')
            {
               BT_state = 2;
            }
            else if (ch == 'O')
            {
               BT_state = 22;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 2:
         {
            if (ch == 'D')
            {
               response = BT_CMD; // CMD
               BT_state = 40;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 11:
         {
            if (ch == 'O')
            {
               BT_state = 12;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 12:
         {
            if (ch == 'K')
            {
               response = BT_AOK; // AOK
               BT_state = 40;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 22:
         {
            if (ch == 'N')
            {
               BT_state = 23;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 23:
         {
            if (ch == 'N')
            {
               response = BT_CONN; // SlaveCONNECTmikroE
               response_rcvd = true;
               responseID = response;
            }
            BT_state = 0;
            break;
         }

         case 31:
         {
            if (ch == 'N')
            {
               BT_state = 32;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 32:
         {
            if (ch == 'D')
            {
               response = BT_END; // END
               BT_state = 40;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 40:
         {
            if (ch == 13)
            {
               BT_state = 41;
            }
            else
            {
               BT_state = 0;
            }
            break;
         }

         case 41:
         {
            if (ch == 10)
            {
               response_rcvd = true;
               responseID = response;
            }
            BT_state = 0;
            break;
         }

         default:
         {
            BT_state = 0;
            break;
         }
      }
   }
   else
   {
      if (ch == 13)
      {
         //txt [i] = 0; // Putting 0 at the end of the string
         DataReady = true; // Data is received
      }
      else
      {
         //txt [i] = ch; // Moving the data received from UART to string txt[]
         //i++; // Increment counter
      }
   }
}


// Get BlueTooth response, if there is any

enum BT_RESPONSES_T BT_Get_Response (void)
{
   if (response_rcvd == true)
   {
      response_rcvd = false;
      return responseID;
   }
   else
   {
      return BT_NONE;
   }
}

void BT_Configure (void)
{

   do
   {
      EUSART2_Write_Text ("$$$"); // Enter command mode
      __delay_ms (500);
   } while (BT_Get_Response () != BT_CMD);

   do
   {
      EUSART2_Write_Text ("SN,BlueTooth-Click"); // Name of device
      EUSART1_Write (13); // CR
      __delay_ms (500);
   } while (BT_Get_Response () != BT_AOK);

   do
   {
      EUSART2_Write_Text ("SO,Slave"); // Extended status string
      EUSART1_Write (13); // CR
      __delay_ms (500);
   } while (BT_Get_Response () != BT_AOK);

   do
   {
      EUSART2_Write_Text ("SM,0"); // Set mode (0 = slave, 1 = master, 2 = trigger, 3 = auto, 4 = DTR, 5 = ANY)
      EUSART1_Write (13); // CR
      __delay_ms (500);
   } while (BT_Get_Response () != BT_AOK);

   do
   {
      EUSART2_Write_Text ("SA,1"); // Authentication (1 to enable, 0 to disable)
      EUSART1_Write (13); // CR
      __delay_ms (500);
   } while (BT_Get_Response () != BT_AOK);

   do
   {
      EUSART2_Write_Text ("SP,1234"); // Security pin code (mikroe)
      EUSART1_Write (13); // CR
      __delay_ms (500);
   } while (BT_Get_Response () != BT_AOK);

   do
   {
      EUSART2_Write_Text ("---"); // Security pin code (mikroe)
      EUSART1_Write (13); // CR
      __delay_ms (500);
   } while (BT_Get_Response () != BT_END);
}