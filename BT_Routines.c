  //  Configure BlueTooth-Click module
// responses to parse
extern const BT_CMD;
extern const BT_AOK;
extern const BT_CONN;
extern const BT_END;
extern char BT_Get_Response();

void  BT_Configure() {

  do
  {
    EUSART2_Write_Text ("$$$");                  // Enter command mode
    __delay_ms (500);
  } while (BT_Get_Response() != BT_CMD);

  do
  {
    EUSART2_Write_Text ("SN,BlueTooth-Click");   // Name of device
    UART1_Write(13);                          // CR
    __delay_ms (500);
  } while (BT_Get_Response() != BT_AOK);

   do
   {
    EUSART2_Write_Text ("SO,Slave");             // Extended status string
    UART1_Write(13);                          // CR
    __delay_ms (500);
  } while (BT_Get_Response() != BT_AOK);

  do
  {
    EUSART2_Write_Text ("SM,0");                 // Set mode (0 = slave, 1 = master, 2 = trigger, 3 = auto, 4 = DTR, 5 = ANY)
    UART1_Write(13);                          // CR
    __delay_ms (500);
  } while (BT_Get_Response() != BT_AOK);

  do
  {
    EUSART2_Write_Text ("SA,1");                 // Authentication (1 to enable, 0 to disable)
    UART1_Write(13);                          // CR
    __delay_ms (500);
  } while (BT_Get_Response() != BT_AOK);

  do
  {
    EUSART2_Write_Text ("SP,1234");              // Security pin code (mikroe)
    UART1_Write(13);                          // CR
    __delay_ms (500);
  } while (BT_Get_Response() != BT_AOK);

  do
  {
    EUSART2_Write_Text ("---");                  // Security pin code (mikroe)
    UART1_Write(13);                          // CR
    __delay_ms (500);
  } while (BT_Get_Response() != BT_END);
}