#include "Common.h"




//SD Card
FIL myDir;
FIL myFile;
// Array that holds the data
FATFS fs;
byte sdBuffer[512];


//remember folder number to create a new folder for every save
int foldern;
char folder[36];

// File browser
char fileName[FILENAME_LENGTH];
char filePath[FILEPATH_LENGTH];
byte currPage;
byte lastPage;
byte numPages;
boolean root = 0;


// Common
char romName[64];
unsigned long sramSize = 0;
int romType = 0;
byte saveType;
word romSize = 0;
word numBanks = 128;
char checksumStr[5];
bool errorLvl = 0;
boolean ignoreError = 0;
//
//
//String CRC1 = "";
//String CRC2 = "";
//
char flashid[5];
//char vendorID[5];
//
//unsigned long sramBase;
//unsigned long flashBanks;



// Variable to count errors
unsigned long writeErrors;

/*********************************************************************
*
System base parts

*/

static volatile int ticks = 0;

void SysClockInit()
{
  // Enable SysTick timer interrupt
  SysTick->LOAD = (SystemCoreClock / 1000) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) 
{
  ticks++;
}

int getSystick()
{
  return ticks;
}

void delay(int n) {
  unsigned endTicks = ticks + n;
  while (ticks < endTicks);
 
}

void ResetSystem()
{
  //
  __set_FAULTMASK(1);//关闭总中断
  NVIC_SystemReset();//请求单片机重启
}

void SysClockFree()
{
  // Disable SysTick interrupt
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


void delayMicroseconds(uint16_t us)
{
  //
  for(int i = 0;i<us;i++)
  {    
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  }
}

void load_foldern(char* system, char* type, char* romName)
{
  char numFileName[64];

  sprintf(numFileName, "/%s/NUM/%s/%s.txt", system, type, romName);

  FIL tf;
  char sfoldern[16];

  if (f_open(&tf, numFileName, FA_READ) == FR_OK)
  {
    if (!f_gets(&sfoldern, 16, &tf))
    {
      print_Error("Can't read num file!", false);
    }
    else
    {
      foldern = atoi(sfoldern);
    }

    f_close(&tf);
  }
  else
  {
    foldern = 0;
  }
}

void save_foldern(char* system, char* type, char* romName)
{
  char numFileName[64];

  sprintf(numFileName, "/%s/NUM/%s/%s.txt", system, type, romName);

  FIL tf;

  if (f_open(&tf, numFileName, FA_CREATE_ALWAYS|FA_WRITE) == FR_OK)
  {
    f_printf(&tf, "%d", foldern);
    f_close(&tf);
  }
  else
  {
    print_Error("Can't create num file!", false);
  }
}
