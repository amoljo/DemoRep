#ifndef MDL_Dimming_H
#define MDL_Dimming_H

#ifndef _QAC_

#endif  // #ifndef _QAC_

#include "basedef.h"

#ifdef PRIVATE
// TODO: Put all private symbol declarations here!
#endif // #ifdef PRIVATE


//==============================================================================
// PRIVATE MACROS
//==============================================================================

typedef uint32 tDimmingMode;
#define cDimmingMode_Automatic ((tDimmingMode) 0xD3AAAAAA)
#define cDimmingMode_Manual ((tDimmingMode) 0xD3555555)

#define cDimming_False	(uint32)0xD3123456
#define cDimming_True	(uint32)0xD3ABCDEF

//==============================================================================
// PRIVATE TYPES
//==============================================================================

//==============================================================================
// PRIVATE DATA
//==============================================================================

//==============================================================================
// CONSTANT PRIVATE DATA
//==============================================================================

//==============================================================================
// FORWARD DECLARATIONS OF PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================
extern void MDL_Dimming_SetDimmingMode(void);
extern void MDL_Hyb_Task( void );
extern void Rte_Adapter_MDL_OBC_Read_InfoConsumUnit(ConsumUnitType *pInfoConsumUnit);

//QACJ 0862: adimiten: Include of all headers is required by 
//                     SWA component template
#endif // #ifndef MDL_Dimming_H
