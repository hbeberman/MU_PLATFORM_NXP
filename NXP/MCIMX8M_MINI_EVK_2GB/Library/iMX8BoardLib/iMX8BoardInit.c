/** @file
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Ppi/ArmMpCoreInfo.h>

#include "iMX8.h"
#include "iMX8ClkPwr.h"
#include "iMX8MMiniIoMux.h"

ARM_CORE_INFO iMX8Ppi[] =
{
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value.
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },

#if FixedPcdGet32(PcdCoreCount) > 1
  {
    // Cluster 0, Core 1
    0x0, 0x1,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
#endif // FixedPcdGet32(PcdCoreCount) > 1

#if FixedPcdGet32(PcdCoreCount) > 2
  {
    // Cluster 0, Core 2
    0x0, 0x2,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },

  {
    // Cluster 0, Core 3
    0x0, 0x3,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  }
#endif // FixedPcdGet32(PcdCoreCount) > 2
};

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  // Only support one cluster
  *CoreCount    = sizeof(iMX8Ppi) / sizeof(ARM_CORE_INFO);
  ASSERT (*CoreCount == FixedPcdGet32 (PcdCoreCount));
  *ArmCoreTable = iMX8Ppi;
  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof(gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}

typedef enum {
  IMX_PAD_SAI3_MCLK_SAI3_MCLK = _IMX_MAKE_PADCFG (
                                IMX_DSE_6_X6,
                                IMX_FSEL_2_FAST,
                                IMX_ODE_0_Open_Drain_Disabled,
                                IMX_PUE_1_Pull_Up_Enabled,
                                IMX_HYS_ENABLED,
                                IMX_PE_1_Pull_Resistors_Enabled,
                                IMX_SION_DISABLED,
                                IMX_IOMUXC_SAI3_MCLK_ALT0_SAI3_MCLK),

  IMX_PAD_SAI3_TXFS_SAI3_TX_SYNC = _IMX_MAKE_PADCFG (
                                   IMX_DSE_6_X6,
                                   IMX_FSEL_2_FAST,
                                   IMX_ODE_0_Open_Drain_Disabled,
                                   IMX_PUE_1_Pull_Up_Enabled,
                                   IMX_HYS_ENABLED,
                                   IMX_PE_1_Pull_Resistors_Enabled,
                                   IMX_SION_DISABLED,
                                   IMX_IOMUXC_SAI3_TXFS_ALT0_SAI3_TX_SYNC),

  IMX_PAD_SAI3_TXC_SAI3_TX_BCLK =  _IMX_MAKE_PADCFG (
                                  IMX_DSE_6_X6,
                                  IMX_FSEL_2_FAST,
                                  IMX_ODE_0_Open_Drain_Disabled,
                                  IMX_PUE_1_Pull_Up_Enabled,
                                  IMX_HYS_ENABLED,
                                  IMX_PE_1_Pull_Resistors_Enabled,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_SAI3_TXC_ALT0_SAI3_TX_BCLK),

  IMX_PAD_SAI3_TXD_SAI3_DATA0 = _IMX_MAKE_PADCFG (
                                 IMX_DSE_6_X6,
                                 IMX_FSEL_2_FAST,
                                 IMX_ODE_0_Open_Drain_Disabled,
                                 IMX_PUE_1_Pull_Up_Enabled,
                                 IMX_HYS_ENABLED,
                                 IMX_PE_1_Pull_Resistors_Enabled,
                                 IMX_SION_DISABLED,
                                 IMX_IOMUXC_SAI3_TXD_ALT0_SAI3_TX_DATA0),
//Mute pin
  IMX_PAD_I2C4_SCL_GPIO5_IO20 = _IMX_MAKE_PADCFG (
                                  IMX_DSE_6_X6,
                                  IMX_FSEL_2_FAST,
                                  IMX_ODE_0_Open_Drain_Disabled,
                                  IMX_PUE_1_Pull_Up_Enabled,
                                  IMX_HYS_ENABLED,
                                  IMX_PE_1_Pull_Resistors_Enabled,
                                  IMX_SION_DISABLED,
                                  IMX_IOMUXC_I2C4_SCL_ALT5_GPIO5_IO20),
} IMX_AUDIO_PADCFG;

/**
  Initalize the Audio system
**/
VOID
AudioInit (VOID)
{
  EFI_STATUS status;

  // Mux the SAI2 pins to wm8524 codec
  ImxPadConfig (IMX_PAD_SAI3_MCLK, IMX_PAD_SAI3_MCLK_SAI3_MCLK);
  ImxPadConfig (IMX_PAD_SAI3_TXFS, IMX_PAD_SAI3_TXFS_SAI3_TX_SYNC);
  ImxPadConfig (IMX_PAD_SAI3_TXC, IMX_PAD_SAI3_TXC_SAI3_TX_BCLK);
  ImxPadConfig (IMX_PAD_SAI3_TXD, IMX_PAD_SAI3_TXD_SAI3_DATA0);

  // unmute audio
  ImxPadConfig (IMX_PAD_I2C4_SDA,  IMX_PAD_I2C4_SCL_GPIO5_IO20);
  ImxGpioDirection (IMX_GPIO_BANK1, 20, IMX_GPIO_DIR_OUTPUT);
  ImxGpioWrite (IMX_GPIO_BANK1, 20, IMX_GPIO_HIGH);

  // enable the AUDIO_PLL - 44,100 Hz * 256
  status = ImxSetSAI2ClockRate (11289600);
  if (EFI_ERROR (status)) {
    DebugPrint (DEBUG_ERROR, "AudioInit - ImxSetAudioMclkClockRate failed");
  }
}

typedef enum {

  IMX_PAD_I2C3_SDA_PWM3_OUT = _IMX_MAKE_PADCFG (
                                IMX_DSE_6_X6,
                                IMX_FSEL_2_FAST,
                                IMX_ODE_0_Open_Drain_Disabled,
                                IMX_PUE_1_Pull_Up_Enabled,
                                IMX_HYS_ENABLED,
                                IMX_PE_1_Pull_Resistors_Enabled,
                                IMX_SION_DISABLED,
                                IMX_IOMUXC_I2C3_SDA_ALT1_PWM3_OUT),

  IMX_PAD_I2C3_SCL_PWM4_OUT = _IMX_MAKE_PADCFG (
                                IMX_DSE_6_X6,
                                IMX_FSEL_2_FAST,
                                IMX_ODE_0_Open_Drain_Disabled,
                                IMX_PUE_1_Pull_Up_Enabled,
                                IMX_HYS_ENABLED,
                                IMX_PE_1_Pull_Resistors_Enabled,
                                IMX_SION_DISABLED,
                                IMX_IOMUXC_I2C3_SCL_ALT1_PWM4_OUT),

} IMX_PWM_PADCFG;

/**
  Initalize the PWM
**/
VOID
PwmInit (VOID)
{

  // Mux the I2C3 pins to PWM3 and PWM4 codec
  ImxPadConfig (IMX_PAD_I2C3_SDA, IMX_PAD_I2C3_SDA_PWM3_OUT);
  ImxPadConfig (IMX_PAD_I2C3_SCL, IMX_PAD_I2C3_SCL_PWM4_OUT);

}

/**
  Initialize controllers that must setup at the early stage
**/
RETURN_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  if (!ArmPlatformIsPrimaryCore (MpId)) {
    return RETURN_SUCCESS;
  }

  // Initialize debug serial port
  SerialPortInitialize ();
  SerialPortWrite (
    (UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG,
    (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));

  // Initialize peripherals
  ImxUngateActiveClock ();
  AudioInit ();
  PwmInit();

  return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform
**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (VOID)
{
  return BOOT_WITH_FULL_CONFIGURATION;
}
