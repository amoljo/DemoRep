#ifndef _QAC_

#endif  // #ifndef _QAC_

#include "Rte_MDL_HYB.h"

#ifdef UNIT_TEST
extern void Rte_Adapter_MDL_OBC_Read_InfoConsumUnit(ConsumUnitType *pInfoConsumUnit);
extern void Rte_Adapter_Read_INFO_HMI_STATE_ENGINE(StateEngineStateType *pStateEngine);
#endif

#define RI_Get_DAT_LI_HY_STATE_ELEC_ENG                 Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ETAT_MEL
#define RI_Get_DAT_LI_GAUGE_HYBRIDE_POWER               Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_GAUGE_HYBRIDE_POWER
#define RI_Get_GAUGE_HYBRIDE_POWER_POINTER_0            Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_POWER_POINTER_0
#define RI_Get_GAUGE_HYBRIDE_POWER_POINTER_1            Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_POWER_POINTER_1
#define RI_Get_GAUGE_HYBRIDE_POWER_POINTER_2            Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_POWER_POINTER_2
#define RI_Get_GAUGE_HYBRIDE_POWER_POINTER_3            Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_POWER_POINTER_3
#define RI_Get_GAUGE_HYBRIDE_POWER_POINTER_INVALID 		Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_POWER_POINTER_INVALID
#define RI_Get_GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE 	Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_POWER_POINTER_UNAVAILABLE
#define RI_Get_GAUGE_HYBRIDE_TAQUET						Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_HYBRIDE_TAQUET
#define RI_Get_GAUGE_BATTERY_SOC_DIGITAL				Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_BATTERY_SOC_DIGITAL
#define RI_Get_DAT_LI_GAUGE_BATTERY_SOC					Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_GAUGE_BATTERY_SOC
#define RI_Get_HY_PRESENT								Rte_Call_rpCS_rpSRVCfg_Settings_TISRVCfg_Settings_Read_HY_PRESENT
#define RI_Get_DAT_LI_HY_AUTONOMIE_ZEV					Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_AUTONOMIE_ZEV
#define RI_Get_DAT_LI_HY_BAT_LEVEL						Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_BAT_LEVEL
#define RI_Get_DAT_LI_HY_RECHARGE						Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_RECHARGE
#define RI_Get_DAT_LI_HY_STATE_ELEC_AV_ENG				Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_STATE_ELEC_AV_ENG
#define RI_Get_HY_MTH									Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_MTH
#define RI_Get_HY_MEL_AR								Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_MEL_AR
#define RI_Get_DAT_LI_HY_CONSO_ZEV_TRIP1				Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_CONSO_ZEV_TRIP1
#define RI_Get_ST_SET_CONSUM_UNIT						Rte_Adapter_MDL_OBC_Read_InfoConsumUnit
#define RI_Get_DAT_LI_HY_CONSO_ZEV_TRIP2				Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_CONSO_ZEV_TRIP2
#define RI_Get_DAT_LI_HY_CONSO_ZEV_INST					Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_CONSO_ZEV_INST
#define RI_Read_DAT_DG_HY_BAT_LEVEL0                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL0
#define RI_Read_DAT_DG_HY_BAT_LEVEL1                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL1
#define RI_Read_DAT_DG_HY_BAT_LEVEL2                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL2
#define RI_Read_DAT_DG_HY_BAT_LEVEL3                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL3
#define RI_Read_DAT_DG_HY_BAT_LEVEL4                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL4
#define RI_Read_DAT_DG_HY_BAT_LEVEL5                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL5
#define RI_Read_DAT_DG_HY_BAT_LEVEL6                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL6
#define RI_Read_DAT_DG_HY_BAT_LEVEL7                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL7
#define RI_Read_DAT_DG_HY_BAT_LEVEL8                    Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL8
#define RI_Read_DAT_DG_HY_BAT_LEVEL9					Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_LEVEL9
#define RI_Read_DAT_DG_HY_BAT_HYST_UP                   Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_HYST_UP
#define RI_Read_DAT_DG_HY_BAT_HYST_DOWN                 Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_HY_BAT_HYST_DOWN
#define RI_Read_MdlCarModePanelModesData				Rte_Read_rpSR_rpMDLCarModes_PanelModes_TIMDLCarModes_PanelModes_MdlCarModePanelModesData

#define RI_Write_MDL_Hyb								Rte_Write_ppSR_ppMDLHyb_TIMDLHyb_TIMDLHybData


#define RI_Get_DAT_LI_ONOFF_TYPE_RECHARGE				Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_TYPE_RECHARGE
#define RI_Get_DAT_LI_ONOFF_CHARGE_REMAINING_TIME		Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_CHARGE_REMAINING_TIME
#define RI_Get_DAT_LI_ONOFF_PLUGGED						Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_PLUGGED
#define RI_Get_DAT_LI_ONOFF_ON_UNIT						Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_ON_UNIT
#define RI_Get_DAT_LI_ONOFF_ECO_MODE					Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_ECO_MODE
#define RI_Get_DAT_LI_ONOFF_STATE_VHL_ELEC_SYSTEM		Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_STATE_VHL_ELEC_SYSTEM
#define RI_Get_DAT_LI_ONOFF_CHARGE_RATE					Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_CHARGE_RATE
#define RI_Get_DAT_LI_ONOFF_RECHARGE_DEBUT				Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_RECHARGE_DEBUT
#define RI_Get_SOCKET_SIDE								Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_SOCKET_SIDE
#define RI_Get_DAT_LI_HY_VITV							Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_HY_VITV
#define RI_Get_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL			Rte_Call_rpCS_rpSRVCfg_Hyb_TISRVCfg_Hyb_Read_GAUGE_ENERGIE_CLIM_ELEC_DIGITAL
#define RI_Get_DAT_LI_GAUGE_ENERGIE_CLIM_ELEC			Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_GAUGE_ENERGIE_CLIM_ELEC
#define RI_Get_DAT_LI_ONOFF_CHARGE_REMAINING_TIME		Rte_Call_rpCS_rpSRVComExt_Hyb_TISRVComExt_Hyb_Read_MDL_HYB_ONOFF_CHARGE_REMAINING_TIME

