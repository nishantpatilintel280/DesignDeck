//-----------------------------------------------------------------------------------------------------------
//
// INTEL CONFIDENTIAL
// Copyright (C) 2021-2024 Intel Corporation
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they were provided to you ("License").
// Unless the License provides otherwise, you may not use, modify, copy, publish,
// distribute, disclose or transmit this software or the related documents without
// Intel's prior written permission. This software and the related documents are provided
// as is, with no express or implied warranties, other than those that are expressly stated in the License.
//
//-----------------------------------------------------------------------------------------------------------



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "GenericDisplayInfoRoutines.c"          // NOLINT [build/include]
#include "DisplayInfoParser/GlobalTimings.c"     // NOLINT [build/include]
#include "DisplayInfoParser/DisplayInfoParser.c" // NOLINT [build/include]
#include "OfflineHeaderFiles/MemoryAllocation.h"

#define MAX_TIMINGS 200
/**
 * @brief
 *
 * @param  pInputTable
 * @param  pEntryToAdd
 * @param  ForceAdd
 * @return DDSTATUS
 */
DDSTATUS addEntry(void* pInputTable, const void* pEntryToAdd, DD_BOOL ForceAdd)
{
    void* pEntry = nullptr;
    uint32_t Count = 0;
    uint32_t IndexForAdd = 0;
    DDSTATUS Status;
    DD_TABLE* pTable = nullptr;

    DISP_FUNC_ENTRY();
    DDASSERT(pInputTable);

    pTable = static_cast<DD_TABLE*>(pInputTable);

    pEntry = pTable->pEntry;
    Status = DDS_UNSUCCESSFUL;
    // Check whether the mode already exists
    for (Count = 0; Count < pTable->NumEntries; Count++)
    {
        if (TRUE == pTable->pfnMatchEntry(pEntryToAdd, pEntry, FALSE))
        {
            if (FALSE == ForceAdd)
            {
                Status = DDS_SUCCESS_ENTRY_EXISTS_NO_UPDATE;
            }
            else
            {
                IndexForAdd = Count;
                Status = DDS_SUCCESS_ENTRY_EXISTS_REPLACED;
            }
            break;
        }
        pEntry = DD_VOID_PTR_INC(pEntry, pTable->EntrySizeInBytes);
    }

    if (DDS_SUCCESS_ENTRY_EXISTS_NO_UPDATE == Status)
    {
        goto _Exit;
    }
    else if (DDS_SUCCESS_ENTRY_EXISTS_REPLACED == Status)
    {
        pEntry = DD_VOID_PTR_INC(pTable->pEntry, (pTable->EntrySizeInBytes * IndexForAdd));
        if (pTable->pfnReplaceEntry)
        {
            pTable->pfnReplaceEntry(pEntry, pEntryToAdd);
            // ReplaceEntry Function might replace only if certain conditions are satisfied.
            // So we compare values of both entries to check if it is replaced and update Status accordingly
            if (0 == DD_MEM_CMP(pEntry, pEntryToAdd, sizeof(DD_TIMING_INFO)))
            {
                //EXPECT_EQ(ValidateTimingInfo(static_cast<DD_TIMING_INFO *>(pEntry)), TRUE);
            }
            else
            {
                Status = DDS_SUCCESS_ENTRY_EXISTS_NO_UPDATE;
            }
        }
        else
        {
            DD_MEM_COPY_SAFE(pEntry, pTable->EntrySizeInBytes, pEntryToAdd, pTable->EntrySizeInBytes);
            //EXPECT_EQ(ValidateTimingInfo(static_cast<DD_TIMING_INFO *>(pEntry)), TRUE);
        }
        goto _Exit;
    }

    if (pTable->TableSize == pTable->NumEntries)
    {
        // Status = expandTable(pTable);
        printf("\nError:  Num timings in EDID exceeded!!\n");
    }

    pEntry = DD_VOID_PTR_INC(pTable->pEntry, (pTable->NumEntries * pTable->EntrySizeInBytes));
    DD_MEM_COPY_SAFE(pEntry, pTable->EntrySizeInBytes, pEntryToAdd, pTable->EntrySizeInBytes);
    pTable->NumEntries++;

_Exit:
    DISP_FUNC_EXIT();

    return Status;
}


/**
 * @brief
 *
 *
 * @param  pTimingInfo
 * @return DD_MEDIA_RR_TYPE
 */
DD_MEDIA_RR_TYPE GetMediaRRType(const DD_TIMING_INFO* pTimingInfo)
{
    uint8_t RrList[] = { 24, 30, 60 };
    uint32_t IntRrRangeMin = 0, IntRrRangeMax = 0, Index = 0;
    DD_BOOL MatchFound = FALSE;
    DD_MEDIA_RR_TYPE RrType = DD_MEDIA_RR_NONE;
    unsigned long long IntDcRangeMin = 0, IntDcRangeMax = 0, OrigDC = 0, DerivedDC = 0;

    DISP_FUNC_WO_STATUS_ENTRY();

    // Find rounded integer RR
    OrigDC = pTimingInfo->DotClockInHz;
    if (TRUE == pTimingInfo->IsInterlaced)
    {
        OrigDC = OrigDC * 2;
    }
    for (Index = 0; Index < std::size(RrList); Index++)
    {
        if (RrList[Index] == pTimingInfo->VRoundedRR)
        {
            MatchFound = TRUE;
            break;
        }
    }
    if (FALSE == MatchFound)
    {
        goto _Exit;
    }

    IntRrRangeMin = RrList[Index] * 9995; // Precision Factor of 10000, (1 - 0.0005) * 10000 = 9995
    IntRrRangeMax = RrList[Index] * 10005; // Precision Factor of 10000, (1 + 0.0005) * 10000 = 10005
    OrigDC = OrigDC * 10000; // Adjust Precision Factor
    IntDcRangeMin = static_cast<unsigned long long>(pTimingInfo->HTotal) * pTimingInfo->VTotal * IntRrRangeMin;
    IntDcRangeMax = static_cast<unsigned long long>(pTimingInfo->HTotal) * pTimingInfo->VTotal * IntRrRangeMax;
    if ((OrigDC >= IntDcRangeMin) && (OrigDC <= IntDcRangeMax))
    {
        RrType = DD_MEDIA_RR_INTEGER;
        goto _Exit;
    }
    // if original dot clock is not integer then check if it is fractional...
    // we convert original dot clock (now assumed to be fractional),into integer dot clock
    // by multiplying it by 1.001 and then compare with allowed range for integer dot clock
    DerivedDC = (OrigDC * 1001) / 1000;
    if ((DerivedDC >= IntDcRangeMin) && (DerivedDC <= IntDcRangeMax))
    {
        RrType = DD_MEDIA_RR_FRACTIONAL;
        goto _Exit;
    }

_Exit:
    DISP_FUNC_WO_STATUS_EXIT();

    return RrType;
}

/**
 * @brief
 *
 * @param pInputMode1
 * @param pInputMode2
 * @param IsWireformatMatchNeeded
 * @return DD_BOOL
 */
DD_BOOL matchTargetMode(const void* pInputMode1, const void* pInputMode2, DD_BOOL IsWireformatMatchNeeded)
{
    const DD_TIMING_INFO* pMode1 = const_cast<DD_TIMING_INFO*>(static_cast<const DD_TIMING_INFO*>(pInputMode1));
    const DD_TIMING_INFO* pMode2 = const_cast<DD_TIMING_INFO*>(static_cast<const DD_TIMING_INFO*>(pInputMode2));

    if ((pMode1->HActive == pMode2->HActive) && (pMode1->VActive == pMode2->VActive) && (pMode1->VRoundedRR == pMode2->
        VRoundedRR) &&
        (GetMediaRRType(pMode1) == GetMediaRRType(pMode2)) && (pMode1->IsInterlaced == pMode2->IsInterlaced) &&
        (pMode1->CeData.SamplingMode.Rgb == pMode2->CeData.SamplingMode.Rgb) && (pMode1->CeData.SamplingMode.Yuv420 ==
            pMode2->CeData.SamplingMode.Yuv420))
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief
 *
 * @param  pModeToReplace
 * @param  pNewMode
 * @return void
 */
void replaceTargetMode(void* pModeToReplace, const void* pNewMode)
{
    DD_TIMING_INFO* pCurModeToReplace = static_cast<DD_TIMING_INFO*>(pModeToReplace);
    const DD_TIMING_INFO* pCurNewMode = const_cast<DD_TIMING_INFO*>(static_cast<const DD_TIMING_INFO*>(pNewMode));

    DDASSERT(pModeToReplace);
    DDASSERT(pNewMode);
    // Mode Should not be replaced if the existing mode is a preferred mode and the new mode is a non preferred mode

    if ((TRUE == pCurModeToReplace->Flags.PreferredMode) && (FALSE == pCurNewMode->Flags.PreferredMode))
    {
        return;
    }

    *pCurModeToReplace = *pCurNewMode;

    return;
}

/**
 * @brief
 *
 * @param  os
 * @param  pPnpId
 * @return ostream
 */
std::ostream& operator<<(std::ostream& os, const DD_PNP_ID pPnpId)
{
    os << std::hex;
    os << "      <PnpId ";
    os << "ManufacturerID = \"";
    os << "0x" << static_cast<uint16_t>(pPnpId.ManufacturerID[0]);
    os << " 0x" << static_cast<uint16_t>(pPnpId.ManufacturerID[1]) << "\"";
    os << " ProductID = \"";
    os << "0x" << static_cast<uint16_t>(pPnpId.ProductID[0]);
    os << " 0x" << static_cast<uint16_t>(pPnpId.ProductID[1]) << "\"";
    os << " SerialNumber = \"";
    os << "0x" << static_cast<uint16_t>(pPnpId.SerialNumber[0]);
    os << " 0x" << static_cast<uint16_t>(pPnpId.SerialNumber[1]);
    os << " 0x" << static_cast<uint16_t>(pPnpId.SerialNumber[2]);
    os << " 0x" << static_cast<uint16_t>(pPnpId.SerialNumber[3]) << "\"";
    os << " WeekOfManufacture = \"0x" << static_cast<uint16_t>(pPnpId.WeekOfManufacture) << "\"";
    os << " YearOfManufacture = \"0x" << static_cast<uint16_t>(pPnpId.YearOfManufacture) << "\"";
    os << "/>" << std::endl;
    os << std::dec;

    return os;
}

/**
 * @brief
 *
 *
 * @param  os
 * @param  pChromaLumaData
 * @return ostream
 */
std::ostream& operator<<(std::ostream& os, const DD_CHROMA_AND_LUMA_DATA pChromaLumaData)
{
    os << "<Chroma Data= \"";

    os << "0x" << pChromaLumaData.RedX;
    os << " 0x" << pChromaLumaData.RedY;
    os << " 0x" << static_cast<uint16_t>(pChromaLumaData.GreenX);
    os << " 0x" << static_cast<uint16_t>(pChromaLumaData.GreenY);
    os << " 0x" << static_cast<uint16_t>(pChromaLumaData.BlueX);
    os << " 0x" << static_cast<uint16_t>(pChromaLumaData.BlueY);
    os << " 0x" << static_cast<uint16_t>(pChromaLumaData.WhiteX);
    os << " 0x" << static_cast<uint16_t>(pChromaLumaData.WhiteY);

    os << "\"/>\n";
    return os;
}

/**
 * @brief
 *
 *
 * @param  &os
 * @param  CEA_861B_ADB
 * @return ostream&
 */
std::ostream& operator<<(std::ostream& os, const CEA_861B_ADB& Sad)
{
    os << "MaxChannels = \"" << static_cast<uint32_t>(Sad.MaxChannels) << "\" ";
    os << "AudioFormatCode = \"" << static_cast<uint32_t>(Sad.AudioFormatCode) << "\"";

    os << " _32kHzSupport = \"" << static_cast<uint32_t>(Sad._32kHz) << "\" ";
    os << " _44kHzSupport = \"" << static_cast<uint32_t>(Sad._44kHz) << "\" ";
    os << " _48kHzSupport = \"" << static_cast<uint32_t>(Sad._32kHz) << "\" ";
    os << " _88kHzSupport = \"" << static_cast<uint32_t>(Sad._48kHz) << "\" ";
    os << " _96kHzSupport = \"" << static_cast<uint32_t>(Sad._32kHz) << "\" ";
    os << " _176kHzSupport = \"" << static_cast<uint32_t>(Sad._88kHz) << "\" ";
    os << " _192kHzSupport = \"" << static_cast<uint32_t>(Sad._32kHz) << "\"" << std::endl;

    os << " _16BitSupport = \"" << static_cast<uint32_t>(Sad._16Bit) << "\" ";
    os << " _20BitSupport = \"" << static_cast<uint32_t>(Sad._20Bit) << "\" ";
    os << " _24BitSupport = \"" << static_cast<uint32_t>(Sad._24Bit) << "\" >" << std::endl;

    return os;
}

/**
 * @brief
 *
 *
 * @param  &os
 * @param  DD_EDID_AUDIO_CAPS
 * @return ostream&
 */
std::ostream& operator<<(std::ostream& os, const DD_EDID_AUDIO_CAPS& AudioCaps)
{
    uint32_t Count = 0;
    os << "    <CeAudioCaps ";
    os << "IsLpcmSadFound = \"" << static_cast<uint32_t>(AudioCaps.IsLpcmSadFound) << "\" ";
    os << "SpeakerAllocationBlock = \"" << static_cast<uint32_t>(AudioCaps.SpeakerAllocationBlock) << "\"" << std::endl;
    os << "NumSADBlocks = \"" << static_cast<uint32_t>(AudioCaps.NumSADBytes) << "\">" << std::endl;

    if (AudioCaps.IsLpcmSadFound)
    {
        os << "      <LpcmSAD ";
        os << AudioCaps.LpcmSad;
        os << "      </LpcmSAD>" << std::endl;

        Count++;
    }

    for (; Count < AudioCaps.NumSADBytes; Count++)
    {
        os << "      <SADBlock" << Count << " ";
        os << AudioCaps.SadBlock[Count];
        os << "      </SADBlock" << Count << ">" << std::endl;
    }
    os << "    </CeAudioCaps>" << std::endl;

    return os;
}

/**
 * @brief
 *
 *
 * @param  os
 * @param  TiledBlock
 * @return ostream
 */
std::ostream& operator<<(std::ostream& os, const DD_TILED_DISPLAY_INFO_BLOCK TiledBlock)
{
    if (TiledBlock.IsValidBlock)
    {
        os << "    <TiledBlock " << std::endl;
        os << "      IsValidBlock = \"" << static_cast<uint32_t>(TiledBlock.IsValidBlock) << "\" ";
        os << "InSinglePhysicalDisplayEnclosure = \"" << static_cast<uint32_t>(TiledBlock.
            InSinglePhysicalDisplayEnclosure) << "\" ";
        os << "IsBezelInfoAvailable = \"" << static_cast<uint32_t>(TiledBlock.IsBezelInfoAvailable) << "\" ";
        os << "TiledScaling = \"" << TiledBlock.Scaling << "\" ";
        os << "      BlockRevision = \"" << static_cast<uint32_t>(TiledBlock.BlockRevision) << "\"" << std::endl;

        os << "      TotalNumberOfHTiles = \"" << static_cast<uint32_t>(TiledBlock.TotalNumberOfHTiles) << "\" ";
        os << "TotalNumberOfVTiles = \"" << static_cast<uint32_t>(TiledBlock.TotalNumberOfVTiles) << "\" ";
        os << "HTileLocation = \"" << static_cast<uint32_t>(TiledBlock.HTileLocation) << "\" ";
        os << "VTileLocation = \"" << static_cast<uint32_t>(TiledBlock.VTileLocation) << "\" ";
        os << "HTileSizeInPixels = \"" << TiledBlock.HTileSizeInPixels << "\" ";
        os << " VTileSizeInLines = \"" << TiledBlock.VTileSizeInLines << "\" ";
        os << "PixelMultiplier = \"" << static_cast<uint32_t>(TiledBlock.PixelMultiplier) << "\" >" << std::endl;

        os << "      <BezelInfo ";
        os << "TopBezelsize = \"" << static_cast<uint32_t>(TiledBlock.TopBezelsize) << "\" ";
        os << "BottomBezelsize = \"" << static_cast<uint32_t>(TiledBlock.BottomBezelsize) << "\" ";
        os << "RightBezelsize = \"" << static_cast<uint32_t>(TiledBlock.RightBezelsize) << "\" ";
        os << "LeftBezelsize = \"" << static_cast<uint32_t>(TiledBlock.LeftBezelsize) << "\" ";
        os << "> </BezelInfo>" << std::endl;

        DD_PNP_ID PnpId;
        DD_ZERO_MEM(&PnpId, sizeof(PnpId));
        PnpId.ManufacturerID[0] = TiledBlock.ManufacturerID[0];
        PnpId.ManufacturerID[1] = TiledBlock.ManufacturerID[1];
        ////PnpId.ManufacturerID[2] = pTiledBlock.ManufacturerID[2]; Not
        // printing
        PnpId.ProductID[0] = TiledBlock.ProductID[0];
        PnpId.ProductID[1] = TiledBlock.ProductID[1];
        PnpId.SerialNumber[0] = TiledBlock.SerialNumber[0];
        PnpId.SerialNumber[1] = TiledBlock.SerialNumber[1];
        PnpId.SerialNumber[2] = TiledBlock.SerialNumber[2];
        PnpId.SerialNumber[3] = TiledBlock.SerialNumber[3];
        os << PnpId;
        os << "    </TiledBlock>" << std::endl;
    }

    return os;
}

/**
 * @brief
 *
 * @param  os
 * @param  RrSwitchCaps
 * @return ostream
 */
std::ostream& operator<<(std::ostream& os, const DD_RR_SWITCH_CAPS RrSwitchCaps)
{
    os << "    <VrrDisplayCaps ";
    os << "MinRR=\"" << uint32_t(RrSwitchCaps.VariableRrCaps.MinRr1000 / 1000) << "\" ";
    os << "MaxRR=\"" << uint32_t(RrSwitchCaps.VariableRrCaps.MaxRr1000 / 1000) << "\" />" << std::endl;

    os << "    <RrSwitchCaps ";
    os << "IsFullRrRangeSupported=\"" << static_cast<bool>(RrSwitchCaps.FixedRrCaps.IsFullRrRangeSupported) << "\" ";
    os << "IsFlickerParamsValid=\"" << static_cast<bool>(RrSwitchCaps.IsFlickerParamsValid) << "\" ";
    os << "MinRr1000=\"" << static_cast<uint32_t>(RrSwitchCaps.VariableRrCaps.MinRr1000) << "\" ";
    os << "MaxRr1000=\"" << static_cast<uint32_t>(RrSwitchCaps.VariableRrCaps.MaxRr1000) << "\" ";
    os << "SfditInUs=\"" << static_cast<uint32_t>(RrSwitchCaps.SfditInUs) << "\" ";
    os << "SfddtInUs=\"" << static_cast<uint32_t>(RrSwitchCaps.SfddtInUs) << "\" ";
    os << "NumberOfAsDescriptors=\"" << static_cast<uint32_t>(RrSwitchCaps.NumberOfAsDescriptors) << "\" >" <<
        std::endl;

    for (int i = 0; i < RrSwitchCaps.NumberOfAsDescriptors; i++)
    {
        os << "<AsDescriptor NumAdded= \"" << i+1 << "\" ";
        os << "IsAvtSupported=\"" << static_cast<bool>(RrSwitchCaps.DidAsDescriptors[i].IsAvtSupported) << "\" ";
        os << "MinRr1000=\"" << static_cast<uint32_t>(RrSwitchCaps.DidAsDescriptors[i].MinRr1000) << "\" ";
        os << "MaxRr1000=\"" << static_cast<uint32_t>(RrSwitchCaps.DidAsDescriptors[i].MaxRr1000) << "\" ";
        os << "SfditInUs=\"" << static_cast<uint32_t>(RrSwitchCaps.DidAsDescriptors[i].SfditInUs) << "\" ";
        os << "SfddtInUs=\"" << static_cast<uint32_t>(RrSwitchCaps.DidAsDescriptors[i].SfddtInUs) << "\" />";
    }

    os << "    </RrSwitchCaps> ";

    return os;
}

/**
 * @brief
 *
 * @param  os
 * @param  Caps
 * @return ostream
 */
std::ostream& operator<<(std::stringstream& os, const DD_DISPLAY_CAPS Caps)
{
    os << "<EdidCaps>" << std::endl;
    os << "<FtrSupport ";
    os << "IsDigitalInput=\"" << static_cast<bool>(Caps.FtrSupport.IsDigitalInput) << "\" ";
    os << "IsDisplayIDData=\"" << static_cast<bool>(Caps.FtrSupport.IsDisplayIDData) << "\" ";
    /*os << "IsCeExtnDisplay=\"" << static_cast<bool>(Caps.FtrSupport.IsCeExtnDisplay) << "\" ";
    os << "IsVideoCapBlockBlockPresent=\"" << static_cast<bool>(Caps.FtrSupport.IsVideoCapBlockBlockPresent) << "\" ";
    os << "IsYuv420VideoBlockPresent=\"" << static_cast<bool>(Caps.FtrSupport.IsYuv420VideoBlockPresent) << "\" ";
    os << "IsHdmiVsdbValid=\"" << static_cast<bool>(Caps.FtrSupport.IsHdmiVsdbValid) << "\" ";
    os << "IsDviSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsDviSupported) << "\" ";
    os << "IsDisplayPortSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsDisplayPortSupported) << "\" ";
    os << "IsAudioSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsAudioSupported) << "\" ";
    os << "IsContinuousFreqSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsContinuousFreqSupported) << "\" ";
    os << "IsGTFSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsGTFSupported) << "\" ";
    os << "IsCVTSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsCVTSupported) << "\" ";
    os << "IsCVTRedBlankSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsCVTRedBlankSupported) << "\" ";
    os << "IsS3DLRFramesSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsS3DLRFramesSupported) << "\" ";*/
    os << "IsMRLBlockPresent=\"" << static_cast<bool>(Caps.FtrSupport.IsMRLBlockPresent) << "\" ";
    os << "IsDidAsDataBlockPresent=\"" << static_cast<bool>(Caps.FtrSupport.IsDidAsDataBlockPresent) << "\" ";
    os << "IsFecCapable=\"" << static_cast<bool>(Caps.FtrSupport.IsFecCapable) << "\" ";
    os << "IsHdcpCapable=\"" << static_cast<bool>(Caps.FtrSupport.IsHdcpCapable) << "\" ";
    os << "IsHdrSupported=\"" << static_cast<bool>(Caps.FtrSupport.IsHdrSupported) << "\" ";
    os << "/>" << std::endl;

    os << "<BasicDisplayCaps ";
    os << "MonitorName=\"";
    for (unsigned char Count : Caps.BasicDisplayCaps.MonitorName)
    {
        if ((Count == 0xA) || (Count == 0x0))
            break;
        os << Count;
    }
    os << "\" ";
    os << "MonitorNameLength=\"" << static_cast<uint32_t>(Caps.BasicDisplayCaps.MonitorNameLength) << "\" ";
    os << "DisplayGamma=\"0x" << std::hex << static_cast<uint32_t>(Caps.BasicDisplayCaps.DisplayGamma) << std::dec <<
        "\" ";
    os << "BpcsSupportedForAllModes=\"" << static_cast<uint32_t>(Caps.BasicDisplayCaps.BpcsSupportedForAllModes.
        ColorDepthMask) <<
        "\" ";
    os << "BpcsSupportedFor420Modes=\"" << static_cast<uint32_t>(Caps.BasicDisplayCaps.BpcsSupportedFor420Modes.
        ColorDepthMask) <<
        "\" ";
    os << "MaxDotClockSupportedInHz=\"" << Caps.BasicDisplayCaps.MaxDotClockSupportedInHz << "\" ";
    os << "HSize=\"" << static_cast<uint32_t>(Caps.BasicDisplayCaps.HSize) << "\" ";
    os << "VSize=\"" << static_cast<uint32_t>(Caps.BasicDisplayCaps.VSize) << "\" ";
    os << "MaxSymbolClockInMHz=\"" << Caps.BasicDisplayCaps.MaxSymbolClockInMHz << "\" >" << std::endl;
    os << Caps.BasicDisplayCaps.BaseBlkPnpID;
    os << Caps.BasicDisplayCaps.ChromaLumaData;
    os << "</BasicDisplayCaps>" << std::endl;

    os << "<CeExtnCaps " << std::endl;

/*    os << "SupportsYCBCR422= \"" << static_cast<uint32_t>(Caps.FtrSupport.YCbCr422Supported) << "\"  ";
    os << "SupportsYCBCR444  = \"" << static_cast<uint32_t>(Caps.FtrSupport.YCbCr444Supported) << "\"  ";
    os << "UnderscansITFormats = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.UnderscansITFormats) << "\">"
        << std::endl;

    os << "      <ITContentCaps ";
    os << "GraphicsContent = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.GraphicsContent) << "\"  ";
    os << "PhotoContent= \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.PhotoContent) << "\"  ";
    os << "CinemaContent  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.CinemaContent) << "\"  ";
    os << "GameContent  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.GameContent) << "\" />" << std::endl;

    os << "      <HFVSDBInfo ";
    os << "Version = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.Version) << "\"  ";
    os << "IsHfVsdbInfoValid = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsHfVsdbInfoValid) << "\"  ";
    os << "IsScdcPresent= \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsScdcPresent) << "\"  ";
    os << "IsScdcRrCapable  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsScdcRrCapable) << "\"  ";
    os << "IsLte_340Mcsc_Scramble  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsLte_340Mcsc_Scramble) <<
        "\"  ";
    os << "IsFrlModeSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsFrlModeSupported) << "\"  ";
    os << "IsFapaStartLocationAfter1stHBlank  = \"" << static_cast<uint32_t>(
        Caps.HdmiCaps.HfVsdbInfo.IsFapaStartLocationAfter1stHBlank) << "\"  ";
    os << "IsAllmSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsAllmSupported) << "\"  ";
    os << "IsFvaSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsFvaSupported) << "\"  ";
    os << "IsNegativeMVrrSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsNegativeMVrrSupported) <<
        "\"  ";
    os << "IsQmsSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsQmsSupported) << "\"  ";
    os << "IsQmsTfrMinSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsQmsTfrMinSupported) <<
        "\"  ";
    os << "IsQmsTfrMaxSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsQmsTfrMaxSupported) <<
        "\"  ";
    os << "IsMDeltaSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsMDeltaSupported) << "\"  ";
    os << "IsDsc1p2Supported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsDsc1p2Supported) << "\"  ";
    os << "IsDscAllBpcSupported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsDscAllBppSupported) <<
        "\"  ";
    os << "IsDscNative420Supported  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.IsDscNative420Supported) <<
        "\"  ";
    os << "MaxFrlInMbps  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.MaxFrlInMbps) << "\"  ";
    os << "MaxFrlLaneCount  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.MaxFrlLaneCount) << "\"  ";
    os << "CompressionBpc  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.CompressionBpc.Value) << "\"  ";
    os << "MaxDscSlices  = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.MaxDscSlices) << "\"  ";
    os << "MaxDscFrlInMbps   = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.MaxDscFrlInMbps) << "\"  ";
    os << "MaxDscFrlLaneCount   = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.MaxDscFrlLaneCount) << "\"  ";
    os << "DscTotalChunkKBytes   = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HfVsdbInfo.DscTotalChunkKBytes) << "\"  ";*/
    os << "IsMsoCapable   = \"" << static_cast<uint32_t>(Caps.MsoDisplayCaps.IsMsoCapable) << "\"  ";
    os << "NumOfLinks   = \"" << static_cast<uint32_t>(Caps.MsoDisplayCaps.NumOfLinks) << "\"  ";
    os << "OverlapPixelCount   = \"" << static_cast<uint32_t>(Caps.MsoDisplayCaps.OverlapPixelCount) << "\" />" <<
        std::endl;

  /*  os << "      <HDRStaticMetaData ";
    os << "HdrMetaDataBlockFound = \"" << static_cast<uint32_t>(Caps.HdrCaps.CeEdidData.HdrMetaDataBlockFound)
        << "\"  ";
    os << "EOTFSupported = \"" << static_cast<uint32_t>(Caps.HdrCaps.CeEdidData.EOTFSupported.Value) <<
        "\"  ";
    os << "HdrStaticMetaDataType = \"" << static_cast<uint32_t>(Caps.HdrCaps.CeEdidData.HdrStaticMetaDataType.
        Value) <<
        "\"  ";
    os << "DesiredMaxCLL  = \"" << static_cast<uint32_t>(Caps.HdrCaps.CeEdidData.DesiredMaxCLL) << "\"  ";
    os << "DesiredMaxFALL  = \"" << static_cast<uint32_t>(Caps.HdrCaps.CeEdidData.DesiredMaxFALL) << "\" ";
    os << "DesiredMinCLL = \"" << static_cast<uint32_t>(Caps.HdrCaps.CeEdidData.DesiredMinCLL) << "\" />" <<
        std::endl;

    os << "      <CEColorimetryData ColorimetryType = \"" << static_cast<uint32_t>(
        Caps.HdrCaps.CeColorimetryData.ColorimetryType.Value) << "\" />" << std::endl;

    os << "      <CEVideoCapInfo CEScanBehavior = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.CEScanBehavior)
        << "\"  ";
    os << "ITScanBehavior = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.ITScanBehavior) << "\"  ";
    os << "PTScanBehavior = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.PTScanBehavior) << "\"  ";
    os << "IsQuantRangeSelectable = \"" << static_cast<uint32_t>(Caps.HdmiCaps.HdmiCeCaps.IsQuantRangeSelectable)
        << "\" />" <<
        std::endl;

    os << "    </CeExtnCaps>" << std::endl;

    os << Caps.CeAudioCaps;
    os << Caps.TiledDisplayInfo;*/
    os << Caps.RrSwitchCaps;

    os << "  </EdidCaps>" << std::endl;

    return os;
}

/**
 * @brief
 *
 * @param  os
 * @param  modes
 * @return stringstream
 */
std::stringstream& operator<<(std::stringstream& os, const DD_GET_EDID_MODES& modes)
{
    for (uint32_t entry = 0; entry < modes.pModeTable->NumEntries; entry++)
    {
        os << "  <TimingInfo NumAdded = \"" << entry + 1 << "\">  " << std::endl;

        DD_TIMING_INFO* pMode = (static_cast<DD_TIMING_INFO*>(modes.pModeTable->pEntry) + entry);
        os << "    <Timings \n";
        os << "HActive = \"" << pMode->HActive << "\" ";
        os << "HTotal = \"" << pMode->HTotal << "\" ";
        os << "DotClockInHz = \"" << pMode->DotClockInHz << "\" ";
        os << "HBlankStart = \"" << pMode->HBlankStart << "\" ";
        os << "HBlankEnd = \"" << pMode->HBlankEnd << "\" ";
        os << "HSyncStart = \"" << pMode->HSyncStart << "\" ";
        os << "HSyncEnd = \"" << pMode->HSyncEnd << "\" ";
        os << "HRefresh = \"" << pMode->HRefresh << "\" ";
        os << "VActive = \"" << pMode->VActive << "\" ";
        os << "VTotal = \"" << pMode->VTotal << "\" ";
        os << "VBlankStart = \"" << pMode->VBlankStart << "\" ";
        os << "VBlankEnd = \"" << pMode->VBlankEnd << "\" ";
        os << "VSyncStart = \"" << pMode->VSyncStart << "\" ";
        os << "VSyncEnd = \"" << pMode->VSyncEnd << "\" ";
        os << "VRoundedRR = \"" << pMode->VRoundedRR << "\" ";
        os << "IsInterlaced = \"" << static_cast<uint32_t>(pMode->IsInterlaced) << "\" ";
        os << "HSyncPolarity = \"" << static_cast<uint32_t>(pMode->HSyncPolarity) << "\" ";
        os << "VSyncPolarity = \"" << static_cast<uint32_t>(pMode->VSyncPolarity) << "\" ";
        os << "ModeType = \"" << static_cast<uint32_t>(pMode->ModeType) << "\" ";
        os << "S3DFormat = \"0x" << std::hex << pMode->S3DFormatMask << std::dec << "\" ";
        os << "SignalStandard = \"" << static_cast<uint32_t>(pMode->SignalStandard) << "\" ";
        os << "FvaFactorM1 = \"" << static_cast<uint32_t>(pMode->FvaFactorM1) << "\" ";
        os << "PreferredMode =  \"" << static_cast<bool>(pMode->Flags.PreferredMode) << "\" ";
        os << "SupportedBPCMask = \"" << pMode->Flags.SupportedBPCMask.ColorDepthMask << "\" ";
        os << "PixelReplication =  \"" << static_cast<uint32_t>(pMode->CeData.PixelReplication) << "\" ";
        os << "IsCeaNativeFormat = \"" << static_cast<uint32_t>(pMode->CeData.IsNativeFormat[0]) << "\" ";
        os << "SamplingMode = \"" << static_cast<uint32_t>(pMode->CeData.SamplingMode.Value) << "\" ";
        os << "VicID = \"" << static_cast<uint32_t>(pMode->CeData.VicId[0]) << "\"" << std::endl;
        os << "AspectRatio = \"" << static_cast<uint32_t>(pMode->CeData.Par[0].Value) << "\" ";
        if ((pMode->CeData.VicId[0] != VIC_UNDEFINED) && (pMode->CeData.VicId[1] != VIC_UNDEFINED))
        {
            os << "IsCeaNativeFormat1 = \"" << static_cast<uint32_t>(pMode->CeData.IsNativeFormat[1]) << "\" ";
            os << "VicID1 = \"" << static_cast<uint32_t>(pMode->CeData.VicId[1]) << "\" " << std::endl;
            os << "AspectRatio1 = \"" << static_cast<uint32_t>(pMode->CeData.Par[1].Value) << "\" ";
        }
        os << "VicId4k2k = \"" << static_cast<uint32_t>(pMode->CeData.VicId4k2k) << "\" ";
        os << "PixelOverlapCount = \"" << static_cast<uint32_t>(pMode->Flags.PixelOverlapCount) << "\" ";
        os << "NumLinks = \"" << static_cast<uint32_t>(pMode->Flags.NumLinks) << "\" ";
        os << "DscCapable = \"" << static_cast<uint32_t>(pMode->Flags.DscCapable) << "\" ";
        os << "Is64BppPossible = \"" << static_cast<uint32_t>(pMode->Flags.Is64BppPossible) << "\" ";
        os << "/>" << std::endl;

        os << "</TimingInfo>" << std::endl;
    }

    os << std::endl;
    os << "<TotalEDIDModes Num = \"" << modes.pModeTable->NumEntries << "\"/> " << std::endl << std::endl;
    os << std::endl;

    return os;
}


int main(int argc, char* argv[])
{
    try {

        DDSTATUS Status;
        DD_GET_EDID_MODES GetEdidModes = { 0 };
        DD_GET_EDID_CAPS GetEdidCaps = { 0 };
        DD_DISPLAY_CAPS EdidCaps = { 0 };
        DD_TIMING_INFO g_TimingInfo[MAX_TIMINGS];
        DD_TABLE* pmModeTable = NULL;

        pmModeTable = static_cast<DD_TABLE*>(UTF_MALLOC(sizeof(DD_TABLE) + (100 * sizeof(DD_TIMING_INFO))));
        pmModeTable->TableSize = MAX_TIMINGS;
        pmModeTable->EntrySizeInBytes = sizeof(DD_TIMING_INFO);
        pmModeTable->pfnAddEntry = addEntry;
        pmModeTable->pfnMatchEntry = matchTargetMode;
        pmModeTable->pfnReplaceEntry = replaceTargetMode;
        pmModeTable->pEntry = &(g_TimingInfo[0]);

        memset(g_TimingInfo, 0, sizeof(g_TimingInfo));
        memset(&EdidCaps, 0, sizeof(DD_DISPLAY_CAPS));
        pmModeTable->NumEntries = 0;

        if (argv[1] == NULL)
            return 1;

        std::string inFile = argv[1];

        std::ifstream fileinput;
        std::stringstream output;

        output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
        output << "<EDID>" << std::endl;

        fileinput.open(inFile, std::ios::binary);

        std::vector<uint8_t> EdidData((std::istreambuf_iterator<char>(fileinput)), std::istreambuf_iterator<char>());
        std::cout << EdidData.size();
        memset(&EdidCaps, 0, sizeof(DD_DISPLAY_CAPS));
        memset(g_TimingInfo, 0, sizeof(g_TimingInfo));

        GetEdidCaps.BufSizeInBytes = (DDU32)EdidData.size();
        GetEdidCaps.pEdidOrDisplayIDBuf = EdidData.data();
        GetEdidCaps.pData = &EdidCaps;
        GetEdidCaps.NumEdidExtensionsParsed = EdidData.at(126);
        Status = DisplayInfoParserGetEdidCaps(&GetEdidCaps);

        output << EdidCaps;

        GetEdidModes.BufSizeInBytes = (DDU32)EdidData.size();
        GetEdidModes.pEdidOrDisplayIDBuf = EdidData.data();
        GetEdidModes.NumEdidExtensionsParsed = EdidData.at(126);
        GetEdidModes.pModeTable = pmModeTable;
        Status = DisplayInfoParserGetEdidModes(&GetEdidModes);

        output << GetEdidModes;
        output << "</EDID>" << std::endl << std::endl;

        std::ofstream outputFile("parsedEdid.xml");
        outputFile << output.rdbuf();

        return 0;

    }
    catch (const std::exception& ex) {
        // Handle the exception gracefully
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
        // Additional error handling code if needed
    }
    catch (...) {
        // Handle any other unknown exceptions
        std::cerr << "Unknown exception occurred" << std::endl;
        // Additional error handling code if needed
    }
}