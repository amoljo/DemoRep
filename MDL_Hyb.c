#ifndef _QAC_
//******************************************************************************
//
// Company:      Johnson Controls Inc.
//-----------------------------------------------------------------------------
// Copyright:    This software is JCI property.
//               Duplication or disclosure without JCI written authorization
//               is prohibited.
//-----------------------------------------------------------------------------
// Project:
// Language:     ANSI-C
//-----------------------------------------------------------------------------
/// \file
/// \brief  Component:    MDL_Hyb - Implementation.
//-----------------------------------------------------------------------------
// $Log:   //EL_DI_PSA_X81_IC_SOP_2016/archives/MC/Development/DEV/AS/CMP/SWC/MDL_Hyb/MDL_Hyb.c-arc  $
//
//
//
//******************************************************************************

#endif  // #ifndef _QAC_

//==============================================================================
// Component trace identification
//==============================================================================
/// Component trace identification
#define MDL_Hyb   "MDL_Hyb"

//==============================================================================
// INCLUDED FILES
//==============================================================================


#ifndef PRIVATE
#define PRIVATE
#endif

#include "MDL_Hyb_RI.h"
#include "MDL_Hyb.h"
#include "LIB_BaseCmp.h"

//==============================================================================
// PRIVATE MACROS
//==============================================================================
#define Disable     cFalse
#define False       cFalse
#define Absence     cFalse
#define True        cTrue
#define Present     cTrue
#define NotPresent  cFalse

#define Invalid                                 0xFFU
#define cHystDownOffset                         (sint8)3
#define Invalid_HY_BAT_LEVEL                    (uint8)0x7FU
#define HY_CONSO_ZEV_MAX                        0x1F4U // 500 // 0x1FFU
#define cDAT_LI_HY_BAT_LEVEL                    2U


#define STATE_HY_BAT_LEVEL                      0U
#define STATE_HY_BAT_LEVEL_INVALID              3U
#define STATE_HY_BAT_LEVEL_NOMINAL              4U
#define STATE_HY_BAT_LEVEL_WITH_HYST            2U
#define STATE_HY_BAT_LEVEL_WITHOUT_HYST         1U

#define STATE_HY_ACTIVE                         1U
#define STATE_HY_INACTIVE                       2U
#define cDAT_DG_HY_PRESENT                      1U

#define cu8_TI_DEGRADE                          1U
#define cu8_TI_DISPO_DEM                        2U

#define cFifty                                  50
#define cSeventy                                70

#define cInfoGaugeBattarySoc_Disable            InfoGaugeBattarySoc_Disable
#define cInfoGaugeBattarySoc_Invalid            InfoGaugeBattarySoc_Invalid
#define cInfoGaugeHybridPower_Disable           InfoGaugeHybridPower_Disable
#define cInfoGaugeHybridPower_Invalid           InfoGaugeHybridPower_Invalid
#define cInfoGaugeHybridPower_MaxValue          InfoGaugeHybridPower_MaxValue
#define cInfoGaugeHybridPower_Unavailable       InfoGaugeHybridPower_Unavailable
#define cInfoHyAutonomieZev_Invalid             InfoHyAutonomieZev_Invalid
#define cInfoHyConsoZevInst_Invalid             InfoHyConsoZevInst_Invalid
#define cInfoHyConsoZevTrip1_Invalid            InfoHyConsoZevTrip1_Invalid
#define cInfoHyConsoZevTrip2_Invalid            InfoHyConsoZevTrip2_Invalid

#define cHyAutonomieZev_Invalid                 0x1FF

///Hyb Speed invalid value
#define cInfoHySpeedInvalidValue                (uint16)0xffff
#define cOnOffRechargeHDebut_Unavailable        (UInt16)0x7FF
#define cOnOffChargeRemainingTime_Unavailable   0x7F
#define cOnOffChargeRemainingTime_GreaterThan12 0x7E

//ajoshi8: Story 545558 -> Modification as per comment in RTC
#define cValueTwo                               (U8)2U

//==============================================================================
// PRIVATE TYPES
//==============================================================================

//==============================================================================
// FORWARD DECLARATIONS OF PRIVATE FUNCTIONS
//==============================================================================
static void state_Init(void);
static void state_Diag(void);
static void state_Started(void);
static eTransitionResult trans_ToInit(void);
static eTransitionResult trans_ToDiag(void);
static eTransitionResult trans_ToStarted(void);
static eTransitionResult trans_ToStopped(void);

static uint8 ch(uint8 Var);
static void Charging_Page(void);
static void ConsumptionHistogram(void);
static void Manage_STATE_HY_BAT_LEVEL_NOMINAL(void);
static void ManageTractionModeState(void);
//==============================================================================
// PRIVATE DATA
//==============================================================================
REGISTER_STATE_FUNCTIONS(MDL_Hyb, NULL_STATE, &state_Init, &state_Started, &state_Diag, NULL_STATE);
REGISTER_TRANS_FUNCTIONS(MDL_Hyb, &trans_ToInit, &trans_ToStarted, &trans_ToDiag, &trans_ToStopped);
DEFINE_CMP_CONTEXT(MDL_Hyb);

//==============================================================================
// CONSTANT PRIVATE DATA
//==============================================================================
static U8 States1018;

Boolean ST_HY_MTH=0U;
Boolean ST_HY_MEL_AR=0U;
//uint8 ST_HY_RECHARGE=0;
Boolean FirstTransition;
uint8 DAT_LI_HY_BAT_LEVEL;
Boolean DAT_DG_HY_PRESENT;
sint8 VAR_HY_BAT_SEUIL_MIN;
uint8 VAR_HY_BAT_SEUIL_MAX;
Boolean ST_HY_PRESENT_CFG=0U;
//SInt16 GAUGE_HYBRIDE_TAQUET=0;
UInt16 ST_HY_AUTONOMIE_ZEV=0U;
UInt16 ST_HY_CONSO_ZEV_INST=0U;
Boolean DAT_DG_HY_PRESENT_PREV;
uint8 DAT_LI_HY_BAT_LEVEL_PREV;
HyBatLevelType ST_HY_BAT_LEVEL;
UInt16 ST_HY_CONSO_ZEV_TRIP1=0U;
UInt16 ST_HY_CONSO_ZEV_TRIP2=0U;
uint8 ST_HY_STATE_ELEC_AV_ENG=0U;
UInt8 GAUGE_HYBRIDE_POWER_POINTER_0=0U;
UInt8 GAUGE_HYBRIDE_POWER_POINTER_1=0U;
UInt8 GAUGE_HYBRIDE_POWER_POINTER_2=0U;
UInt8 GAUGE_HYBRIDE_POWER_POINTER_3=0U;
UInt16 GAUGE_HYBRIDE_POWER_POINTER_INVALID=0U;
UInt16 GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE=0U;

static Boolean INFO_DRIVING_CAR=0U;
static uint8 INFO_ONOFF_CHARGE_RATE=0U;
static uint8 INFO_ONOFF_STATUS_CHARGE=0U;
static Boolean INFO_GAUGE_SOCKET_SIDE =0U;
static uint8 INFO_ONOFF_RECHARGE_H_DEBUT = 0U;
static uint8 INFO_ONOFF_RECHARGE_MIN_DEBUT = 0U;
static Boolean INFO_ONOFF_STATUS_PAGE_CHARGE =0U;
static uint8 INFO_ONOFF_CHARGE_REMAINING_TIME_H = 0U;
static uint8 INFO_ONOFF_CHARGE_REMAINING_TIME_MIN = 0U;
//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================
//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Init State functionality
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void state_Init(void)
{

}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static uint8 ch(uint8 Var)
{
    uint8 RetVal=False;
    if(Var == cDAT_LI_HY_BAT_LEVEL)
    {
        if(DAT_LI_HY_BAT_LEVEL != DAT_LI_HY_BAT_LEVEL_PREV)
        {
            RetVal=True;
        }
        else
        {
            RetVal=False;
        }
    }
    else if(Var == cDAT_DG_HY_PRESENT)
    {
        if(DAT_DG_HY_PRESENT != DAT_DG_HY_PRESENT_PREV)
        {
            RetVal=True;
        }
        else
        {
            RetVal=False;
        }
    }
    else
    {
        JCIAssert(cFalse);
    }
    return RetVal;
}


//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Charging_Page
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void Charging_Page(void)
{
    uint8 DAT_LI_ONOFF_PLUGGED;
    uint8 DAT_LI_ONOFF_ECO_MODE;
    Boolean DAT_LI_ONOFF_ON_UNIT;
    ComExtSignalStateType ComExtState;
    uint16 DAT_LI_ONOFF_RECHARGE_DEBUT;
    Boolean DAT_LI_ONOFF_TYPE_RECHARGE;
    uint8 DAT_LI_ONOFF_CHARGE_REMAINING_TIME;
    uint8 DAT_LI_ONOFF_STATE_VHL_ELEC_SYSTEM;

    //  _j_Req_Identifier :  SysRS16_Hybrid-1627
    RI_Get_DAT_LI_ONOFF_PLUGGED(&DAT_LI_ONOFF_PLUGGED,&ComExtState);
    RI_Get_DAT_LI_ONOFF_TYPE_RECHARGE(&DAT_LI_ONOFF_TYPE_RECHARGE,&ComExtState);
    RI_Get_DAT_LI_ONOFF_CHARGE_REMAINING_TIME(&DAT_LI_ONOFF_CHARGE_REMAINING_TIME,&ComExtState);
    RI_Get_DAT_LI_ONOFF_ECO_MODE(&DAT_LI_ONOFF_ECO_MODE,&ComExtState);

    if((DAT_LI_ONOFF_PLUGGED != 0U) && (DAT_DG_HY_PRESENT ==cTrue ) &&
    		((DAT_LI_ONOFF_ECO_MODE != cu8_TI_DEGRADE) && (DAT_LI_ONOFF_ECO_MODE != cu8_TI_DISPO_DEM)))
    {
        INFO_ONOFF_STATUS_PAGE_CHARGE = cTrue;
        if(DAT_LI_ONOFF_TYPE_RECHARGE == cTrue)
        {
            INFO_ONOFF_STATUS_CHARGE = InfoOnOffStatusCharge_DIFFERED;
        }
        else if(DAT_LI_ONOFF_CHARGE_REMAINING_TIME == 0U)
        {
            INFO_ONOFF_STATUS_CHARGE = InfoOnOffStatusCharge_FINISHED;
        }
        else
        {
            INFO_ONOFF_STATUS_CHARGE = InfoOnOffStatusCharge_IMMEDIATE;
        }
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-1628
    //  _j_Req_Identifier :  SysRS16_Hybrid-1631
    //  _j_Req_Identifier :  SysRS16_Hybrid-1630
    RI_Get_DAT_LI_ONOFF_ON_UNIT(&DAT_LI_ONOFF_ON_UNIT,&ComExtState);
    RI_Get_DAT_LI_ONOFF_STATE_VHL_ELEC_SYSTEM(&DAT_LI_ONOFF_STATE_VHL_ELEC_SYSTEM,&ComExtState);

    if((DAT_LI_ONOFF_ON_UNIT == cTrue)                                                            &&
       ((DAT_LI_ONOFF_ECO_MODE != cu8_TI_DEGRADE) && (DAT_LI_ONOFF_ECO_MODE != cu8_TI_DISPO_DEM)) &&
       (DAT_LI_ONOFF_STATE_VHL_ELEC_SYSTEM == 0U)                                                 &&
       (DAT_LI_ONOFF_PLUGGED != 0U)                                                               &&
       (DAT_DG_HY_PRESENT ==cTrue ))
    {
        RI_Get_DAT_LI_ONOFF_CHARGE_RATE(&INFO_ONOFF_CHARGE_RATE,&ComExtState);
        RI_Get_DAT_LI_ONOFF_RECHARGE_DEBUT(&DAT_LI_ONOFF_RECHARGE_DEBUT,&ComExtState);

        if(DAT_LI_ONOFF_RECHARGE_DEBUT == cOnOffRechargeHDebut_Unavailable)
        {
            INFO_ONOFF_RECHARGE_H_DEBUT = InfoOnOffRechargeHDebut_Unavailable;
            INFO_ONOFF_RECHARGE_MIN_DEBUT = InfoOnOffRechargeMinDebut_Unavailable;
        }
        else
        {
            INFO_ONOFF_RECHARGE_H_DEBUT = (uint8)(DAT_LI_ONOFF_RECHARGE_DEBUT/60U);
            INFO_ONOFF_RECHARGE_MIN_DEBUT = (uint8)(DAT_LI_ONOFF_RECHARGE_DEBUT%60U);
        }

        if( DAT_LI_ONOFF_CHARGE_REMAINING_TIME == cOnOffChargeRemainingTime_Unavailable)
        {
            INFO_ONOFF_CHARGE_REMAINING_TIME_H =InfoOnOffChargeRemainingTimeH_Unavailable;
            INFO_ONOFF_CHARGE_REMAINING_TIME_MIN =InfoOnOffChargeRemainingTimeMin_Unavailable;
        }
        else if(DAT_LI_ONOFF_CHARGE_REMAINING_TIME == cOnOffChargeRemainingTime_GreaterThan12)
        {
        	INFO_ONOFF_CHARGE_REMAINING_TIME_H = InfoOnOffChargeRemainingTimeH_GreaterThan12;
        	INFO_ONOFF_CHARGE_REMAINING_TIME_MIN = 0U;
        }
        else
        {
            INFO_ONOFF_CHARGE_REMAINING_TIME_H = (DAT_LI_ONOFF_CHARGE_REMAINING_TIME * 5)/60;
            INFO_ONOFF_CHARGE_REMAINING_TIME_MIN = (DAT_LI_ONOFF_CHARGE_REMAINING_TIME * 5)%60;
        }
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-1632
    if((DAT_LI_ONOFF_ON_UNIT == cFalse)                                                           ||
       ((DAT_LI_ONOFF_ECO_MODE == cu8_TI_DEGRADE) || (DAT_LI_ONOFF_ECO_MODE == cu8_TI_DISPO_DEM)) ||
       (DAT_LI_ONOFF_PLUGGED == 0U)                                                               ||
       (DAT_DG_HY_PRESENT ==cFalse )
    )
    {
        INFO_ONOFF_STATUS_PAGE_CHARGE = cFalse;
        INFO_ONOFF_STATUS_CHARGE = InfoOnOffStatusCharge_IMMEDIATE;
    }
}
//----------------------------------------------------------------------------------------------------------------------
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------

static void ConsumptionHistogram(void)
{
    ConsumUnitType ST_SET_CONSUM_UNIT=0U;
    UInt16 DAT_LI_HY_CONSO_ZEV_TRIP1=0U;
    UInt16 DAT_LI_HY_CONSO_ZEV_TRIP2=0U;
    UInt16 DAT_LI_HY_CONSO_ZEV_INST=0U;
    ComExtSignalStateType ComExtState;

    //  2.4.5   Histogram of consumption
    //
    //  _j_Req_Identifier :  SysRS16_Hybrid-1093
    //  Display the ZEV average consumption for trip 1 in STATE_HY1_ACTIVE:
    //
    //  If  (DAT_LI_HY_CONSO_ZEV_TRIP1 = Invalid)
    //          ST_HY_CONSO_ZEV_TRIP1 := (100 / DAT_LI_HY_CONSO_ZEV_TRIP1)
    //Else If (ST_SET_CONSUM_UNIT = mile/kWh)
    //     If (DAT_LI_HY_CONSO_ZEV_TRIP1 = 0)
    //          Then ST_HY_CONSO_ZEV_TRIP1 := 999.0
    //     Else
    //          ST_HY_CONSO_ZEV_TRIP1 := (100 / DAT_LI_HY_CONSO_ZEV_TRIP1) * (1 / 1.609)
    RI_Get_DAT_LI_HY_CONSO_ZEV_TRIP1(&DAT_LI_HY_CONSO_ZEV_TRIP1,&ComExtState);
    RI_Get_DAT_LI_HY_CONSO_ZEV_TRIP2(&DAT_LI_HY_CONSO_ZEV_TRIP2,&ComExtState);
    RI_Get_DAT_LI_HY_CONSO_ZEV_INST(&DAT_LI_HY_CONSO_ZEV_INST,&ComExtState);
    RI_Get_ST_SET_CONSUM_UNIT(&ST_SET_CONSUM_UNIT);

    if(DAT_LI_HY_CONSO_ZEV_TRIP1 > HY_CONSO_ZEV_MAX)
    {
        ST_HY_CONSO_ZEV_TRIP1 = cInfoHyConsoZevTrip1_Invalid;
    }
    else
    {
        if  ((ST_SET_CONSUM_UNIT == ConsumUnit_KWhPer100Km) || (ST_SET_CONSUM_UNIT == ConsumUnit_Invalid))
        {
            ST_HY_CONSO_ZEV_TRIP1 = DAT_LI_HY_CONSO_ZEV_TRIP1;
        }
        else if ( (ST_SET_CONSUM_UNIT == ConsumUnit_KmPerKWh))
        {
        	//ajoshi8: Story 545558 -> Modification as per comment in RTC
        	if(DAT_LI_HY_CONSO_ZEV_TRIP1 < cValueTwo)
            {
                ST_HY_CONSO_ZEV_TRIP1 = 9990U;
            }
            else
            {
                //100*10*10 = 1000
                ST_HY_CONSO_ZEV_TRIP1 = 10000U / DAT_LI_HY_CONSO_ZEV_TRIP1;// rounded to the lower value (resolution 0.1)
            }
        }
        else if (( ST_SET_CONSUM_UNIT == ConsumUnit_MilesPerKWh))
        {
            if(DAT_LI_HY_CONSO_ZEV_TRIP1==0U)
            {
                ST_HY_CONSO_ZEV_TRIP1 = 9990U;
            }
            else
            {
                //((100*10)/1.609 =621.504)
                ST_HY_CONSO_ZEV_TRIP1 = 6215U / DAT_LI_HY_CONSO_ZEV_TRIP1;// rounded to the lower value (resolution 0.1)
            }
        }
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-1100
    //  Display the ZEV average consumption for trip 2 in STATE_HY1_ACTIVE:
    //  If (DAT_LI_HY_CONSO_ZEV_TRIP2 = Invalid)
    //       Then ST_HY_CONSO_ZEV_TRIP2 := Invalid
    //  Else
    //                 ST_HY_CONSO_ZEV_TRIP2 := (100 / DAT_LI_HY_CONSO_ZEV_TRIP2)
    //       Else If (ST_SET_CONSUM_UNIT = mile/kWh)
    //            If (DAT_LI_HY_CONSO_ZEV_TRIP2 = 0)
    //                 Then ST_HY_CONSO_ZEV_TRIP2 := 999.0
    //            Else
    //                 ST_HY_CONSO_ZEV_TRIP2 := (100 / DAT_LI_HY_CONSO_ZEV_TRIP2) * (1 / 1.609)
    if(DAT_LI_HY_CONSO_ZEV_TRIP2 > HY_CONSO_ZEV_MAX)
    {
        ST_HY_CONSO_ZEV_TRIP2 = cInfoHyConsoZevTrip2_Invalid;
    }
    else
    {
        if  ((ST_SET_CONSUM_UNIT == ConsumUnit_KWhPer100Km) || (ST_SET_CONSUM_UNIT == ConsumUnit_Invalid))
        {
            ST_HY_CONSO_ZEV_TRIP2 = DAT_LI_HY_CONSO_ZEV_TRIP2;
        }
        else if ( (ST_SET_CONSUM_UNIT == ConsumUnit_KmPerKWh))
        {
        	//ajoshi8: Story 545558 -> Modification as per comment in RTC
        	if(DAT_LI_HY_CONSO_ZEV_TRIP2 < cValueTwo)
            {
                ST_HY_CONSO_ZEV_TRIP2 = 9990U;
            }
            else
            {
                //100*10 = 1000
                ST_HY_CONSO_ZEV_TRIP2 = 10000U / DAT_LI_HY_CONSO_ZEV_TRIP2;// rounded to the lower value (resolution 0.1)
            }
        }
        else if (( ST_SET_CONSUM_UNIT == ConsumUnit_MilesPerKWh))
        {
            if(DAT_LI_HY_CONSO_ZEV_TRIP2 == 0U)
            {
                ST_HY_CONSO_ZEV_TRIP2 = 9990U;
            }
            else
            {
                //((100*10)/1.609 =621.504)
                ST_HY_CONSO_ZEV_TRIP2 = 6215U / DAT_LI_HY_CONSO_ZEV_TRIP2;// rounded to the lower value (resolution 0.1)
            }
        }
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-1101
    //  Display the instantaneous ZEV consumption in STATE_HY1_ACTIVE:
    //
    //  If (DAT_LI_HY_CONSO_ZEV_INST = Invalid)
    //       Then ST_HY_CONSO_ZEV_INST := Invalid
    //                 ST_HY_CONSO_ZEV_INST := (100 / DAT_LI_HY_CONSO_ZEV_INST)
    //       Else If (ST_SET_CONSUM_UNIT = mpg or mile/kWh)
    //            If (DAT_LI_HY_CONSO_ZEV_INST = 0)
    //                 Then ST_HY_CONSO_ZEV_INST := 999.0
    //            Else
    //                 ST_HY_CONSO_ZEV_INST := (100 / DAT_LI_HY_CONSO_ZEV_INST) * (1 / 1.609)
    if(DAT_LI_HY_CONSO_ZEV_INST > HY_CONSO_ZEV_MAX)
    {
        ST_HY_CONSO_ZEV_INST = cInfoHyConsoZevInst_Invalid;
    }
    else
    {
        if  ((ST_SET_CONSUM_UNIT == ConsumUnit_lp100) || (ST_SET_CONSUM_UNIT == ConsumUnit_KWhPer100Km) || (ST_SET_CONSUM_UNIT == ConsumUnit_Invalid))
        {
            ST_HY_CONSO_ZEV_INST = DAT_LI_HY_CONSO_ZEV_INST;
        }
        else if ((ST_SET_CONSUM_UNIT == ConsumUnit_kmpl) || (ST_SET_CONSUM_UNIT == ConsumUnit_KmPerKWh))
        {
        	//ajoshi8: Story 545558 -> Modification as per comment in RTC
        	if(DAT_LI_HY_CONSO_ZEV_INST < cValueTwo)
            {
                ST_HY_CONSO_ZEV_INST = 9990U;
            }
            else
            {
                //100*10 = 1000
                ST_HY_CONSO_ZEV_INST = 10000U / DAT_LI_HY_CONSO_ZEV_INST;// rounded to the lower value (resolution 0.1)
            }
        }
        else if ((ST_SET_CONSUM_UNIT == ConsumUnit_mpg) || ( ST_SET_CONSUM_UNIT == ConsumUnit_MilesPerKWh))
        {
            if(DAT_LI_HY_CONSO_ZEV_INST == 0U)
            {
                ST_HY_CONSO_ZEV_INST = 9990U;
            }
            else
            {
                //((100*10)/1.609 =621.504)
                ST_HY_CONSO_ZEV_INST = 6215U / DAT_LI_HY_CONSO_ZEV_INST;// rounded to the lower value (resolution 0.1)
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         ManageStateHYBatLevelWithoutHyst
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void ManageStateHYBatLevelWithoutHyst(void)
{
    uint8 DAT_DG_HY_BAT_LEVEL0=0U;
    uint8 DAT_DG_HY_BAT_LEVEL1=0U;
    uint8 DAT_DG_HY_BAT_LEVEL2=0U;
    uint8 DAT_DG_HY_BAT_LEVEL3=0U;
    uint8 DAT_DG_HY_BAT_LEVEL4=0U;
    uint8 DAT_DG_HY_BAT_LEVEL5=0U;
    uint8 DAT_DG_HY_BAT_LEVEL6=0U;
    uint8 DAT_DG_HY_BAT_LEVEL7=0U;
    uint8 DAT_DG_HY_BAT_LEVEL8=0U;
    uint8 DAT_DG_HY_BAT_LEVEL9=0U;
    uint8 DAT_DG_HY_BAT_HYST_DOWN=0U;
    uint8 DAT_DG_HY_BAT_HYST_UP=0U;

    RI_Read_DAT_DG_HY_BAT_LEVEL0(&DAT_DG_HY_BAT_LEVEL0);
    RI_Read_DAT_DG_HY_BAT_LEVEL1(&DAT_DG_HY_BAT_LEVEL1);
    RI_Read_DAT_DG_HY_BAT_LEVEL2(&DAT_DG_HY_BAT_LEVEL2);
    RI_Read_DAT_DG_HY_BAT_LEVEL3(&DAT_DG_HY_BAT_LEVEL3);
    RI_Read_DAT_DG_HY_BAT_LEVEL4(&DAT_DG_HY_BAT_LEVEL4);
    RI_Read_DAT_DG_HY_BAT_LEVEL5(&DAT_DG_HY_BAT_LEVEL5);
    RI_Read_DAT_DG_HY_BAT_LEVEL6(&DAT_DG_HY_BAT_LEVEL6);
    RI_Read_DAT_DG_HY_BAT_LEVEL7(&DAT_DG_HY_BAT_LEVEL7);
    RI_Read_DAT_DG_HY_BAT_LEVEL8(&DAT_DG_HY_BAT_LEVEL8);
    RI_Read_DAT_DG_HY_BAT_LEVEL9(&DAT_DG_HY_BAT_LEVEL9);
    RI_Read_DAT_DG_HY_BAT_HYST_UP(&DAT_DG_HY_BAT_HYST_UP);
    RI_Read_DAT_DG_HY_BAT_HYST_DOWN(&DAT_DG_HY_BAT_HYST_DOWN);

    if(States1018==STATE_HY_BAT_LEVEL_WITHOUT_HYST)
    {
        if( (DAT_DG_HY_BAT_LEVEL0 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL1)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL0) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)( DAT_DG_HY_BAT_LEVEL1 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_0; }
        else if( (DAT_DG_HY_BAT_LEVEL1 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL2)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL1) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL2 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_1; }
        else if( (DAT_DG_HY_BAT_LEVEL2 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL3)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL2) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL3 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_2; }
        else if( (DAT_DG_HY_BAT_LEVEL3 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL4)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL3) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL4 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_3; }
        else if( (DAT_DG_HY_BAT_LEVEL4 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL5)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL4) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL5 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_4; }
        else if( (DAT_DG_HY_BAT_LEVEL5 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL6)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL5) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL6 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_5; }
        else if( (DAT_DG_HY_BAT_LEVEL6 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL7)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL6) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL7 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_6; }
        else if( (DAT_DG_HY_BAT_LEVEL7 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL8)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL7) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL8 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_7; }
        else if( (DAT_DG_HY_BAT_LEVEL8 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL <= DAT_DG_HY_BAT_LEVEL9)) { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL8) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(   DAT_DG_HY_BAT_LEVEL9 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_8; }
        else if( (DAT_LI_HY_BAT_LEVEL == Invalid_HY_BAT_LEVEL) )                                                { VAR_HY_BAT_SEUIL_MIN = (sint8)(((sint8)DAT_DG_HY_BAT_LEVEL0) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset)); VAR_HY_BAT_SEUIL_MAX = (uint8)(    DAT_DG_HY_BAT_LEVEL1 + DAT_DG_HY_BAT_HYST_UP); ST_HY_BAT_LEVEL = HyBatLevel_Inv; }
        else {}
    }

}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:SysRS16_Hybrid_1302
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void Manage_STATE_HY_BAT_LEVEL_NOMINAL(void)
{
    uint8 DAT_DG_HY_BAT_LEVEL0=0U;
    uint8 DAT_DG_HY_BAT_LEVEL1=0U;
    uint8 DAT_DG_HY_BAT_LEVEL2=0U;
    uint8 DAT_DG_HY_BAT_LEVEL3=0U;
    uint8 DAT_DG_HY_BAT_LEVEL4=0U;
    uint8 DAT_DG_HY_BAT_LEVEL5=0U;
    uint8 DAT_DG_HY_BAT_LEVEL6=0U;
    uint8 DAT_DG_HY_BAT_LEVEL7=0U;
    uint8 DAT_DG_HY_BAT_LEVEL8=0U;
    uint8 DAT_DG_HY_BAT_LEVEL9=0U;
    uint8 DAT_DG_HY_BAT_HYST_DOWN=0U;
    uint8 DAT_DG_HY_BAT_HYST_UP=0U;

    RI_Read_DAT_DG_HY_BAT_LEVEL0(&DAT_DG_HY_BAT_LEVEL0);
    RI_Read_DAT_DG_HY_BAT_LEVEL1(&DAT_DG_HY_BAT_LEVEL1);
    RI_Read_DAT_DG_HY_BAT_LEVEL2(&DAT_DG_HY_BAT_LEVEL2);
    RI_Read_DAT_DG_HY_BAT_LEVEL3(&DAT_DG_HY_BAT_LEVEL3);
    RI_Read_DAT_DG_HY_BAT_LEVEL4(&DAT_DG_HY_BAT_LEVEL4);
    RI_Read_DAT_DG_HY_BAT_LEVEL5(&DAT_DG_HY_BAT_LEVEL5);
    RI_Read_DAT_DG_HY_BAT_LEVEL6(&DAT_DG_HY_BAT_LEVEL6);
    RI_Read_DAT_DG_HY_BAT_LEVEL7(&DAT_DG_HY_BAT_LEVEL7);
    RI_Read_DAT_DG_HY_BAT_LEVEL8(&DAT_DG_HY_BAT_LEVEL8);
    RI_Read_DAT_DG_HY_BAT_LEVEL9(&DAT_DG_HY_BAT_LEVEL9);
    RI_Read_DAT_DG_HY_BAT_HYST_UP(&DAT_DG_HY_BAT_HYST_UP);
    RI_Read_DAT_DG_HY_BAT_HYST_DOWN(&DAT_DG_HY_BAT_HYST_DOWN);

//      _j_Req_Identifier :  SysRS16_Hybrid-1302
//      When TRACTION_MODE is in STATE_HY_BAT_LEVEL_NOMINAL state then:
    if(States1018 == STATE_HY_BAT_LEVEL_NOMINAL)
    {
        if((VAR_HY_BAT_SEUIL_MIN <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < VAR_HY_BAT_SEUIL_MAX))
        {
            //Do nothing
        }
        else
        {
            if ((DAT_DG_HY_BAT_LEVEL0 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL1))  {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL0) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL1 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_0; }
            else if ((DAT_DG_HY_BAT_LEVEL1 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL2)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL1) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL2 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_1; }
            else if ((DAT_DG_HY_BAT_LEVEL2 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL3)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL2) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL3 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_2; }
            else if ((DAT_DG_HY_BAT_LEVEL3 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL4)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL3) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL4 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_3; }
            else if ((DAT_DG_HY_BAT_LEVEL4 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL5)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL4) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL5 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_4; }
            else if ((DAT_DG_HY_BAT_LEVEL5 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL6)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL5) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL6 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_5; }
            else if ((DAT_DG_HY_BAT_LEVEL6 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL7)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL6) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL7 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_6; }
            else if ((DAT_DG_HY_BAT_LEVEL7 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL < DAT_DG_HY_BAT_LEVEL8)) {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL7) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL8 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_7; }
            else if ((DAT_DG_HY_BAT_LEVEL8 <= DAT_LI_HY_BAT_LEVEL) && (DAT_LI_HY_BAT_LEVEL <= DAT_DG_HY_BAT_LEVEL9))    {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL8) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL9 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_8; }
            //schougul: 17V6 update
//          else if ((Invalid_HY_BAT_LEVEL == DAT_LI_HY_BAT_LEVEL))                                                 {(VAR_HY_BAT_SEUIL_MIN = (sint8)((sint8)DAT_DG_HY_BAT_LEVEL0) + (sint8)((sint8)DAT_DG_HY_BAT_HYST_DOWN - cHystDownOffset));   VAR_HY_BAT_SEUIL_MAX =  (uint8)( DAT_DG_HY_BAT_LEVEL1 + DAT_DG_HY_BAT_HYST_UP);   ST_HY_BAT_LEVEL = HyBatLevel_Inv; }
            else {}
        }
    }
}



//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void ManageTractionModeState(void)
{
    //- STATE_HY_BAT_LEVEL : Configuration of the BATTERY LEVEL mode
    //- STATE_HY_BAT_LEVEL_WITHOUT_HYST : The BATTERY LEVEL mode is managed without filter
    //- STATE_HY_BAT_LEVEL_WITH_HYST : The BATTERY LEVEL mode is managed with filter
    //- STATE_HY_BAT_LEVEL_INVALID : The BATTERY LEVEL mode is managed in case Invalid (no filter)
    //- STATE_HY_BAT_LEVEL_NOMINAL : The BATTERY LEVEL mode is managed with filter
    switch(States1018)
    {
        case STATE_HY_BAT_LEVEL                 :
            States1018 = STATE_HY_BAT_LEVEL_WITHOUT_HYST;
            break;

        case STATE_HY_BAT_LEVEL_WITHOUT_HYST    :
            if(ch(cDAT_LI_HY_BAT_LEVEL) == True)
            {
                States1018 = STATE_HY_BAT_LEVEL_WITH_HYST;
            }
            ManageStateHYBatLevelWithoutHyst();
            break;

        case STATE_HY_BAT_LEVEL_WITH_HYST       :
            if(DAT_LI_HY_BAT_LEVEL == Invalid_HY_BAT_LEVEL)
            {
                States1018 = STATE_HY_BAT_LEVEL_INVALID;
            }
            else
            {
                States1018 = STATE_HY_BAT_LEVEL_NOMINAL;
            }
            break;

        case STATE_HY_BAT_LEVEL_INVALID         :

            //  _j_Req_Identifier :  SysRS16_Hybrid-1076
            //  When TRACTION_MODE is in STATE_HY_BAT_LEVEL_INVALID state then the function is managed as panel below:
                //schougul: 17V6 update

                ST_HY_BAT_LEVEL      = HyBatLevel_Inv;
                //ajoshi8: BL_P2Q_D5.2.1.1 Update : SysRS16_Hybrid-1018 -> While in STATE_HY_BAT_LEVEL_INVALID, if input becomes valid, we need to first go back to STATE_HY_BAT_LEVEL_WITHOUT_HYST
                if(ch(cDAT_LI_HY_BAT_LEVEL) == True)
				{
					if(DAT_LI_HY_BAT_LEVEL != Invalid_HY_BAT_LEVEL)
					{
						States1018 = STATE_HY_BAT_LEVEL_WITHOUT_HYST;
					}
				}
            break;

        case STATE_HY_BAT_LEVEL_NOMINAL         :
            if(ch(cDAT_LI_HY_BAT_LEVEL) == True)
            {
                States1018 = STATE_HY_BAT_LEVEL_WITH_HYST;
            }
            if(DAT_LI_HY_BAT_LEVEL == Invalid_HY_BAT_LEVEL)
            {
                States1018 = STATE_HY_BAT_LEVEL_INVALID;
            }

            Manage_STATE_HY_BAT_LEVEL_NOMINAL();
            break;

        default :
            JCIAssert(cFalse);
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         functionality once the Component is initialized and started.
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void state_Started(void)
{
    ComExtSignalStateType ComExtState;
    StateEngineStateType  INFO_GAUGE_ON=0U;
    Boolean  GAUGE_BATTERY_SOC_DIGITAL=0U;
    UInt16 DAT_LI_HY_VITV = 0U;
    static uint8 DISPLAY_SETTINGS=0U;
    MdlHybDataType  MdlHybData={0};
    MdlCarModePanelModesDataType MdlCarModePanelModesData;
    Boolean GAUGE_ENERGIE_CLIM_ELEC_DIGITAL=0U;
    UInt16 INFO_GAUGE_HYBRIDE_POWER=0U;
    uint8 INFO_GAUGE_BATTERY_SOC=0U;
    uint8 INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL=0U;
    uint8 ST_HY_STATE_TH_ENG = 0U;
    uint8 ST_HY_STATE_ELEC_ENG =0U;

    //===================================================================================================================
    //===================================================================================================================
    //===================================================================================================================
    //GET VALUES
    //  INFO_GAUGE_ON : TODO : Take it from CARMODES

    DAT_DG_HY_PRESENT_PREV = DAT_DG_HY_PRESENT;
    RI_Get_HY_PRESENT(&DAT_DG_HY_PRESENT);

    DAT_LI_HY_BAT_LEVEL_PREV=DAT_LI_HY_BAT_LEVEL;
    RI_Get_DAT_LI_HY_BAT_LEVEL(&DAT_LI_HY_BAT_LEVEL,&ComExtState);
    //CCOV : schougul: It is not possible to cover below condition as false, because address of local variable can not be pass as NULL.
    RI_Read_MdlCarModePanelModesData(&MdlCarModePanelModesData);
    INFO_GAUGE_ON = MdlCarModePanelModesData.InfoIsGaugeOn;


    //===================================================================================================================
    //===================================================================================================================
    //===================================================================================================================
    //_j_Req_Identifier :  SysRS16_Hybrid-875
    //When ACTIVATION_STATES is in STATE_GAUGE_IDLE state (i.e. INFO_GAUGE_ON = False) then, for the “Displaying of the hybrid power” :
    //
    //INFO_GAUGE_HYBRIDE_POWER := “Disable”
    //_j_Req_Identifier :  SysRS16_Hybrid-876
    //When ACTIVATION_STATES is in STATE_GAUGE_ACTIVE state (i.e. INFO_GAUGE_ON = True) then, for the “Displaying of the hybrid power” :
    //
    //INFO_GAUGE_HYBRIDE_POWER := DAT_LI_GAUGE_HYBRIDE_POWER
    if(INFO_GAUGE_ON == cFalse)
    {
        INFO_GAUGE_HYBRIDE_POWER = cInfoGaugeHybridPower_Disable;
    }
    else
    {
        RI_Get_DAT_LI_GAUGE_HYBRIDE_POWER(&INFO_GAUGE_HYBRIDE_POWER,&ComExtState);
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-867
    //  When ACTIVATION_STATES is in STATE_GAUGE_ACTIVE state (i.e. INFO_GAUGE_ON = True) then, for the “Displaying of the hybride power” :
    //
    //  For Each XXX =
    //          POINTER_0
    //          POINTER_1
    //          POINTER_2
    //          POINTER_3
    //          POINTER_INVALID
    //          POINTER_UNAVAILABLE
    //
    //  INFO_GAUGE_HYBRIDE_POWER_XXX  :=  DAT_DG_GAUGE_HYBRIDE_POWER_XXX
    if(INFO_GAUGE_ON == True)
    {
        RI_Get_GAUGE_HYBRIDE_POWER_POINTER_0           ((&GAUGE_HYBRIDE_POWER_POINTER_0));
        RI_Get_GAUGE_HYBRIDE_POWER_POINTER_1           ((&GAUGE_HYBRIDE_POWER_POINTER_1));
        RI_Get_GAUGE_HYBRIDE_POWER_POINTER_2           ((&GAUGE_HYBRIDE_POWER_POINTER_2));
        RI_Get_GAUGE_HYBRIDE_POWER_POINTER_3           ((&GAUGE_HYBRIDE_POWER_POINTER_3));
        RI_Get_GAUGE_HYBRIDE_POWER_POINTER_INVALID     ((&GAUGE_HYBRIDE_POWER_POINTER_INVALID));
        RI_Get_GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE ((&GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE));
    }

    //schougul: Below requirement is removed in 17V6 spec update
    //  _j_Req_Identifier :  SysRS16_Hybrid-868
    //  When ACTIVATION_STATES is in STATE_GAUGE_ACTIVE state (i.e. INFO_GAUGE_ON = True) then, for the “Displaying of the hybrid power” :
    //
    //  INFO_GAUGE_HYBRIDE_TAQUET := DAT_DG_GAUGE_HYBRIDE_TAQUET
    //  if(INFO_GAUGE_ON == True)
    //  {
    //      RI_Get_GAUGE_HYBRIDE_TAQUET((UInt8 *)(&GAUGE_HYBRIDE_TAQUET));
    //  }

    //  _j_Req_Identifier :  SysRS16_Hybrid-874
    //  If
    //      DAT_DG_GAUGE_BATTERY_SOC_DIGITAL= Absence
    //  Then
    //      INFO_GAUGE_BATTERY_SOC := Disable
    //  Else
    //      The gauge will be defined as in the table below:

    //  DAT_LI_GAUGE_BATTERY_SOC    INFO_GAUGE_BATTERY_SOC
    //  0 to 100%                   0 to 100%
    //  Invalid                     Invalid
    RI_Get_GAUGE_BATTERY_SOC_DIGITAL(&GAUGE_BATTERY_SOC_DIGITAL);
    if (GAUGE_BATTERY_SOC_DIGITAL == Absence)
    {
        INFO_GAUGE_BATTERY_SOC = cInfoGaugeBattarySoc_Disable;
    }
    else
    {
        RI_Get_DAT_LI_GAUGE_BATTERY_SOC(&INFO_GAUGE_BATTERY_SOC,&ComExtState);
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-1659
    //  When ACTIVATION_STATES is in STATE_GAUGE_ACTIVE state then, for the “Displaying of socket side” :
    //  INFO_GAUGE_SOCKET_SIDE :=  DAT_DG_SOCKET_SIDE
    if(INFO_GAUGE_ON == True)
    {
        RI_Get_SOCKET_SIDE(&INFO_GAUGE_SOCKET_SIDE);
    }

    //  _j_Req_Identifier :  SysRS16_Hybrid-1015
    if(FirstTransition==cFalse)
    {
        FirstTransition=cTrue;
        if(DAT_DG_HY_PRESENT == Present)
        {
            DISPLAY_SETTINGS = STATE_HY_ACTIVE;
            ST_HY_PRESENT_CFG = Present;
        }
        else
        {
            DISPLAY_SETTINGS = STATE_HY_INACTIVE;
            ST_HY_PRESENT_CFG = NotPresent;
        }
    }
    else if(ch(cDAT_DG_HY_PRESENT) == True)
    {
        if(DAT_DG_HY_PRESENT == Present)
        {
            DISPLAY_SETTINGS = STATE_HY_ACTIVE;
            ST_HY_PRESENT_CFG = Present;
        }
        else
        {
            DISPLAY_SETTINGS = STATE_HY_INACTIVE;
            ST_HY_PRESENT_CFG = NotPresent;
        }
    }

    //  2.4.4   Traction mode states
    //
    //schougul: Below requirement is removed in 17V6 spec update
    //  _j_Req_Identifier :  SysRS16_Hybrid-1016
    //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
    //
    //  ST_HY_RECHARGE := DAT_LI_HY_RECHARGE

    /*if(DISPLAY_SETTINGS == STATE_HY_ACTIVE )
    {
        RI_Get_DAT_LI_HY_RECHARGE(&ST_HY_RECHARGE,&ComExtState);
    }*/

    //  _j_Req_Identifier :  SysRS16_Hybrid-1642
    //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
    //
    //  If
    //      DAT_LI_HY_VITV = “0” or “Invalid”
    //  Then
    //      INFO_DRIVING_CAR := “False”
    //  Else
    //      INFO_DRIVING_CAR := “True”
    //  Endif


    //  _j_Req_Identifier :  SysRS16_Hybrid-1017
    //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
    //
    //  ST_HY_STATE_ELEC_AV_ENG := DAT_LI_HY_STATE_ELEC_AV_ENG
    RI_Get_DAT_LI_HY_VITV(&DAT_LI_HY_VITV,&ComExtState);
    if(DISPLAY_SETTINGS == STATE_HY_ACTIVE )
    {
        if(( DAT_LI_HY_VITV == 0U )|| ( DAT_LI_HY_VITV == cInfoHySpeedInvalidValue))
        {
            INFO_DRIVING_CAR = cFalse;
        }
        else
        {
            INFO_DRIVING_CAR = cTrue;
        }
        RI_Get_DAT_LI_HY_STATE_ELEC_AV_ENG(&ST_HY_STATE_ELEC_AV_ENG,&ComExtState);

        //  _j_Req_Identifier :  SysRS16_Hybrid-1018
        //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state then the TRACTION_MODE function is managed as diagram below:
        ManageTractionModeState();

        //  _j_Req_Identifier :  SysRS16_Hybrid-1089
        //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
        //
        //  ST_HY_AUTONOMIE_ZEV := DAT_LI_HY_AUTONOMIE_ZEV
        RI_Get_DAT_LI_HY_AUTONOMIE_ZEV(&ST_HY_AUTONOMIE_ZEV,&ComExtState);
        //Invalid value check performed below
        if(ST_HY_AUTONOMIE_ZEV == cHyAutonomieZev_Invalid)
        {
        	ST_HY_AUTONOMIE_ZEV = cInfoHyAutonomieZev_Invalid;
        }
        else
        {
            //Resolution : 2
            ST_HY_AUTONOMIE_ZEV =ST_HY_AUTONOMIE_ZEV*2U;
        }

        //  _j_Req_Identifier :  SysRS16_Hybrid-1090
        //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
        //
        //  ST_HY_MTH := DAT_DG_HY_MTH
        RI_Get_HY_MTH(&ST_HY_MTH);

        //  _j_Req_Identifier :  SysRS16_Hybrid-1091
        //  When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
        //
        //  ST_HY_MEL_AR := DAT_DG_HY_MEL_AR
        RI_Get_HY_MEL_AR(&ST_HY_MEL_AR);

//      _j_Req_Identifier :  SysRS16_Hybrid-1864
//      When DISPLAY_SETTINGS is in STATE_HY_ACTIVE state, then :
//
//      ST_HY_STATE_ELEC_ENG := DAT_LI_HY_STATE_ELEC_ENG
        RI_Get_DAT_LI_HY_STATE_ELEC_ENG(&ST_HY_STATE_ELEC_ENG,&ComExtState);
    }

    //  2.5.7   Displaying of the Digital level of power consumption of accessories
    //
    //  _j_Req_Identifier :  SysRS16_Hybrid-1583
    //  When ACTIVATION_STATES is in STATE_GAUGE_IDLE state then, for the “Displaying of the power consumption of thermal confort accessories”:
    //
    //  INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL := Disable

    //_j_Req_Identifier :  SysRS16_Hybrid-1584
    //When ACTIVATION_STATES is in STATE_GAUGE_ACTIVE state then, for the “Displaying of the power consumption of thermal confort accessories”:
    //If
    //DAT_DG_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL = Absence
    //Then
    //INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL := Disable
    //Else
    //INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL := DAT_LI_GAUGE_ENERGIE_CLIM_ELEC
    RI_Get_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL(&GAUGE_ENERGIE_CLIM_ELEC_DIGITAL);
    if(INFO_GAUGE_ON == cFalse)
    {
        INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL = InfoGaugeEnergieClimElecDigital_Disabled;
    }
    else
    {
        if(GAUGE_ENERGIE_CLIM_ELEC_DIGITAL == cFalse)
        {
            INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL = InfoGaugeEnergieClimElecDigital_Disabled;
        }
        else
        {
            RI_Get_DAT_LI_GAUGE_ENERGIE_CLIM_ELEC(&INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL,&ComExtState);
            if(
            //(InfoGaugeEnergieClimElecDigital_Min > INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL) || //cannot keep this condition because value of INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL will not be below zero
            (INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL > InfoGaugeEnergieClimElecDigital_Max))
            {
            	INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL = InfoGaugeEnergieClimElecDigital_InvalidValue;
            }
        }
    }

    Charging_Page();

    //  2.4.5   Histogram of consumption
    //
     ConsumptionHistogram();


//  _j_Req_Identifier :  SysRS16_Hybrid-1837
//  The output signal ST_HY_STATE_TH_ENG has fixed value:
//  ST_HY_STATE_TH_ENG := Not activated;
    ST_HY_STATE_TH_ENG = HyStateThEngStatus_NotActivated;

    //  2.5.1   ECO mode
    //
    //  _j_Req_Identifier :  SysRS16_Hybrid-869
    //  For turning ON/OFF ECO mode see [R3]
    //  ajoshi8: The assumption Note inside SysRS16_Hybrid-869 has been removed. Hence as per this, now there is no need to zero out the Hyb data being sent to HMI
    //  while entering ECO mode or in ECO mode. This assumption note was present until BL_P2Q_D5.1.1.0 & it was removed in BL_P2Q_D5.2.1.1 of SysRS16_Display_Hybrid_Information
    //
    //  2.5.2   Black panel mode
    //
    //  _j_Req_Identifier :  SysRS16_Hybrid-870
    //  See [R3]
    //
    //  Assumption (K.Vasilev):
    //  Implementation shall copy Black panel behavior from SysRS03_Display_fuel_level.
    //ajoshi8: Fix for Defect 476611 -> Based on the assumption note in SysRS16_Hybrid-869, the 'Hyb' data sent to HMI was zeroed during entering ECO mode or in ECO mode.
    //Now this assumption note is removed. Hence not zeroing out the entire 'Hyb' data being sent to HMI while entering ECO mode or in ECO mode.
    //Therefore, not checking for 'EcoMode_Requested' or 'EcoMode_Activated' values.

        //  _j_Req_Identifier :  SysRS16_Hybrid-873
        //  The requirements of the "Management of processed invalid values" section in document [R2] are applicable for the parameters and the values defined below.
        //
        //  Input:
        //  ID_ETAT_ARTIV_ET_HY\\PUISS_PTRAC
        //
        //  Output:
        //  DAT_LI_GAUGE_HYBRIDE_POWER := 0x1FF (invalid)
    	if(INFO_GAUGE_HYBRIDE_POWER > cInfoGaugeHybridPower_MaxValue)
    	{
    		if(cInfoGaugeHybridPower_Disable == INFO_GAUGE_HYBRIDE_POWER)
    		{
    		    if(INFO_GAUGE_ON == True)
    		    {
    		    	MdlHybData.InfoGaugeHybridPower=(SInt16)cInfoGaugeHybridPower_Invalid;
    		    }
    		    else
    		    {
    		    	MdlHybData.InfoGaugeHybridPower=INFO_GAUGE_HYBRIDE_POWER;
    		    }
    		}
    		else if(cInfoGaugeHybridPower_Unavailable == INFO_GAUGE_HYBRIDE_POWER)
    		{
    			MdlHybData.InfoGaugeHybridPower=(SInt16)cInfoGaugeHybridPower_Unavailable;
    		}
    		else
    		{
    			MdlHybData.InfoGaugeHybridPower=(SInt16)cInfoGaugeHybridPower_Invalid;
    		}

    	}
    	else
    	{
            MdlHybData.InfoGaugeHybridPower                    = ((SInt16)((((SInt16)INFO_GAUGE_HYBRIDE_POWER)/2)-  cFifty));
        }
        MdlHybData.InfoGaugeHybridPowerPointer0            = ((SInt16)(((SInt16)GAUGE_HYBRIDE_POWER_POINTER_0) - cFifty));
        MdlHybData.InfoGaugeHybridPowerPointer1            = ((SInt16)(((SInt16)GAUGE_HYBRIDE_POWER_POINTER_1) - cFifty));
        MdlHybData.InfoGaugeHybridPowerPointer2            = ((SInt16)(((SInt16)GAUGE_HYBRIDE_POWER_POINTER_2) - cFifty));
        MdlHybData.InfoGaugeHybridPowerPointer3            = ((SInt16)(((SInt16)GAUGE_HYBRIDE_POWER_POINTER_3) - cFifty));
        MdlHybData.InfoGaugeHybridPowerPointerInvalid      = ((SInt16)(((SInt16)GAUGE_HYBRIDE_POWER_POINTER_INVALID) - cSeventy));
        MdlHybData.InfoGaugeHybridPowerPointerUnavailable  = ((SInt16)(((SInt16)GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE) - cSeventy));
        //schougul: removed in 17V6 spec update
        //  MdlHybData.InfoGaugeHybrideTaquet           = GAUGE_HYBRIDE_TAQUET;

        MdlHybData.InfoGaugeBattarySoc  = INFO_GAUGE_BATTERY_SOC;
        MdlHybData.InfoHyAutonomieZev   = ST_HY_AUTONOMIE_ZEV;
        MdlHybData.InfoHyConsoZevTrip1   = ST_HY_CONSO_ZEV_TRIP1;
        MdlHybData.InfoHyConsoZevTrip2   = ST_HY_CONSO_ZEV_TRIP2;
        MdlHybData.InfoHyConsoZevInst    = ST_HY_CONSO_ZEV_INST;
        //schougul: removed in 17V6 spec update
        //  MdlHybData.InfoHyRecharge = ST_HY_RECHARGE;
        MdlHybData.InfoHyBatLevel = ST_HY_BAT_LEVEL;
        MdlHybData.InfoHyStateElecAvEng = ST_HY_STATE_ELEC_AV_ENG;
        MdlHybData.InfoIsHyMthPresent = ST_HY_MTH;
        MdlHybData.InfoIsHyMelArPreset = ST_HY_MEL_AR;
        MdlHybData.InfoHyPresentCfg = ST_HY_PRESENT_CFG;

        MdlHybData.InfoOnOffStatusCharge = INFO_ONOFF_STATUS_CHARGE;
        MdlHybData.InfoGaugeSocketSide = INFO_GAUGE_SOCKET_SIDE;
        MdlHybData.InfoOnOffStatusPageCharge = INFO_ONOFF_STATUS_PAGE_CHARGE;
        MdlHybData.InfoDrivingCar = INFO_DRIVING_CAR;
        MdlHybData.InfoGaugeEnergieClimElecDigital = INFO_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL;

        //resolution of 2km applied below
        //Defect 321206:Range check for invalid values added
        if(((uint16)INFO_ONOFF_CHARGE_RATE * 2U) > InfoOnOffChargeRate_Max )
        {
        	MdlHybData.InfoOnOffChargeRate = InfoOnOffChargeRate_Unavailable;
        }
        else
        {
        	MdlHybData.InfoOnOffChargeRate = (uint16)((uint16)INFO_ONOFF_CHARGE_RATE * 2U) ;
        }
        MdlHybData.InfoOnOffRechargeHDebut = INFO_ONOFF_RECHARGE_H_DEBUT;
        MdlHybData.InfoOnOffRechargeMinDebut = INFO_ONOFF_RECHARGE_MIN_DEBUT;
        MdlHybData.InfoOnOffChargeRemainingTimeH = INFO_ONOFF_CHARGE_REMAINING_TIME_H;
        MdlHybData.InfoOnOffChargeRemainingTimeMin = INFO_ONOFF_CHARGE_REMAINING_TIME_MIN;
        MdlHybData.InfoHyStateThEngStatus = ST_HY_STATE_TH_ENG;
        MdlHybData.InfoHyStateElecEng = ST_HY_STATE_ELEC_ENG;


#ifdef UNIT_TEST
    VfbMDL_HYBppSR_ppMDLHyb_TIMDLHybTIMDLHybData=MdlHybData;
#else
    RI_Write_MDL_Hyb(&MdlHybData);
#endif

    //  2.5.4   Frame loss
    //
    //  _j_Req_Identifier :  SysRS16_Hybrid-872
    //  When INFO_STATE_UNIT_ACTIVE = On:
    //  If the frame ID_ETAT_ARTIV_ET_HY is confirmed lost, then the chapter "Loss of frame confirmation" of the document [R2] shall be applied to the frame ID_ETAT_ARTIV_ET_HY.
    //DONE  IN SRV COM EXT
}
//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Diagnostic State functionality
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static void state_Diag(void)
{
    /* add code to be executed while Started diag state is active */
}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Transition to Init state
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static eTransitionResult trans_ToInit(void)
{

    return etrSuccessful;
}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Transition to Started state
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static eTransitionResult trans_ToStarted(void)
{
    //  2.5.3   Initialization
    //
    //  _j_Req_Identifier :  SysRS16_Hybrid-871
    //  Each time the cluster goes in the NORMAL state (INFO_STATE_UNIT_ACTIVE = On),
    //  Then the chapter "Initialization Phase" from document [R2] shall be applied, with the following values:
    //  DAT_LI_GAUGE_HYBRIDE_POWER:
    //  CONS_INIT := 0x1FE
     DAT_DG_HY_PRESENT=0U;
     DAT_DG_HY_PRESENT_PREV=0U;
     DAT_LI_HY_BAT_LEVEL=0U;
     DAT_LI_HY_BAT_LEVEL_PREV=0U;
     FirstTransition=0U;
     VAR_HY_BAT_SEUIL_MIN=(sint8)0;
     VAR_HY_BAT_SEUIL_MAX=0U;
     ST_HY_BAT_LEVEL=0U;
     GAUGE_HYBRIDE_POWER_POINTER_0=0U;
     GAUGE_HYBRIDE_POWER_POINTER_1=0U;
     GAUGE_HYBRIDE_POWER_POINTER_2=0U;
     GAUGE_HYBRIDE_POWER_POINTER_3=0U;
     GAUGE_HYBRIDE_POWER_POINTER_INVALID=0U;
     GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE=0U;
//   GAUGE_HYBRIDE_TAQUET=0U;
     ST_HY_AUTONOMIE_ZEV=0U;
//   ST_HY_RECHARGE=0U;
     ST_HY_MTH=0U;
     ST_HY_MEL_AR=0U;
     ST_HY_CONSO_ZEV_TRIP1=0U;
     ST_HY_CONSO_ZEV_TRIP2=0U;
     ST_HY_CONSO_ZEV_INST=0U;
     ST_HY_STATE_ELEC_AV_ENG=0U;
     ST_HY_PRESENT_CFG=0U;
     INFO_GAUGE_SOCKET_SIDE=0U;

    return etrSuccessful;
}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Transition to Diagnostic state
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static eTransitionResult trans_ToDiag(void)
{

    return etrSuccessful;
}

//----------------------------------------------------------------------------------------------------------------------
// DESCRIPTION:         Transition to Stopped state
//
// PARAMETERS:          None
//
// RETURN VALUE:        None
//
// DESIGN INFORMATION:  None.
//----------------------------------------------------------------------------------------------------------------------
static eTransitionResult trans_ToStopped(void)
{
    return etrSuccessful;
}



//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================
//------------------------------------------------------------------------------
/// DESCRIPTION:         Requests a system event to be processed by the component.
///
/// PARAMETERS:          \param eventP: unique system event identifier
///                      \param paramP: an eventP-specific parameter (0 - if unused)
///
/// RETURN VALUE:        \return standard result codes apply
//------------------------------------------------------------------------------
Std_ReturnType MDL_HYB_OnSysMsg(const eSysMsgType eventP,
                                const tSysMsgParamType paramP)
{

    /* finally delegate all other events to BaseCmp library implementation */
    return LIB_BaseCmp_OnSysMsg(This(MDL_Hyb), eventP, paramP);
}

//------------------------------------------------------------------------------
/// DESCRIPTION:         Used to execute the main functionality of the component.
///                      The task call is done from the OS/System StateManager.
///                      The scheduling mechanism is defined by the OS
///                      configuration dependent on the needs of
///                      the individual components.
///
/// PARAMETERS:          \param None
///
/// RETURN VALUE:        \return None
//------------------------------------------------------------------------------
void MDL_Hyb_Task( void )
{
    LIB_BaseCmp_Task(This(MDL_Hyb));
}


