/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH57x_flash.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description
 *******************************************************************************/

#include "CH57x_common.h"

/*******************************************************************************
 * Function Name  : FLASH_ROM_READ
 * Description    : Read Flash
 * Input          :
 * Return         : None
 *******************************************************************************/
void FLASH_ROM_READ( UINT32 StartAddr, PVOID Buffer, UINT32 len )
{
  UINT32 i, Length = ( len + 3 ) >> 2;
  PUINT32 pCode = ( PUINT32 ) StartAddr;
  PUINT32 pBuf = ( PUINT32 ) Buffer;

  for ( i = 0; i < Length; i++ )
  {
    *pBuf++ = *pCode++;
  }
}

