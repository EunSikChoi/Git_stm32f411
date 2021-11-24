/*
 * fatfs.c
 *
 *  Created on: 2021. 11. 17.
 *      Author: 82109
 */




#include "fatfs.h"
#include "cli.h"
#include "flash.h"

#ifdef _USE_HW_FATFS
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "button.h"

#define WRITE_BLOCK_LENGTH     512

static bool is_init = false;
static uint32_t res, number;
int32_t getFileSize(char *file_name);

FATFS SDFatFs;  /* File system object for SD card logical drive */
char SDPath[4]; /* SD card logical drive path */

FIL log_file;

#ifdef _USE_HW_CLI
static void cliFatfs(cli_args_t *args);
#endif

bool fatfsInit(void)
{
  bool ret = true;

   /*##-1- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
  	/*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
    {
      is_init = true;
    }
  }

#ifdef _USE_HW_CLI
  cliAdd("fatfs", cliFatfs);
#endif

  return ret;
}


#ifdef _USE_HW_CLI

FRESULT fatfsDir(char* path)
{
  FRESULT res;
  DIR dir;
  FILINFO fno;

  // 디렉토리를 열어서 파일 및 데렉토리 이름 출력하는 함수//
  res = f_opendir(&dir, path);                       /* Open the directory */
  if (res == FR_OK)
  {
    for (;;)
    {
      res = f_readdir(&dir, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fattrib & AM_DIR)
      {                    /* It is a directory */
        cliPrintf(" %s/%s \n", path, fno.fname); // 디렉토리 이름 출력
      }
      else
      {                                       /* It is a file. */
        cliPrintf(" %s/%32s \t%d bytes\n", path, fno.fname, (int)fno.fsize);  // 파일 이름 및 사이즈 출력
      }
    }
    f_closedir(&dir);
  }

  return res;
}

void cliFatfs(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("fatfs init \t: %d\n", is_init);

    if (is_init == true)
    {
			 FATFS *fs;
       DWORD fre_clust, fre_sect, tot_sect;
       FRESULT res;

       /* Get volume information and free clusters of drive 1 */
       res = f_getfree("", &fre_clust, &fs); // 현재 sd에 남아있는 정보 GET //
       if (res == FR_OK)
       {
         /* Get total sectors and free sectors */
         tot_sect = (fs->n_fatent - 2) * fs->csize;
         fre_sect = fre_clust * fs->csize;

         /* Print the free space (assuming 512 bytes/sector) */
         cliPrintf("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);
         // 전체 토탈 사이즈 및 남아있는 용량 출력 //
       }
       else
       {
         cliPrintf(" err : %d\n", res);
       }
    }

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "dir") == true)
  {
    FRESULT res;

    res = fatfsDir("/"); // CES 폴더 안에 있는 파일 출력 // ces 폴더 있어야함 //

    res = fatfsDir("ces"); // 루트 디렉토리에 있는  파일 출력 //

    res = fatfsDir("ces/test"); // 루트 디렉토리에  ces 하위 test 폴더 출력 //

    if (res != FR_OK)
    {
      cliPrintf(" err : %d\n", res);
    }

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "test") == true)
  {
    FRESULT fp_ret;
    FIL log_file;
    uint32_t pre_time;

    pre_time = millis();

    cliPrintf("Button  : %d \n", buttonGetPressed(0));

    /*##-4- Create and Open a new text file object with write access #####*/
    fp_ret = f_open(&log_file, "1.csv", FA_CREATE_ALWAYS | FA_WRITE | FA_READ); // 파일 생성 // 이름 1.csv //
    if (fp_ret == FR_OK)
    {
    	for(int i = 0 ; i < 5 ; i++)
    	{
				f_printf(&log_file, "Alram %02d  , " , i);
				f_printf(&log_file, "Time  %s, " , __TIME__);
				f_printf(&log_file, "code  %s, " , __DATE__);
				f_printf(&log_file, ", ");
				f_printf(&log_file, "\n");
    	}

      f_rewind(&log_file); // 생성한 파일 가져오기 //

      UINT len;
      uint8_t data;

      while(cliKeepLoop())  // 생성한 파일 출력
      {
        len = 0;


        /*##-8- Read data from the text file ###########################*/
        fp_ret = f_read (&log_file, &data, 1, &len); // 1BYTE씩 READ //

        if (fp_ret != FR_OK)
        {
          break;
        }
        if (len == 0)
        {
          break;
        }

        cliPrintf("%c", data);
      }

      /*##-9- Close the open text file #############################*/
      f_close(&log_file);
    }
    else
    {
      cliPrintf("f_open fail\r\n");
    }
    cliPrintf("%d ms\r\n", millis()-pre_time);

    ret = true;
  }

  	if (args->argc == 1 && args->isStr(0, "event") == true)
    {

      FRESULT fp_ret;
      FIL log_file;

      DIR dir;
      FILINFO fno;

      uint32_t pre_time;


      pre_time = millis();

      cliPrintf("Button  : %d \n", buttonGetPressed(0));

      if (buttonGetPressed(0) == 0) // if press key button , save event // 1 : button press // 0 not press //
      {

      	f_findfirst(&dir,&fno,"","event.csv");             // search file name //
      	cliPrintf("finded file name : %s\r\n", fno.fname); // Display the object name

      	if (strcmp( fno.fname, "event.csv") == 0)          // compare string // 0 = same // 1 = not file //
      	{
      		/*##-4- Create and Open a new text file object with write access #####*/
					fp_ret = f_open(&log_file, "event.csv", FA_OPEN_EXISTING | FA_WRITE | FA_READ); // if exist file  , open file
					cliPrintf("existing file...\n");

					res = f_size(&log_file);  // check where is end of file //
					number = res / 53; 			  // 1 line 53byte  ( size / 53 ) == index //
      	}
      	else
      	{
      		/*##-4- Create and Open a new text file object with write access #####*/
					fp_ret = f_open(&log_file, "event.csv", FA_CREATE_ALWAYS | FA_WRITE | FA_READ); // if not exist file, make file
					cliPrintf("make file...\n");
      	}

				if (fp_ret == FR_OK)
				{

					f_lseek(&log_file, res); // Move read/write pointer

					f_printf(&log_file, "index %02d ," ,  number  );
					f_printf(&log_file, "Toggle   ,"  );
					f_printf(&log_file, "Time  %s ," , __TIME__);
					f_printf(&log_file, "code  %s ," , __DATE__);
					f_printf(&log_file, ","); //
					f_printf(&log_file, "\n");

					res = f_tell(&log_file); // Get current read/write pointer // not used
					number++;

					f_rewind(&log_file); // read make file //

					UINT len;
					uint8_t data;

					while(cliKeepLoop())  // read & prinf to notebook
					{
						len = 0;

						/*##-8- Read data from the text file ###########################*/
						fp_ret = f_read (&log_file, &data, 1, &len); // 1BYTE READ //

						if (fp_ret != FR_OK)
						{
							break;
						}
						if (len == 0)
						{
							break;
						}

						cliPrintf("%c", data);
					}

					/*##-9- Close the open text file #############################*/
					f_close(&log_file);
				}
				else
				{
					cliPrintf("f_open fail\r\n");
				}
				cliPrintf("%d ms\r\n", millis()-pre_time);

     }
      else
      {
      	cliPrintf("Push Not Key Button \n");
      }
      ret = true;
    }

  //	extern bool flashErase(uint32_t addr, uint32_t length);

  	if (args->argc == 2 && args->isStr(0, "boot") == true)
  	  {
  	  	FRESULT res;

  	    DIR dir;
  	    FILINFO fno;

  	    uint32_t pre_time;
  	    uint32_t exe_time;

  	    uint32_t file_addr;
  	    char     file_name[256];
  	    int32_t  file_size;
  	    uint8_t  err_code;
  	    bool     get_size_done = true;
  	    bool     write_done 	 = false;

  	    //-- Set Flash Write Addr
  	    //
  	    file_addr = 0x08044000;
  	    cliPrintf("file addr \t: 0x%X\n", file_addr);

  	    //-- Get Name Cli cmd 2, User input name  using cli cmd --//
  	    //
  	    strcpy(file_name, args->argv[1]);
  	    cliPrintf("file name \t: %s\n", file_name);

  	    //-- Check File name
  	    //
  	    f_findfirst(&dir,&fno,"",file_name);             // search file name //

  	    if (strcmp( fno.fname, file_name) != 0)          // compare string // 0 = same // 1 = not file //
  	    {
  	    	cliPrintf("file name error\n");
  	    }

  	    //-- Get File Size
  	    //
  	    file_size = getFileSize(file_name);
  	    if (file_size <= 0)
  	    {
  	    	cliPrintf("file size error\n");
  	    	get_size_done = false;
  	    }
  	    cliPrintf("file size \t: %d bytes\n", file_size);

  	    if(get_size_done == true)
  	    {

  				 //-- Flash Erase
  				 //
  				 cliPrintf("flash erase \t: ");
  				 pre_time = millis();
  				 err_code = flashErase(file_addr, file_size);
  				 exe_time = millis()-pre_time;

  				 if (err_code != true)
  				 {
  					 cliPrintf("\nbootCmdFlashErase fail : %d\n", err_code);
  				 }

  				 cliPrintf("OK (%dms)\n", exe_time);

  				 //-- Flash Write
  				 //
  				 if( (f_open(&log_file, file_name, FA_OPEN_EXISTING | FA_WRITE | FA_READ)) !=  FR_OK )    // if exist file  , open file
  				 {
  					 cliPrintf("unable open file\n");
  				 }

  				 uint32_t addr;
  				 uint16_t write_percent;
  				 uint16_t pre_percent = 0;

  				 UINT len;
  				 uint8_t data[WRITE_BLOCK_LENGTH];

  				 addr = file_addr;
  				 //file_addr : flash Write 시작주소
  				 //addr  : 다음 루프에 써야할 Flash 주소

  				 pre_time = millis();

  				 while(1)
  				 {
  					 if(!f_eof(&log_file))
  					 {
  						 //fp_ret = f_read (&log_file, &data, 1, &len); 						// 1BYTE READ //  ( 읽을 파일 주소 , 저장할 버퍼 , 한번에 읽을 갯수 , 읽은 갯수 정보가 저장될 변수 주소 )
  						 res = f_read (&log_file, data, WRITE_BLOCK_LENGTH, &len);  // 512BYTE READ //

  						 flashWrite(addr, data, len); 															// 512바이트 크기를 Write // feof 종료시까지 //

  						 addr += len; // next Write addr //

  						 write_percent = (addr-file_addr) * 100 / file_size; 				// (8100 - 8100)*100 /  400 = 0% , (8500 - 8100)*100 / 400 = 100%

  						 if ((write_percent/10) != pre_percent)
  						 {
  							 cliPrintf("flash write \t: %d%%\r", write_percent);
  							 pre_percent = (write_percent/10);
  						 }

  						// cliPrintf("len : %x \n", len);
  						// cliPrintf("add : %x \n", addr);
  						// cliPrintf("READ file : %s \n", data);

  						 if ((addr-file_addr) >= file_size)// 써야할 주소 - 시작주소  의미는  현재까지 쓴 데이터의 크기 의미 즉 파일크기와 현재까지 쓴 크기가 같으면 전체 Write END//
  						 {
  								write_done = true; // Flash Write END //
  								break;
  						 }

  						 if ((res != FR_OK ) || (len == 0))
  						 {
  								break;
  						 }
  					 }
  				 }
  	    }

  			/*##-9- Close the open text file #############################*/
  			f_close(&log_file);

				 //-- Boot jump , Not used
				 //

//  			if( write_done == true)
//  			{
//  				 void (**jump_func)() = (void (**)())(0x8010000 + 4);
//
//  				 if ((uint32_t)(*jump_func) != 0xFFFFFFFF)
//  				 {
//  					 HAL_RCC_DeInit();
//  					 HAL_DeInit();
//
//  					 for (int i=0; i<8; i++)
//  					 {
//  						 NVIC->ICER[i] = 0xFFFFFFFF;
//  						 __DSB();
//  						 __ISB();
//  					 }
//  					 SysTick->CTRL = 0;
//
//  					 (*jump_func)();
//  				 }
//  			}

  	    ret = true;
  	  }

  if (ret != true)
  {
    cliPrintf("fatfs info\n");
    cliPrintf("fatfs dir\n");
    cliPrintf("fatfs test\n");
    cliPrintf("fatfs event\n");
    cliPrintf("fatfs boot\n");
  }
}

#endif


int32_t getFileSize(char *file_name)
{
  int32_t ret = -1;
 //FIL log_file;

  if (f_open(&log_file, file_name, FA_OPEN_EXISTING | FA_WRITE | FA_READ)    ) // if exist file  , open file
  {
  	cliPrintf("false open file\n");
    return -1;
  }
  else
  {
  	ret = f_size(&log_file);  // check where is end of file //
  	f_close(&log_file);
  }

  return ret;
}


#endif
