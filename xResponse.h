//=================================================================================================================================
#ifndef X_COMMAND_BASE_H
#define X_COMMAND_BASE_H
//=================================================================================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
//=================================================================================================================================
typedef enum { XBASE_MODE_OBJECT, XBASE_MODE_CONTENT } XCBASE_MODE_ENUM;
//=================================================================================================================================
typedef xObject (*xEvtResponse)(xObject context, xObject Obj, uint16_t size);
//=================================================================================================================================
typedef struct{
  xObject Header;
  uint8_t HeaderLength;
  uint8_t Mode;
  xEvtResponse Receiver;
}xCommandT;
//=================================================================================================================================
int8_t xResponseIdentify(xObject context, xCommandT commands[], uint8_t commands_count, uint8_t data[], uint16_t data_length);
//=================================================================================================================================
#endif
