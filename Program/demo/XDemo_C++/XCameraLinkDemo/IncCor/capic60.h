//
// Coreco API - C60 memory file access functions.
//

#ifndef _CAPIC60_H_
#define _CAPIC60_H_

#include <capidef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Application programming interface */

/* Load/Save from/to file */
CORSTATUS CORAPIFUNC CorC60LoadProgram( CORSERVER hServer, UINT32 nDSP, PCSTR filename);

CORSTATUS CORAPIFUNC CorC60LoadData( CORSERVER hServer, UINT32 nDSP, UINT32 C60Addr, UINT32 length, PCSTR filename);
CORSTATUS CORAPIFUNC CorC60SaveData( CORSERVER hServer, UINT32 nDSP, UINT32 C60Addr, UINT32 length, PCSTR filename);

CORSTATUS CORAPIFUNC CorC60ReadData( CORSERVER hServer, UINT32 nDSP, UINT32 C60Addr, UINT32 length, void *array);
CORSTATUS CORAPIFUNC CorC60WriteData( CORSERVER hServer, UINT32 nDSP, UINT32 C60Addr, UINT32 length, void *array);

CORSTATUS CORAPIFUNC CorC60Reset( CORSERVER hServer );
CORSTATUS CORAPIFUNC CorC60Start( CORSERVER hServer, UINT32 nDSP);

CORSTATUS CORAPIFUNC CorC60GetCount( CORSERVER hServer, PUINT32 nDSP);
#ifdef __cplusplus
}
#endif

#endif   // _CAPIC60_H_
