#ifndef MDL_Dimming_H
#define MDL_Dimming_H

#ifndef _QAC_
//******************************************************************************
// Company:      Johnson Controls Inc.
// -----------------------------------------------------------------------------
// Copyright:    This software is JCI property.
//               Duplication or disclosure without JCI written authorization
//               is prohibited.
// -----------------------------------------------------------------------------
// Project:      EL_DI_PSA_X81_IC_SOP_2016
// Language:     ANSI-C
// -----------------------------------------------------------------------------
/// \file
/// \brief  Component:    APP_Cmp - Functional Interface
// -----------------------------------------------------------------------------
// $Log:   //EL_DI_PSA_X81_IC_SOP_2016/archives/MC/Development/DEV/AS/CMP/SWC/MDL_Dimming/MDL_Dimming.h-arc  $
// 
//    Rev 1.3   21 Sep 2015 15:49:56   adey1
// Cm085853: SWFMEA
// 
//    Rev 1.2   Apr 06 2015 20:33:16   adeya1
// Cm075163: Review Comments fixed
// 
//    Rev 1.1   Feb 25 2015 15:08:24   agawadsw
// Cm078905: L2 DEV: Code Migration from Old PVCS structure to new PVCS structure
// 
//    Rev 1.1   Jul 28 2014 18:41:14   adeya1
// Cm069980: MDL_Dimming updated as per model
// 
//    Rev 1.4   31 May 2013 12:14:20   astamok
// Cm053685: L1T: Implementation of feature Fp36_dimming_of_the_cluster, ProtoA0 Scope -> fix some functional issues
// 
//    Rev 1.3   27 May 2013 18:50:38   astamok
// Cm053685: L1T: Implementation of feature Fp36_dimming_of_the_cluster, ProtoA0 Scope. Com-ponent update after code review
// 
//    Rev 1.2   20 May 2013 15:38:18   astamok
// Cm053685: L1T: Implementation of feature Fp36_dimming_of_the_cluster, ProtoA0 Scope. Update required and provided interfaces
// 
//    Rev 1.1   16 May 2013 18:13:02   astamok
// Cm053685: L1T: Implementation of feature Fp36_dimming_of_the_cluster, ProtoA0 Scope - fix QAC warnings
// 
//    Rev 1.0   16 May 2013 14:59:08   astamok
// Initial revision.
// 
//    Rev 1.0   15 Apr 2013 17:17:44   agenchr
// Initial revision.
// 
//    Rev 1.0   26 Mar 2013 14:08:32   agenchr
// Initial revision.
//******************************************************************************
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
