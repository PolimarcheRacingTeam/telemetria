/*
 * logging.c
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef __logging_H
#define __logging_H


#include "fatfs.h"



FATFS SDFatFs;  /* File system object for SD disk logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD disk logical drive path */

static void testSDCard()
{
	FRESULT res;                                          /* FatFs function common result code */
	uint32_t byteswritten, bytesread;                     /* File write/read counts */
	uint8_t wtext[] = "Hello World\n"; /* File write buffer */
	uint8_t rtext[100];                                   /* File read buffer */

	/*##-1- Link the SD disk I/O driver ########################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		/*##-2- Register the file system object to the FatFs module ##############*/
		if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
		{
			printf("failed to mount\n");
			Error_Handler();
		}
		else
		{
			/*##-3- Create a FAT file system (format) on the logical drive #########*/
			if(0)//f_mkfs((TCHAR const*)SDPath, 0, 0) != FR_OK)
			{
				printf("failed to format\n");
				Error_Handler();
			}
			else
			{
				/*##-4- Create and Open a new text file object with write access #####*/
				if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
				{
					/* 'STM32.TXT' file Open for write Error */
					printf("failed to open\n");
					Error_Handler();
				}
				else
				{
					/*##-5- Write data to the text file ################################*/
					res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

					if((byteswritten == 0) || (res != FR_OK))
					{
						/* 'STM32.TXT' file Write or EOF Error */
						printf("failed to write\n");
						Error_Handler();
					}
					else
					{
						/*##-6- Close the open text file #################################*/
						f_close(&MyFile);

						/*##-7- Open the text file object with read access ###############*/
						if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
						{
							/* 'STM32.TXT' file Open for read Error */
							printf("file open error\n");
							Error_Handler();
						}
						else
						{
							/*##-8- Read data from the text file ###########################*/
							res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);

							if((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
							{
								/* 'STM32.TXT' file Read or EOF Error */
								printf("failed to read\n");
								Error_Handler();
							}
							else
							{
								/*##-9- Close the open text file #############################*/
								f_close(&MyFile);

								/*##-10- Compare read data with the expected data ############*/
								if ((bytesread != byteswritten))
								{
									/* Read data is different from the expected data */
									printf("i din't write this\n");
									Error_Handler();
								}
								else
								{
									/* Success of the demo: no error occurrence */
									printf("yay\n");
								}
							}
						}
					}
				}
			}
		}
	}


	/*##-11- Unlink the micro SD disk I/O driver ###############################*/
	FATFS_UnLinkDriver(SDPath);

	/* Infinite Loop */
	for( ;; )
	{
	}
}

#endif
