//-----------------------------------------------------------------------------------------------------------
//
// INTEL CONFIDENTIAL
// Copyright (C) 2000-2024 Intel Corporation
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they were provided to you ("License").
// Unless the License provides otherwise, you may not use, modify, copy, publish,
// distribute, disclose or transmit this software or the related documents without
// Intel's prior written permission. This software and the related documents are provided
// as is, with no express or implied warranties, other than those that are expressly stated in the License.
//
//-----------------------------------------------------------------------------------------------------------

/**
 *
 * @file  DisplayInfoParser.c
 * @brief This file contains all the EDID and Display ID parser related functions
 *
 */

#include "DisplayInfoParser.h"

 /*
  *
  * Local Function Declarations
  *
  */

  // Generic Display Info parser related functions
DDSTATUS DisplayInfoParserAddEDIDModeToList(ADD_MODES* pAddModes, EDID_DATA* pEdidData);
static DD_BOOL DisplayInfoParserIsCeExtnDisplay(CE_EDID_EXTN_BLOCK* pCExtnBlock);
static DDU32 DisplayInfoParserConvertHexArrayToInt(DDU8* pArray, DDU8 SzOfArray);
static DD_BPC_SUPPORTED DisplayInfoParserGetSupportedBpcMaskFromColorDepth(DDU32 ColorBitDepth);
static void DisplayInfoParserProcessSerialNumberBlock(DD_BB_EDID_CONTAINER* pEdidDataBuffer, DDU32 DataIndex, DDU8* pSerialNumBlock);
DDSTATUS DisplayInfoParserUpdateModeTable(DD_GET_EDID_MODES* pEdidModes, EDID_DATA* pEdidData);
DDSTATUS DisplayInfoParserAddTimingToModeList(EDID_DATA* pEdidData, DD_TIMING_INFO* pTimingInfo, DD_BOOL ForceAdd);
static DDSTATUS AddEdidModeToModeTable(ADD_MODES* pAddModes, EDID_DATA* pEdidData);
static DDSTATUS AddRgbSamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData);
static DDSTATUS AddYCbCr420SamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData);
static DDSTATUS AddYCbCr444SamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData);
static DDSTATUS AddYCbCr422SamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData);

// EDID parser related functions
void EdidParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
DDSTATUS EdidParserVerifyDataBlocks(DDU8* pEdidOrDisplayIDBuf, DDU32 BufSizeInBytes, DDU8 BlockNum);
void EdidParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserGetBaseBlockMonitorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserUpdateMaxDotClockSupport(DDU64 EdidMaxPixelClockInMHz, EDID_DATA* pEdidData);
void EdidParserGetMonitorDescriptorDetails(MONITOR_DESCRIPTOR* pMonitorDescriptor, EDID_DATA* pEdidData);
void EdidParserParseBaseEstablishedModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserParseBaseStdModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserParseEstTimingIIIBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
static DDSTATUS EdidParserAddBaseBlockDTDsToModelist(EDID_DATA* pEdidData, DD_TIMING_INFO* pTimingInfo);
void EdidParserParseBaseBlockDTDs(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserParseCeExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserParseDisplayIdExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidGetSupportedBPCFromBaseBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void EdidParserMarkVfpdbTimingsAsPreferred(EDID_DATA* pEdidData);
DDSTATUS DisplayInfoParserParseBaseBlockDTDsForeDP(DDU8* pEdidOrDisplayIDBuf, DDU32 BufSizeInBytes, DD_DTD_MODE_INFO* pDtDModeinfo);
void DisplayInfoParserParseTypeXTiming(DISPLAYID_2_0_FORMULA_TIMING_X* pDidFormula10, CEA_EXT_FORMULA_TIMING_X* pCtaFormula10, DD_TABLE* pModeTable, EDID_DATA* pEdidData,
    DD_BOOL IsTimingAddedInCtaBlk);
DDSTATUS DisplayInfoParserParseEnumTimings(SHORT_TIMING_4_REVID* pTiming4HdrOverride, DDU8 TimingCodeSizeInByte, EDID_DATA* pEdidData, DDU8 EnumeratedTimingTag, DDU8* pDidTimingInfo4,
    DDU8 NumType4Descriptors);
DDSTATUS DisplayInfoParserParseCtaType8Timing(CEA_EXT_FORMULA_TIMING_VIII* pType8TimingBlock, EDID_DATA* pEdidData);

// CE Info parser related functions
DDU8 CeInfoParserIsCeMode(DD_TIMING_INFO* pTimingInfo);
void CeInfoParserUpdateCeModeList(EDID_DATA* pEdidData, CE_MODE_LIST* pCeModeData, MODE_SOURCE ModeSource);
DD_BOOL CeInfoParserIsModeAlreadyAdded(DD_TIMING_INFO* pTimingInfo, EDID_DATA* pEdidData, DDU8 VicId);
DDSTATUS CeInfoParserAddPrModes(DD_TIMING_INFO* pTimingInfo, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAddCeModesToList(EDID_DATA* pEdidData, CE_MODE_LIST* pCeModeList);
static void CeInfoParserGetCeaExtnBlockByID(GET_BLOCK_BY_ID_ARGS* pGetBlockById);
void CeInfoParserFillGetBlockByIdArgs(GET_BLOCK_BY_ID_ARGS* pGetBlockById, DDU8 BlockIdOrTag, DDU8 CeaExtendedTag, DDU32 IEEERegNum, DDU8 InstanceOfBlock, DDU8* pInputBuf, DDU32 InputBlockLength);
static DDU8 CeInfoParserGetBlockCount(GET_BLOCK_BY_ID_ARGS* pGetBlockById);
DD_BOOL CeInfoParserIsVicPresentInList(DDU32 VicCount, CE_MODE_LIST* pCeModeList, DDU32 VicID);
DDSTATUS CeInfoParserParseDisplayIDTimings(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseCeExtensionForModes(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
void CeInfoParserParseCeExtensionForCaps(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseVideoCapabilityDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseColorimetryDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseAudioDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseSpeakerAllocationDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAddModesFromCeExtnDtdBlock(CE_EDID_EXTN_BLOCK* pCeEdidExtn, DD_TABLE* pModeTable, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserGetSvdListFromVideoDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAddModesFromVideoDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserIs420VideoDataBlockPresent(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAddModesFrom420VideoDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserIs420CapabilityBlockPresent(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParse420CapabilityMapDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseVicsFromVfpdb(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseHdrCapDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserParseHdmiVsdb(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData, DD_BOOL* pIsValidVicOffset, DDU8* pVicOffset);
DDSTATUS CeInfoParserParseHFVSDB(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
DD_BOOL CeInfoParserIsS3DFrameSupported(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAddS3DModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAddMandatoryS3DModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData);
DDSTATUS CeInfoParserAdd3DMultiPresentIndicatedS3DModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData, S3D_FORMATS S3DStruct, DDU32 S3DVics);
DDSTATUS CeInfoParserAdd4k2kModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData);
void CeInfoParserGetSupportedBPCFromCeExtBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
void CeInfoParserGetSupportedBPCFromCeHFVSDBBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
void CeInfoParserGetSupportedBPCFromCeHdmiVSDBBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData);
static DDU8 CeInfoParserGetExtensionCountFromHdmiEeodb(DDU8* pEdidData);
// Common functions between DisplayID 1.3 & DisplayID 2.0
void DisplayIdParserFillGetBlockByIdArgs(GET_BLOCK_BY_ID_ARGS* pGetBlockById, DDU8* pEdidOrDisplayIDBuf, DDU8 BlockIdOrTag, DDU8 InstanceOfBlock);
DDSTATUS DisplayIdParserGetDisplayIDBlockByID(GET_BLOCK_BY_ID_ARGS* pGetBlockById);
DDSTATUS DisplayIdParserVerifyDataBlocks(DDU8* pEdidOrDisplayIDBuf);
void DisplayIdParserParseTiledTopologyBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 TiledDisplayTag);
DDSTATUS DisplayIdParserParseDTDTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 DTDTimingTag, DDU32 DotClockScale);
void DisplayIdParserGetProductIDDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 ProductTag);
DDSTATUS DisplayIdParserParseEnumeratedTimingBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 EnumeratedTimingTag, DDU8 TimingCodeSizeInByte);
void DisplayIdParserParseCeExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 CEAExtTag);
void DisplayIdParserRemoveSerialNumber(DD_BB_EDID_CONTAINER* pEdidDataBuffer, DDU32 DataIndex);

// Display ID parser related functions
void DisplayIdParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserGetMonitorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserGetMonitorDescriptorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserParseType1Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserParseType2Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserParseType3Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserParseType4Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserParseStandardTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdParserParseCeaTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdGetSupportedBPC(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);

// Display ID V2 parser related functions
void DisplayIdV2ParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2GetSupportedBPC(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserParseType7Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserParseType8EnumeratedTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserGetMonitorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserParseHdrCapDataBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserGetMonitorDescriptorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV2ParserParseMsoCapsFromVSDBBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);

// Display ID V2.1 parser related functions
void DisplayIdV21ParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);
void DisplayIdV21ParserGetAdaptiveSyncDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData);

//-----------------------------------------------------------------------------
//
//  Video Timing Modes - Detailed Timing "Type I timing" data block
//  Tag ID: 0x03 - DID_DATA_BLOCK_DETAILED_TIMING_1
//  Size - Variable size block -- 20 bytes per timing * N # of Timings
//
//-----------------------------------------------------------------------------
static const ASPECT_RATIO DidAspectRatioTable[] = {
    { 1, 1 }, { 5, 4 }, { 4, 3 }, { 15, 9 }, { 16, 9 }, { 16, 10 }, { 256, 135 },
};

/* This Variable is commented to avoid "Unused variable" warning
 * @todo: VSDI-22878 */
 /*
 static const ASPECT_RATIO DidV2AspectRatioTable[] = {
     { 1, 1 }, { 5, 4 }, { 4, 3 }, { 15, 9 }, { 16, 9 }, { 16, 10 }, { 64, 27 }, { 256, 135 },
 };
 */

static const ASPECT_RATIO EdidAspectRatioTable[] = {
    { 16, 10 },
    { 4, 3 },
    { 5, 4 },
    { 16, 9 },
};

// Max content luminance level. Derived from PeakLuminanceIndex for HDR10+
#define DD_MAX_PEAK_LUMINANCE_INDEX 16
static const DDU16 MaxCLLFromPeakLuminanceIndex[DD_MAX_PEAK_LUMINANCE_INDEX] = { 0, 200, 300, 400, 500, 600, 800, 1000, 1200, 1500, 2000, 2500, 3000, 4000, 6000, 8000 };

// Max frame average luminance level. Derived from FullFramePeakLuminanceIndex and PeakLuminanceIndex for HDR10+
// Each row in below table is having multiplier = (2^index). Hence, 1D array is used with runtime calculation of multiplier.
// --------------------------------------------------------------------------------------------------------------------------------
// Full Frame Peak	|									Peak Luminance (Index-value in nits)
// Luminance Index	|0-rev|1-200|2-300|3-400|4-500|5-600|6-800|7-1000|8-1200|9-1500|10-2000|11-2500|12-3000|13-4000|14-6000|15-8000|
// ----------------|---------------------------------------------------------------------------------------------------------------
//        0	       |0-rev|20	|30	  |40	|50	  |60	|80	  |100	 |120	|150   |200	   |250	   |300	   |400	   |600	   |800	   |
//        1	       |0-rev|40	|60	  |80	|100  |120	|160  |200	 |240	|300   |400	   |500	   |600	   |800	   |1200   |1600   |
//        2	       |0-rev|80	|120  |160	|200  |240	|320  |400	 |480	|600   |800	   |1000   |1200   |1600   |2400   |3200   |
//        3	       |0-rev|160	|240  |320	|400  |480	|640  |800	 |960	|1200  |1600   |2000   |2400   |3200   |4800   |6400   |

static const DDU16 MaxFALLFromFullFramePeakLuminanceIndexAndPeakLuminanceIndex[DD_MAX_PEAK_LUMINANCE_INDEX] = { 0, 20, 30, 40, 50, 60, 80, 100, 120, 150, 200, 250, 300, 400, 600, 800 };

/***************************************************************
 *
 * Function implementation
 *
 ***************************************************************/

 /***************************************************************
  *
  * Interface Function Declarations -- START
  *
  ***************************************************************/

  /***************************************************************
   * @brief  This function parses entire EDID/DisplayID block and.
   *
   *      adds all target modes to the mode - table.
   *
   * @param pGetEdidModes
   * @return DDSTATUS
   ***************************************************************/
DDSTATUS DisplayInfoParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes)
{
    DDSTATUS Status;
    DISPLAYID_HEADER DidExtnBlockHeader;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    DDU8 DisplayIdExtnCount;
    CE_MODE_LIST* pCeModeList = NULL;
    EDID_DATA EdidData;
    DD_DISPLAY_CAPS* pData;
    DD_GET_EDID_MODES GetEdidModes;
    DDU8* pEdidOrDisplayIDBuf = NULL;
    DDU8 Count, Did2ExtensionCount, CeaBlockTag;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    MODES_LIST* pModes;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pGetEdidModes);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pData = (DD_DISPLAY_CAPS*)(DD_ALLOC_MEM(sizeof(DD_DISPLAY_CAPS)));

    if (NULL == pData)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_NO_MEMORY);
        return DDS_NO_MEMORY;
    }

    pCeModeList = (CE_MODE_LIST*)(DD_ALLOC_MEM(sizeof(CE_MODE_LIST) * MAX_VIC_DEFINED));
    // pCeModeList (Ce Mode-list) is a temp array of all CE modes we get from EDID
    // This is mainly used to filter out same CE modes from various blocks of CE extension
    if (pCeModeList == NULL)
    {
        DD_SAFE_FREE(pData);
        DISP_FUNC_EXIT_W_STATUS(DDS_NO_MEMORY);
        return DDS_NO_MEMORY;
    }

    pModes = (MODES_LIST*)(DD_ALLOC_MEM(sizeof(MODES_LIST) * MAX_MODES_DEFINED));
    // pModes (Temp Timing Info List) is a temporary timing info list used to store all the DTDs information in order from Base block -> Extn block 1 -> Extn block 2 & so on
    // this will help in counting the DTDs as per there occurrence, to be marked as preferred (as per the VFPDB, CT 861-G/H)

    if (pModes == NULL)
    {
        DD_SAFE_FREE(pData);
        DD_SAFE_FREE(pCeModeList);
        DISP_FUNC_EXIT_W_STATUS(DDS_NO_MEMORY);
        return DDS_NO_MEMORY;
    }

    DD_ZERO_MEM(pData, sizeof(DD_DISPLAY_CAPS));
    DD_ZERO_MEM(pCeModeList, sizeof(CE_MODE_LIST) * MAX_VIC_DEFINED);
    DD_ZERO_MEM(pModes, sizeof(MODES_LIST) * MAX_MODES_DEFINED);

    // Init all VICs as undefined
    for (Count = 0; Count < MAX_VIC_DEFINED; Count++)
    {
        pCeModeList[Count].VicId[0] = VIC_UNDEFINED;
        pCeModeList[Count].VicId[1] = VIC_UNDEFINED;
    }

    EdidData.pCeModeList = pCeModeList;
    EdidData.pData = pData;
    EdidData.pModes = pModes;
    EdidData.Vfpdb.DtdMask = 0;
    EdidData.Vfpdb.DtdCount = 0;
    EdidData.NumTimingInfo = 0;
    EdidData.NumBaseBlockDtds = 0;
    EdidData.VicCount = 0;
    EdidData.NumModesInEdidOrDID = 0;
    EdidData.IsPreferredModeAdded = FALSE;

    if (0 == memcmp(BaseEdidHeader, pGetEdidModes->pEdidOrDisplayIDBuf, EDID_HEADER_SIZE))
    {
        // EDID
        pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;
        EdidData.IsDisplayIdBaseBlock = FALSE;
        EdidData.EdidOrDidVerAndRev = (pEdidBaseBlock->Version << 4) | (pEdidBaseBlock->Revision);

        // Parse and add modes to mode - list
        EdidParserGetEdidModes(pGetEdidModes, &EdidData);
    }
    else
    {
        // Display ID
        EdidData.IsDisplayIdBaseBlock = TRUE;
        DidExtnBlockHeader.Value = ((DISPLAYID_HEADER*)(pGetEdidModes->pEdidOrDisplayIDBuf))->Value;
        Did2ExtensionCount = DidExtnBlockHeader.ExtensionCount;
        EdidData.EdidOrDidVerAndRev = DidExtnBlockHeader.RevisionAndVersion;
        CeaBlockTag = DID_DATA_BLOCK_CEA_SPECIFIC;
        if ((DISPLAY_ID_VER_2_0 == EdidData.EdidOrDidVerAndRev) || (DISPLAY_ID_VER_2_1 == EdidData.EdidOrDidVerAndRev))
        {
            CeaBlockTag = DID2_DATA_BLOCK_CTA_DISPLAY_ID;
        }

        GetEdidModes.pModeTable = pGetEdidModes->pModeTable;

        // Parse and add modes to mode - list
        pEdidOrDisplayIDBuf = pGetEdidModes->pEdidOrDisplayIDBuf;
        DisplayIdExtnCount = 0;

        do
        {
            DidExtnBlockHeader.Value = ((DISPLAYID_HEADER*)(pEdidOrDisplayIDBuf))->Value;

            // Not checking for multiple instances of CE block yet. Will do on need basis
            DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pEdidOrDisplayIDBuf, CeaBlockTag, 0);
            Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
            if (IS_DDSTATUS_SUCCESS(Status))
            {
                // This data needed to know if its a CE extension display when we parse some of base block modes before CE block parsing
                EdidData.pData->FtrSupport.IsCeExtnDisplay = TRUE;
            }

            GetEdidModes.pEdidOrDisplayIDBuf = pEdidOrDisplayIDBuf;
            GetEdidModes.BufSizeInBytes = DidExtnBlockHeader.BytesInSection;
            // Display ID Header starts from the next byte in the block
            switch (DidExtnBlockHeader.RevisionAndVersion)
            {
            case DISPLAY_ID_VER_2_1:
            case DISPLAY_ID_VER_2_0:
                DisplayIdV2ParserGetEdidModes(&GetEdidModes, &EdidData);
                break;
            default:
                DisplayIdParserGetEdidModes(&GetEdidModes, &EdidData);
                break;
            }

            // Point to next block which starts at current block Start +  Header size + Data Size + 1 byte Checksum
            pEdidOrDisplayIDBuf = ((DDU8*)(pEdidOrDisplayIDBuf)+DidExtnBlockHeader.BytesInSection + sizeof(DISPLAYID_HEADER) + 1);
        } while (++DisplayIdExtnCount <= Did2ExtensionCount);
    }

    // Mark the preferred modes from Base/CE extn blocks
    EdidParserMarkVfpdbTimingsAsPreferred(&EdidData);

    // modes are added, transfer them to mode table
    Status = DisplayInfoParserUpdateModeTable(pGetEdidModes, &EdidData);

    DD_SAFE_FREE(pModes);
    DD_SAFE_FREE(pCeModeList);
    DD_SAFE_FREE(pData);

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief This function parses entire EDID/DisplayID block and.
 *
 *      collects the capability information from EDID/Display ID.
 *
 * @param pGetEdidCaps
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserGetEdidCaps(DD_GET_EDID_CAPS* pGetEdidCaps)
{
    DDSTATUS Status;
    DISPLAYID_HEADER DidExtnBlockHeader;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    DD_GET_EDID_MODES GetEdidModes;
    EDID_DATA EdidData;
    CE_MODE_LIST* pCeModeList = NULL;
    DDU8 DisplayIdExtnCount, Did2ExtensionCount;
    DDU8* pEdidOrDisplayIDBuf = NULL;
    DDU8 Count, CeaBlockTag;
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pGetEdidCaps);
    DDASSERT(pGetEdidCaps->pEdidOrDisplayIDBuf);
    DDASSERT(pGetEdidCaps->pData);

    pCeModeList = (CE_MODE_LIST*)(DD_ALLOC_MEM(sizeof(CE_MODE_LIST) * MAX_VIC_DEFINED));
    if (pCeModeList == NULL)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_NO_MEMORY);
        return DDS_NO_MEMORY;
    }

    // Init all VICs as undefined
    for (Count = 0; Count < MAX_VIC_DEFINED; Count++)
    {
        pCeModeList[Count].VicId[0] = VIC_UNDEFINED;
        pCeModeList[Count].VicId[1] = VIC_UNDEFINED;
    }

    GetEdidModes.pEdidOrDisplayIDBuf = pGetEdidCaps->pEdidOrDisplayIDBuf;
    GetEdidModes.BufSizeInBytes = pGetEdidCaps->BufSizeInBytes;
    GetEdidModes.pModeTable = NULL; // Call is for getting mode caps.
    GetEdidModes.NumEdidExtensionsParsed = pGetEdidCaps->NumEdidExtensionsParsed;

    EdidData.pCeModeList = pCeModeList;
    EdidData.pData = pGetEdidCaps->pData;
    EdidData.VicCount = 0;
    EdidData.NumModesInEdidOrDID = 0;
    EdidData.Vfpdb.DtdCount = 0;
    EdidData.Vfpdb.DtdMask = 0;

    if (0 == memcmp(BaseEdidHeader, pGetEdidCaps->pEdidOrDisplayIDBuf, EDID_HEADER_SIZE))
    {
        // EDID
        pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidCaps->pEdidOrDisplayIDBuf;
        EdidData.IsDisplayIdBaseBlock = FALSE;
        EdidData.EdidOrDidVerAndRev = (pEdidBaseBlock->Version << 4) | pEdidBaseBlock->Revision;
        pGetEdidCaps->pData->FtrSupport.IsDisplayIDData = FALSE;

        // Parse caps
        EdidParserGetEdidCaps(&GetEdidModes, &EdidData);
    }
    else
    {
        // Display ID
        EdidData.IsDisplayIdBaseBlock = TRUE;
        DidExtnBlockHeader.Value = ((DISPLAYID_HEADER*)(pGetEdidCaps->pEdidOrDisplayIDBuf))->Value;
        Did2ExtensionCount = DidExtnBlockHeader.ExtensionCount;
        EdidData.EdidOrDidVerAndRev = DidExtnBlockHeader.RevisionAndVersion;
        pGetEdidCaps->pData->FtrSupport.IsDisplayIDData = TRUE;
        CeaBlockTag = DID_DATA_BLOCK_CEA_SPECIFIC;
        if ((DISPLAY_ID_VER_2_0 == EdidData.EdidOrDidVerAndRev) || (DISPLAY_ID_VER_2_1 == EdidData.EdidOrDidVerAndRev))
        {
            CeaBlockTag = DID2_DATA_BLOCK_CTA_DISPLAY_ID;
        }

        GetEdidModes.pModeTable = NULL;
        pEdidOrDisplayIDBuf = pGetEdidCaps->pEdidOrDisplayIDBuf;

        // Parse caps
        DisplayIdExtnCount = 0;
        do
        {
            DidExtnBlockHeader.Value = ((DISPLAYID_HEADER*)(pEdidOrDisplayIDBuf))->Value;

            // Not checking for multiple instances of CE block yet. Will do on need basis
            DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pEdidOrDisplayIDBuf, CeaBlockTag, 0);
            Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
            if (IS_DDSTATUS_SUCCESS(Status))
            {
                // This data needed to know if its a CE extension display when we parse some of base block modes before CE block parsing
                EdidData.pData->FtrSupport.IsCeExtnDisplay = TRUE;
            }

            GetEdidModes.pEdidOrDisplayIDBuf = pEdidOrDisplayIDBuf;
            GetEdidModes.BufSizeInBytes = DidExtnBlockHeader.BytesInSection;
            // Display ID Header starts from the next byte in the block
            switch (DidExtnBlockHeader.RevisionAndVersion)
            {
            case DISPLAY_ID_VER_2_1:
                DisplayIdV21ParserGetEdidCaps(&GetEdidModes, &EdidData);
                break;
            case DISPLAY_ID_VER_2_0:
                DisplayIdV2ParserGetEdidCaps(&GetEdidModes, &EdidData);
                break;
            default:
                DisplayIdParserGetEdidCaps(&GetEdidModes, &EdidData);
                break;
            }

            // Point to next block which starts at current block Start +  Header size + Data Size + 1 byte Checksum
            pEdidOrDisplayIDBuf = ((DDU8*)(pEdidOrDisplayIDBuf)+DidExtnBlockHeader.BytesInSection + sizeof(DISPLAYID_HEADER) + 1);
        } while (++DisplayIdExtnCount <= Did2ExtensionCount);
    }

    DD_SAFE_FREE(pCeModeList);

    DISP_FUNC_EXIT_W_STATUS(DDS_SUCCESS);
    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Display InfoParser ParseBase Block DTDs Fore DP.
 *
 *  Method to parse and add modes from Base block in EDID.
 *      EDID 1.3 - can have 4 DTDs in base block and anything can be preferred (DTD-0 is preferred if PTM flag is set)
 *      EDID 1.4 - can have 4 DTDs in base block and DTD-0 is always preferred as per spec.
 * @param  pEdidOrDisplayIDBuf
 * @param BufSizeInBytes
 * @param pDtDModeinfo
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserParseBaseBlockDTDsForeDP(DDU8* pEdidOrDisplayIDBuf, DDU32 BufSizeInBytes, DD_DTD_MODE_INFO* pDtDModeinfo)
{
    DDSTATUS Status;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    DDU32 DtdCounter = 0;
    EDID_DTD_TIMING* pDTD = NULL;
    DD_TIMING_INFO TimingInfo;

    // Arg validation
    DDASSERT(pEdidOrDisplayIDBuf);
    DDASSERT(BufSizeInBytes != 0);

    if (pEdidOrDisplayIDBuf)
    {
        Status = DDS_UNSUCCESSFUL; // Assume Status failed
        pEdidBaseBlock = (EDID_BASE_BLOCK*)pEdidOrDisplayIDBuf;

        for (DtdCounter = 0; DtdCounter < MAX_EDID_DTD_BLOCKS; DtdCounter++)
        {
            pDTD = &(pEdidBaseBlock->EdidTiming.Edid_13_DTD[DtdCounter]);

            if (pDTD->Timing.PixelClock <= 0x101)
            {
                // Is this a valid DTD?
                continue;
            }

            // Note: The EDID 1.x spec requires that all DTD blocks, when not
            //         being used as Monitor Descriptors nor DTD, should be filled
            //         with valid timing data. However, some monitors are still
            //         using data fill pattern, most commonly with all "0x01", to
            //         fill the DTD blocks. The monitor vendors should be contacted
            //         for EDID spec violation. However, we have added an workaround
            //         here to handle this. In the future if other data fill patterns
            //         should be discovered, the data "0x101" should be updated.
            Status = DisplayInfoRoutinesGetTimingFromDTD(pDTD, &TimingInfo);
            if (IS_DDSTATUS_ERROR(Status))
            {
                // Timing info not valid
                continue;
            }

            pDtDModeinfo[DtdCounter].DotClockInHz = TimingInfo.DotClockInHz;
            pDtDModeinfo[DtdCounter].HActive = TimingInfo.HActive;
            pDtDModeinfo[DtdCounter].VActive = TimingInfo.VActive;
            pDtDModeinfo[DtdCounter].VRoundedRR = TimingInfo.VRoundedRR;
        } // For loop ends
    }
    else
    {
        Status = DDS_INVALID_DATA;
    }
    // Status = DDS_SUCCESS;
    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Protocol Interface to remove Serial Number from EDID
 *     Block. Serial Number is present in EDID Base Block Header,
 *     DisplayId ProductID Block and Tiled Display Block.
 *
 * @param pEdidDataBuffer
 * @param DataIndex
 * @return void
 ***************************************************************/
void DisplayInfoParserClearSerialNumberFromEdidBlock(DD_BB_EDID_CONTAINER* pEdidDataBuffer, DDU32 DataIndex)
{
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;

    DDASSERT(pEdidDataBuffer);
    pEdidDataBuffer->EdidList[DataIndex].SerialNumChecksum = 0;
    pEdidDataBuffer->EdidList[DataIndex].IsEdidBlockModified = FALSE;

    if (0 == memcmp(BaseEdidHeader, pEdidDataBuffer->EdidList[DataIndex].EDIDBlock, EDID_HEADER_SIZE))
    {
        // EDID Block
        pEdidBaseBlock = (EDID_BASE_BLOCK*)pEdidDataBuffer->EdidList[DataIndex].EDIDBlock;
        DisplayInfoParserProcessSerialNumberBlock(pEdidDataBuffer, DataIndex, pEdidBaseBlock->PnpID.SerialNumber);
    }
    else
    {
        // DisplayId Block
        DisplayIdParserRemoveSerialNumber(pEdidDataBuffer, DataIndex);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Interface Function Declarations - END
 *
 ***************************************************************/

 /*-----------------------------------------------------------------------------
  *
  * Local function implementation
  *
  ***************************************************************/

  /*-----------------------------------------------------------------------------
   *
   * Generic Display Info parser related functions -- START
   *
   ***************************************************************/

   /***************************************************************
    * @brief Display Info Parser Add EDID Mode To List.
    *
    *  Add filtering logic to this once entire list is built.
    *      Assumption now is modes are parsed from EDID and built in the increasing order of priority
    *      Will need to add more filtering logic here if its not true in future.
    * @param pAddModes
    * @param pEdidData
    * @return DDSTATUS
    ***************************************************************/
DDSTATUS DisplayInfoParserAddEDIDModeToList(ADD_MODES* pAddModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pAddModes);
    DDASSERT(pAddModes->pTimingInfo);

    Status = DisplayInfoParserAddTimingToModeList(pEdidData, pAddModes->pTimingInfo, pAddModes->ForceAdd);

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Get Vic Index Based On Ce Index.
 *
 *  With CTA861-G, CeIndex is not a sequential index to
 *              global struct g_SupportedCeShortVideoModes, return
 *              VicId which will work as index to the struct.
 * @param  CeIndex
 * @param  pVicId
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS GetVicIndexBasedOnCeIndex(DDU8 CeIndex, DDU8* pVicId)
{
    // As per CTA-861G, Upto VIC ID-64 Native format can be declared using 7th bit
    // So, for VIC Id >64, the CeIndex are directly used as VicID
    *pVicId = (CeIndex & CEA_SHORT_VIDEO_DESCRIPTOR_CODE_MASK);
    *pVicId = (*pVicId <= CEA_MAX_VIC_SUPPORTING_NATIVE_FORMAT) ? (*pVicId) : (CeIndex);
    // Index to the array is calculated by adjusting for forbidden or not supported CeIndex(es)
    *pVicId = (*pVicId > 127) ? GET_VIC_INDEX(*pVicId) : (*pVicId);
    if ((*pVicId > MAX_CE_VICID) || (g_SupportedCeShortVideoModes[*pVicId].pTimingInfo == NULL))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_DPL_INVALID_VIC_ID);
        return DDS_DPL_INVALID_VIC_ID;
    }
    else
    {
        return DDS_SUCCESS;
    }
}

/***************************************************************
 * @brief  Adds Yuv444 Sampling Mode to the Mode Table.
 *
 * @param  pAddModes
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS AddYCbCr444SamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData)
{
    pAddModes->pTimingInfo->CeData.SamplingMode.Value = 0;
    pAddModes->pTimingInfo->CeData.SamplingMode.Yuv444 = 1;
    pAddModes->pTimingInfo->Flags.SupportedBPCMask = pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes;
    return AddEdidModeToModeTable(pAddModes, pEdidData);
}

/***************************************************************
 * @brief  Adds RGB Sampling Mode to the Mode Table.
 *
 * @param  pAddModes
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS AddRgbSamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData)
{
    pAddModes->pTimingInfo->CeData.SamplingMode.Value = 0;
    pAddModes->pTimingInfo->CeData.SamplingMode.Rgb = 1;
    pAddModes->pTimingInfo->Flags.SupportedBPCMask = pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes;
    return AddEdidModeToModeTable(pAddModes, pEdidData);
}

/***************************************************************
 * @brief Adds YUV420 Sampling Mode to the Mode Table.
 *
 * @param  pAddModes
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS AddYCbCr420SamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData)
{
    pAddModes->pTimingInfo->CeData.SamplingMode.Value = 0;
    pAddModes->pTimingInfo->CeData.SamplingMode.Yuv420 = 1;

    // Also populate Color depth from HF-VSDB is present to the timing info
    if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.ColorDepthMask != 0)
    {
        pAddModes->pTimingInfo->Flags.SupportedBPCMask = pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes;
    }
    else
    {
        // Assume only 8bits/color is supported which is mandatory for HDMI
        pAddModes->pTimingInfo->Flags.SupportedBPCMask.ColorDepthMask = 0;
        pAddModes->pTimingInfo->Flags.SupportedBPCMask.Supports8BitsPerColor = 1;
    }

    return AddEdidModeToModeTable(pAddModes, pEdidData);
}

/***************************************************************
 * @brief  Adds Yuv422 Sampling Mode to the Mode Table.
 *
 * @param  pAddModes
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS AddYCbCr422SamplingMode(ADD_MODES* pAddModes, EDID_DATA* pEdidData)
{
    pAddModes->pTimingInfo->CeData.SamplingMode.Value = 0;
    pAddModes->pTimingInfo->CeData.SamplingMode.Yuv422 = 1;

    pAddModes->pTimingInfo->Flags.SupportedBPCMask = pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes;
    return AddEdidModeToModeTable(pAddModes, pEdidData);
}

/***************************************************************
 * @brief Adds Edid Mode to the Mode Table.
 *
 * @param  pAddModes
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS AddEdidModeToModeTable(ADD_MODES* pAddModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;

    DISP_FUNC_ENTRY();

    DDRLSASSERT_EXPECT_NE(pAddModes->pTimingInfo->HActive, 0, DD_DIAG_FEATURE_EDID_DID, "HActive should not be zero");
    DDRLSASSERT_EXPECT_NE(pAddModes->pTimingInfo->VActive, 0, DD_DIAG_FEATURE_EDID_DID, "VActive should not be zero");
    DDRLSASSERT_EXPECT_NE(pAddModes->pTimingInfo->DotClockInHz, 0, DD_DIAG_FEATURE_EDID_DID, "DotClockInHz should not be zero");

    if ((pAddModes->pTimingInfo->HActive == 0) || (pAddModes->pTimingInfo->VActive == 0) || (pAddModes->pTimingInfo->DotClockInHz == 0))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_PARAM);
        return DDS_INVALID_PARAM;
    }

    Status = pAddModes->pTargetModeTable->pfnAddEntry(pAddModes->pTargetModeTable, pAddModes->pTimingInfo, pAddModes->ForceAdd);

    if (IS_DDSTATUS_ERROR(Status))
    {
        DISP_FUNC_EXIT();
        return Status;
    }

    if (Status == DDS_SUCCESS_ENTRY_NEW_ADDED)
    {
        pEdidData->NumModesInEdidOrDID++;
    }

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Update Mode Table with all the timings.
 *
 * All the timings will be check for supported sampling modes and accordingly they will be added to the mode table.
 * @param  pEdidModes
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserUpdateModeTable(DD_GET_EDID_MODES* pEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    DDU8 Count;
    DD_SAMPLING_MODE SamplingMode;
    ADD_MODES AddModes;
    DD_TILED_DISPLAY_INFO_BLOCK* pTiledBlock = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pModes);
    DDASSERT(pEdidModes);
    DDASSERT(pEdidModes->pModeTable);
    Status = DDS_SUCCESS;

    for (Count = 0; Count < pEdidData->NumTimingInfo; Count++)
    {
        DD_ZERO_MEM(&AddModes, sizeof(ADD_MODES));

        AddModes.pTimingInfo = &pEdidData->pModes[Count].TimingInfo;
        AddModes.pTargetModeTable = pEdidModes->pModeTable;
        AddModes.ForceAdd = pEdidData->pModes[Count].ForceAdd;

        AddModes.pTimingInfo->ModeType = DD_EDID_MODE;

        pTiledBlock = &pEdidData->pData->TiledDisplayInfo;

        if ((NULL != pTiledBlock) && (pTiledBlock->IsValidBlock))
        {
            if ((pTiledBlock->VTileSizeInLines == 0) || (AddModes.pTimingInfo->VActive == 0))
            {
                // DD_IS_TILED_MODE(pTiledBlock, AddModes.pTimingInfo) below will expand to check equality in aspect ratio between Tiled Mode params & TimingInfo params, which might cause a divide by
                // Zero error, hence asserting when the respective denominator values are 0
                DDASSERT(pTiledBlock->VTileSizeInLines); // DD_GET_ASPECT_RATIO(pTiledBlock->HTileSizeInPixels, pTiledBlock->VTileSizeInLines)
                DDASSERT(AddModes.pTimingInfo->VActive); // DD_GET_ASPECT_RATIO(pTimingInfo->HActive, pTimingInfo->VActive)
            }
            else
            {
                AddModes.pTimingInfo->ModeType = (DD_IS_TILED_MODE(pTiledBlock, AddModes.pTimingInfo) == TRUE) ? DD_TILED_MODE : DD_EDID_MODE;
            }
        }

        AddModes.pTimingInfo->Flags.SupportedBPCMask.ColorDepthMask = 0;
        SamplingMode.Value = AddModes.pTimingInfo->CeData.SamplingMode.Value;

        // Assume a case where there are identical Active timings in the EDID in different data blocks (only one timing is set as preferred in the EDID).
        // As part of the parsing of different data blocks of EDID if the timing which is not preferred gets added to the ModeList, the below code ensures
        // Assume a case where there are identical Active timings in the EDID in different data blocks, the below code ensures
        // that the preferred timing gets added to the ModeList by replacing the existing timing in the ModeList

        // DisplayInfoParser can report multiple preferred modes
        // Modes are already getting added in decreasing order of priority.
        // So, except for the first preferred mode getting added, do not "force add" subsequent preferred modes
        if (pEdidData->pModes[Count].TimingInfo.Flags.PreferredMode)
        {
            AddModes.pTimingInfo->Flags.PreferredMode = TRUE;
            AddModes.ForceAdd = (pEdidData->IsPreferredModeAdded == FALSE) ? TRUE : FALSE;
            pEdidData->IsPreferredModeAdded = TRUE;
        }
        else
        {
            AddModes.pTimingInfo->Flags.PreferredMode = FALSE;
        }

        // Add RGB Mode
        if ((SamplingMode.Rgb) || (0 == SamplingMode.Value))
        {
            // We try to add RGB Sampling Mode to the mode table for the below cases :
            //   1. If it is a RGB Sampling mode
            //   2. If the Sampling is Unknown (this happens for Non CE mode)
            Status = AddRgbSamplingMode(&AddModes, pEdidData);
            if (IS_DDSTATUS_ERROR(Status))
                continue;

            // Check if Sink supports Yuv444. If it does, add corresponding RGB/Non CE mode as Yuv444 mode
            if (TRUE == pEdidData->pData->FtrSupport.YCbCr444Supported)
            {
                Status = AddYCbCr444SamplingMode(&AddModes, pEdidData);
                if (IS_DDSTATUS_ERROR(Status))
                    continue;
            }

            // Add YCbCr422 Mode
            // Check if Sink supports YCbCr422. If it does, add corresponding RGB/Non CE mode as YCbCr422 mode
            if (TRUE == pEdidData->pData->FtrSupport.YCbCr422Supported)
            {
                Status = AddYCbCr422SamplingMode(&AddModes, pEdidData);
                if (IS_DDSTATUS_ERROR(Status))
                    continue;
            }
        }

        // Add YUV420 Mode
        if (SamplingMode.Yuv420)
        {
            Status = AddYCbCr420SamplingMode(&AddModes, pEdidData);
            if (IS_DDSTATUS_ERROR(Status))
                continue;
        }
    }

    Status = IS_DDSTATUS_ERROR(Status) ? DDS_UNSUCCESSFUL : DDS_SUCCESS;

    return Status;
}

/***************************************************************
 * @brief Checks the CE extension tags and revision fields and returns if its CE display or not.
 *
 * @param pCExtnBlock :
 * @return DD_BOOL
 ***************************************************************/
static DD_BOOL DisplayInfoParserIsCeExtnDisplay(CE_EDID_EXTN_BLOCK* pCExtnBlock)
{
    // Cross Check the Tag and Version
    // Adding support for Rev 3 and above and also Rev 1
    // Rev 1 corresponds to CEA 861 spec, it has DTDs but no SVDs
    // Adding support for Rev 1 since 861B specifies that 861B and above are
    // backward compatible with 861. Rev 1 extensions may be present in DVI monitors

    if ((CEA_EXT_TAG == pCExtnBlock->CeTag) && ((pCExtnBlock->Revision >= CEA_861_B_VERSION) || (pCExtnBlock->Revision == CEA_861_VERSION_1)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/***************************************************************
 * @brief Used to convert a 3 byte DDU8 array into integer.
 *
 * @param pArray
 * @param SzOfArray:
 * @return DDU32
 ***************************************************************/
static DDU32 DisplayInfoParserConvertHexArrayToInt(DDU8* pArray, DDU8 SzOfArray)
{
    DDU32 RetVal = 0;

    while (SzOfArray--)
    {
        RetVal |= pArray[SzOfArray] << (SzOfArray * BYTE_LENGTH);
    };

    return RetVal;
}

/***************************************************************
 * @brief Returns supported BPC mask from color depth.
 *
 * @param ColorBitDepth
 * @return DD_BPC_SUPPORTED
 ***************************************************************/
static DD_BPC_SUPPORTED DisplayInfoParserGetSupportedBpcMaskFromColorDepth(DDU32 ColorBitDepth)
{
    DD_BPC_SUPPORTED SupportedBpc;

    SupportedBpc.ColorDepthMask = 0;
    switch (ColorBitDepth)
    {
    case COLOR_DEPTH_48BPP:
        SupportedBpc.Supports16BitsPerColor = 1;
    case COLOR_DEPTH_42BPP:
        SupportedBpc.Supports14BitsPerColor = 1;
    case COLOR_DEPTH_36BPP:
        SupportedBpc.Supports12BitsPerColor = 1;
    case COLOR_DEPTH_30BPP:
        SupportedBpc.Supports10BitsPerColor = 1;
    case COLOR_DEPTH_24BPP:
        SupportedBpc.Supports8BitsPerColor = 1;
    case COLOR_DEPTH_18BPP:
        SupportedBpc.Supports6BitsPerColor = 1;
        break;
    case UNDEFINED_COLOR_DEPTH:
    default:
        SupportedBpc.ColorDepthMask = 0;
        break;
    };

    return SupportedBpc;
}

/***************************************************************
 * @brief This method will be used to calculate the checksum of the
 *      Serial Num and afterwards zero it out.
 *
 * @param pEdidDataBuffer
 * @param DataIndex
 * @param pSerialNumBlock
 * @return void
 ***************************************************************/
static void DisplayInfoParserProcessSerialNumberBlock(DD_BB_EDID_CONTAINER* pEdidDataBuffer, DDU32 DataIndex, DDU8* pSerialNumBlock)
{
    DDASSERT(pEdidDataBuffer);
    DDASSERT(pSerialNumBlock);

    // If the Serial Number is already zeroed out, we do not make any changes
    if ((0 == pSerialNumBlock[0]) && (0 == pSerialNumBlock[1]) && (0 == pSerialNumBlock[2]) && (0 == pSerialNumBlock[3]))
    {
        return;
    }

    // Save the Serial Num Checksum
    pEdidDataBuffer->EdidList[DataIndex].SerialNumChecksum = 0x100 - ((pSerialNumBlock[0] + pSerialNumBlock[1] + pSerialNumBlock[2] + pSerialNumBlock[3]) & 0xFF);

    // Zero Out the Serial Number
    pSerialNumBlock[0] = 0;
    pSerialNumBlock[1] = 0;
    pSerialNumBlock[2] = 0;
    pSerialNumBlock[3] = 0;

    pEdidDataBuffer->EdidList[DataIndex].IsEdidBlockModified = TRUE;
}

/***************************************************************
 * @brief This function parses the Type 4 and type 8 timings
 *      Type 4 timing is added in DID 1.x block, type 8 timing is added through DID2.x block and CTA block consisting of DID timings.
 *
 * @param pTiming4HdrOverride
 * @param TimingCodeSizeInByte
 * @param pEdidData
 * @param EnumeratedTimingTag
 * @param pDidTimingInfo4
 * @param NumType4Descriptors
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserParseEnumTimings(SHORT_TIMING_4_REVID* pTiming4HdrOverride, DDU8 TimingCodeSizeInByte, EDID_DATA* pEdidData, DDU8 EnumeratedTimingTag, DDU8* pDidTimingInfo4,
    DDU8 NumType4Descriptors)
{
    DDSTATUS Status;
    DDU32 TimingCodeCtr;
    DDU8 IdCode, VicId;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    CE_MODE_LIST CeVicData;

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));
    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    AddModes.pTimingInfo = &TimingInfo;
    AddModes.ForceAdd = TRUE;
    Status = DDS_SUCCESS; // Initialization

    // ToDo: VSDI-45682 DID 2.0 & 2.1 have difference of one bit to show support of YUV420

    switch (pTiming4HdrOverride->TimingCode)
    {
    case TIMING_CODE_DMT:
        for (TimingCodeCtr = 0; TimingCodeCtr < NumType4Descriptors; TimingCodeCtr++)
        {
            IdCode = (TimingCodeSizeInByte > 1) ? DisplayInfoParserConvertHexArrayToInt(&pDidTimingInfo4[TimingCodeCtr * TimingCodeSizeInByte], TimingCodeSizeInByte) : pDidTimingInfo4[TimingCodeCtr];
            if ((IdCode >= g_ulTotalDisplayID_VESA_DMTmodes) || (g_DisplayID_VESA_DMTModeTable[IdCode].pTimingInfo == NULL))
            {
                continue;
            }

            TimingInfo = *(g_DisplayID_VESA_DMTModeTable[IdCode].pTimingInfo);
            VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;
            // Add mode to the list
            TimingInfo.ModeSource = DID_DTD;
            TimingInfo.CeData.VicId[0] = VicId;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
            TimingInfo.SignalStandard = DD_VESA_DMT;
            TimingInfo.Flags.PreferredMode = FALSE; // These are not preferred modes as per Display ID spec
            if (DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII == EnumeratedTimingTag)
            {
                TimingInfo.CeData.SamplingMode = pEdidData->pData->BasicDisplayCaps.DidSamplingMode;
            }

            Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }
        break;
    case TIMING_CODE_CEA_VIC:
        for (TimingCodeCtr = 0; TimingCodeCtr < NumType4Descriptors; TimingCodeCtr++)
        {
            VicId = 0;
            IdCode = pDidTimingInfo4[TimingCodeCtr];
            Status = GetVicIndexBasedOnCeIndex(IdCode, &VicId);

            if (IS_DDSTATUS_ERROR(Status) || (VicId >= g_ulTotalDisplayID_CEA_modes) || (g_DisplayID_CeStandardTiming[VicId].pTimingInfo == NULL))
            {
                continue;
            }

            TimingInfo = *(g_DisplayID_CeStandardTiming[VicId].pTimingInfo);

            // CE extension display and baseblock DTD has CE timings
            // Add it to the VIClist with all required params
            CeVicData.IsPreferred = FALSE;
            CeVicData.SignalStandard = DD_CEA_861B; // It is still monitor timings
            CeVicData.SamplingMode.Rgb = 1;
            if (DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII == EnumeratedTimingTag)
            {
                CeVicData.SamplingMode = pEdidData->pData->BasicDisplayCaps.DidSamplingMode;
            }
            // Since CEIndex here is <64, no need to convert it
            CeVicData.VicId[0] = g_DisplayID_CeStandardTiming[VicId].CEIndex;
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
        }
        break;
    case TIMING_CODE_HDMI_VIC:
        for (TimingCodeCtr = 0; TimingCodeCtr < NumType4Descriptors; TimingCodeCtr++)
        {
            VicId = pDidTimingInfo4[TimingCodeCtr];
            IdCode = GET_HDMI_4K2K_VIC(VicId);

            if ((IdCode == 0) || (IdCode >= g_ulNum4kx2kModes) || (g_4kx2kModes[IdCode].pTimingInfo == NULL))
            {
                continue;
            }

            TimingInfo = *(g_4kx2kModes[IdCode].pTimingInfo);
            VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

            // Add mode to the list
            TimingInfo.ModeSource = DID_DTD;
            TimingInfo.CeData.VicId[0] = VicId;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
            TimingInfo.SignalStandard = DD_CEA_861B;
            TimingInfo.Flags.PreferredMode = FALSE; // These are not preferred modes as per Display ID spec
            if (DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII == EnumeratedTimingTag)
            {
                TimingInfo.CeData.SamplingMode = pEdidData->pData->BasicDisplayCaps.DidSamplingMode;
            }
            Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }
        break;
    default: // This is an error in the Display ID block
        Status = DDS_MONITOR_NO_MORE_DESCRIPTOR_DATA;
        break;
    }

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief This function parses DID type 8 timings from CTA block
 *      Parse the needed information and call DisplayInfoParserParseEnumTimings.
 *
 * @param pType8TimingBlock
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserParseCtaType8Timing(CEA_EXT_FORMULA_TIMING_VIII* pType8TimingBlock, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    DDU8 NumType4Descriptors, TimingCodeSizeInByte;
    SHORT_TIMING_4_REVID Timing4HdrOverride;
    DDU8* pDidTimingInfo8 = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    TimingCodeSizeInByte = pType8TimingBlock->TimingInfo8.TimingCodesize + 1;
    NumType4Descriptors = (pType8TimingBlock->CeExtHeader.CeHdr.Length - 2) / (sizeof(DDU8) * TimingCodeSizeInByte);
    Timing4HdrOverride.RevAndOtherData = pType8TimingBlock->TimingInfo8.RevAndOtherData;
    pDidTimingInfo8 = &(pType8TimingBlock->DidTimingInfo8);

    Status = DisplayInfoParserParseEnumTimings(&Timing4HdrOverride, TimingCodeSizeInByte, pEdidData, CEA_TYPE_VIII_VIDEO_TIMING_DATA_BLOCK, pDidTimingInfo8, NumType4Descriptors);
    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief This function parses the Type 1 and type 7 timings
 *      Type 7 timings can be added in an EDID through both CTA extension block & DisplayID extension block.
 *
 * @param pEdidData
 * @param DTDTimingTag
 * @param DotClockScale
 * @param pDidTimingInfo
 * @param NumberofDtds
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS DisplayInfoParserParseDTDTiming(EDID_DATA* pEdidData, DDU8 DTDTimingTag, DDU32 DotClockScale, TIMINGINFO_1* pDidTimingInfo, DDU8 NumberofDtds)
{
    DDSTATUS Status;
    DDU8 VicId, DtdCounter;
    DDU32 XRes, YRes, RRate, HTotal, VTotal, HBlank, VBlank, HSPW, VSPW, HSO, VSO;
    DDU64 PixelClock;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DDU32 FrameScanTypeMultiplier;

    Status = DDS_SUCCESS;

    for (DtdCounter = 0; DtdCounter < NumberofDtds; DtdCounter++, pDidTimingInfo++)
    {
        DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

        FrameScanTypeMultiplier = (pDidTimingInfo->FrameScanType == SCAN_TYPE_INTERLACED) ? 2 : 1;

        XRes = pDidTimingInfo->HActive + 1;
        YRes = pDidTimingInfo->VActive + 1;

        YRes *= FrameScanTypeMultiplier;

        PixelClock = DisplayInfoParserConvertHexArrayToInt(pDidTimingInfo->PixelClock, 3); // 3 bytes of pixel clock;
        // Pixel clock is represented in units of 10,000 in DID 1.3 & 1,000 in DID 2.0
        PixelClock = (PixelClock + 1) * DotClockScale;

        if ((PixelClock == DotClockScale) || (pDidTimingInfo->HActive == 0) || (pDidTimingInfo->VActive == 0))
        {
            // Invalid DTD, parse next one
            DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid DTD in the Display ID");
            continue;
        }

        if (DID2_DATA_BLOCK_DETAIL_TIMING_VII == DTDTimingTag)
        {
            TimingInfo.CeData.SamplingMode = pEdidData->pData->BasicDisplayCaps.DidSamplingMode;
            if (TimingInfo.CeData.SamplingMode.Yuv420 && pEdidData->pData->BasicDisplayCaps.DidMinPixelRateForYCbCr420Modes)
            {
                if (PixelClock < ((pEdidData->pData->BasicDisplayCaps.DidMinPixelRateForYCbCr420Modes * DISPLAYID_TYPE7_PIXELRATE_MULTIPLIER * (DDU64)(DD_1K * DD_1K)) / DD_CENT))
                {
                    TimingInfo.CeData.SamplingMode.Yuv420 = 0;
                }
                if (0 == TimingInfo.CeData.SamplingMode.Value)
                {
                    continue;
                }
            }
        }

        HBlank = pDidTimingInfo->HBlank + 1;
        HTotal = XRes + HBlank;

        VBlank = pDidTimingInfo->VBlank + 1;
        VBlank *= FrameScanTypeMultiplier;

        VTotal = YRes + VBlank;
        if ((HTotal * VTotal) > 0) // HTotal, VTotal can be 0 and even HTotal * VTotal can overflow to 0, checking for the same
        {
            RRate = (DDU32)(DD_ROUND_DIV((PixelClock), (HTotal * VTotal)));
        }
        else
        {
            continue;
        }

        RRate *= FrameScanTypeMultiplier;

        HSPW = pDidTimingInfo->HSyncWidth + 1;
        VSPW = pDidTimingInfo->VSyncWidth + 1;
        VSPW *= FrameScanTypeMultiplier;

        HSO = pDidTimingInfo->HSyncOffset + 1;
        VSO = pDidTimingInfo->VSyncOffset + 1;
        VSO *= FrameScanTypeMultiplier;

        if (((HSO + HSPW) > HBlank) || ((VSO + VSPW) > VBlank))
        {
            // Invalid DTD, parse next one
            continue;
        }

        TimingInfo.DotClockInHz = PixelClock;
        TimingInfo.HTotal = HTotal;
        TimingInfo.HActive = XRes;
        TimingInfo.HBlankStart = XRes;
        TimingInfo.HBlankEnd = HTotal - 1;
        TimingInfo.HSyncStart = XRes + HSO;
        TimingInfo.HSyncEnd = TimingInfo.HSyncStart + HSPW - 1;
        TimingInfo.HRefresh = (DDU32)(PixelClock / HTotal);
        TimingInfo.VTotal = VTotal;
        TimingInfo.VActive = YRes;
        TimingInfo.VBlankStart = YRes;
        TimingInfo.VBlankEnd = VTotal - 1;
        TimingInfo.VSyncStart = YRes + VSO;

        TimingInfo.VSyncEnd = TimingInfo.VSyncStart + VSPW - 1;
        TimingInfo.VRoundedRR = RRate;
        TimingInfo.IsInterlaced = pDidTimingInfo->FrameScanType;
        TimingInfo.HSyncPolarity = (pDidTimingInfo->HSyncPolarity) ? FALSE : TRUE;
        TimingInfo.VSyncPolarity = (pDidTimingInfo->VSyncPolarity) ? FALSE : TRUE;
        if (pDidTimingInfo->Preferedflag)
        {
            TimingInfo.Flags.PreferredMode = TRUE;
            AddModes.ForceAdd = TRUE;
        }
        else
        {
            AddModes.ForceAdd = FALSE;
        }

        VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

        // Add mode to the list
        TimingInfo.ModeSource = DID_DTD;
        TimingInfo.CeData.VicId[0] = VicId;
        TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
        TimingInfo.SignalStandard = DD_SIGNAL_UNKNOWN;

        AddModes.pTimingInfo = &TimingInfo;

        Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
    } // for loop for DTDCounter ends
    return Status;
}

/***************************************************************
 * @brief This function parses DID type 7 timings from CTA block
 *      Parse the needed information and call .
 *
 * @param pType7TimingBlock
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserParseCtaType7Timing(CEA_EXT_FORMULA_TIMING_VII* pType7TimingBlock, EDID_DATA* pEdidData)
{
    DDU8 NumberofDtds;
    TIMINGINFO_1* pDidTiming;
    DDSTATUS Status;

    pDidTiming = &pType7TimingBlock->TimingInfo7;
    NumberofDtds = (pType7TimingBlock->CeExtHeader.CeHdr.Length - 2) / sizeof(TIMINGINFO_1);

    Status = DisplayInfoParserParseDTDTiming(pEdidData, CEA_TYPE_VII_VIDEO_TIMING_DATA_BLOCK, DID_2_0_DOT_CLOCK_SCALE, pDidTiming, NumberofDtds);
    return Status;
}

/*-------------------------------------------------------------
 *
 * Generic Display Info parser related functions -- END
 *
 ***************************************************************/

 /*
  *
  * EDID parser related functions -- START
  *
  */

  /***************************************************************
   * @brief Edid Parser Verify Data Blocks.
   *
   *  This function verifies if incoming EDID block data is valid by verifying.
   *       1. Buffer size passed.
   *       2. Checksum for each EDID/CE extension block.
   * @param pEdidOrDisplayIDBuf
   * @param BufSizeInBytes
   * @param BlockNum
   * @return DDSTATUS
   ***************************************************************/
DDSTATUS EdidParserVerifyDataBlocks(DDU8* pEdidOrDisplayIDBuf, DDU32 BufSizeInBytes, DDU8 BlockNum)
{
    DDSTATUS Status;
    EDID_BASE_BLOCK* pEdidBaseBlock;
    DDU8 EdidVerAndRev;

    // Arg Validation
    DDASSERT(pEdidOrDisplayIDBuf);

    // If Block size if not EDID_BLOCK_SIZE (128)
    if (EDID_BLOCK_SIZE != BufSizeInBytes)
    {
        // If the passed in total EDID block size != size as indicated by EDID data
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_DATA);
        return DDS_INVALID_DATA;
    }

    // Verify checksum
    if (pEdidOrDisplayIDBuf[EDID_BLOCK_SIZE - 1] != DisplayInfoRoutinesCalcChksum(pEdidOrDisplayIDBuf, EDID_BLOCK_SIZE))
    {
        // Invalid buffer passed in.
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_DATA);
        return DDS_INVALID_DATA;
    }

    // If Block is extension block, check if it has valid TAG.
    if (BlockNum > 0)
    {
        // Check if it is a valid Extn tag
        switch (pEdidOrDisplayIDBuf[0])
        {
        case CEA_EXT_TAG:            // CEA 861 Series Extension
        case VTB_EXT_TAG:            // Video Timing Block Extension
        case EDID_2_0_EXT_TAG:       // EDID 2.0 Extension
        case DI_EXT_TAG:             // Display Information Block Extension
        case LS_EXT_TAG:             // Localized String Extension
        case DPVL_EXT_TAG:           // Digital Packet Video Extension
        case DID_EXT_TAG:            // DisplayID Extension to EDID
        case BLOCK_MAP_EXT_TAG:      // Block Map Extension
        case MANF_SPECIFIED_EXT_TAG: // Extensions Defined by Manufactures
            Status = DDS_SUCCESS;
            break;
        default:
            // Some of the EDID might have base block that might advertize of extension block's presence, but may not have any extension block.
            // If extension Tag and checksum is found to be 0, consider extension block to be absent, and do not fail. Consider base with any valid extension block for parsing.
            Status = ((0 == pEdidOrDisplayIDBuf[0]) && (0 == pEdidOrDisplayIDBuf[EDID_BLOCK_SIZE - 1])) ? DDS_SUCCESS : DDS_INVALID_DATA;
            break;
        }

        DISP_FUNC_EXIT();
        return Status;
    }

    // Check if Header read is Valid.
    if (0 != memcmp(BaseEdidHeader, pEdidOrDisplayIDBuf, EDID_HEADER_SIZE))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_DATA);
        return DDS_INVALID_DATA;
    }

    // Check if EDID version read is Valid
    pEdidBaseBlock = (EDID_BASE_BLOCK*)pEdidOrDisplayIDBuf;
    EdidVerAndRev = (pEdidBaseBlock->Version << 4) | (pEdidBaseBlock->Revision);

    switch (EdidVerAndRev)
    {
    case EDID_VERSION_1_2:
    case EDID_VERSION_1_3:
    case EDID_VERSION_1_4:
        Status = DDS_SUCCESS;
        break;
    default:
        Status = DDS_INVALID_DATA;
        break;
    }

    // There is no error, so return success
    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Edid Parser Parse Display Id Extension.
 *
 *  Base function to Parse CE Extension block present as part of EDID extension.
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *      Note: Ensure that the order of these calls doesn't change
 *              as the 1st 2 calls is where we get the VicList parsed and populated
 *          1. EdidParserAddModesFromVideoDataBlock to add SVD modes
 *          2. EdidParserAddModesFrom420VideoDataBlock to add 420 modes
 *          3. EdidParserParseCapabilityBlock to update 420 modes
 *          4. EdidParserParseVideoFormatPreferenceBlock to update preferred mode
 *          5. EdidParserParseHdrCapDataBlock to parse HDR block
 *          6. EdidParserParseAudioDataBlock to update Audio caps
 *          7. Parse and add DTDs if any
 *          8. Parse HDMI VSDB Block to add S3D and 4k2k Modes
 *          9. EdidParserParseHFVSDB to update HDMI caps.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserParseDisplayIdExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    DDU32 BlockNum;
    DDU32 NumExtBlocks;
    DD_GET_EDID_MODES GetEdidModes;
    DISPLAYID_EDID_EXTN_HEADER* pDisplayIdHeader = NULL;

    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;
    NumExtBlocks = pGetEdidModes->NumEdidExtensionsParsed;

    for (BlockNum = 1; (BlockNum <= NumExtBlocks) && (BlockNum < MAX_EDID_EXTENSIONS_SUPPORTED); BlockNum++)
    {
        if (pGetEdidModes->BufSizeInBytes < (BlockNum * EDID_BLOCK_SIZE))
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "Wrong length for buffer passed in by called. Hence failing ");
            break;
        }

        pDisplayIdHeader = (DISPLAYID_EDID_EXTN_HEADER*)((DDU8*)pEdidBaseBlock + (BlockNum * EDID_BLOCK_SIZE));
        if (pDisplayIdHeader->TagId != DID_EXT_TAG)
        {
            // This is not a Display ID block, check for next one
            continue;
        }

        GetEdidModes.BufSizeInBytes = EDID_BLOCK_SIZE;

        // Display ID Header starts from the next byte in the block
        GetEdidModes.pEdidOrDisplayIDBuf = ((DDU8*)pDisplayIdHeader + 1);
        GetEdidModes.pModeTable = pGetEdidModes->pModeTable;

        if (pGetEdidModes->pModeTable == NULL)
        {
            // Call only get caps function if pModeTable was NULL
            // DID2 can also be extension to EDID
            switch (pDisplayIdHeader->DisplayIdHeader.RevisionAndVersion)
            {
            case DISPLAY_ID_VER_2_1:
                DisplayIdV21ParserGetEdidCaps(&GetEdidModes, pEdidData);
                break;
            case DISPLAY_ID_VER_2_0:
                DisplayIdV2ParserGetEdidCaps(&GetEdidModes, pEdidData);
                break;
            default:
                DisplayIdParserGetEdidCaps(&GetEdidModes, pEdidData);
                break;
            }
        }
        else
        {
            // DID2 can also be extension to EDID
            switch (pDisplayIdHeader->DisplayIdHeader.RevisionAndVersion)
            {
            case DISPLAY_ID_VER_2_1:
            case DISPLAY_ID_VER_2_0:
                // A new formula based timing block got added in DID 2.1. Separate functions will be
                // required post formula based timing implementation. Reusing the same function for now.
                DisplayIdV2ParserGetEdidModes(&GetEdidModes, pEdidData);
                break;
            default:
                DisplayIdParserGetEdidModes(&GetEdidModes, pEdidData);
                break;
            }
        }
    } // for loop for all EDID extension blocks

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Edid Parser Mark Preferred Timings mentioned as DTDs in VFPDB.
 *
 *  Method to mark the preferred EDID modes in EDID.
 *         This will be done as per the CTA 861-H spec
 *      IF CTA extension version 1  (condition 1) or there is no Extn block present
 *          Then, Mark First DTD in Block 0 as preferred, noted for marking as preferred in EdidParserParseBaseBlockDTDs()
 *      Else IF VFPDB Block is present (condition 2)
 *          Then, mark DTDs from VFPDB block are preferred, in this function
 *      Else    (condition 3)
 *         First DTD of Block 0 and First SVD, marked as preferred in CeInfoParserAddModesFromVideoDataBlock()
 *
 *      Note: as of now, there is no way of marking Modes as preferred in order of any priority
 *              they can be just be marked as preferred.
 *
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserMarkVfpdbTimingsAsPreferred(EDID_DATA* pEdidData)
{
    DDU8 BitNum, BitNumMod;
    DDU8 Index;
    DDU16 VfpdbDtdCount;
    DDU16 VfpDtdMask;
    MODE_SOURCE SourceMode;
    MODE_SOURCE ReqdSourceMode;

    DDASSERT(pEdidData);

    VfpdbDtdCount = pEdidData->Vfpdb.DtdCount;
    VfpDtdMask = pEdidData->Vfpdb.DtdMask;
    BitNum = 0;
    BitNumMod = 0;
    Index = 0;

    // if VfpdbDtdCount is Non-Zero, that means CEA_861_B_VERSION, it means condition 1 mentioned in function description is FALSE
    // vice-versa, if Extn blocks or VFPDB not present, Base block DTD needs to be marked as preferred, taken care while parsing base block DTDs
    if (VfpdbDtdCount == 0)
    {
        return;
    }

    for (BitNum = 0; BitNum < DD_MAX_DTD_IN_VFPDB_COUNT; BitNum++)
    {
        // based on the DTD number mentioned in VFPDB, and the, find out which timing needs to be marked as preferred
        // BitNum reflects that DTD number decremented by 1 (starting index 0)
        if (FALSE == DD_IS_BIT_SET(VfpDtdMask, BitNum))
        {
            continue;
        }

        if (BitNum < pEdidData->NumBaseBlockDtds)
        {
            // if DTD mask number was lesser than Base Block DTD count, it has to be a timing from Base block
            ReqdSourceMode = BASE_DTD;
        }
        else
        {
            ReqdSourceMode = CE_DTD; // higher than BASE block DTDs, it will be Extn block DTDs
        }

        for (DDU8 DtdCount = 0, BaseIndex = 0, CeIndex = 0; DtdCount < pEdidData->NumTimingInfo; DtdCount++)
        {
            // for VFPDB, Base block DTDs & CE extn block DTDs are included
            // Index is used as a variable to traverse through the list, where all CE_DTDs are in order of their appearance in EDID, but BASE_DTDs are parsed later to CE_DTDs
            // hence, Index + NumBaseBlockDtds is needed to point at correct DTD

            SourceMode = pEdidData->pModes[DtdCount].TimingInfo.ModeSource;

            Index = (BitNum < pEdidData->NumBaseBlockDtds) ? BaseIndex : CeIndex;
            BitNumMod = (BitNum < pEdidData->NumBaseBlockDtds) ? BitNum : BitNum - pEdidData->NumBaseBlockDtds;

            if ((BitNumMod == Index) && (ReqdSourceMode == SourceMode))
            {
                pEdidData->pModes[DtdCount].TimingInfo.Flags.PreferredMode = TRUE;
                break;
            }

            BaseIndex += (SourceMode == BASE_DTD) ? 1 : 0;
            CeIndex += (SourceMode == CE_DTD) ? 1 : 0;
        }
    }
}

/***************************************************************
 * @brief Edid Parser Get Edid Modes.
 *
 *  Method to Get all EDID modes present in EDID.
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *
 *      Note: Below calls has be in order of decreasing priority.
 *            Higher priority modes are added 1st and lower priority modes are added later
 *
 *          1. EdidGetSupportedBPCFromBaseBlock to get the Supported BPC from base block.
 *          2. EdidParserParseCeExtension to parse CE extension blocks
 *          3. EdidParserParseDisplayIdExtension to parse all Display ID modes
 *          4. EdidParserParseBaseBlockDTDs to parse Base Block DTDs
 *          5. EdidParserParseBaseEstablishedModes to parse Established modes
 *          6. EdidParserParseBaseStdModes to parse Standard modes
 *          7. EdidParserParseEstTimingIIIBlock to parse Established Timing III modes.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // 1. Get Supported BPC from base block. This is required for filling the Timing information.
    EdidGetSupportedBPCFromBaseBlock(pGetEdidModes, pEdidData);

    // 2. Get base block monitor details
    EdidParserGetBaseBlockMonitorDetails(pGetEdidModes, pEdidData);

    // 3. Parse CE extension block -- Will add any non-CE modes to mode-table
    EdidParserParseCeExtension(pGetEdidModes, pEdidData);

    // 4. Parse DID extension -- Will add any non-CE modes to mode-table
    EdidParserParseDisplayIdExtension(pGetEdidModes, pEdidData);

    // 5. Parse DTD section
    EdidParserParseBaseBlockDTDs(pGetEdidModes, pEdidData);

    // 6. Parse established section
    EdidParserParseBaseEstablishedModes(pGetEdidModes, pEdidData);

    // 7. Parse Standard timing section
    EdidParserParseBaseStdModes(pGetEdidModes, pEdidData);

    // 8. Parse Established Timing III section
    // EDID 1.4  has support for ESTABLISHED TIMINGS III BLOCK MONITOR DESCRIPTOR
    // For EDID 1.3, this function will return zero for ulNumModes
    EdidParserParseEstTimingIIIBlock(pGetEdidModes, pEdidData);

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Edid Parser Get Edid Caps.
 *
 *  Method to Get all EDID Caps present in EDID.
 *         This will inturn call various subfunctions below which inturn will parse all caps
 *
 *          1. Get Supported BPC from Base Block
 *          2. EdidParserGetBaseBlockMonitorDetails to gather all generic info from the monitor
 *          3. EdidParserParseCeExtension to parse CE extension blocks
 *          4. DisplayIdParserGetEdidModes to parse Display ID extension block.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // 1. Get the Supported BPC from Base block. This is required to fill the Supported BPC info in timing structure.
    EdidGetSupportedBPCFromBaseBlock(pGetEdidModes, pEdidData);

    // 2. Get base block monitor details
    EdidParserGetBaseBlockMonitorDetails(pGetEdidModes, pEdidData);

    // 3. Parse CE extension block for caps
    EdidParserParseCeExtension(pGetEdidModes, pEdidData);

    // 4. Parse DID extension for caps
    EdidParserParseDisplayIdExtension(pGetEdidModes, pEdidData);

    return;
}

/***************************************************************
 * @brief Edid Parser Get Base Block Monitor Details.
 *
 *  Method to parse Monitor details (based on EDID version):
 *      1. Monitor Range limits
 *      2. Monitor Name
 *      3. Min/max RR and dotclock supported
 *      4. Monitor supported Gamma
 *      5. Monitor Interface supported - DP/HDMI/DVI
 *      6. Monitor support for CVT/GTF modes
 *      7. Chromaticity info
 *      8. Monitor support BPC.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void EdidParserGetBaseBlockMonitorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU8* pTempEdidBaseBlock = NULL;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    EDID_PNPPID* pPnpId = NULL;
    DDU32 DTDNumber;
    DD_PNP_ID TempPnpID;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Making TempPnpID Zero for Zero check comparison with parsed PnpID from EDID
    DD_ZERO_MEM(&TempPnpID, sizeof(DD_PNP_ID));

    pEdidData->pData->FtrSupport.IsCVTSupported = FALSE;
    pEdidData->pData->FtrSupport.IsCVTRedBlankSupported = FALSE;
    pEdidData->pData->FtrSupport.IsGTFSupported = FALSE;
    pEdidData->pData->FtrSupport.IsMRLBlockPresent = FALSE;

    pTempEdidBaseBlock = pGetEdidModes->pEdidOrDisplayIDBuf;

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pTempEdidBaseBlock;

    // Parse common display parameters between EDID 1.3 and EDID 1.4

    // Get Vendor product ID
    pPnpId = &pEdidBaseBlock->PnpID;
    pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[0] = pPnpId->ManufacturerID[0];
    pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[1] = pPnpId->ManufacturerID[1];
    pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[2] = 0; // EDID block does not define this

    DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ProductID, sizeof(pPnpId->ProductID), pPnpId->ProductID, sizeof(pPnpId->ProductID));
    DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.SerialNumber, sizeof(pPnpId->SerialNumber), pPnpId->SerialNumber, sizeof(pPnpId->SerialNumber));

    pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.WeekOfManufacture = pPnpId->WeekOfManufacture;
    pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.YearOfManufacture = pPnpId->YearOfManufacture;

    // EDID spec mandates Manufacturer ID to be non-zero making PnpID a non-zero value, marking validity acoordingly
    if (0 != memcmp(&pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID, &TempPnpID, sizeof(DD_PNP_ID)))
    {
        pEdidData->pData->BasicDisplayCaps.IsBaseBlkPnpIDValid = TRUE;
    }

    // Get Chromaticity data and color space
    pEdidData->pData->BasicDisplayCaps.ColorModel = (pEdidBaseBlock->BasicDisplayParams.ColorSpace == 1) ? DD_COLOR_MODEL_RGB : DD_COLOR_MODEL_CUSTOM;
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.RedX = (pEdidBaseBlock->ChromaticityData.RedXUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.RedXLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.RedY = (pEdidBaseBlock->ChromaticityData.RedYUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.RedYLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.GreenX = (pEdidBaseBlock->ChromaticityData.GreenXUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.GreenXLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.GreenY = (pEdidBaseBlock->ChromaticityData.GreenYUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.GreenYLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.BlueX = (pEdidBaseBlock->ChromaticityData.BlueXUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.BlueXLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.BlueY = (pEdidBaseBlock->ChromaticityData.BlueYUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.BlueYLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.WhiteX = (pEdidBaseBlock->ChromaticityData.WhiteXUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.WhiteXLowBits);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.WhiteY = (pEdidBaseBlock->ChromaticityData.WhiteYUpperBits << 2) | (pEdidBaseBlock->ChromaticityData.WhiteYLowBits);

    if (pEdidBaseBlock->BasicDisplayParams.Gamma == 0xFF)
    {
        // Return default of 0x78 if the read from the base EDID block fails
        //   as SoftBIOS currently does not support parsing the DI-EXT block.
        //   The 0x78 translates to a default gamma value of 2.2 (0x78 + 100 / 100)
        //   supported by the majority of LFP panels.
        pEdidData->pData->BasicDisplayCaps.DisplayGamma = 0x78;
    }
    else
    {
        pEdidData->pData->BasicDisplayCaps.DisplayGamma = pEdidBaseBlock->BasicDisplayParams.Gamma;
    }
    pEdidData->pData->FtrSupport.IsDigitalInput = pEdidBaseBlock->BasicDisplayParams.IsDigitalInput;
    pEdidData->pData->BasicDisplayCaps.HSize = pEdidBaseBlock->BasicDisplayParams.MaxHImageSize;
    pEdidData->pData->BasicDisplayCaps.VSize = pEdidBaseBlock->BasicDisplayParams.MaxVImageSize;

    if ((pEdidData->EdidOrDidVerAndRev < EDID_VERSION_1_4))
    {
        // Find a block in DTD[0..3] that defines the monitor Range limits :
        // DTD[0] is contains timing data but DTD[1..3] can be either
        // a timing descriptor or a monitor descriptor.
        // Every DTD block has DataTypeTag which describes the type of the data that it contains.
        // DataTypeTag is defined as MONITOR_RANGE_LIMITS_TAG(FDh) for Monitor Range Limits.
        // Note : "The first descriptor block shall be used to indicate the display's preferred timing mode"
        // Displays using EDID version 1 revision 0-2 may not meet all of these requirements.

        // Go through DTD[0..3] and find a descriptor for Monitor Range Limits.

        pEdidData->pData->FtrSupport.IsGTFSupported = pEdidBaseBlock->BasicDisplayParams.IsGTFSupported;
        pEdidData->pData->FtrSupport.IsContinuousFreqSupported = FALSE;

        for (DTDNumber = 0; DTDNumber < MAX_EDID_DTD_BLOCKS; DTDNumber++)
        {
            // Get monitor descriptor data
            EdidParserGetMonitorDescriptorDetails(&pEdidBaseBlock->EdidTiming.Edid_13_MonitorInfo[DTDNumber], pEdidData);
        }
    }
    else
    { // EDID 1.4 or higher
        pEdidData->pData->FtrSupport.IsContinuousFreqSupported = pEdidBaseBlock->BasicDisplayParams.IsContinuousFreqSupported;

        // Check if the Video Input is digital or analog
        if (pEdidBaseBlock->BasicDisplayParams.IsDigitalInput == TRUE)
        {
            // DisplayPort is supported if Digital video interface is 5 as per EDID spec 1.4

            if (pEdidBaseBlock->BasicDisplayParams.DigitalVideoInterface == VIDEO_INTERFACE_DISPLAYPORT)
            {
                pEdidData->pData->FtrSupport.IsDisplayPortSupported = TRUE;
            }

            // HDMI is supported if Digital video interface is 2 or 3 as per EDID spec 1.4
            // if 2, HDMI-a is supported.
            // if 3, HDMI-b is supported.
            // However currently we dont have support for HDMI-b.
            // so no need to check for the value 3.
            if ((pEdidBaseBlock->BasicDisplayParams.DigitalVideoInterface == VIDEO_INTERFACE_HDMI_A) || (pEdidBaseBlock->BasicDisplayParams.DigitalVideoInterface == VIDEO_INTERFACE_HDMI_B))
            {
                pEdidData->pData->BasicDisplayCaps.HdmiDisplayVersion = HDMI_VERSION_1_4;
            }

            // DVI is supported if Digital video interface is 1 as per EDID spec 1.4
            if (pEdidBaseBlock->BasicDisplayParams.DigitalVideoInterface == VIDEO_INTERFACE_DVI)
            {
                pEdidData->pData->FtrSupport.IsDviSupported = TRUE;
            }
        }

        // As per EDID spec 1.4, the first block 18 byte descriptor is always Preferred Timing Mode.
        // Hence no need to parse that for this info.
        // Parse only the remaining 3 blocks

        // Go through MonitorInfo[0..2] and find a descriptor for Monitor Range Limits.

        for (DTDNumber = 0; DTDNumber < EDID14_MAX_MONITOR_DESCRIPTORS; DTDNumber++)
        {
            // Get monitor descriptor data
            EdidParserGetMonitorDescriptorDetails(&pEdidBaseBlock->EdidTiming.Edid_14_MonitorInfo[DTDNumber], pEdidData);
        }

        /*
        For a 1.4 complaint source, if a display range limit descriptor is not
        listed in EDID,then any format listed in the standard timing section
        that does not have a matching DMT shall be derived using the CVT formula
        with normal blanking, non-interlaced & no-borders
        */
        if (pEdidData->pData->FtrSupport.IsMRLBlockPresent == FALSE)
        {
            pEdidData->pData->FtrSupport.IsCVTSupported = TRUE;
        }
    }

    return;
}

/**********************************************************************************
 * @brief Update maximum supported DotClock based on EDID.
 *
 *  Method to determine maximum supported DotClock based on Monitor Range Limit info from EDID.
 * @param EdidMaxPixelClockInMHz
 * @param pEdidData
 * @return void
 *********************************************************************************/
void EdidParserUpdateMaxDotClockSupport(DDU64 EdidMaxPixelClockInMHz, EDID_DATA* pEdidData)
{
    // HSD-14021641357: Panel(MSI Optix G24C) blankout since all the modes are pruned in driver
    // WA: There can be some panel EDIDs with Maximum Pixel Clock Field misconfigured(less than min supported value) but still having valid timing info
    // For such cases, fallback to default Pixel Clock(300 MHz - Max supported by HDMI1.4) supported to avoid pruning valid modes
    pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = pEdidData->pData->BasicDisplayCaps.MonitorRangeLimitsMaxPixelClk =
        (EdidMaxPixelClockInMHz < TMDS_CHAR_RATE_25MCSC) ? TMDS_CHAR_RATE_300MCSC : EdidMaxPixelClockInMHz;
}

/***************************************************************
 * @brief Edid Parser Get Monitor Descriptor Details.
 *
 *  Method to parse Monitor descripot details (based on EDID version):
 *      1. Monitor Range limits
 *      2. Monitor Name
 *      3. Min/max RR and dotclock supported.
 * @param pMonitorDescriptor
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserGetMonitorDescriptorDetails(MONITOR_DESCRIPTOR* pMonitorDescriptor, EDID_DATA* pEdidData)
{
    MONITOR_RANGE_LIMITS MonitorRangeLimits;
    EDID_RANGE_LIMITS_FLAGS RangeLimitOffsetFlags;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pMonitorDescriptor);

    if ((pEdidData->EdidOrDidVerAndRev < EDID_VERSION_1_4))
    {
        switch (pMonitorDescriptor->MonitorDescHeader.DataTypeTag)
        {
        case DISPLAY_RANGE_LIMITS:

            // This block defines the Monitor Range Limits
            // Descriptor Data starts from the byte-5 in the block.
            MonitorRangeLimits = pMonitorDescriptor->MonitorRangeLimits;
            EdidParserUpdateMaxDotClockSupport((DDU64)(MonitorRangeLimits.MaxPixelClock) * TEN * MEGA_HERTZ, pEdidData);
            RangeLimitOffsetFlags.VerticalRateOffset = 0; // For 1.3 and below

            break;
        case DISPLAY_MONITOR_NAME:
            DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.MonitorName, MONITOR_NAME_LENGTH, pMonitorDescriptor->MonitorName, MONITOR_NAME_LENGTH);
            pEdidData->pData->BasicDisplayCaps.MonitorNameLength = GET_MONITOR_NAME_LENGTH(pEdidData->pData->BasicDisplayCaps.MonitorName);
            break;
        default:
            DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid Monitor Tag 0x%x = ", pMonitorDescriptor->MonitorDescHeader.DataTypeTag);
            break;
        }
        return; // Break out after EDID 1.3 parsing
    }

    // EDID 1.4 or higher --> if code comes here
    switch (pMonitorDescriptor->MonitorDescHeader.DataTypeTag)
    {
    case DISPLAY_RANGE_LIMITS:
        // This block defines the Monitor Range Limits
        // Descriptor Data starts from the byte-5 in the block.
        pEdidData->pData->FtrSupport.IsMRLBlockPresent = TRUE;

        MonitorRangeLimits = pMonitorDescriptor->MonitorRangeLimits;
        RangeLimitOffsetFlags.RangeLimitOffsetFlags = pMonitorDescriptor->MonitorDescHeader.Flag2;

        if (pEdidData->pData->FtrSupport.IsContinuousFreqSupported == FALSE)
        {
            break;
        }

        pEdidData->pData->RrSwitchCaps.FixedRrCaps.IsFullRrRangeSupported = TRUE;

        // Continuous Display flag should be set to indicate support for GTF or CVT
        if (MonitorRangeLimits.TimingFormulaSupport == CVT_TIMING_SUPPORTED)
        {
            pEdidData->pData->FtrSupport.IsCVTSupported = TRUE;

            if (MonitorRangeLimits.CVTRedBlankSupport)
            {
                pEdidData->pData->FtrSupport.IsCVTRedBlankSupported = TRUE;
            }
        }
        else if ((MonitorRangeLimits.TimingFormulaSupport == DEFAULT_GTF_SUPPORTED) || (MonitorRangeLimits.TimingFormulaSupport == SECONDARY_GTF_SUPPORTED))
        {
            pEdidData->pData->FtrSupport.IsGTFSupported = TRUE;
        }

        EdidParserUpdateMaxDotClockSupport((DDU64)(MonitorRangeLimits.MaxPixelClock) * TEN * MEGA_HERTZ, pEdidData);
        if (RangeLimitOffsetFlags.VerticalRateOffset == 0)
        {
            // if the Vertical Rate Offset is 0, then its the same as EDID 1.3.
            // i.e, Monitor Range Limits will be having the same value.
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = MonitorRangeLimits.MinVFrequency * KILO_HERTZ;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = MonitorRangeLimits.MaxVFrequency * KILO_HERTZ;
        }
        else if (RangeLimitOffsetFlags.VerticalRateOffset == 2)
        {
            // if the Vertical Rate Offset is 2, then add 255 to Max Vertical Rate
            // Min Vertical Rate remains the same.
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = MonitorRangeLimits.MinVFrequency * KILO_HERTZ;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = (MonitorRangeLimits.MaxVFrequency + 255) * KILO_HERTZ;
        }
        else if (RangeLimitOffsetFlags.VerticalRateOffset == 3)
        {
            // if the Vertical Rate Offset is 3, then add 255 to Max Vertical Rate
            // Add 255 to Min Vertical Rate also.
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = (MonitorRangeLimits.MinVFrequency + 255) * KILO_HERTZ;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = (MonitorRangeLimits.MaxVFrequency + 255) * KILO_HERTZ;
        }

        break;
    case DISPLAY_MONITOR_NAME:
        DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.MonitorName, MONITOR_NAME_LENGTH, pMonitorDescriptor->MonitorName, MONITOR_NAME_LENGTH);
        pEdidData->pData->BasicDisplayCaps.MonitorNameLength = GET_MONITOR_NAME_LENGTH(pEdidData->pData->BasicDisplayCaps.MonitorName);
        break;
    default:
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid Monitor Tag 0x%x = ", pMonitorDescriptor->MonitorDescHeader.DataTypeTag);
        break;
    };

    return;
}

/***************************************************************
 * @brief Method to parse and add established modes in EDID.
 *
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserParseBaseEstablishedModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU32 EstablishedModes;
    DDU32 Counter;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DDU8 VicId; // Assume VIC undefined

    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    // Create the combined bit mask indicating established modes
    EstablishedModes = DisplayInfoParserConvertHexArrayToInt(pEdidBaseBlock->EstablishedTimings.EstTimingData, 3);

    if (EstablishedModes == 0)
    {
        return;
    }

    for (Counter = 0; Counter < g_ulTotalEstablishedModes; Counter++)
    {
        // Is mode supported in EDID  and Mode supported in our implementation?
        if ((EstablishedModes & (1 << Counter)) && (g_EstablishedModeTable[Counter].pTimingInfo))
        {
            TimingInfo = *(g_EstablishedModeTable[Counter].pTimingInfo);
            AddModes.pTimingInfo = &TimingInfo;
            AddModes.ForceAdd = FALSE;

            VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

            TimingInfo.SignalStandard = DD_VESA_DMT;
            TimingInfo.CeData.VicId[0] = VicId;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
            TimingInfo.Flags.PreferredMode = FALSE;
            TimingInfo.ModeSource = EST_TIMING;
            DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }
    } // end of for loop

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Edid Parser Parse Std Modes.
 *
 *  Method to parse and add Standard monitor modes in EDID
 *      This is a generic routine to parse STD modes from base block.
 * @param pParseStdTiming
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS EdidParserParseStdModes(PARSE_STDTIMING_ARGS* pParseStdTiming, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    DDU32 Counter;
    EDID_STD_TIMING StdTiming;
    DDU16 AspectRatioX, AspectRatioY;
    DDU32 XRes, YRes, RRate;
    ADD_MODES AddModes;
    DDU32 StaticModeCount;
    DD_TIMING_INFO TimingInfo;
    CREATE_GTF_TIMING_ARGS CreateGTFTimingArgs;
    CREATE_CVT_TIMING_ARGS CreateCVTTimingArgs;
    DD_BOOL Found = FALSE;
    DDU8 VicId;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pParseStdTiming->pStdTiming);

    for (Counter = 0; Counter < NUM_STANDARD_TIMING; Counter++)
    {
        DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

        Found = FALSE;
        StdTiming.StdTiming = pParseStdTiming->pStdTiming[Counter].StdTiming;

        // Check for unused fields, which are filled with 0x01 value as per EDID spec. Bug# 2439669
        if ((StdTiming.StdTiming == 0x0101) || (StdTiming.StdTiming == 0x0))
        {
            continue;
        }

        AspectRatioX = EdidAspectRatioTable[StdTiming.AspectRatio].XRatio;
        AspectRatioY = EdidAspectRatioTable[StdTiming.AspectRatio].YRatio;

        if (((StdTiming.AspectRatio == 0) && ((pEdidData->EdidOrDidVerAndRev < EDID_VERSION_1_3))))
        {
            AspectRatioX = 1;
            AspectRatioY = 1;
        }

        XRes = (StdTiming.HActive + 31) * 8;
        YRes = (XRes * AspectRatioY) / AspectRatioX;
        RRate = StdTiming.RefreshRate + 60;

        // Check for present DMTS timings.
        for (StaticModeCount = 0; StaticModeCount < g_ulTotalStaticModes; StaticModeCount++)
        {
            if ((g_StaticModeTable[StaticModeCount].pTimingInfo) && (XRes == g_StaticModeTable[StaticModeCount].pTimingInfo->HActive) &&
                (YRes == g_StaticModeTable[StaticModeCount].pTimingInfo->VActive) && (RRate == g_StaticModeTable[StaticModeCount].pTimingInfo->VRoundedRR))
            {
                Found = TRUE;

                // Standard entries
                TimingInfo = *g_StaticModeTable[StaticModeCount].pTimingInfo;
                AddModes.pTimingInfo = &TimingInfo;
                TimingInfo.SignalStandard = DD_VESA_DMT;
                break;
            }
        }

        // if it doesn't match with DMTS timing. Send GTF timing.
        if (Found == FALSE)
        {
            if (pEdidData->pData->FtrSupport.IsCVTSupported)
            {
                CreateCVTTimingArgs.XRes = XRes;
                CreateCVTTimingArgs.YRes = YRes;
                CreateCVTTimingArgs.RRate = RRate;
                CreateCVTTimingArgs.IsInterLaced = FALSE;
                CreateCVTTimingArgs.IsMargin_Req = FALSE;
                CreateCVTTimingArgs.IsRed_Blank_Req = pEdidData->pData->FtrSupport.IsCVTRedBlankSupported ? TRUE : FALSE;
                CreateCVTTimingArgs.pTimingInfo = &TimingInfo;
                Status = DisplayInfoRoutinesCreateCVTTiming(&CreateCVTTimingArgs);
                TimingInfo.SignalStandard = DD_VESA_CVT;
            }
            else
            {
                // Send GTF timing if CVT not supported
                // (For EDID 1.3 panels, CVT is assumed to be not supported)

                CreateGTFTimingArgs.XRes = XRes;
                CreateGTFTimingArgs.YRes = YRes;
                CreateGTFTimingArgs.RRate = RRate;
                CreateGTFTimingArgs.pTimingInfo = &TimingInfo;
                CreateGTFTimingArgs.IsProgressiveMode = TRUE;

                Status = DisplayInfoRoutinesCreateGTFTiming(&CreateGTFTimingArgs);
                TimingInfo.SignalStandard = DD_VESA_GTF;
            }

            if (IS_DDSTATUS_SUCCESS(Status))
            {
                Found = TRUE;
                AddModes.pTimingInfo = &TimingInfo;
            }
        }

        // Add the mode timing info
        if ((TRUE == Found) && (AddModes.pTimingInfo))
        {
            // Any timing outside these limits may cause the monitor to enter a self - protection mode.
            // The host shall always verify that an intended timing is always less than or equal to MonitorRangeLimitsMaxPixelClk.
            // When MonitorRangeLimitsPixelClk is Valid(!=0) , Dotclk should be less than the MRLPixelClk otherwise prune the mode.
            if ((0 != pEdidData->pData->BasicDisplayCaps.MonitorRangeLimitsMaxPixelClk) && (TimingInfo.DotClockInHz > pEdidData->pData->BasicDisplayCaps.MonitorRangeLimitsMaxPixelClk))
            {
                continue;
            }

            VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

            TimingInfo.ModeSource = STD_TIMING;
            TimingInfo.CeData.VicId[0] = VicId;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;

            AddModes.ForceAdd = FALSE;

            DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }
    } // for loop for going through all STD timings

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Edid Parser Parse Base Std Modes.
 *
 *  Method to parse and add base Standard monitor modes in EDID
 *      This will inturn call EdidParserParseStdModes to add modes to mode-table.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserParseBaseStdModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    PARSE_STDTIMING_ARGS ParseStdTiming;
    EDID_BASE_BLOCK* pBaseEdidBlock = NULL;

    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pBaseEdidBlock = (EDID_BASE_BLOCK*)(pGetEdidModes->pEdidOrDisplayIDBuf);
    ParseStdTiming.pStdTiming = (EDID_STD_TIMING*)(&(pBaseEdidBlock->StdTiming));
    ParseStdTiming.pModeTable = pGetEdidModes->pModeTable;

    EdidParserParseStdModes(&ParseStdTiming, pEdidData);

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Edid Parser Parse Est Timing III Block.
 *
 *  Method to parse and add Established Timing III block in EDID
 *      Only EDID 1.4  has support for Established Timing III block Monitor descriptor.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserParseEstTimingIIIBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU32 DTDNumber;
    DD_BOOL FoundEstTimingIIIBlock = FALSE;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    DDU32 ByteCount;
    DDU32 BitPosition;
    DDU32 TableEntry;
    ADD_MODES AddModes;
    EST_TIMINGS_III_BLOCK EstTimingsIIIBlock;
    DD_TIMING_INFO TimingInfo;
    DDU8 VicId;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    if (pEdidData->EdidOrDidVerAndRev < EDID_VERSION_1_4)
    {
        // Established Timings III block not present until EDID 1.4 revision
        return;
    }

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;

    // Check if Established Timings III block is present or not
    for (DTDNumber = 0; DTDNumber < EDID14_MAX_MONITOR_DESCRIPTORS; DTDNumber++)
    {
        if (DISPLAY_ESTABLISHED_TIMING3 == pEdidBaseBlock->EdidTiming.Edid_14_MonitorInfo[DTDNumber].MonitorDescHeader.DataTypeTag)
        {
            // this defines the Established Timings III Block
            //<Parse the block>
            EstTimingsIIIBlock = pEdidBaseBlock->EdidTiming.Edid_14_MonitorInfo[DTDNumber].EstTimingsIIIBlock;
            FoundEstTimingIIIBlock = TRUE;
            break;
        }
    }

    if (FALSE == FoundEstTimingIIIBlock)
    {
        // Established Timings III block not present
        return;
    }

    for (ByteCount = 0; (ByteCount < EST_TIMINGS_III_BLOCK_DATA_LENGTH); ByteCount++)
    {
        for (BitPosition = 0; BitPosition < BYTE_LENGTH; BitPosition++)
        {
            if (FALSE == (EstTimingsIIIBlock.TimingBitMask[ByteCount] & (1 << BitPosition)))
            {
                // If Est3 timing support bit not set for this byte/bit position
                continue;
            }

            // Est3 timing support bit is set for this byte/bit position
            TableEntry = (ByteCount * BYTE_LENGTH) + BitPosition + 1;

            if (g_EstTiming3ModeTable[TableEntry].pTimingInfo)
            {
                TimingInfo = *(g_EstTiming3ModeTable[TableEntry].pTimingInfo);
                AddModes.pTimingInfo = &TimingInfo;
                AddModes.ForceAdd = FALSE;

                VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

                TimingInfo.ModeSource = EST_TIMING_III;
                TimingInfo.CeData.VicId[0] = VicId;
                TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
                TimingInfo.SignalStandard = DD_VESA_DMT;

                // Found Timing Info in the table, add this mode to the mode table
                DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
            }
        } // for loop for bit position
    }     // for loop for byte count

    return;
}

/***************************************************************
 * @brief Edid Parser Add Base Block DTDs To Modelist.
 *
 *  Method to add Base block DTDs to mode-list
 *      There are 2 cases handled here.
 *      Case 1: If the base block DTD is a CE mode, add that mode to internal CE mode-list and don't make an entry in mode-table yet.
 *              The final CE mode list will be added after all the parsing is complete
 *      Case 2: If the base block DTD is not a CE mode, add the mode to the mode-list.
 * @param pEdidData
 * @param pTimingInfo
 * @return DDSTATUS
 ***************************************************************/
static DDSTATUS EdidParserAddBaseBlockDTDsToModelist(EDID_DATA* pEdidData, DD_TIMING_INFO* pTimingInfo)
{
    DDSTATUS Status;
    ADD_MODES AddModes;
    CE_MODE_LIST CeVicData;
    DDU8 VicId = VIC_UNDEFINED; // Assume VIC undefined

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    if (pEdidData->pData->FtrSupport.IsCeExtnDisplay)
    {
        // CE extension display
        VicId = CeInfoParserIsCeMode(pTimingInfo);
    }

    if (IS_VALID_VICID(VicId))
    {
        // CE extension display and baseblock DTD has CE timings
        // Add it to the VIClist with all required params
        CeVicData.IsPreferred = (DD_BOOL)pTimingInfo->Flags.PreferredMode;
        CeVicData.SignalStandard = DD_CEA_861B;
        CeVicData.SamplingMode.Rgb = 1;
        CeVicData.VicId[0] = VicId;
        CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, BASE_DTD);
        Status = DDS_SUCCESS;
    }
    else
    {
        pTimingInfo->CeData.VicId[0] = VIC_UNDEFINED;
        pTimingInfo->CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
        pTimingInfo->SignalStandard = DD_VESA_DMT;
        AddModes.pTimingInfo = pTimingInfo;
        AddModes.ForceAdd = FALSE;
        Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
    }

    return Status;
}

/***************************************************************
 * @brief Display Info Parser Add Timing To Mode List.
 *
 *  Method to add DTDs to a temp timing Info list
 *      The DTDs are parsed in order of Extn BLocks, Display ID Blocks, then Base blocks
 *      but, caching Base block Dtd Count as VFPDB requires Base block DTDs to be considered for counting the DTDs in order they appear in EDID.
 * @param pEdidData
 * @param pTimingInfo
 * @param ForceAdd
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserAddTimingToModeList(EDID_DATA* pEdidData, DD_TIMING_INFO* pTimingInfo, DD_BOOL ForceAdd)
{
    if (pEdidData->NumTimingInfo >= MAX_MODES_DEFINED)
    {
        //  list is full with timings, increase MAX_MODES_DEFINED
        DISP_FUNC_EXIT_W_STATUS(DDS_BUFFER_OVERFLOW);
        return DDS_BUFFER_OVERFLOW;
    }

    if (pTimingInfo->ModeSource == BASE_DTD)
    {
        pEdidData->NumBaseBlockDtds++;
    }

    DD_MEM_COPY_SAFE(&pEdidData->pModes[pEdidData->NumTimingInfo].TimingInfo, sizeof(DD_TIMING_INFO), pTimingInfo, sizeof(DD_TIMING_INFO));
    // cache Force Add flag to use it while updating the mode table
    pEdidData->pModes[pEdidData->NumTimingInfo].ForceAdd = ForceAdd;
    pEdidData->NumTimingInfo++;

    DISP_FUNC_EXIT_W_STATUS(DDS_SUCCESS);
    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Edid Parser Parse Base Block DTDs.
 *
 *  Method to parse and add modes from Base block in EDID
 *      EDID 1.3 - can have 4 DTDs in base block and anything can be preferred (DTD-0 is preferred in PTM flag is set)
 *      EDID 1.4 - can have 4 DTDs in base block and DTD-0 is always preferred as per spec.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserParseBaseBlockDTDs(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    DDU32 DtdCounter;
    EDID_DTD_TIMING* pDTD = NULL;
    DD_TIMING_INFO TimingInfo;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    if ((pEdidData->EdidOrDidVerAndRev < EDID_VERSION_1_4))
    {
        for (DtdCounter = 0; DtdCounter < MAX_EDID_DTD_BLOCKS; DtdCounter++)
        {
            pDTD = &(pEdidBaseBlock->EdidTiming.Edid_13_DTD[DtdCounter]);

            if (pDTD->Timing.PixelClock <= 0x101)
            {
                // Is this a valid DTD?
                continue;
            }

            // Note: The EDID 1.x spec requires that all DTD blocks, when not
            //         being used as Monitor Descriptors nor DTD, should be filled
            //         with valid timing data. However, some monitors are still
            //         using data fill pattern, most commonly with all "0x01", to
            //         fill the DTD blocks. The monitor vendors should be contacted
            //         for EDID spec violation. However, we have added an workaround
            //         here to handle this. In the future if other data fill patterns
            //         should be discovered, the data "0x101" should be updated.
            Status = DisplayInfoRoutinesGetTimingFromDTD(pDTD, &TimingInfo);
            if (IS_DDSTATUS_ERROR(Status))
            {
                // Timing info not valid, check next one
                continue;
            }

            // 1st EDID is taken as preferred mode
            if ((DtdCounter == 0) && (pEdidBaseBlock->BasicDisplayParams.IsPreferredTimingMode) && (pEdidData->Vfpdb.DtdCount == 0))
            {
                TimingInfo.Flags.PreferredMode = TRUE;
            }

            TimingInfo.ModeSource = BASE_DTD;

            EdidParserAddBaseBlockDTDsToModelist(pEdidData, &TimingInfo);

        } // For loop ends

        return; // Break out as soon as EDID 1.3 parsing is done
    }

    // EDID 1.4 or later -- > if code reaches here
    for (DtdCounter = 0; DtdCounter < MAX_EDID_DTD_BLOCKS; DtdCounter++)
    {
        if (DtdCounter == 0)
        {
            pDTD = &(pEdidBaseBlock->EdidTiming.PreferedTimingMode);
        }
        else
        {
            pDTD = &(pEdidBaseBlock->EdidTiming.Edid_14_DTD[DtdCounter - 1]);
        }

        if (pDTD->Timing.PixelClock <= 0x101)
        {
            // Is this a valid DTD?
            continue;
        }

        // Note: The EDID 1.x spec requires that all DTD blocks, when not
        //         being used as Monitor Descriptors nor DTD, should be filled
        //         with valid timing data. However, some monitors are still
        //         using data fill pattern, most commonly with all "0x01", to
        //         fill the DTD blocks. The monitor vendors should be contacted
        //         for EDID spec violation. However, we have added an workaround
        //         here to handle this. In the future if other data fill patterns
        //         should be discovered, the data "0x101" should be updated.
        Status = DisplayInfoRoutinesGetTimingFromDTD(pDTD, &TimingInfo);
        if (IS_DDSTATUS_ERROR(Status))
        {
            // Timing info not valid, check next one
            continue;
        }

        // Set preferred flag as TRUE only when preferred Timing Mode flag is set and extension blocks are 0
        if ((DtdCounter == 0) && (pEdidData->Vfpdb.DtdCount == 0))
        {
            if ((pEdidBaseBlock->BasicDisplayParams.IsPreferredTimingMode != 0) || ((pEdidBaseBlock->BasicDisplayParams.IsPreferredTimingMode == 0) && (pGetEdidModes->NumEdidExtensionsParsed == 0)))
            {
                TimingInfo.Flags.PreferredMode = TRUE;
            }
        }
        else
        {
            TimingInfo.Flags.PreferredMode = FALSE;
        }

        TimingInfo.ModeSource = BASE_DTD;

        EdidParserAddBaseBlockDTDsToModelist(pEdidData, &TimingInfo);
    }

    return;
}

// Currently we dont have any physical panel or sample EDIDs with VTB extension hence commenting this out
#if 0
/***************************************************************
 * @brief Parser VTB extension DTD blocks and add the modes to mode-table.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
***************************************************************/
DDSTATUS EdidParserParseVTBExtDTDs(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS         Status;
    DD_TIMING_INFO   TimingInfo;
    ADD_MODES        AddModes;
    DDU32            NumDTD;        // Number of DTDs
    DDU32            DTDCounter;
    EDID_DTD_TIMING* pDTD = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    pDTD = (EDID_DTD_TIMING*)pParseExtDataBlkArgs->pDataBlock;
    NumDTD = (pParseExtDataBlkArgs->LenDataBlock / VTB_DTD_SIZE);

    if (NumDTD == 0)
    {
        return DDS_SUCCESS; // No VTB extension DTDs found
    }

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    for (DTDCounter = 0; DTDCounter < NumDTD; DTDCounter++, pDTD++)
    {
        Status = DisplayInfoRoutinesGetTimingFromDTD(pDTD, &TimingInfo);
        if (IS_DDSTATUS_SUCCESS(Status))
        {
            TimingInfo.CeData.VicId[0] = VIC_UNDEFINED;
            TimingInfo.CeData.Par[0].Value = 0;   // AVI_PAR_NODATA;
            TimingInfo.SignalStandard = DD_VESA_DMT;

            AddModes.pTimingInfo = &TimingInfo;
            AddModes.ForceAdd = TRUE;

            Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }
    } // For loop for # of DTDs

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief  Parser VTB extension CVT blocks and add the modes to mode-table.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
***************************************************************/
DDSTATUS EdidParserParseVTBExtCVTs(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS        Status;
    VTB_CVT_TIMING* pCVTTiming;
    DDU32           NumCVT;
    DDU32           CVTCntr;
    DDU32           YRes, XRes;
    DDU32           Available_RRs[5] = { 60, 85, 75, 60, 50 };
    DDU32           RRCount;
    DD_BOOL         IsReducedBlanking = FALSE;
    ADD_MODES       AddModes;
    CREATE_CVT_TIMING_ARGS  CreateCVTTimingArgs;
    DD_TIMING_INFO  TimingInfo;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    NumCVT = (pParseExtDataBlkArgs->LenDataBlock / VTB_CVT_SIZE);

    for (CVTCntr = 0; CVTCntr < NumCVT; CVTCntr++)
    {
        pCVTTiming = (VTB_CVT_TIMING*)pParseExtDataBlkArgs->pDataBlock[CVTCntr];

        YRes = ((pCVTTiming->VA_high << 8) + pCVTTiming->VA_low + 1) * 2;

        switch (pCVTTiming->AspectRatio)
        {
        case 0x00:
            XRes = (YRes * 4 / 3);
            break;

        case 0x01:
            XRes = (YRes * 16 / 9);
            break;

        case 0x02:
            XRes = (YRes * 16 / 10);
            break;

        case 0x03:
        default:
            XRes = 0;
            break;          // Undefined/reserved case...
        }

        if (XRes == 0 || YRes == 0)
        {
            continue;
        }

        CreateCVTTimingArgs.XRes = XRes;
        CreateCVTTimingArgs.YRes = YRes;
        CreateCVTTimingArgs.IsInterLaced = FALSE;
        CreateCVTTimingArgs.IsMargin_Req = FALSE;

        for (RRCount = 0, IsReducedBlanking = TRUE; RRCount < VTB_MAX_RR; RRCount++)
        {
            if (RRCount != 0)
            {
                IsReducedBlanking = FALSE;  // For all RRCount != 0, ReducedBlanking is FALSE
            }

            if (FALSE == ((1 << RRCount) && (pCVTTiming->Refresh_Rate_Bits & VTB_RR_SUPPORTMASK)))
            {
                // VTB extension CVT support not set for this RR
                continue;
            }

            // VTB extension CVT support is set for this RR
            CreateCVTTimingArgs.RRate = Available_RRs[RRCount];
            CreateCVTTimingArgs.IsRed_Blank_Req = IsReducedBlanking;
            CreateCVTTimingArgs.pTimingInfo = &TimingInfo;
            Status = DisplayInfoRoutinesCreateCVTTiming(&CreateCVTTimingArgs);
            if (IS_DDSTATUS_ERROR(Status))
            {
                // CVT timings could not be ceated
                continue;
            }

            TimingInfo.CeData.VicId[0] = VIC_UNDEFINED;
            TimingInfo.CeData.Par[0].Value = 0;   // AVI_PAR_NODATA;
            TimingInfo.SignalStandard = DD_VESA_CVT;

            AddModes.pTimingInfo = &TimingInfo;
            AddModes.ForceAdd = TRUE;
            Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }// end of RRcount for loop
    }// end of CVT count for loop

    Status = DDS_SUCCESS;

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Edid Parser Parse VTB Extension.
 *
 * Base function to Parse VTB Extension block.
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *          1. EdidParserParseVTBExtDTDs to parser VTB CVTs
 *          2. EdidParserParseVTBExtCVTs to parse VTB DTDs
 *          3. EdidParserParseStdModes to parse VTB Standard modesParser VTB extension CVT blocks and add the modes to mode-table.
 * @param pGetEdidModes
 * @param pEdidData
 * @return DDSTATUS
***************************************************************/
DDSTATUS EdidParserParseVTBExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS         Status;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    VTB_EXT* pVTBExt = NULL;
    DDU32            BlockNum;
    DDU32            NumExtBlocks;
    PARSE_EXTBLK_ARGS ParseExtDataBlkArgs;
    DDU8* pTemp = NULL;
    DDU8             CheckSum;
    DDU32            Count;
    DDU8             TotalDataByteSz;
    PARSE_STDTIMING_ARGS  ParseStdTiming;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;
    NumExtBlocks = pEdidBaseBlock->NumExtBlocks;

    for (BlockNum = 1; (BlockNum <= NumExtBlocks) && (BlockNum < MAX_EDID_EXTENSIONS_SUPPORTED); BlockNum++)
    {
        pVTBExt = (VTB_EXT*)((DDU8*)pEdidBaseBlock + (BlockNum * EDID_BLOCK_SIZE));
        pTemp = (DDU8*)(pVTBExt);

        if (VTB_EXT_TAG != pVTBExt->Tag)
        {
            // If not VTB extension
            continue;
        }

        // VTB extension
        Count = 0;
        CheckSum = 0;
        do
        {
            CheckSum += pTemp[Count];
        } while (++Count < EDID_BLOCK_SIZE);

        if (0 != CheckSum)
        {
            // DoTraceMessage(SB_MSG, "::Critical::%s:: ERROR - Failed checksum calculation\n", __FUNCTION__);
            // DoTraceMessage(SB_MSG, "::Critical::%s::VTB Extesnion Block not processed : VTB Block is Invalid!\n", __FUNCTION__);
            break;
        }

        // Calculating total DATA bytes. which should be <= 122.
        TotalDataByteSz = (pVTBExt->NumDTD * VTB_DTD_SIZE) + (pVTBExt->NumCVT * VTB_CVT_SIZE) + (pVTBExt->NumST * VTB_ST_SIZE);

        // Checks for no. of DTDs, CVTs, STs with respect to max possible.
        if ((pVTBExt->NumDTD > VTB_MAX_DTD_TIMINGS) ||
            (pVTBExt->NumCVT > VTB_MAX_CVT_TIMINGS) ||
            (pVTBExt->NumST > VTB_MAX_STANDARD_TIMINGS) ||
            (TotalDataByteSz > VTB_DATA_SIZE))
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, " ERROR - No. of DTD or CVT or ST blocks are invalid");
            DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
            return DDS_UNSUCCESSFUL;
        }

        // 1. parse VTB DTDs
        if (0 != pVTBExt->NumDTD)
        {
            pTemp = (DDU8*)(&(pVTBExt->DATA[0]));
            ParseExtDataBlkArgs.pDataBlock = pTemp;
            ParseExtDataBlkArgs.LenDataBlock = pVTBExt->NumDTD * VTB_DTD_SIZE;
            ParseExtDataBlkArgs.pModeTable = pGetEdidModes->pModeTable;

            Status = EdidParserParseVTBExtDTDs(&ParseExtDataBlkArgs, pEdidData);
            if (IS_DDSTATUS_ERROR(Status))
            {
                DISP_DBG_MSG(GFXDBG_CRITICAL, "DTD Timing Blocks in VTB Extesnion Block can not processed !");
                return Status;
            }
        }

        // 2. Parser VTB CVTs
        if (0 != pVTBExt->NumCVT)
        {
            pTemp = (DDU8*)(&(pVTBExt->DATA[VTB_DTD_SIZE * (pVTBExt->NumDTD)]));
            ParseExtDataBlkArgs.pDataBlock = pTemp;
            ParseExtDataBlkArgs.LenDataBlock = pVTBExt->NumCVT * VTB_CVT_SIZE;
            ParseExtDataBlkArgs.pModeTable = pGetEdidModes->pModeTable;

            Status = EdidParserParseVTBExtCVTs(&ParseExtDataBlkArgs, pEdidData);
            if (IS_DDSTATUS_ERROR(Status))
            {
                DISP_DBG_MSG(GFXDBG_CRITICAL, "::Critical::%s::CVT Timing Blocks in VTB Extesnion Block can not be processed !");
                return Status;
            }
        }

        // 3. Parse VTB Standard modes
        if (0 != pVTBExt->NumST)
        {
            pTemp = (DDU8*)(&(pVTBExt->DATA[VTB_DTD_SIZE * (pVTBExt->NumDTD) + VTB_CVT_SIZE * (pVTBExt->NumCVT)]));

            ParseStdTiming.pStdTiming = (EDID_STD_TIMING*)(pTemp);
            ParseStdTiming.pModeTable = pGetEdidModes->pModeTable;
            Status = EdidParserParseStdModes(&ParseStdTiming, pEdidData);
            if (IS_DDSTATUS_ERROR(Status))
            {
                DISP_DBG_MSG(GFXDBG_CRITICAL, "STD Timing Blocks in VTB Extesnion Block can not processed !");
                return Status;
            }
        }
    }

    Status = DDS_SUCCESS;

    DISP_FUNC_EXIT();
    return Status;
}
#endif // 0

/***************************************************************
 * @brief Methof to get BPC supported in the panel based on EDID revision from base block.
 *
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidGetSupportedBPCFromBaseBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU8* pTempEdidBaseBlock = NULL;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    if ((pEdidData->EdidOrDidVerAndRev < EDID_VERSION_1_4))
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports8BitsPerColor = TRUE;
    }
    else
    {
        pTempEdidBaseBlock = pGetEdidModes->pEdidOrDisplayIDBuf;
        pEdidBaseBlock = (EDID_BASE_BLOCK*)pTempEdidBaseBlock;

        // Get EDID base block color depth
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes = DisplayInfoParserGetSupportedBpcMaskFromColorDepth(pEdidBaseBlock->BasicDisplayParams.ColorBitDepth);

        if (0 == pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask)
        {
            // WA - If EDID BPC comes is 0 we set the default value of 6 and 8 bpc for both Analog and Digital Input.
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports6BitsPerColor = TRUE;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports8BitsPerColor = TRUE;
        }
    }
    return;
}

/*-------------------------------------------------------------
 *
 * EDID parser related functions -- END
 *
 ***************************************************************/

 /*-------------------------------------------------------------
  *
  * CE Info parser related functions -- START
  *
  ***************************************************************/

  /***************************************************************
   * @brief Method to know wether input Mode is CE mode and return VIC ID
   *          if yes, update the input mode with CE PAR and CE VIC ID values.
   *
   * @param pTimingInfo
   * @return DDU8
   ***************************************************************/
DDU8 CeInfoParserIsCeMode(DD_TIMING_INFO* pTimingInfo)
{
    DDU8 VicId = VIC_UNDEFINED; // Assume VIC undefined
    CE_SHORT_VIDEO_MODE* pCeShortVideoDes = NULL;
    DDU8 Counter;
    PIXELCLK_COMPARE_ARGS PixelCompareArgs;

    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pTimingInfo);

    pCeShortVideoDes = &g_SupportedCeShortVideoModes[0];

    for (Counter = 0; Counter < g_ulNumSupportedCEModes; Counter++, pCeShortVideoDes++)
    {
        if (pCeShortVideoDes->pTimingInfo == NULL)
        {
            continue;
        }

        PixelCompareArgs.PixelClock1 = pCeShortVideoDes->pTimingInfo->DotClockInHz;
        PixelCompareArgs.PixelClock2 = pTimingInfo->DotClockInHz;

        if (TRUE == DisplayInfoRoutinesIsSamePixelClock(&(PixelCompareArgs)) && // Same pixel clock
            (pCeShortVideoDes->pTimingInfo->HActive == pTimingInfo->HActive) && (pCeShortVideoDes->pTimingInfo->VActive == pTimingInfo->VActive) &&
            (pCeShortVideoDes->pTimingInfo->HTotal == pTimingInfo->HTotal) && (pCeShortVideoDes->pTimingInfo->IsInterlaced == pTimingInfo->IsInterlaced))
        {
            VicId = pCeShortVideoDes->CEIndex;
            break;
        }
    }

    DISP_FUNC_WO_STATUS_EXIT();

    return VicId;
}

/***************************************************************
 * @brief Update Hdmi4K2K VIC ID for 2D modes.
 *
 * @param pEdidData
 * @param pCeModeData
 * @return void
 ***************************************************************/
static void UpdateHdmiVicFor4K2KTimings(EDID_DATA* pEdidData, CE_MODE_LIST* pCeModeData)
{
    DDU8 VicIdHdmi4K2k;

    // As per HDMI 2.1a Specification Annexure E, VIC 93-95 and 98 are not supposed to be sent unless these modes appear in Sink's VDB and UHD_VIC is set in HF-SCDC block.
    // Need to fill valid VicId as well along with HDMI VIC ID. This is because Timing info will be selected based on VicId and HDMI Vic need to be update in post timing selection.
    // Actual timings would be selected based on the CTA VIC. If VicId[0] is overwritten here for HDMI 4K2K modes, wrong timings would be updated. Hence VicId[0] Id is unmodified.
    // For HDMI4K2K modes, VicId[0] would be set to 0 as per spec requirement, before adding mode to the list.
    // | ---------------------------------------------------------------------------------------------------|
    // | 3D Video   |   HDMI features | UHD_VIC = 0              | UHD_VIC = 1                              |
    // | ---------------------------------------------------------------------------------------------------|
    // |    Y       |   Y             | CTA_VIC in AVI_IF        | CTA_VIC in AVI_IF                        |
    // |    N       |   Y             | CTA_VIC in AVI_IF        | CTA_VIC in AVI_IF                        |
    // |    Y       |   N             | CTA_VIC in AVI_IF        | CTA_VIC in AVI_IF                        |
    // |    N       |   N             | HDMI_VIC in H14bVSIF     | CTA_VIC in AVI_IF / HDMI_VIC in H14bVSIF |
    // | ---------------------------------------------------------------------------------------------------|
    if (pCeModeData->S3DFormatMask == 0)
    {
        VicIdHdmi4K2k = GET_HDMI_4K2K_VIC(pCeModeData->VicId[0]);
        pCeModeData->VicId4k2k = ((0 != VicIdHdmi4K2k) && ((1 == pEdidData->pData->HdmiCaps.HfVsdbInfo.IsUhdVicSupported) || (1 == pEdidData->pData->HdmiCaps.HfVsdbInfo.IsAllmSupported) ||
            (1 == pEdidData->pData->HdrCaps.Hdr10PlusCaps.IsValidHdr10PlusVsVdbFound))) ?
            0 :
            VicIdHdmi4K2k;
    }
}

/***************************************************************
 * @brief Ce Info Parser Update Ce Mode List.
 *
 *  Method to Add or udate VIC List into Edid Data.
 *  Below is followed:
 *     VIC ID present?
 *          i.  No, Go through TimingInfoPointers stored
 *                  1) Found ? Indicate VIC Found : Goto(ii below)
 *                  2) Not Found, add it to list with all other input params (Native format, aspect ratio / preferred bits)
 *          ii. Yes,
 *                  1) Update  Aspect Ratio Bit mask : SVD, 420 / 420Caps cases
 *                  2) Update Preferred bit for preference Block cases(This can already be set by base block)
 *                  3) Update S3Flags incase of S3D blocks.
 * @param pEdidData
 * @param pCeModeData
 * @return void
 ***************************************************************/
void CeInfoParserUpdateCeModeList(EDID_DATA* pEdidData, CE_MODE_LIST* pCeModeData, MODE_SOURCE ModeSource)
{
    DD_BOOL VicFoundInList;
    DDU8 Count;
    DDU8 VicInstance;
    DDU8 VicId;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pCeModeData);

    // As per HDMI 2.1 spec Table 10-2
    // When the Video format being transmitted corresponds to one of the Video Formats in Table 10-3 (4K2K modes),
    // the H14b-VSIF shall be utilized unless 3D Video is being transmitted, features listed in Table 10-1 are active.
    UpdateHdmiVicFor4K2KTimings(pEdidData, pCeModeData);

    VicId = pCeModeData->VicId[0]; // Caller will always send Vic ID in 0th instance

    pCeModeData->VicId[0] = g_SupportedCeShortVideoModes[pCeModeData->VicId[0]].CEIndex;

    // Update TimingInfo, PAR data into input pCeModeData from Global timings
    // Will be used to update pEdidData->pCeModeList
    pCeModeData->pTimingInfo = g_SupportedCeShortVideoModes[VicId].pTimingInfo;
    pCeModeData->Par[0].Is_Avi_Par_4_3 = (g_SupportedCeShortVideoModes[VicId].AspectRatio == AVI_PAR_4_3) ? TRUE : FALSE;
    pCeModeData->Par[0].Is_Avi_Par_16_9 = (g_SupportedCeShortVideoModes[VicId].AspectRatio == AVI_PAR_16_9) ? TRUE : FALSE;
    pCeModeData->Par[0].Is_Avi_Par_64_27 = (g_SupportedCeShortVideoModes[VicId].AspectRatio == AVI_PAR_64_27) ? TRUE : FALSE;
    pCeModeData->pTimingInfo->ModeSource = ModeSource;

    DDASSERT(pCeModeData->pTimingInfo->S3DFormatMask == 0);

    VicFoundInList = FALSE; // Assume VIC not found in list
    VicInstance = 0;

    if (pEdidData->VicCount >= MAX_VIC_DEFINED)
    {
        // Not enough pre-allocated memory to store parsed VICs
        DDRLSASSERT_UNEXPECTED(0, ""); // To catch # of VICs in EDID < MAX_VIC_DEFINED
        return;
    }

    // Go through already parsed VIC mode list
    for (Count = 0; Count < pEdidData->VicCount; Count++)
    {
        // If VIC already added or same timing already present in the list with different VIC (different Aspect ratio), indicate found
        if (pEdidData->pCeModeList[Count].VicId[0] == pCeModeData->VicId[0])
        {
            VicFoundInList = TRUE;
            VicInstance = 0; // 1st instance of VIC matched
            break;
        }
        else if (pEdidData->pCeModeList[Count].VicId[1] == pCeModeData->VicId[0])
        {
            VicFoundInList = TRUE;
            VicInstance = 1; // 2nd instance of VIC matched
            break;
        }
        else if (pEdidData->pCeModeList[Count].pTimingInfo == pCeModeData->pTimingInfo)
        {
            // Input VIC is not matching with VICId[0] or VicId[1], but matching with Timingpointer
            // This means that its a new VIC that is found with same timing, but different aspect ratio
            // So, update input VIC ID into 2nd instance
            pEdidData->pCeModeList[Count].VicId[1] = pCeModeData->VicId[0];

            VicFoundInList = TRUE;
            VicInstance = 1; // Update all other info to 2nd instance of VIC as 1st instance didnt match and only timing matched
            break;
        }
    }

    if (VicFoundInList)
    {
        // Update rest of the Params on the VicInstance where VIC was found
        pEdidData->pCeModeList[Count].IsPreferred |= pCeModeData->IsPreferred;      // Preserve and update Preferred flag
        pEdidData->pCeModeList[Count].SignalStandard = pCeModeData->SignalStandard; // Update Signal standard, for CE modes its always 861B
        pEdidData->pCeModeList[Count].SamplingMode.Value |= pCeModeData->SamplingMode.Value;
        pEdidData->pCeModeList[Count].NativeFormat[VicInstance] |= pCeModeData->NativeFormat[0];             // Preserve and update native format
        pEdidData->pCeModeList[Count].Par[VicInstance].Is_Avi_Par_4_3 |= pCeModeData->Par[0].Is_Avi_Par_4_3; // Preserve and update Aspect ratio
        pEdidData->pCeModeList[Count].Par[VicInstance].Is_Avi_Par_16_9 |= pCeModeData->Par[0].Is_Avi_Par_16_9;
        pEdidData->pCeModeList[Count].Par[VicInstance].Is_Avi_Par_64_27 |= pCeModeData->Par[0].Is_Avi_Par_64_27;
        pEdidData->pCeModeList[Count].S3DFormatMask |= pCeModeData->S3DFormatMask; // Preserve and update S3D format
    }
    else
    {
        // Update all data to the list and increment VIC Count
        pEdidData->pCeModeList[Count].pTimingInfo = pCeModeData->pTimingInfo;
        pEdidData->pCeModeList[Count].VicId[VicInstance] = pCeModeData->VicId[0];
        pEdidData->pCeModeList[Count].VicId4k2k = pCeModeData->VicId4k2k;
        pEdidData->pCeModeList[Count].IsPreferred = pCeModeData->IsPreferred;        // Update Preferred flag
        pEdidData->pCeModeList[Count].SignalStandard = pCeModeData->SignalStandard;     // Update Signal standard
        pEdidData->pCeModeList[Count].SamplingMode.Value = pCeModeData->SamplingMode.Value; // Update Sampling mode

        pEdidData->pCeModeList[Count].NativeFormat[VicInstance] = pCeModeData->NativeFormat[VicInstance];       // Update native format
        pEdidData->pCeModeList[Count].Par[VicInstance].Is_Avi_Par_4_3 = pCeModeData->Par[VicInstance].Is_Avi_Par_4_3; // Update Aspect ratio
        pEdidData->pCeModeList[Count].Par[VicInstance].Is_Avi_Par_16_9 = pCeModeData->Par[VicInstance].Is_Avi_Par_16_9;
        pEdidData->pCeModeList[Count].Par[VicInstance].Is_Avi_Par_64_27 = pCeModeData->Par[VicInstance].Is_Avi_Par_64_27;
        pEdidData->pCeModeList[Count].S3DFormatMask = pCeModeData->S3DFormatMask; // Update S3D format
        pEdidData->VicCount++;
    }

    CeInfoParserAddCeModesToList(pEdidData, &pEdidData->pCeModeList[Count]);

    return;
}

/***************************************************************
 * @brief Method to check if there is another mode (different VIC ID) with same timings already exists in the CE mode table.
 *
 * @param pTimingInfo
 * @param pEdidData
 * @param VicId
 * @return DD_BOOL
 ***************************************************************/
DD_BOOL CeInfoParserIsModeAlreadyAdded(DD_TIMING_INFO* pTimingInfo, EDID_DATA* pEdidData, DDU8 VicId)
{
    DD_BOOL ModeAlreadyAdded = FALSE;

    for (DDU32 Count = 0; Count < pEdidData->VicCount; Count++)
    {
        if (pEdidData->pCeModeList[Count].pTimingInfo == NULL)
        {
            // Error, should never happen!!
            break;
        }

        // Check if there is another mode (different VIC ID) with same timings already exists in the CE mode table
        if ((pEdidData->pCeModeList[Count].VicId[0] != g_SupportedCeShortVideoModes[VicId].CEIndex) &&
            (TRUE == DisplayInfoRoutinesIsSameTiming(pEdidData->pCeModeList[Count].pTimingInfo, pTimingInfo)))
        {
            // If timing already added as part of non-pixel repeated mode, skip that
            ModeAlreadyAdded = TRUE;
            break;
        }
    }

    return ModeAlreadyAdded;
}

/***************************************************************
 * @brief Method to add Pixel repeated modes if indicated as part of CE VIC.
 *
 * @param pTimingInfo
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddPrModes(DD_TIMING_INFO* pTimingInfo, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DDU8 VicId;
    DDU32 SupportedPrMask;
    DDU8 PrMask;
    DDU32 PrMaskCount;
    DD_TIMING_INFO PrTimingInfo;

    DISP_FUNC_ENTRY();

    // Arg Validation
    DDASSERT(pTimingInfo);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));
    DD_ZERO_MEM(&PrTimingInfo, sizeof(DD_TIMING_INFO));

    // Even in case if 2 VICs with different PAR, TimingInfo and PRMask will be same
    Status = GetVicIndexBasedOnCeIndex(pTimingInfo->CeData.VicId[0], &VicId);
    if (IS_DDSTATUS_ERROR(Status))
    {
        return Status;
    }

    PrTimingInfo = *g_SupportedCeShortVideoModes[VicId].pTimingInfo;

    SupportedPrMask = g_SupportedCeShortVideoModes[VicId].PRMask;

    Status = DDS_SUCCESS; // Assume Success

    // Now add all supported PR modes for 1X, 2X and 4X modes
    // We currently support only 2x and 4x, PR mask 3x is not supported
    for (PrMaskCount = 0; PrMaskCount < NUM_PR_MASK_SUPPORTED; PrMaskCount++)
    {
        PrMask = ((SupportedPrMask) & (BIT0 << PrMaskCount));

        if ((PrMask == 0) || ((FALSE == pTimingInfo->CeData.SamplingMode.Rgb) && ((PrMask > 1) || (PrTimingInfo.IsInterlaced))))
        {
            // PR mask not set for Bit "PrMaskCount" or
            // YUV420 is not supported for repetition modes and Interlaced Modes
            continue;
        }

        DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

        PrTimingInfo.Flags = pTimingInfo->Flags;
        DisplayInfoRoutinesGetTimingFromPRTimingForPRMask(&PrTimingInfo, &TimingInfo, PrMask);

        // Check if another mode with same timings already added, if yes, skip adding that mode
        if (TRUE == CeInfoParserIsModeAlreadyAdded(&TimingInfo, pEdidData, VicId))
        {
            continue;
        }

        TimingInfo.S3DFormatMask = pTimingInfo->S3DFormatMask;
        TimingInfo.SignalStandard = pTimingInfo->SignalStandard;
        TimingInfo.CeData.SamplingMode.Value = pTimingInfo->CeData.SamplingMode.Value;
        TimingInfo.CeData.Par[0].Value = pTimingInfo->CeData.Par[0].Value;
        TimingInfo.CeData.Par[1].Value = pTimingInfo->CeData.Par[1].Value;
        TimingInfo.CeData.IsNativeFormat[0] = pTimingInfo->CeData.IsNativeFormat[0];
        TimingInfo.CeData.IsNativeFormat[1] = pTimingInfo->CeData.IsNativeFormat[1];

        // If VicId4k2k is a non zero, then its a HDMI 4K 2K mode.
        // As per HDMI spec, CTA VIC (VicId[0], VicId[1]) should be set to 0 and HDMI VIC (VicId4k2k) should be equal to value in range [1 ... 4]
        if (0 != pTimingInfo->CeData.VicId4k2k)
        {
            TimingInfo.CeData.VicId[0] = 0;
            TimingInfo.CeData.VicId[1] = 0;
            TimingInfo.CeData.VicId4k2k = pTimingInfo->CeData.VicId4k2k;
        }
        else
        {
            TimingInfo.CeData.VicId[0] = pTimingInfo->CeData.VicId[0];
            TimingInfo.CeData.VicId[1] = pTimingInfo->CeData.VicId[1];
            TimingInfo.CeData.VicId4k2k = 0;
        }

        AddModes.pTimingInfo = &TimingInfo;
        AddModes.ForceAdd = TRUE;
        // Insert into the mode list
        Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
    }

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief  Method to add CE modes to final mode - list.
 *
 * @param pEdidData
 * @param pCeModeList
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddCeModesToList(EDID_DATA* pEdidData, CE_MODE_LIST* pCeModeList)
{
    DDSTATUS Status;
    DD_TIMING_INFO TimingInfo;

    DISP_FUNC_ENTRY();

    // Arg Validation
    DDASSERT(pEdidData);
    DDASSERT(pCeModeList);

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    Status = DDS_SUCCESS; // Assume success

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));
    if (pCeModeList->pTimingInfo == NULL)
    {
        // Error, should never happen!!
        Status = DDS_INVALID_DATA;
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_DATA);
        return Status;
    }

    TimingInfo = *(pCeModeList->pTimingInfo);
    TimingInfo.Flags.PreferredMode = pCeModeList->IsPreferred;
    TimingInfo.CeData.SamplingMode.Value = pCeModeList->SamplingMode.Value;
    TimingInfo.SignalStandard = (DD_SIGNAL_STANDARD)pCeModeList->SignalStandard;
    TimingInfo.CeData.Par[0].Value = pCeModeList->Par[0].Value;
    TimingInfo.CeData.Par[1].Value = pCeModeList->Par[1].Value;
    TimingInfo.CeData.IsNativeFormat[0] = pCeModeList->NativeFormat[0];
    TimingInfo.CeData.IsNativeFormat[1] = pCeModeList->NativeFormat[1];
    TimingInfo.CeData.VicId[0] = pCeModeList->VicId[0];
    TimingInfo.CeData.VicId[1] = pCeModeList->VicId[1];
    TimingInfo.CeData.VicId4k2k = pCeModeList->VicId4k2k;
    TimingInfo.S3DFormatMask = pCeModeList->S3DFormatMask;

    CeInfoParserAddPrModes(&TimingInfo, pEdidData);

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Method to return pointer corresponding to input Display ID Block.
 *
 * @param pGetBlockById
 * @return void
 ***************************************************************/
void CeInfoParserFillGetBlockByIdArgs(GET_BLOCK_BY_ID_ARGS* pGetBlockById, DDU8 BlockIdOrTag, DDU8 CeaExtendedTag, DDU32 IEEERegNum, DDU8 InstanceOfBlock, DDU8* pInputBuf, DDU32 InputBlockLength)
{
    pGetBlockById->BlockIdOrTag = BlockIdOrTag;
    pGetBlockById->CeaExtendedTag = CeaExtendedTag;
    pGetBlockById->IEEERegNum = IEEERegNum;
    pGetBlockById->InstanceOfBlock = InstanceOfBlock; // Valid only for Display ID base block/extension block; helpful to search if there are multiple instances of timing blocks
    pGetBlockById->pInputBuf = pInputBuf;
    pGetBlockById->InputBlockLength = InputBlockLength;
    pGetBlockById->pOutputBlock = NULL;
}

/***************************************************************
 * @brief Method to return Count of input Display ID Block
 *
 * @param pGetBlockById
 * @return DDU8
 ***************************************************************/
static DDU8 CeInfoParserGetBlockCount(GET_BLOCK_BY_ID_ARGS* pGetBlockById)
{
    CE_EXT_BLK_HEADER CeDatablkHeader;
    DDU8 Offset;
    DDU8 Count;

    // Arg validation
    DDASSERT(pGetBlockById);
    DDASSERT(pGetBlockById->pInputBuf);

    if (pGetBlockById->BlockIdOrTag > CEA_USE_EXTENDED_TAG)
    {
        // Invalid block ID found or block parsing not implemented yet
        return 0;
    }
    Offset = 0;
    Count = 0;
    do
    {
        CeDatablkHeader.Value = *((DDU16*)((pGetBlockById->pInputBuf + Offset)));

        if ((CeDatablkHeader.CeHdr.Tag != pGetBlockById->BlockIdOrTag) || (CeDatablkHeader.CeHdr.Length < 1))
        {
            // Requested data block was not found. So look into the next Block
            // Next Block's offset is <current offset + 1 (for Tag byte) + Size of this block>
            Offset += CeDatablkHeader.CeHdr.Length + 1;

            // Continue searching for next block if Tags are not matching
            continue;
        }
        else if (CeDatablkHeader.CeHdr.Tag == pGetBlockById->BlockIdOrTag)
        {
            // Increase the count if Tag or block id is found
            Count++;
        }
        Offset += CeDatablkHeader.CeHdr.Length + 1;
    } while (Offset < pGetBlockById->InputBlockLength);

    return Count;
}

/***************************************************************
 * @brief Method to return pointer corresponding to input Display ID Block.
 *
 * @param pGetBlockById
 * @return void
 ***************************************************************/
static void CeInfoParserGetCeaExtnBlockByID(GET_BLOCK_BY_ID_ARGS* pGetBlockById)
{
    CE_EXT_BLK_HEADER CeDatablkHeader;
    DDU8 Offset = 0;
    HDMI_VSDB* pHdmiVsdbBlock = NULL;
    DDU32 IEEERegNum = 0;
    DDU8 BlockInstanceId = 0;
    // DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pGetBlockById);
    DDASSERT(pGetBlockById->pInputBuf);

    if (pGetBlockById->BlockIdOrTag > CEA_USE_EXTENDED_TAG)
    {
        // Invalid block ID found or block parsing not implemented yet
        return;
    }

    Offset = 0;

    do
    {
        CeDatablkHeader.Value = *((DDU16*)((pGetBlockById->pInputBuf + Offset)));

        if ((CeDatablkHeader.CeHdr.Tag != pGetBlockById->BlockIdOrTag) || (CeDatablkHeader.CeHdr.Length < 1))
        {
            // Requested data block was not found. So look into the next Block
            // Next Block's offset is <current offset + 1 (for Tag byte) + Size of this block>
            Offset += CeDatablkHeader.CeHdr.Length + 1;

            // Continue searching for next block if Tags are not matching
            continue;
        }

        // Match inout IEEE registration ID as well for vendor Data block
        if (CeDatablkHeader.CeHdr.Tag == CEA_VENDOR_DATABLOCK)
        {
            pHdmiVsdbBlock = (HDMI_VSDB*)(pGetBlockById->pInputBuf + Offset);
            IEEERegNum = DisplayInfoParserConvertHexArrayToInt(pHdmiVsdbBlock->IEEERegID, 3);
            if (IEEERegNum == pGetBlockById->IEEERegNum)
            {
                pGetBlockById->pOutputBlock = pGetBlockById->pInputBuf + Offset;
                break;
            }
        }
        else
        {
            if (CeDatablkHeader.CeHdr.Tag != CEA_USE_EXTENDED_TAG)
            {
                // Match Extended tag as well if CEA_USE_EXTENDED_TAG is specified
                if (BlockInstanceId < pGetBlockById->InstanceOfBlock)
                {
                    // except Display ID block and Audio Data block the block id instance is always set to 0. Once we get multiple audio block internally we update it to new instance id
                    Offset += CeDatablkHeader.CeHdr.Length + 1;
                    BlockInstanceId += 1;
                    continue;
                }
                // Return success if TagID matches for non Extended tag
                pGetBlockById->pOutputBlock = pGetBlockById->pInputBuf + Offset;
                break;
            }
            else if (CeDatablkHeader.ExtendedTag == pGetBlockById->CeaExtendedTag)
            {
                // Return success if TagID matches for non Extended tag
                pGetBlockById->pOutputBlock = pGetBlockById->pInputBuf + Offset;
                break;
            }
        }

        // Requested data block was not found. So look into the next Block
        // Next Block's offset is <current offset + 1 (for Tag byte) + Size of this block>
        Offset += CeDatablkHeader.CeHdr.Length + 1;
    } while (Offset < pGetBlockById->InputBlockLength);
}

/***************************************************************
 * @brief Method to parse Viclist stored in context data and return TRUE if matching with input VicID.
 *
 * @param VicCount
 * @param  pCeModeList
 * @param  VicID
 * @return DD_BOOL
 ***************************************************************/
DD_BOOL CeInfoParserIsVicPresentInList(DDU32 VicCount, CE_MODE_LIST* pCeModeList, DDU32 VicID)
{
    DD_BOOL IsViCPresentinList = FALSE; // Assume VIC not present in list
    DDU8 Count;

    // Arg validation
    DDASSERT(pCeModeList);

    for (Count = 0; ((Count < MAX_VIC_DEFINED) && (Count < VicCount)); Count++)
    {
        if ((pCeModeList[Count].VicId[0] == VicID) || (pCeModeList[Count].VicId[1] == VicID))
        {
            IsViCPresentinList = TRUE;
            break;
        }
    }

    return IsViCPresentinList;
}

/***************************************************************
 * @brief Method to parse the Video Capability Block in CE-Extension.
 *      This method will cache relevant into DispInfoParserCtxt Data.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseVideoCapabilityDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CEA_VIDEO_CAPABILITY_BLOCK* pCeVideoCapDesc = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_VIDEO_CAP_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Colorimetry Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pCeVideoCapDesc = (CEA_VIDEO_CAPABILITY_BLOCK*)(GetBlockById.pOutputBlock);
    pEdidData->pData->FtrSupport.IsVideoCapBlockBlockPresent = TRUE;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.CEScanBehavior = pCeVideoCapDesc->VideoCapinfo.CEScanBehavior;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.IsQuantRangeSelectable = pCeVideoCapDesc->VideoCapinfo.IsQuantRangeSelectable;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.ITScanBehavior = pCeVideoCapDesc->VideoCapinfo.ITScanBehavior;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.PTScanBehavior = pCeVideoCapDesc->VideoCapinfo.PTScanBehavior;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief  Method to parse the VICs/DTDs mentioned in the Video Format Preference data Block in CE-Extension.
 *      This method will cache relevant info like, no of preferred DTDs & their indices in the mode list.
 *
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return void
 ***************************************************************/
void CeInfoParserParseVfpdbList(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    CEA_VIDEOFORMAT_PREF_BLOCK* pVideoFormatPrefBlk = NULL;
    DDU8* pVideoFormatPrefList = NULL;
    DDU8 Index;
    DDU8 VicId = VIC_UNDEFINED;
    DDU32 DTDnum;
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_VIDEO_FORMAT_PREFERENCE_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Video format preference Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return;
    }

    pVideoFormatPrefBlk = (CEA_VIDEOFORMAT_PREF_BLOCK*)(GetBlockById.pOutputBlock);
    pVideoFormatPrefList = &(pVideoFormatPrefBlk->PVideoFormatPrefList);

    // No. of video descriptors = Length - 1 extended Tag byte
    for (Index = 0; Index < (pVideoFormatPrefBlk->CeExtHeader.CeHdr.Length - 1); Index++)
    {
        VicId = pVideoFormatPrefList[Index];
        if ((DD_IS_IN_RANGE(VicId, VICID_18BYTE_DTDRANGE_START, VICID_18BYTE_DTDRANGE_END)))
        {
            // Short Video reference(SVR) value is between 129 to 144 --> these are 18 byte DTD timings, spec CTA-861H, section 7.5.12

            DTDnum = (VicId - (VICID_18BYTE_DTDRANGE_START - 1));
            pEdidData->Vfpdb.DtdCount++;
            DD_SET_BIT(pEdidData->Vfpdb.DtdMask, (DTDnum - 1));
        }
    } // for loop for Index ends

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief  Method to parse the Colorimetry data Block in CE-Extension.
 *      This method will cache relevant into DispInfoParserCtxt Data.
 *
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseColorimetryDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CEA_EXT_COLORIMETRY_DATA_BLOCK* pColorimetryDescBlock = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_COLORIMETRY_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Colorimetry Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pColorimetryDescBlock = (CEA_EXT_COLORIMETRY_DATA_BLOCK*)(GetBlockById.pOutputBlock);

    pEdidData->pData->HdrCaps.CeColorimetryData.ColorimetryType.Value = pColorimetryDescBlock->CEColorimetryData;

    // For Future gamut-related metadata, of type CE_PROFILE_SUPPORT
    // pEdidData->pData->HdrCaps.CeColorimetryData.CeProfileSupported = pColorimetryDescBlock->CEGamutBoundaryDataProfile;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Method to parse the Audio data Block in of CE-Extension.
 *
 *         Will set the AudioSupported flag in Context data if block found
 *
 * -----------+--------------------+-----------------------------------------+
 *            |    1   | Audio Tag |Length = total #of audio bytes following |
 *            |        |    Code   |this byte (L1)                           |
 *            |--------------------+-----------------------------------------+
 *  Audio     |    2   |                                                     |
 *  Data      |--------+                                                     |
 *  Block     |    3   | CEA Short Audio Descriptor 1                        |
 *            |--------+                                                     |
 *            |    4   |                                                     |
 *            |--------------------------------------------------------------+
 *            | ...    |                                                     |
 *            |        |                                                     |
 *            |        |                                                     |
 *            | ...    |                                                     |
 *            |---------------------------------------------------------------
 *            |  L1-1  |                                                     |
 *            |--------|                                                     |
 *            |  L1    | CEA Short Audio Descriptor L1/3                     |
 *            |--------|                                                     |
 *            |  1+L1  |                                                     |
 * -----------+--------------------------------------------------------------+.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseAudioDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CEA_SHORT_AUDIO_DESCR_BLOCK* pShortAudioDescBlock = NULL;
    CEA_861B_ADB* pShortAudioDescList = NULL;
    DDU8 Counter;
    DDU32 AudioDataBlockCount;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_AUDIO_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_AUDIO_DATABLOCK, 0xFF, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    AudioDataBlockCount = CeInfoParserGetBlockCount(&GetBlockById);
    for (DDU8 BlockInstanceId = 0; BlockInstanceId < AudioDataBlockCount; BlockInstanceId++)
    {
        CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_AUDIO_DATABLOCK, 0xFF, 0, BlockInstanceId, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
        CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
        if (GetBlockById.pOutputBlock == NULL)
        {
            // Audio Data block not found, not a critical error
            DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
            return DDS_CE_EXTN_BLOCK_NOT_FOUND;
        }
        pEdidData->pData->FtrSupport.IsAudioSupported = TRUE;

        pShortAudioDescBlock = (CEA_SHORT_AUDIO_DESCR_BLOCK*)(GetBlockById.pOutputBlock);
        pShortAudioDescList = &(pShortAudioDescBlock->ShortAudioDesc[0]);

        // Each Audio descriptor block is of 3 bytes in length
        for (Counter = 0; ((Counter < DD_MAX_SAD_BLOCK_COUNT) && (Counter < (pShortAudioDescBlock->CeHeader.Length) / 3)); Counter++, pShortAudioDescList++)
        {
            // Maximum number of allowed SAD blocks are 14. Once we have those it'll break the loop
            if (DD_MAX_SAD_BLOCK_COUNT == pEdidData->pData->CeAudioCaps.NumSADBytes)
            {
                break;
            }

            switch (pShortAudioDescList->AudioFormatCode)
            {
                // uncompressed audio (Linear PCM)
            case AUDIO_LPCM:
                pEdidData->pData->CeAudioCaps.IsLpcmSadFound = TRUE;

                // Keeping a seperate copy of LPCM SAD as it is used to prune Audio modes for EELD
                DD_MEM_COPY_SAFE(&(pEdidData->pData->CeAudioCaps.LpcmSad), sizeof(CEA_861B_ADB), pShortAudioDescList, sizeof(CEA_861B_ADB));
                // No break as we also want to copy this to SAD block
            // compressed audio
            case AUDIO_AC3:
            case AUDIO_MPEG1:
            case AUDIO_MP3:
            case AUDIO_MPEG2:
            case AUDIO_DTS:
            case AUDIO_DOLBY_DIGITAL:
            case AUDIO_DST:

                // Below formats would also need the SKU to support HBR Audio (Audio Content Protection)
                // if(pPlatformParams->pSkuTable->FtrEnableAudioContentProtection)
            case AUDIO_OBA:
            case AUDIO_MAT:
            case AUDIO_DTS_HD:
            case AUDIO_AAC:
            case AUDIO_ATRAC:
            case AUDIO_WMA_PRO:
                DD_MEM_COPY_SAFE(&(pEdidData->pData->CeAudioCaps.SadBlock[pEdidData->pData->CeAudioCaps.NumSADBytes]), sizeof(CEA_861B_ADB), pShortAudioDescList, sizeof(CEA_861B_ADB));
                pEdidData->pData->CeAudioCaps.NumSADBytes++;
                break;
            default:
                // Invalid/unsupported Audio format code
                break;
            }
        }
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Method to parse the Speaker Allocation Data Block data Block in of CE-Extension.
 *
 *         Will set the required flags in Context data if block found
 *
 *
 * -----------+--------------------------------------------------------------+
 *            |   1    |  Speaker  |Length = total #of SA bytes following    |
 *            |        | Tag Code  |this byte (3)                            |
 *  Speaker   |--------------------------------------------------------------+
 *  Allocation|   2    |                                                     |
 *  Data      |--------|                                                     |
 *  Block     |   3    | Speaker Allocation Data Block Payload(3 bytes)      |
 *            |--------|                                                     |
 *            |   4    |                                                     |
 * -----------+--------------------------------------------------------------+.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/

DDSTATUS CeInfoParserParseSpeakerAllocationDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CEA_SPEAKER_ALLOCATION_BLOCK* pSpeakerAllocationBlock = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_SPEAKER_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_SPEAKER_DATABLOCK, 0xFF, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Audio Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pSpeakerAllocationBlock = (CEA_SPEAKER_ALLOCATION_BLOCK*)GetBlockById.pOutputBlock;
    pEdidData->pData->CeAudioCaps.SpeakerAllocationBlock = pSpeakerAllocationBlock->SpeakerAllocationBlock[0];

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief  Method to parse the HDR10+ VSVDB (Vendor Specific Video Data Block)
 *
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return void
 ***************************************************************/
void CeInfoParserParseHdr10PlusVsVdb(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CEA_EXT_HDR10P_VENDOR_SPECIFIC_VIDEO_DATA_BLOCK* pVendorSpecificVideoBlock = NULL;
    DD_CE_HDR10_PLUS_CAPS* pHdr10PlusCaps = &pEdidData->pData->HdrCaps.Hdr10PlusCaps;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_VENDOR_SPECIFIC_VIDEO_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Vendor Specific Video Data Block not found, not a critical error
        pHdr10PlusCaps->IsValidHdr10PlusVsVdbFound = FALSE;
        DISP_FUNC_EXIT_W_STATUS(DDS_HDR10PLUS_VSVDB_NOT_FOUND);
        return;
    }

    pVendorSpecificVideoBlock = (CEA_EXT_HDR10P_VENDOR_SPECIFIC_VIDEO_DATA_BLOCK*)(GetBlockById.pOutputBlock);

    pHdr10PlusCaps->IsValidHdr10PlusVsVdbFound = FALSE;

    pHdr10PlusCaps->IEEERegID[0] = pVendorSpecificVideoBlock->IEEERegID[0];
    pHdr10PlusCaps->IEEERegID[1] = pVendorSpecificVideoBlock->IEEERegID[1];
    pHdr10PlusCaps->IEEERegID[2] = pVendorSpecificVideoBlock->IEEERegID[2];

    pHdr10PlusCaps->ApplicationVersion = pVendorSpecificVideoBlock->ApplicationVersion;
    pHdr10PlusCaps->FullFramePeakLuminanceIndex = pVendorSpecificVideoBlock->FullFramePeakLuminanceIndex;
    pHdr10PlusCaps->PeakLuminanceIndex = pVendorSpecificVideoBlock->PeakLuminanceIndex;

    // Validate EDID values
    // Peak luminance value must be within[1..15]
    // Full frame luminance must be within[0..3]
    if ((pHdr10PlusCaps->PeakLuminanceIndex != 0) && (pHdr10PlusCaps->PeakLuminanceIndex <= 15) && (pHdr10PlusCaps->FullFramePeakLuminanceIndex <= 3))
    {
        // Multiplier is (2^index) which is quivaltent of left shift by index.
        pHdr10PlusCaps->MaxFALL = (MaxFALLFromFullFramePeakLuminanceIndexAndPeakLuminanceIndex[pHdr10PlusCaps->PeakLuminanceIndex] << (pHdr10PlusCaps->FullFramePeakLuminanceIndex));
        pHdr10PlusCaps->MaxCLL = MaxCLLFromPeakLuminanceIndex[pHdr10PlusCaps->PeakLuminanceIndex];

        pHdr10PlusCaps->IsValidHdr10PlusVsVdbFound = TRUE;
    }

    pHdr10PlusCaps->Hdr10PlusCtrl.LowLatency = DD_HDR10P_LOW_LATENCY_MODE_DEFAULT;
    pHdr10PlusCaps->Hdr10PlusCtrl.SourceToneMapping = DD_HDR10P_SOURCE_TM_DEFAULT;

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief  Method to parse the parse the DTD blocks within CE-Extension.
 *         It will also add corresponding CE modes to mode-table.
 *
 * @param  pCeEdidExtn
 * @param  pModeTable
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddModesFromCeExtnDtdBlock(CE_EDID_EXTN_BLOCK* pCeEdidExtn, DD_TABLE* pModeTable, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    DDU32 NumDTD; // Number of DTDs
    EDID_DTD_TIMING* pDTD = NULL;
    DDU32 DtdCntr;
    DD_TIMING_INFO TimingInfo;
    ADD_MODES AddModes;
    DDU8 VicId = VIC_UNDEFINED;
    CE_MODE_LIST CeVicData;

    DISP_FUNC_ENTRY();

    DDASSERT(pCeEdidExtn);
    DDASSERT(pModeTable);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));
    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    // Determining the number of DTDs :
    // Since some of the CE Vendors are not following the spec
    // we are using following method to get the number of DTD's
    // instead of a doing it as per the Spec : 'ulNumDTD = pCeEdid->Capabilty & 0x0F;'
    // The number of DTD's is calculated from the offset excluding the checksum byte
    NumDTD = (128 - pCeEdidExtn->DTDOffset - 1) / sizeof(EDID_DTD_TIMING);

    // Check for valid Number of DTDs is included (nDTD <= 6).
    if ((0 == NumDTD) || (NumDTD > 6))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_DTD_NOT_FOUND);
        return DDS_CE_EXTN_DTD_NOT_FOUND;
    }
    Status = DDS_SUCCESS;
    // Extract mode entries from DTD blocks
    // DTD Block starts at ulDTDOffset in the Ce-Extension block i.e., <pCeEdid + 'ucDTDOffset' Number of Bytes>
    pDTD = (EDID_DTD_TIMING*)((DDU8*)pCeEdidExtn + pCeEdidExtn->DTDOffset);
    for (DtdCntr = 0; DtdCntr < NumDTD; DtdCntr++, pDTD++)
    {
        DisplayInfoRoutinesGetTimingFromDTD(pDTD, &TimingInfo);
        // Since some of the CE Vendors are not following the spec
        // we are using methode described above to find the number of DTDs
        // this might result in invalid call to gettiming, hence check
        // the timing returned and ignore it if invalid
        if (TimingInfo.HActive == 0 || TimingInfo.VActive == 0)
        {
            // Mode not found
            continue;
        }

        TimingInfo.CeData.VicId[0] = VIC_UNDEFINED;
        TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
        TimingInfo.CeData.SamplingMode.Rgb = 1;
        TimingInfo.SignalStandard = DD_VESA_DMT;
        TimingInfo.ModeSource = CE_DTD;

        AddModes.pTimingInfo = &TimingInfo;
        AddModes.ForceAdd = TRUE;

        // CE extension display
        VicId = CeInfoParserIsCeMode(&TimingInfo);
        if (IS_VALID_VICID(VicId))
        {
            // CE Extn DTD timing is CE Mode
            // Add it to the VIClist with all required params
            CeVicData.IsPreferred = FALSE;
            CeVicData.SignalStandard = DD_CEA_861B; // It is still monitor timings
            CeVicData.SamplingMode.Rgb = 1;
            CeVicData.VicId[0] = VicId;
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_DTD);
            Status = DDS_SUCCESS;
        }
        else
        {
            Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }

    } // for loop for DTD ends

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief  Method to parse the parse the Short Video Descriptors (SVD) in of CE-Extension.
 *         and just return SVD list.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserGetSvdListFromVideoDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    CEA_SHORT_VIDEO_DESCR_BLOCK* pShortVideoDesc = NULL;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU8* pSVDList = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VIDEO_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VIDEO_DATABLOCK, 0xFF, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Short Video descriptor Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pShortVideoDesc = (CEA_SHORT_VIDEO_DESCR_BLOCK*)(GetBlockById.pOutputBlock);
    pSVDList = &(pShortVideoDesc->SVDList);

    if (pShortVideoDesc->CeHeader.Length > 0)
    {
        // Store the pointer to SVD list so that its useful for future parsing
        pEdidData->pSVDList = pSVDList;
        pEdidData->SvdListLength = pShortVideoDesc->CeHeader.Length;
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Info Parser Add Modes From Svd Block.
 *
 *   Method to parse the parse the Short Video Descriptors (SVD) in of CE-Extension
 *         It will also add corresponding CE modes to mode-table
 *         For PR modes, it will add all corresponding modes to the table.
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddModesFromVideoDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    CEA_SHORT_VIDEO_DESCR_BLOCK* pShortVideoDesc = NULL;
    DDU8* pSVDList = NULL;
    DDU8 Index;
    DDU8 VicId;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CE_MODE_LIST CeVicData;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VIDEO_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VIDEO_DATABLOCK, 0xFF, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Short Video descriptor Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pShortVideoDesc = (CEA_SHORT_VIDEO_DESCR_BLOCK*)(GetBlockById.pOutputBlock);
    pSVDList = &(pShortVideoDesc->SVDList);

    // Breakout with success status if this call was for just to get caps (ensure further from here, there are no caps that are parsed)
    if (pParseExtDataBlkArgs->pModeTable == NULL)
    {
        return DDS_SUCCESS;
    }

    for (Index = 0; Index < pShortVideoDesc->CeHeader.Length; Index++)
    {
        Status = GetVicIndexBasedOnCeIndex(pSVDList[Index], &VicId);
        if (IS_DDSTATUS_ERROR(Status))
        {
            // Invalid VIC ID or Timinginfo not found for input VicID
            continue;
        }

        // Add the VIC found to VICModelist with all required params
        CeVicData.NativeFormat[0] = FALSE;
        if ((VicId <= CEA_MAX_VIC_SUPPORTING_NATIVE_FORMAT) && (pSVDList[Index] & CEA_NATIVE_FORMAT_BIT_MASK))
        {
            CeVicData.NativeFormat[0] = TRUE;
        }
        CeVicData.IsPreferred = FALSE;
        CeVicData.SignalStandard = DD_CEA_861B; // It is still monitor timings
        CeVicData.SamplingMode.Rgb = 1;
        CeVicData.VicId[0] = VicId;
        CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
    } // for loop for Vic List ends

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Method to parse the parse the 420 Data block SVDs in CE-Extension.
 *
 *      and returns if its present. Called from Caps parsing.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserIs420VideoDataBlockPresent(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_420_VIDEO_DATABLOCK, CEA_HDMI_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);

    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // 420 Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pEdidData->pData->FtrSupport.IsYuv420VideoBlockPresent = TRUE;
    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Info Parser Add Modes From 420Video Data Block.
 *
 *  Method to parse the parse the 420 Data block SVDs in CE-Extension
 *         It will also add corresponding CE modes to mode-table
 *         For PR modes, it will add all corresponding modes to the table.
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddModesFrom420VideoDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    CEA_420_DESCR_BLOCK* p420VideoDesc = NULL;
    DDU8* p420VideoDescrList = NULL;
    DDU8 Index;
    DDU8 VicId;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CE_MODE_LIST CeVicData;

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_420_VIDEO_DATABLOCK, CEA_HDMI_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);

    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // 420 Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    p420VideoDesc = (CEA_420_DESCR_BLOCK*)(GetBlockById.pOutputBlock);
    p420VideoDescrList = &(p420VideoDesc->P420VideoDescrList);

    pEdidData->pData->FtrSupport.IsYuv420VideoBlockPresent = TRUE;

    // Breakout with success status if this call was for just to get caps (ensure further from here, there are no caps that are parsed)
    if (pParseExtDataBlkArgs->pModeTable == NULL)
    {
        return DDS_SUCCESS;
    }

    // No. of video descriptors = Length - 1 extended Tag byte
    for (Index = 0; Index < (p420VideoDesc->CeExtHeader.CeHdr.Length - 1); Index++)
    {
        Status = GetVicIndexBasedOnCeIndex(p420VideoDescrList[Index], &VicId);
        if (IS_DDSTATUS_ERROR(Status))
        {
            // Invalid VIC ID or Timinginfo not found for input VicID
            continue;
        }

        // Add the VIC found to VICModelist with all required params
        CeVicData.NativeFormat[0] = FALSE;
        if ((VicId <= CEA_MAX_VIC_SUPPORTING_NATIVE_FORMAT) && (p420VideoDescrList[Index] & CEA_NATIVE_FORMAT_BIT_MASK))
        {
            CeVicData.NativeFormat[0] = TRUE;
        }
        CeVicData.IsPreferred = FALSE;
        CeVicData.SignalStandard = DD_CEA_861B; // It is still monitor timings
        CeVicData.SamplingMode.Yuv420 = 1;
        CeVicData.VicId[0] = VicId;
        CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Method to check if the 420 Capability Data block is present in CE-Extension.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return void
 ***************************************************************/
DDSTATUS CeInfoParserIs420CapabilityBlockPresent(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_420_CAPABILITY_MAP_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // 420 Capability block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    if ((pEdidData->pSVDList == NULL) || (pEdidData->SvdListLength == 0))
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "420 Cap block found, but Short Video descriptor Data block not found, critical error");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    pEdidData->pData->FtrSupport.IsYuv420VideoBlockPresent = TRUE;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Info Parser Parse YCbCr 420 Capability Map Data Block.
 *
 *  Method to parse the parse the YCbCr 420 Capability Map Data block in CE-Extension
 *         It will also add corresponding CE modes to mode-table.
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParse420CapabilityMapDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    CEA_420_CAP_BLOCK* p420CapBlock = NULL;
    DDU8* p420pCapBitMap = NULL;
    DDU8 ByteCount;
    DDU8 BitPosition;
    DDU8 VicId = VIC_UNDEFINED;
    DDU8 VicIndex, VideoDescriptorLength, Yuv420SupportedSvdRange;
    DD_BOOL AllSvdSupportYuv420;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CE_MODE_LIST CeVicData;

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_420_CAPABILITY_MAP_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // 420 Capability block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    // Video block was found. Add all 420 capable VICs present in the regular VDB to the list
    p420CapBlock = (CEA_420_CAP_BLOCK*)(GetBlockById.pOutputBlock);
    p420pCapBitMap = &(p420CapBlock->P420CapBitMap);

    if ((pEdidData->pSVDList == NULL) || (pEdidData->SvdListLength == 0))
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "420 Cap block found, but Short Video descriptor Data block not found, critical error");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    pEdidData->pData->FtrSupport.IsYuv420VideoBlockPresent = TRUE;

    // Breakout with success status if this call was for just to get caps (ensure further from here, there are no caps that are parsed)
    if (pParseExtDataBlkArgs->pModeTable == NULL)
    {
        return DDS_SUCCESS;
    }

    // As per CTA-861 Spec (Section 7.5.11), if Length of CTA data block is set to 1, the Y420CMDB does not include a
    // YCBCR420 Capability Bit Map and all the SVDs in the regular Video Data Block(s) support YCBCR 420 sampling mode.
    if (1 == p420CapBlock->CeExtHeader.CeHdr.Length)
    {
        VideoDescriptorLength = p420CapBlock->CeExtHeader.CeHdr.Length;
        Yuv420SupportedSvdRange = pEdidData->SvdListLength;
        AllSvdSupportYuv420 = TRUE;
    }
    else
    {
        VideoDescriptorLength = (p420CapBlock->CeExtHeader.CeHdr.Length - 1); // No. of video descriptors = Length - 1 extended Tag byte
        Yuv420SupportedSvdRange = BYTE_LENGTH;
        AllSvdSupportYuv420 = FALSE;
    }

    for (ByteCount = 0; ByteCount < VideoDescriptorLength; ByteCount++)
    {
        for (BitPosition = 0; BitPosition < Yuv420SupportedSvdRange; BitPosition++)
        {
            if (((((p420pCapBitMap[ByteCount]) & (1 << BitPosition)) == 0)) && (FALSE == AllSvdSupportYuv420))
            {
                // Cap bit not set for the byte
                continue;
            }

            VicIndex = (ByteCount * BYTE_LENGTH) + BitPosition; // Index starts from 0

            if ((VicIndex >= pEdidData->SvdListLength) || (VicIndex >= MAX_VIC_DEFINED))
            {
                DDASSERT(VicIndex < MAX_VIC_DEFINED); // To catch # of VICs in EDID < MAX_VIC_DEFINED
                // Invalid VicIndex
                break;
            }

            Status = GetVicIndexBasedOnCeIndex(pEdidData->pSVDList[VicIndex], &VicId);
            if (IS_DDSTATUS_ERROR(Status))
            {
                // Invalid VIC ID or Timinginfo not found for input VicID
                continue;
            }

            // Update the VIC found to VIClist with all required params
            CeVicData.NativeFormat[0] = FALSE;
            if ((VicId <= CEA_MAX_VIC_SUPPORTING_NATIVE_FORMAT) && (pEdidData->pSVDList[VicIndex] & CEA_NATIVE_FORMAT_BIT_MASK))
            {
                CeVicData.NativeFormat[0] = TRUE;
            }
            CeVicData.IsPreferred = FALSE;
            CeVicData.SignalStandard = DD_CEA_861B; // It is still monitor timings
            CeVicData.VicId[0] = VicId;
            CeVicData.SamplingMode.Yuv420 = 1;
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
        }
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Info Parser Parse Video Format Preference Block.
 *
 *  Method to parse the parse the Video format preference block in CE-Extension
 *         It will re-add the existing CE mode to mode-table as preferred mode.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseVicsFromVfpdb(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    CEA_VIDEOFORMAT_PREF_BLOCK* pVideoFormatPrefBlk = NULL;
    DDU8* pVideoFormatPrefList = NULL;
    DDU8 Index;
    DDU8 VicId = VIC_UNDEFINED;
    DD_TIMING_INFO TimingInfo;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CE_MODE_LIST CeVicData;
    DD_BOOL IsVicPresentInList;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));
    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_VIDEO_FORMAT_PREFERENCE_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // Video format preference Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pVideoFormatPrefBlk = (CEA_VIDEOFORMAT_PREF_BLOCK*)(GetBlockById.pOutputBlock);
    pVideoFormatPrefList = &(pVideoFormatPrefBlk->PVideoFormatPrefList);

    // No. of video descriptors = Length - 1 extended Tag byte
    for (Index = 0; Index < (pVideoFormatPrefBlk->CeExtHeader.CeHdr.Length - 1); Index++)
    {
        VicId = pVideoFormatPrefList[Index];

        // Short Video reference(SVR) value is between 1   to 127 and 193 to 253 --> its a VIC,
        if (((VicId >= 1) && (VicId <= 127)) || ((VicId >= 193) && (VicId <= 253)))
        {
            IsVicPresentInList = CeInfoParserIsVicPresentInList(pEdidData->VicCount, pEdidData->pCeModeList, VicId);
            if (IsVicPresentInList == FALSE)
            {
                // VIC is not present as part of Video Data Block or YCbCr420 Video Data Block
                continue;
            }

            Status = GetVicIndexBasedOnCeIndex(pVideoFormatPrefList[Index], &VicId);
            if (IS_DDSTATUS_ERROR(Status))
            {
                // Invalid VIC ID or Timinginfo not found for input VicID
                continue;
            }

            // Add it to the VIClist with all required params
            CeVicData.IsPreferred = TRUE;
            CeVicData.SignalStandard = DD_CEA_861B;
            CeVicData.VicId[0] = VicId;
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
        }
        // Short Video reference(SVR) value between 129 to 144 --> its a DTD timing , will be marked as Preferred in EdidParserMarkVfpdbTimingsAsPreferred()

    } // for loop for Index ends

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Info Parser Parse Hdr Cap Data Block.
 *
 *  parsing Below Block
 *       Vendor Specific Data Block (HDR-Cap Data Block) format
 *              HDR SM data block format
 *      | Byte # |    Bits[7-0]                                              |
 *      |--------------------------------------------------------------------|
 *      | 0      |Tag Code(07h)  | Length of data block(in bytes)(03h)       |
 *      |--------------------------------------------------------------------|
 *      | 1      |       Extended Tag Code (06h)                             |
 *      |--------------------------------------------------------------------|
 *      | 2      |Bytes[7:6] = Reserved     |ET5| ET4| ET3 | ET2 | ET1 | ET0 |
 *      |--------------------------------------------------------------------|
 *      | 3      | SM7  | SM6  | SM5  | SM4  | SM3  | SM2  | SM 1 |   SM0    |
 *      |--------------------------------------------------------------------|
 *      | 4      |       MAX CLL                                             |
 *      |--------------------------------------------------------------------|
 *      | 5      |       MAX FALL                                            |
 *      |--------------------------------------------------------------------|
 *      | 6      |       MIN CLL                                             |
 *      |--------------------------------------------------------------------|.
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseHdrCapDataBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    HDR_STATIC_META_DATA_BLOCK* pHdrBlockData = NULL;
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_HDR_STATIC_META_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // HDR Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pHdrBlockData = (HDR_STATIC_META_DATA_BLOCK*)(GetBlockById.pOutputBlock);

    pEdidData->pData->HdrCaps.CeEdidData.HdrMetaDataBlockFound = TRUE;

    switch (pHdrBlockData->CeExtHeader.CeHdr.Length)
    {
    case 5:
        pEdidData->pData->HdrCaps.CeEdidData.DesiredMaxFALL = pHdrBlockData->DesiredMaxFALL;
        // No breaks as we need to parse lower fields as well
    case 4:
        pEdidData->pData->HdrCaps.CeEdidData.DesiredMaxCLL = pHdrBlockData->DesiredMaxCLL;
        // No breaks as we need to parse lower fields as well
    case 3:
        pEdidData->pData->HdrCaps.CeEdidData.HdrStaticMetaDataType.Value = pHdrBlockData->HdrStaticMetaDataType;
        // No breaks as we need to parse lower fields as well
    case 2:
        pEdidData->pData->HdrCaps.CeEdidData.EOTFSupported.Value = pHdrBlockData->EOTFSupported;
        break;
    case 1:
    case 0:
        // No more HDR data, nothing to parse
        break;
    default: // Case for (CeHdr.Length >= 6)
        pEdidData->pData->HdrCaps.CeEdidData.DesiredMinCLL = pHdrBlockData->DesiredMinCLL;
        pEdidData->pData->HdrCaps.CeEdidData.DesiredMaxFALL = pHdrBlockData->DesiredMaxFALL;
        pEdidData->pData->HdrCaps.CeEdidData.DesiredMaxCLL = pHdrBlockData->DesiredMaxCLL;
        pEdidData->pData->HdrCaps.CeEdidData.HdrStaticMetaDataType.Value = pHdrBlockData->HdrStaticMetaDataType;
        pEdidData->pData->HdrCaps.CeEdidData.EOTFSupported.Value = pHdrBlockData->EOTFSupported;
        break;
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Method to parse 4K 2K modes from HDMI VSDB block.
 *
 * @param pHdmiVsdbModeArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAdd4k2kModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData)
{
    S3DCAPS_VICLENGTH S3DCapsVicLength;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DDU8 VicCount;
    DDU8 VicId4k2k;
    DDU8* pCeModeList = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pHdmiVsdbModeArgs->pHdmiVsdbBlock);

    DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

    S3DCapsVicLength = *(((S3DCAPS_VICLENGTH*)((DDU8*)(pHdmiVsdbModeArgs->pHdmiVsdbBlock) + pHdmiVsdbModeArgs->VICOffset)));
    pCeModeList = ((DDU8*)(pHdmiVsdbModeArgs->pHdmiVsdbBlock) + pHdmiVsdbModeArgs->VICOffset + sizeof(S3DCAPS_VICLENGTH));

    // Loop through all VICs present
    for (VicCount = 0; VicCount < S3DCapsVicLength.HdmiVicLen; VicCount++)
    {
        VicId4k2k = pCeModeList[VicCount];

        if (VicId4k2k >= g_ulNum4kx2kModes)
        {
            continue;
        }
        // Since this Ceindex is <127 and >64, no need to covert to VicID
        if (CeInfoParserIsVicPresentInList(pEdidData->VicCount, pEdidData->pCeModeList, g_4kx2kModes[VicId4k2k].CEIndex))
        {
            // If the same timings was already added as part of SVD, skip adding the mode here
            continue;
        }

        if (g_4kx2kModes[VicId4k2k].pTimingInfo == NULL)
        {
            continue;
        }

        // If Vic found in the list
        TimingInfo = *g_4kx2kModes[VicId4k2k].pTimingInfo;
        TimingInfo.ModeSource = CE_SVD;
        AddModes.pTimingInfo = &TimingInfo;
        // Should be zero as it doesn't fall under short video descriptor. We won't use it in AVI-IF.
        // VicID 0 also indicates HDMI 4kx2k mode
        if ((TRUE == pEdidData->pData->HdmiCaps.HfVsdbInfo.IsAllmSupported) || (TRUE == pEdidData->pData->HdrCaps.Hdr10PlusCaps.IsValidHdr10PlusVsVdbFound))
        {
            TimingInfo.CeData.VicId4k2k = 0;
            TimingInfo.CeData.VicId[0] = g_4kx2kModes[VicId4k2k].CEIndex;
        }
        else
        {
            TimingInfo.CeData.VicId[0] = 0;
            TimingInfo.CeData.VicId[1] = 0;
            TimingInfo.CeData.VicId4k2k = VicId4k2k; // This is needed for sending VS info frame
        }

        TimingInfo.CeData.Par[0].Is_Avi_Par_4_3 = (g_4kx2kModes[VicId4k2k].AspectRatio == AVI_PAR_4_3) ? TRUE : FALSE;
        TimingInfo.CeData.Par[0].Is_Avi_Par_16_9 = (g_4kx2kModes[VicId4k2k].AspectRatio == AVI_PAR_16_9) ? TRUE : FALSE;
        TimingInfo.CeData.Par[0].Is_Avi_Par_64_27 = (g_4kx2kModes[VicId4k2k].AspectRatio == AVI_PAR_64_27) ? TRUE : FALSE;
        TimingInfo.Flags.PreferredMode = FALSE;

        AddModes.ForceAdd = TRUE;

        DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
    } // for loop for VicCount

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief  Checks if S3d Block present.
 *
 * @param pHdmiVsdbModeArgs
 * @param pEdidData
 * @return DD_BOOL
 ***************************************************************/
DD_BOOL CeInfoParserIsS3DFrameSupported(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData)
{
    S3DCAPS_VICLENGTH S3DCapsVicLength;
    CE_MODE_LIST CeVicData;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pHdmiVsdbModeArgs->pHdmiVsdbBlock);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    S3DCapsVicLength = *(((S3DCAPS_VICLENGTH*)((DDU8*)(pHdmiVsdbModeArgs->pHdmiVsdbBlock) + pHdmiVsdbModeArgs->VICOffset)));

    if (S3DCapsVicLength.S3DCaps.S3DPresent == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

/***************************************************************
 * @brief Ce Info Parser AddS3D Modes.
 *
 *  Parse and add S3D modes from VSDB block
 *       Vendor Specific Data Block (VSDB) format.
 * @param pHdmiVsdbModeArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddS3DModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    S3DCAPS_VICLENGTH S3DCapsVicLength;
    DDU32 S3DVics;
    DDU8 S3DFormatOffset;
    S3D_FORMATS S3DStruct;
    DDU32 S3DBytesAlreadyRead;
    DDU32 S3DMask;
    DDU8* pS3DStruct = NULL;
    DDU8 VicCount;
    DDU8 VicId = VIC_UNDEFINED;
    DDU32 S3DFormat;
    VIC_ORDER_3D_STRUCT_DETAILS VicOrder;
    CE_MODE_LIST CeVicData;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pHdmiVsdbModeArgs->pHdmiVsdbBlock);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    S3DCapsVicLength = *(((S3DCAPS_VICLENGTH*)((DDU8*)(pHdmiVsdbModeArgs->pHdmiVsdbBlock) + pHdmiVsdbModeArgs->VICOffset)));

    if (S3DCapsVicLength.S3DCaps.S3DPresent == FALSE)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_S3D_MODES_NOT_PRESENT);
        return DDS_CE_EXTN_S3D_MODES_NOT_PRESENT;
    }

    pEdidData->pData->FtrSupport.IsS3DLRFramesSupported = TRUE;

    // Breakout with success status if this call was for just to get caps (ensure further from here, there are no caps that are parsed)
    if (pHdmiVsdbModeArgs->pModeTable == NULL)
    {
        return DDS_SUCCESS;
    }

    // Add mandatory formats to the mode list
    CeInfoParserAddMandatoryS3DModes(pHdmiVsdbModeArgs, pEdidData);

    if (S3DCapsVicLength.Hdmi3DLen <= 0)
    {
        // No more 3D modes to add
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_S3D_MODES_NOT_PRESENT);
        return DDS_CE_EXTN_S3D_MODES_NOT_PRESENT;
    }

    // Check and add other formats which might be supported
    S3DVics = 0xFFFF; // All Vics in the first 16 entries support all 3D formats in 3D structure
    S3DFormatOffset = pHdmiVsdbModeArgs->VICOffset + S3DCapsVicLength.HdmiVicLen + sizeof(S3DCAPS_VICLENGTH);
    pS3DStruct = (DDU8*)(pHdmiVsdbModeArgs->pHdmiVsdbBlock);

    S3DBytesAlreadyRead = 0;
    if (S3DCapsVicLength.S3DCaps.S3DMultiPresent == 1 || S3DCapsVicLength.S3DCaps.S3DMultiPresent == 2)
    {
        S3DStruct.S3DFormats = ((pS3DStruct[S3DFormatOffset] << 8) | ((pS3DStruct[S3DFormatOffset + 1])));
        S3DBytesAlreadyRead = 2;
        S3DFormatOffset += 2; // Move offset to point to next 2 bytes
    }
    else
    {
        S3DStruct.S3DFormats = 0;
    }
    if (S3DCapsVicLength.S3DCaps.S3DMultiPresent == 2)
    {
        // In this case both the 3D_Structure and 3D_Mask are valid
        S3DMask = ((pS3DStruct[S3DFormatOffset] << 8) | ((pS3DStruct[S3DFormatOffset + 1])));
        S3DVics &= S3DMask;
        S3DBytesAlreadyRead += 2;
        S3DFormatOffset += 2; // Move offset to point to next 2 bytes
    }

    if (S3DStruct.S3DFormats == 0)
    {
        // No more 3D modes to add`
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_S3D_MODES_NOT_PRESENT);
        return DDS_CE_EXTN_S3D_MODES_NOT_PRESENT;
    }

    // Add modes from 3D_Strucure_all and 3DMask when indicated by 3D multi present bit
    CeInfoParserAdd3DMultiPresentIndicatedS3DModes(pHdmiVsdbModeArgs, pEdidData, S3DStruct, S3DVics);

    // Point pS3DStruct to start of 2D_VIC_Order_X and 3D_Structure_X bits
    pS3DStruct = ((DDU8*)(pHdmiVsdbModeArgs->pHdmiVsdbBlock) + S3DFormatOffset);

    for (VicCount = 0; ((VicCount) < (S3DCapsVicLength.Hdmi3DLen - S3DBytesAlreadyRead)); VicCount++)
    {
        if (((VicCount + S3DFormatOffset) > pHdmiVsdbModeArgs->pHdmiVsdbBlock->CeDataBlockHdr.Length))
        {
            // double check to ensure we are not going past VSDB block
            break;
        }

        S3DFormat = 0;
        VicOrder.Byte0 = pS3DStruct[VicCount];
        /*
            Value of S2D_VIC_Order_X = 0000 : VIC 1
            Value of S2D_VIC_Order_X = 0001 : VIC 2
            Value of S2D_VIC_Order_X = 1111 : VIC 16
        */
        if (VicOrder.S2D_VIC_Order_X >= pEdidData->SvdListLength)
        {
            // This is an error. S3D caps indicated an SVD which is not even listed in SVD data block (Index exceeded SVD list length)
            break;
        }

        Status = GetVicIndexBasedOnCeIndex(pEdidData->pSVDList[VicOrder.S2D_VIC_Order_X], &VicId);
        if (IS_DDSTATUS_ERROR(Status))
        {
            // Invalid VIC ID or Timinginfo not found for input VicID
            continue;
        }

        switch ((DD_S3D_FORMAT)(VicOrder.S3DStruc_X))
        {
        case S3D_FRAME_PACKING:
            if ((FALSE == g_SupportedCeShortVideoModes[VicId].pTimingInfo->IsInterlaced) && (g_SupportedCeShortVideoModes[VicId].pTimingInfo->VActive >= 720))
            {
                S3DFormat = DD_S3D_FORMAT_MASK(S3D_FRAME_PACKING);
            }
            break;
        case S3D_TOP_BOTTOM:
            S3DFormat = DD_S3D_FORMAT_MASK(S3D_TOP_BOTTOM);
            break;
        case S3D_SIDE_BY_SIDE_HALF:
            VicCount++;
            VicOrder.Byte1 = pS3DStruct[VicCount];

            if (VicOrder.S3DDetail_X == 0)
            {
                // assumption: we only support SBS HalfHorizontalSubsampling/FP/TB so no action for other formats
                // assumption: we do not support Quincunx matrix
                // S3DFormat |= DD_S3D_FORMAT_MASK(S3D_SIDE_BY_SIDE_HALF_HOR_QUINCUNX_ALL4_SUBSAMPLING);
                S3DFormat = DD_S3D_FORMAT_MASK(S3D_SIDE_BY_SIDE_HALF_HOR_SUBSAMPLING);
            }
            else if (VicOrder.S3DDetail_X == 1)
            {
                S3DFormat = DD_S3D_FORMAT_MASK(S3D_SIDE_BY_SIDE_HALF_HOR_SUBSAMPLING);
            }
            else if (VicOrder.S3DDetail_X >= 6 && VicOrder.S3DDetail_X <= 10)
            {
                // assumption: we do not support Quincunx matrix, hence no action
            }
            break;
        default:
            // additional byte present for 3D_Details_X only if 3D_Structure_X lies in 1000~1111, 1000 is already taken care in previous case statement
            if (VicOrder.S3DStruc_X > 8)
            {
                VicCount++;
            }
            break;
        } // switch case

        // Currently we only support TB/SBS(HalfHorizontalsubsampling)/FP S3D formats
        if (S3DFormat != 0)
        {
            // Update the VIC that is present in the list
            CeVicData.IsPreferred = FALSE;
            CeVicData.SignalStandard = DD_CEA_861B;
            CeVicData.SamplingMode.Rgb = 1;
            CeVicData.VicId[0] = VicId;
            CeVicData.S3DFormatMask = S3DFormat; // Update S3D format
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
        }
    } // for loop for 2D Vics ends

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Parse and add S3D modes indicated by 3D_Strucure_all and 3DMask when.
 *               indicated by 3D multi present bit.
 *
 * @param  pHdmiVsdbModeArgs
 * @param  pEdidData
 * @param  S3DStruct
 * @param  S3DVics
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAdd3DMultiPresentIndicatedS3DModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData, S3D_FORMATS S3DStruct, DDU32 S3DVics)
{
    DDSTATUS Status;
    DDU8 VicCount;
    DDU8 VicId;
    DD_BOOL AddMode;
    CE_MODE_LIST CeVicData;

    DDASSERT(pHdmiVsdbModeArgs);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    // Go through all SVD list and update S3D caps wherever indicated
    for (VicCount = 0; VicCount < pEdidData->SvdListLength; VicCount++)
    {
        AddMode = FALSE;
        Status = GetVicIndexBasedOnCeIndex(pEdidData->pSVDList[VicCount], &VicId);
        if (IS_DDSTATUS_ERROR(Status))
        {
            // Invalid VIC ID or Timinginfo not found for input VicID
            continue;
        }

        if (((S3DVics) & (BIT0 << VicCount)) == FALSE)
        {
            continue;
        }

        CeVicData.S3DFormatMask = 0;
        // frame pack support
        // do not add low res. modes (<720p) for FP support : GMM restriction
        if ((S3DStruct.S3DStructureAll_0) && (FALSE == g_SupportedCeShortVideoModes[VicId].pTimingInfo->IsInterlaced) && (g_SupportedCeShortVideoModes[VicId].pTimingInfo->VActive >= 720))
        {
            CeVicData.S3DFormatMask |= DD_S3D_FORMAT_MASK(S3D_FRAME_PACKING);
            AddMode = TRUE;
        }
        // top bottom format support
        if (S3DStruct.S3DStructureAll_6)
        {
            CeVicData.S3DFormatMask |= DD_S3D_FORMAT_MASK(S3D_TOP_BOTTOM);
            AddMode = TRUE;
        }
        // SBS half format support
        if (S3DStruct.S3DStructureAll_8)
        {
            CeVicData.S3DFormatMask |= DD_S3D_FORMAT_MASK(S3D_SIDE_BY_SIDE_HALF_HOR_SUBSAMPLING); // We support only Hor Subsampling for S3D_SIDE_BY_SIDE_HALF
            AddMode = TRUE;
        }

        if (AddMode)
        {
            // Update the VIC that is present in the list
            CeVicData.IsPreferred = FALSE;
            CeVicData.SignalStandard = DD_CEA_861B;
            CeVicData.SamplingMode.Rgb = 1; // This will be preserved from previously added mode
            CeVicData.VicId[0] = VicId;
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
        }
    } // for loop for VicCount ends

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Parse and add mandatory S3D modes from VSDB block if 3D block is present.
 *
 * @param pHdmiVsdbModeArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddMandatoryS3DModes(HDMIVSDB_MODES_ARGS* pHdmiVsdbModeArgs, EDID_DATA* pEdidData)
{
    DDU8 Mandatory3DFormatList[3] = { 32, 04, 19 };
    DDU8 VicCount;
    DDU8 VicId;
    CE_MODE_LIST CeVicData;
    DD_BOOL IsVicPresentInList;

    // Arg validation
    DDASSERT(pHdmiVsdbModeArgs);
    DDASSERT(pEdidData);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    for (VicCount = 0; VicCount < sizeof(Mandatory3DFormatList); VicCount++)
    {
        // no need to convert VicId here as the array contains <64 CeIndex
        VicId = (Mandatory3DFormatList[VicCount] & CEA_SHORT_VIDEO_DESCRIPTOR_CODE_MASK);

        IsVicPresentInList = CeInfoParserIsVicPresentInList(pEdidData->VicCount, pEdidData->pCeModeList, Mandatory3DFormatList[VicCount]);
        if (IsVicPresentInList == FALSE)
        {
            // Vic not present
            continue;
        }

        if (g_SupportedCeShortVideoModes[VicId].pTimingInfo == NULL)
        {
            // Invalid VIC ID or Timinginfo not found for input VicID
            continue;
        }

        // Update the VIC that is present in the list
        CeVicData.IsPreferred = FALSE;
        CeVicData.SignalStandard = DD_CEA_861B;
        CeVicData.SamplingMode.Rgb = 1; // This will be preserved from previously added mode
        CeVicData.S3DFormatMask = DD_S3D_FORMAT_MASK(S3D_TOP_BOTTOM);

        if ((g_SupportedCeShortVideoModes[VicId].pTimingInfo->IsInterlaced == FALSE) && (g_SupportedCeShortVideoModes[VicId].pTimingInfo->VActive >= 720))
        {
            CeVicData.S3DFormatMask |= DD_S3D_FORMAT_MASK(S3D_FRAME_PACKING);
        }
        CeVicData.S3DFormatMask |= DD_S3D_FORMAT_MASK(S3D_TOP_BOTTOM);
        CeVicData.VicId[0] = VicId;
        CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
    } // for loop for VicCount ends

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Parse HfVsdb Frl Info.
 *
 *
 * @param MaxFrlRateIndex
 * @param pEdidData
 * @return void
 ***************************************************************/
static void CeParseHfVsdbFrlInfo(DDU8 MaxFrlRateIndex, EDID_DATA* pEdidData)
{
    // Arg validation
    DDASSERT(pEdidData);
    pEdidData->pData->HdmiCaps.HfVsdbInfo.IsFrlModeSupported = ((MaxFrlRateIndex != 0) && (MaxFrlRateIndex <= NUM_MAX_FRL_RATE_INDEX));
    //  Valid values of MaxFRLRateBWInGbps are from 0 to 6
    if (pEdidData->pData->HdmiCaps.HfVsdbInfo.IsFrlModeSupported)
    {
        // If FRL Mode Supported then its a HDMI 2.1 panel.
        pEdidData->pData->BasicDisplayCaps.HdmiDisplayVersion = HDMI_VERSION_2_1;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxFrlInMbps = DisplayInfoRoutinesGetHdmiLinkBwInMbps(MaxFrlRateIndex);
        // If Edid is giving the FrlRateIndex as Reserved returning the maximum FrlSupported.
        if (MaxFrlRateIndex >= NUM_FRL_LINK_RATES && MaxFrlRateIndex <= NUM_MAX_FRL_RATE_INDEX)
        {
            pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxFrlInMbps = HDMI_LINKRATE_12000_MBPS;
        }
        // If MaxFrlRateIndex is 0, this path itself won't be applicable. Since values 7-15 are reserved, Lane Count is N/A i.e. 0
        pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxFrlLaneCount = ((MaxFrlRateIndex <= 2) ? MAX_HDMI_LANES_TMDS_MODE : MAX_HDMI_LANES_FRL_MODE);
        // MaxFrlRateIndex is a value as index to maximum supported LaneRate
        // E.g., 6 GHz per Lane is 600 MHz Dotclock, represented by value of 2 & 3 with different lane counts
        pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz =
            DD_MAX(((DDU64)(pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxFrlInMbps) * MEGA_HERTZ), pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz);
    }
}

/***************************************************************
 * @brief Gets Max DscSlices and Max PerSlice PixelClock.
 *
 * @param DscMaxSlices
 * @param *pMaxDscSlices
 * @param *pDscMaxPixelClockPerSliceMHz
 * @return void
 ***************************************************************/
static void CeInfoParserGetMaxDscSlicesAndMaxPixelClockPerSlice(DDU8 DscMaxSlices, DDU8* pMaxDscSlices, DDU32* pDscMaxPixelClockPerSliceMHz)
{
    // Arg validation
    DDASSERT(pMaxDscSlices);
    DDASSERT(pDscMaxPixelClockPerSliceMHz);

    switch (DscMaxSlices)
    {
    case 1:
        *pMaxDscSlices = 1;
        *pDscMaxPixelClockPerSliceMHz = 340;
        break;
    case 2:
        *pMaxDscSlices = 2;
        *pDscMaxPixelClockPerSliceMHz = 340;
        break;
    case 3:
        *pMaxDscSlices = 4;
        *pDscMaxPixelClockPerSliceMHz = 340;
        break;
    case 4:
        *pMaxDscSlices = 8;
        *pDscMaxPixelClockPerSliceMHz = 340;
        break;
    case 5:
        *pMaxDscSlices = 8;
        *pDscMaxPixelClockPerSliceMHz = 400;
        break;
    case 6:
        *pMaxDscSlices = 12;
        *pDscMaxPixelClockPerSliceMHz = 400;
        break;
    case 7:
        *pMaxDscSlices = 16;
        *pDscMaxPixelClockPerSliceMHz = 400;
        break;
    default:
        *pMaxDscSlices = 0;
        *pDscMaxPixelClockPerSliceMHz = 0;
    }
}

/***************************************************************
 * @brief Ce Info Parser Parse HFVSDB.
 *
 *  parsing Below Block
 *       Vendor Specific Data Block (HF-VSDB) format
 *    +-----------------------------------------------------------+
 *    |Byte#|Bits5-7|       Bits 0-4                              |
 *    |-----------------------------------------------------------+
 *    |  1  | TAG   | length = total #of bytes following this byte|
 *    |-----------------------------------------------------------+
 *       |  2  |  IEEE OUI, Third Octet                           |
 *    |-----+-----------------------------------------------------|
 *    |  3  |  IEEE OUI, Second Octet                             |
 *    |-----+-----------------------------------------------------|
 *    |  4  |  IEEE OUI, First Octet                              |
 *    |-----------------------------------------------------------+
 *    | ... | Vendor Specific Data block Payload                  |
 *    +-----------------------------------------------------------+.
 * @param  pParseExtDataBlkArgs
 * @param  pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseHFVSDB(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    HDMI_HFVSDB* pHdmiHFVsdbBlock = NULL;
    DDU32 IEEERegNum = 0;
    DDU32 MaxTmdsCharRateCsc = 0;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DD_BOOL HfVsdbBlockFound, SkipCeDataBlockHdrLength9;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VENDOR_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VENDOR_DATABLOCK, 0xFF, CEA_HDMI2_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);

    HfVsdbBlockFound = TRUE; // Assume HF-VSDB Block found
    if (GetBlockById.pOutputBlock == NULL)
    {
        HfVsdbBlockFound = FALSE;
        // If HF-VSDB is not found, check for HF-SCDB as that has the same definition as this block
        CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_HF_SCDB_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
        CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
        if (GetBlockById.pOutputBlock == NULL)
        {
            // HDMI HF-VSDB/HF-SCDB Data block not found, not a critical error
            DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
            return DDS_CE_EXTN_BLOCK_NOT_FOUND;
        }
    }

    pHdmiHFVsdbBlock = (HDMI_HFVSDB*)(GetBlockById.pOutputBlock);

    if (HfVsdbBlockFound)
    {
        // Double check we got the right Block; Both HDMI 2.0 & 2.1 support same IEEE Reg ID
        IEEERegNum = DisplayInfoParserConvertHexArrayToInt(pHdmiHFVsdbBlock->IEEERegID, 3);

        if (IEEERegNum != CEA_HDMI2_IEEE_REG_ID)
        {
            // Not a HF-VSDB block
            DISP_FUNC_EXIT_W_STATUS(DDS_HDMI_VSDB_BLOCK_NOT_FOUND);
            return DDS_HDMI_VSDB_BLOCK_NOT_FOUND;
        }
    }

    if (pHdmiHFVsdbBlock->CeDataBlockHdr.Length < 7)
    {
        // Not a valid HF-VSDB/HF-SCDB block
        DISP_FUNC_EXIT_W_STATUS(DDS_HDMI_VSDB_BLOCK_NOT_FOUND);
        return DDS_HDMI_VSDB_BLOCK_NOT_FOUND;
    }

    SkipCeDataBlockHdrLength9 = FALSE;

    switch (pHdmiHFVsdbBlock->CeDataBlockHdr.Length)
    {
    case 13:
        pEdidData->pData->HdmiCaps.HfVsdbInfo.DscTotalChunkKBytes = pHdmiHFVsdbBlock->DscTotalChunkKBytes;
        // fall back without break to parse lower order bytes as well
    case 12:
        CeInfoParserGetMaxDscSlicesAndMaxPixelClockPerSlice(pHdmiHFVsdbBlock->DscMaxSlices, &pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxDscSlices,
            &pEdidData->pData->HdmiCaps.HfVsdbInfo.DscMaxPixelClockPerSliceMHz);
        pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxDscFrlInMbps = DisplayInfoRoutinesGetHdmiLinkBwInMbps(pHdmiHFVsdbBlock->DscMaxFrlRateIndex);
        pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxDscFrlLaneCount = ((pHdmiHFVsdbBlock->DscMaxFrlRateIndex <= 2) ? MAX_HDMI_LANES_TMDS_MODE : MAX_HDMI_LANES_FRL_MODE);
        // fall back without break to parse lower order bytes as well
    case 11:
        pEdidData->pData->HdmiCaps.HfVsdbInfo.CompressionBpc.DSC_8BPC_Supported = 1; // Should assume 8 bit always supported
        pEdidData->pData->HdmiCaps.HfVsdbInfo.CompressionBpc.DSC_10BPC_Supported = pHdmiHFVsdbBlock->Dsc10BpcSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.CompressionBpc.DSC_12BPC_Supported = pHdmiHFVsdbBlock->Dsc12BpcSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.CompressionBpc.DSC_16BPC_Supported = pHdmiHFVsdbBlock->Dsc16BpcSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsDscAllBppSupported = pHdmiHFVsdbBlock->DscAllBppSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsDscNative420Supported = pHdmiHFVsdbBlock->DscNative420Support;
        // When Sink supported MaxDscSlices/DscMaxPixelClockPerSliceMHz are zero, we can consider it as Compression is not supported (irrespective of Dsc1p2Support)
        if ((0 == pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxDscSlices) || (0 == pEdidData->pData->HdmiCaps.HfVsdbInfo.DscMaxPixelClockPerSliceMHz) ||
            (0 == pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxDscFrlInMbps))
        {
            pEdidData->pData->HdmiCaps.HfVsdbInfo.IsDsc1p2Supported = FALSE;
        }
        else
        {
            pEdidData->pData->HdmiCaps.HfVsdbInfo.IsDsc1p2Supported = pHdmiHFVsdbBlock->Dsc1p2Support;
        }
        // fall back without break to parse lower order bytes as well
    case 10:
        // Override/update VRR caps with HFVSDB Vrr Caps for HDMI
        pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = pHdmiHFVsdbBlock->VrrMin * KILO_HERTZ;
        pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = ((pHdmiHFVsdbBlock->VrrMaxHighBits << 8) | (pHdmiHFVsdbBlock->VrrMaxLowBits)) * KILO_HERTZ;
        SkipCeDataBlockHdrLength9 = TRUE;
        // fall back without break to parse lower order bytes as well
    case 9:
        // Case-9 is a special case. It is only required if 10th byte is not present.
        // Skip this case to avoid overwriting the values, if 10th byte is present.
        if (FALSE == SkipCeDataBlockHdrLength9)
        {
            // Override/update VRR caps with HFVSDB Vrr Caps for HDMI
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = pHdmiHFVsdbBlock->VrrMin * KILO_HERTZ;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = 0;
        }

        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsVrrInfoPresent = TRUE;
        // fall back without break to parse lower order bytes as well
    case 8:
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsFapaStartLocationAfter1stHBlank = pHdmiHFVsdbBlock->FAPA_Start_Location;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsAllmSupported = pHdmiHFVsdbBlock->AllmSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsFvaSupported = pHdmiHFVsdbBlock->FvaSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsNegativeMVrrSupported = pHdmiHFVsdbBlock->NegativeMVrrSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsQmsSupported = pHdmiHFVsdbBlock->QmsSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsQmsTfrMinSupported = pHdmiHFVsdbBlock->QmsTfrMinSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsQmsTfrMaxSupported = pHdmiHFVsdbBlock->QmsTfrMaxSupport;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsMDeltaSupported = pHdmiHFVsdbBlock->MDelta;
        // fall back without break to parse lower order bytes as well
    case 7:
        // Since HFVSDB found, this is a 2.0/2.1 HDMI panel
        pEdidData->pData->BasicDisplayCaps.HdmiDisplayVersion = HDMI_VERSION_2_0;

        // VSDB is a valid HF-VSDB
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsHfVsdbInfoValid = TRUE;

        pEdidData->pData->HdmiCaps.HfVsdbInfo.Version = pHdmiHFVsdbBlock->Version;
        MaxTmdsCharRateCsc = pHdmiHFVsdbBlock->MaxTMDSCharRateIn5Mhz * 5 * MEGA_HERTZ; // In Csc
        pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxTmdsCharRateCsc = MaxTmdsCharRateCsc;

        // Max_TMDS_Clock Rate indicated by Sink in H14VSDB Block, which governs operations between 25Mhz to 340Mhz.
        // As per HDMI 1.4 Spec, Sink should set this to a non-zero value if it supports TMDS Clock frequencues above 165Mhz. Max rate it can hold is upto 340Mhz.
        // As per HDMI 2.1a Spec, Sink is expected to set this field to non-zero if Max_TMDS_Character Rate in HFVSDB is 0. If Max_TMDS_Character Rate is 0, this field should indicate 340Mhz.
        // Source should not enumerate any modes that exceed the rate defined by Max_TMDS_Clock rate.
        // Compliance test HF1-25 with custom EDID is used by ATC to verify this requirement (HSD# 16022145984).
        // It is possible that some Sink's can indicate 340Mhz or less in H14-VSDB/HDMI VSDB, but indicate max 600Mhz in Monitor Mode Limits and expect Source
        // to enumerate all the modes upto 600Mhz. This would violate the Spec requirement and need to be pruned as per HDMI 2.1a spec.
        if (0 == pEdidData->pData->HdmiCaps.HfVsdbInfo.MaxTmdsCharRateCsc)
        {
            pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = DD_MAX(pEdidData->pData->HdmiCaps.HdmiVsdbMaxTmdsClockRate, MaxTmdsCharRateCsc);
        }
        else
        {
            pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = DD_MAX(MaxTmdsCharRateCsc, pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz);
        }

        CeParseHfVsdbFrlInfo(pHdmiHFVsdbBlock->MaxFrlRateIndex, pEdidData);

        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsScdcPresent = pHdmiHFVsdbBlock->IsSCDCPresent;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsScdcRrCapable = pHdmiHFVsdbBlock->IsSCDCRRCapable;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsLte_340Mcsc_Scramble = pHdmiHFVsdbBlock->IsLte340McscScramblingEnabled;
        pEdidData->pData->HdmiCaps.HfVsdbInfo.IsUhdVicSupported = pHdmiHFVsdbBlock->IsUhdVicSupported;
        break;
    default:
        // Futuristic HF-VSDB block, not supported yet
        break;
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief CeInfoParserParseHDMIVSDB.
 *
 * Function: CeInfoParserParseHdmiVsdb:
 * Description: method to get HDMI VSDB info
 *          parsing Below Block
 *       Vendor Specific Data Block (VSDB) format
 *    +-----------------------------------------------------------+
 *    |Byte#|Bits5-7|       Bits 0-4                              |
 *    |-----------------------------------------------------------+
 *    |  1  | TAG   | length = total #of bytes following this byte|
 *    |-----------------------------------------------------------+
 *    |  2  |                                                     |
 *    |-----+                                                     |
 *    |  3  | 24-bit IEEE Registration Identifier (LSB first)     |
 *    |-----+                                                     |
 *    |  4  |                                                     |
 *    |-----------------------------------------------------------+
 *    | ... | Vendor Specific Data block Payload                  |
 *    +-----------------------------------------------------------+.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @param pIsValidVicOffset
 * @param pVicOffset
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseHdmiVsdb(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData, DD_BOOL* pIsValidVicOffset, DDU8* pVicOffset)
{
    DDU8 VICOffset;
    HDMI_VSDB* pHdmiVsdbBlock = NULL;
    DDU32 IEEERegNum;
    HDMIVSDB_MODES_ARGS HdmiVsdbModeArgs;
    DDU32 MaxTmdsCharRateCsc;
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);
    DDASSERT(pIsValidVicOffset);
    DDASSERT(pVicOffset);

    *pIsValidVicOffset = FALSE; // Init

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VENDOR_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VENDOR_DATABLOCK, 0xFF, CEA_HDMI_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);

    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // HDR Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pHdmiVsdbBlock = (HDMI_VSDB*)(GetBlockById.pOutputBlock);

    IEEERegNum = DisplayInfoParserConvertHexArrayToInt(pHdmiVsdbBlock->IEEERegID, 3);

    // Double check we got the right Block
    if (IEEERegNum != CEA_HDMI_IEEE_REG_ID)
    {
        // Not a HDMI-VSDB block
        DISP_FUNC_EXIT_W_STATUS(DDS_HDMI_VSDB_BLOCK_NOT_FOUND);
        return DDS_HDMI_VSDB_BLOCK_NOT_FOUND;
    }

    // As per HDMI1.3/1.4 spec, any device with an HDMI VSDB block of any lenth containing the
    // IEEE Registration Identifier of 0x000C03 shall be treated as an HDMI device.
    pEdidData->pData->FtrSupport.IsHdmiVsdbValid = TRUE;
    pEdidData->pData->BasicDisplayCaps.HdmiDisplayVersion = HDMI_VERSION_1_4; // Since IEEE ID match, treat display as HDMI 1.4
    // It is possible that some of the sink may not set any of the other fields in VSDB and set size == 5.
    // As per HDMI 1.4b Specification, Sink should set correctly and non-zero value if it supports > 165Mhz TMDS Clock Frequency.
    // But, some of the sink's keep Max_TMDS_Clock_Rate field to default 0 or have field size set to minimum viz 5. Ex: BENQ XL2740 HDMI 2.0 Panel.
    // This leads to regression, in order to avoid that, set MaxTmdsClockRate as 340Mhz as default.
    pEdidData->pData->HdmiCaps.HdmiVsdbMaxTmdsClockRate = TMDS_CHAR_RATE_340MCSC; // Initialize.

    if (pHdmiVsdbBlock->CeDataBlockHdr.Length < 6)
    {
        // Nothing more to parse
        return DDS_SUCCESS;
    }

    if ((pHdmiVsdbBlock->CeDataBlockHdr.Length >= 7) && (pHdmiVsdbBlock->MaxTMDSClockIn5Mhz != 0))
    {
        MaxTmdsCharRateCsc = pHdmiVsdbBlock->MaxTMDSClockIn5Mhz * 5 * MEGA_HERTZ; // In Csc
        pEdidData->pData->HdmiCaps.HdmiVsdbMaxTmdsClockRate = MaxTmdsCharRateCsc;
        if (MaxTmdsCharRateCsc > pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz)
        {
            pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = MaxTmdsCharRateCsc;
        }
    }

    if (pHdmiVsdbBlock->CeDataBlockHdr.Length < 8)
    {
        // Nothing more to parse
        return DDS_SUCCESS;
    }

    if (pHdmiVsdbBlock->HdmiVideoPresent == FALSE)
    {
        // Nothing more to parse
        DISP_FUNC_EXIT_W_STATUS(DDS_HDMI_VSDB_BLOCK_NOT_FOUND);
        return DDS_HDMI_VSDB_BLOCK_NOT_FOUND;
    }

    pEdidData->pData->HdmiCaps.HdmiCeCaps.GraphicsContent = pHdmiVsdbBlock->ContentType0;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.PhotoContent = pHdmiVsdbBlock->ContentType1;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.CinemaContent = pHdmiVsdbBlock->ContentType2;
    pEdidData->pData->HdmiCaps.HdmiCeCaps.GameContent = pHdmiVsdbBlock->ContentType3;

    if (pHdmiVsdbBlock->LatencyFieldPresent)
    {
        if (pHdmiVsdbBlock->ILatencyFieldPresent)
        {
            VICOffset = DD_OFFSETOF(HDMI_VSDB, S3DVICCaps);
        }
        else
        {
            VICOffset = DD_OFFSETOF(HDMI_VSDB, InterlacedVideoLatency);
        }
    }
    else
    {
        VICOffset = DD_OFFSETOF(HDMI_VSDB, VideoLatency);
    }

    *pIsValidVicOffset = TRUE;
    *pVicOffset = VICOffset; // Return Vic Offset
    HdmiVsdbModeArgs.pHdmiVsdbBlock = pHdmiVsdbBlock;
    HdmiVsdbModeArgs.VICOffset = VICOffset;
    HdmiVsdbModeArgs.pModeTable = pParseExtDataBlkArgs->pModeTable;

    pEdidData->pData->FtrSupport.IsS3DLRFramesSupported = CeInfoParserIsS3DFrameSupported(&HdmiVsdbModeArgs, pEdidData);

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief  Add modes from HDMI VSDB block, see previos function.
 *          for block details.
 *
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserAddModesFromHdmiVsdb(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    HDMIVSDB_MODES_ARGS HdmiVsdbModeArgs;
    HDMI_VSDB* pHdmiVsdbBlock;
    DDU8 VICOffset;
    DD_BOOL IsValidVicOffset;

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VENDOR_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VENDOR_DATABLOCK, 0xFF, CEA_HDMI_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);

    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        // HDR Data block not found, not a critical error
        DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
        return DDS_CE_EXTN_BLOCK_NOT_FOUND;
    }

    pHdmiVsdbBlock = (HDMI_VSDB*)(GetBlockById.pOutputBlock);

    CeInfoParserParseHdmiVsdb(pParseExtDataBlkArgs, pEdidData, &IsValidVicOffset, &VICOffset);

    Status = DDS_SUCCESS; // Assume success for cases where valid VIC not found

    if (IsValidVicOffset)
    {
        HdmiVsdbModeArgs.pHdmiVsdbBlock = pHdmiVsdbBlock;
        HdmiVsdbModeArgs.VICOffset = VICOffset;
        HdmiVsdbModeArgs.pModeTable = pParseExtDataBlkArgs->pModeTable;

        CeInfoParserAddS3DModes(&HdmiVsdbModeArgs, pEdidData);

        Status = CeInfoParserAdd4k2kModes(&HdmiVsdbModeArgs, pEdidData);
    }

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Ce Info Parser Parse Timing Types from DisplayID.
 *
 *  Base function to Parse defined in DisplayID V2 timing types (type 7, type 8, type 10).
 *  This will inturn call various subfunctions to parse various type of timings.
 *      | Byte # |    Bits[7-0]                                              |
 *      |--------------------------------------------------------------------|
 *      | 0      |Tag Code(07h)  | Length of data block(in bytes)(bit 4 - 0) |
 *      |--------------------------------------------------------------------|
 *      | 1      |       Extended Tag Code (22h/24h/2Ah)                     |
 *      |--------------------------------------------------------------------|
 *      | 2      | From this byte onwards, to be parsed as per timing types  |
 *      |--------------------------------------------------------------------|.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseDisplayIDTimings(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    CEA_EXT_FORMULA_TIMING_X* pFormula10;
    CEA_EXT_FORMULA_TIMING_VIII* pFormula8;
    CEA_EXT_FORMULA_TIMING_VII* pFormula7;
    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_TYPE_VII_VIDEO_TIMING_DATA_BLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);

    if (GetBlockById.pOutputBlock)
    {
        pFormula7 = (CEA_EXT_FORMULA_TIMING_VII*)GetBlockById.pOutputBlock;
        DisplayInfoParserParseCtaType7Timing(pFormula7, pEdidData);
    }

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_TYPE_VIII_VIDEO_TIMING_DATA_BLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);

    if (GetBlockById.pOutputBlock)
    {
        pFormula8 = (CEA_EXT_FORMULA_TIMING_VIII*)GetBlockById.pOutputBlock;
        DisplayInfoParserParseCtaType8Timing(pFormula8, pEdidData);
    }

    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_TYPE_X_VIDEO_TIMING_DATA_BLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);

    if (GetBlockById.pOutputBlock)
    {
        pFormula10 = (CEA_EXT_FORMULA_TIMING_X*)GetBlockById.pOutputBlock;
        DisplayInfoParserParseTypeXTiming(NULL, pFormula10, pParseExtDataBlkArgs->pModeTable, pEdidData, TRUE);
    }

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Ce Info Parser Parse Ce Extension For Modes.
 *
 *  Base function to Parse CE Extension block.
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *      Note: Ensure that the order of these calls doesn't change
 *              as the 1st 2 calls is where we get the VicList parsed and populated
 *          1. CeInfoParserAddModesFromVideoDataBlock to add SVD modes
 *          2. CeInfoParserAddModesFrom420VideoDataBlock to add 420 modes
 *          3. CeInfoParserParse420CapabilityMapDataBlock to update 420 modes
 *          4. CeInfoParserParseVicsFromVfpdb to update preferred Vics
 *          5. Parse and add DTDs if any
 *          6. Parse HDMI VSDB Block to add S3D and 4k2k Modes.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS CeInfoParserParseCeExtensionForModes(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DDSTATUS Status;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    Status = CeInfoParserAddModesFromVideoDataBlock(pParseExtDataBlkArgs, pEdidData);
    if (IS_DDSTATUS_ERROR(Status))
        return Status;

    Status = CeInfoParserAddModesFrom420VideoDataBlock(pParseExtDataBlkArgs, pEdidData);
    if (IS_DDSTATUS_ERROR(Status))
        return Status;

    Status = CeInfoParserParse420CapabilityMapDataBlock(pParseExtDataBlkArgs, pEdidData);
    if (IS_DDSTATUS_ERROR(Status))
        return Status;

    Status = CeInfoParserParseVicsFromVfpdb(pParseExtDataBlkArgs, pEdidData);
    if (IS_DDSTATUS_ERROR(Status))
        return Status;

    // Now parse the VSDB block for 3D caps and add 3d modes to mode list
    Status = CeInfoParserAddModesFromHdmiVsdb(pParseExtDataBlkArgs, pEdidData);
    if (IS_DDSTATUS_ERROR(Status))
        return Status;

    Status = CeInfoParserParseDisplayIDTimings(pParseExtDataBlkArgs, pEdidData);
    if (IS_DDSTATUS_ERROR(Status))
        return Status;

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Ce Info Parser Parse Ce Extension For Caps.
 *
 * Base function to Parse CE Extension block to gather all caps.
 * This will inturn call various subfunctions below which inturn will add modes to the mode-table
 * Note: Ensure that the order of these calls doesn't change
 * as the 1st 2 calls is where we get the VicList parsed and populated
 * 1. CeInfoParserIs420VideoDataBlockPresent to update 420 block presense bit
 * 2. CeInfoParserIs420CapabilityBlockPresent to update 420 present bit
 * 3. CeInfoParserParseHdrCapDataBlock to parse HDR block
 * 4. CeInfoParserParseAudioDataBlock to update Audio caps
 * 5. CeInfoParserParseSpeakerAllocationDataBlock
 * 6. CeInfoParserParseColorimetryDataBlock to update CE colorimetry Type and profile
 * 7. CeInfoParserParseVideoCapabilityDataBlock to get Video capability block details
 * 8. Parse HDMI VSDB Block to add S3D and 4k2k Modes
 * 9. CeInfoParserParseHFVSDB to update HDMI caps.
 * 10. CeInfoParserParseVfpdbList to mark the DTDs in VFPDB block in list
 * 11. CeInfoParserParseHdr10PlusVsVdb to parse HDR10+ luminance from VSVDB
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return void
 ***************************************************************/
void CeInfoParserParseCeExtensionForCaps(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    DD_BOOL IsValidVicOffset;
    DDU8 VicOffset;

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    CeInfoParserGetSvdListFromVideoDataBlock(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserIs420VideoDataBlockPresent(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserIs420CapabilityBlockPresent(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseHdrCapDataBlock(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseAudioDataBlock(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseSpeakerAllocationDataBlock(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseColorimetryDataBlock(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseVideoCapabilityDataBlock(pParseExtDataBlkArgs, pEdidData);

    // Now parse the HDMI14b VSDB block for 3D caps and add 3d modes to mode list
    CeInfoParserParseHdmiVsdb(pParseExtDataBlkArgs, pEdidData, &IsValidVicOffset, &VicOffset);

    // Now parse the HF-VSDB block if present and update the parameters
    CeInfoParserParseHFVSDB(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseVfpdbList(pParseExtDataBlkArgs, pEdidData);

    CeInfoParserParseHdr10PlusVsVdb(pParseExtDataBlkArgs, pEdidData);

    return;
}

/***************************************************************
 * @brief Edid Parser Parse Ce Extension.
 *
 *  Base function to Parse CE Extension block present as part of EDID extension.
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *      Note: Ensure that the order of these calls doesn't change
 *              as the 1st 2 calls is where we get the VicList parsed and populated
 *          1. EdidParserAddModesFromVideoDataBlock to add SVD modes
 *          2. EdidParserAddModesFrom420VideoDataBlock to add 420 modes
 *          3. EdidParserParseCapabilityBlock to update 420 modes
 *          4. EdidParserParseVideoFormatPreferenceBlock to update preferred mode
 *          5. EdidParserParseHdrCapDataBlock to parse HDR block
 *          6. EdidParserParseAudioDataBlock to update Audio caps
 *          7. Parse and add DTDs if any
 *          8. Parse HDMI VSDB Block to add S3D and 4k2k Modes
 *          9. EdidParserParseHFVSDB to update HDMI caps.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void EdidParserParseCeExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    CEA_EXT_CAPS CEAExtCaps;
    EDID_BASE_BLOCK* pEdidBaseBlock = NULL;
    CE_EDID_EXTN_BLOCK* pCeEdidExtn = NULL;
    DDU32 BlockNum;
    DDU32 NumExtBlocks;
    PARSE_EXTBLK_ARGS ParseExtDataBlkArgs;

    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pEdidBaseBlock = (EDID_BASE_BLOCK*)pGetEdidModes->pEdidOrDisplayIDBuf;
    NumExtBlocks = pGetEdidModes->NumEdidExtensionsParsed;

    for (BlockNum = 1; (BlockNum <= NumExtBlocks) && (BlockNum < MAX_EDID_EXTENSIONS_SUPPORTED); BlockNum++)
    {
        if (pGetEdidModes->BufSizeInBytes < (BlockNum * EDID_BLOCK_SIZE))
        {
            // Wrong length for buffer passed in by caller. Hence failing
            break;
        }

        pCeEdidExtn = (CE_EDID_EXTN_BLOCK*)((DDU8*)pEdidBaseBlock + (BlockNum * EDID_BLOCK_SIZE));

        if (FALSE == DisplayInfoParserIsCeExtnDisplay(pCeEdidExtn))
        {
            continue;
        }

        pEdidData->pData->FtrSupport.IsCeExtnDisplay = TRUE;

        // Get the Offset and check whether we have DTD's and Data Blocks
        // If Offset = 0, then we dont have DTD's and Data Blocks
        if (pCeEdidExtn->DTDOffset == 0)
        {
            continue;
        }

        // Unused today, so keeping commented until needed/ SupportsBasicAudio is saved to caps elsewhere
        // pEdidData->pData->CeaExtnCaps.SupportsBasicAudio  = pCeEdidExtn->Capabilty.SupportsBasicAudio;
        // pEdidData->pData->CeExtnCaps.CeaExtnCaps.TotalNativeDTDs     = pCeEdidExtn->Capabilty.TotalNativeDTDs;

        pEdidData->pData->HdmiCaps.HdmiCeCaps.UnderscansITFormats = pCeEdidExtn->Capabilty.UnderscansITFormats;
        pEdidData->pData->FtrSupport.YCbCr422Supported = pCeEdidExtn->Capabilty.SupportsYCBCR422;
        pEdidData->pData->FtrSupport.YCbCr444Supported = pCeEdidExtn->Capabilty.SupportsYCBCR444;

        // Process Data Block if exists:
        if (pGetEdidModes->pModeTable != NULL)
        {
            // Process DTDs , if any
            // Note: This is done before CE-SVD block parsing as SVD block modes are of higher priority
            CeInfoParserAddModesFromCeExtnDtdBlock(pCeEdidExtn, pGetEdidModes->pModeTable, pEdidData);
        }

        // Get Short Video Descriptors from Data Blocks, If any.
        // Data Blocks exist only if  DTD Offset > 4
        if (pCeEdidExtn->DTDOffset <= CEA_EDID_HEADER_SIZE)
        {
            continue;
        }

        // No SVD blocks if CE version < 861-B
        if (pCeEdidExtn->Revision < CEA_861_B_VERSION)
        {
            continue;
        }

        // Parse caps always as we need HF-VSDB caps for enumerating modes with FVA for HDMI 2.1
        // We need to calc. Max FVA factor possible per mode and add it to timing info
        // Since CE block is parsed 1st, we can use the caps data while adding modes later
        ParseExtDataBlkArgs.pDataBlock = pCeEdidExtn->CeBlockData;
        ParseExtDataBlkArgs.LenDataBlock = pCeEdidExtn->DTDOffset - CEA_EDID_HEADER_SIZE;
        ParseExtDataBlkArgs.pModeTable = pGetEdidModes->pModeTable;

        CeInfoParserGetSupportedBPCFromCeExtBlock(&ParseExtDataBlkArgs, pEdidData);

        CeInfoParserParseCeExtensionForCaps(&ParseExtDataBlkArgs, pEdidData);

        ParseExtDataBlkArgs.pDataBlock = pCeEdidExtn->CeBlockData; // Restore pointer to prevent tainted_scalar issue

        if (pGetEdidModes->pModeTable != NULL)
        {
            CeInfoParserParseCeExtensionForModes(&ParseExtDataBlkArgs, pEdidData);
        }

        if (pEdidData->pData->BasicDisplayCaps.HdmiDisplayVersion >= HDMI_VERSION_1_4)
        {
            CEAExtCaps.Value = pCeEdidExtn->Capabilty.Value;

            pEdidData->pData->FtrSupport.IsAudioSupported = CEAExtCaps.SupportsBasicAudio;
        }
        // Breakout if this call was for just to get caps (ensure further from here, there are no caps that are parsed)
        if (NULL == pGetEdidModes->pModeTable)
            return;

    } // for loop for Block Num ends

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Wrapper method to get supported BPC from CE extension block.
 *
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return void
 ***************************************************************/
void CeInfoParserGetSupportedBPCFromCeExtBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    CeInfoParserGetSupportedBPCFromCeHdmiVSDBBlock(pParseExtDataBlkArgs, pEdidData);
    CeInfoParserGetSupportedBPCFromCeHFVSDBBlock(pParseExtDataBlkArgs, pEdidData);
    return;
}

/***************************************************************
 * @brief Ce Info Parser Get Supported BPC From Ce Hdmi VSDB Block.
 *
 *  Method to get supported BPC from CE extension HDMI-VSDB block
 *      This block will indicate the BPC supported for RGB modes.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return void
 ***************************************************************/
void CeInfoParserGetSupportedBPCFromCeHdmiVSDBBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    HDMI_VSDB* pHdmiVsdbBlock = NULL;
    DDU32 IEEERegNum;
    GET_BLOCK_BY_ID_ARGS GetBlockById;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VENDOR_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VENDOR_DATABLOCK, 0xFF, CEA_HDMI_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);

    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
    if (GetBlockById.pOutputBlock == NULL)
    {
        return;
    }

    pHdmiVsdbBlock = (HDMI_VSDB*)(GetBlockById.pOutputBlock);

    IEEERegNum = DisplayInfoParserConvertHexArrayToInt(pHdmiVsdbBlock->IEEERegID, 3);

    // Double check we got the right Block
    if (IEEERegNum != CEA_HDMI_IEEE_REG_ID)
    {
        // Not a HDMI-VSDB block
        return;
    }

    if (pHdmiVsdbBlock->HdmiVideoPresent == FALSE)
    {
        // Nothing more to parse
        return;
    }

    if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask == 0)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports8BitsPerColor = TRUE;
    }

    if (pHdmiVsdbBlock->CeDataBlockHdr.Length < 6)
    {
        // Nothing more to parse
        return;
    }

    if (pHdmiVsdbBlock->DC10BitSupport)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports10BitsPerColor = TRUE;
    }
    if (pHdmiVsdbBlock->DC12BitSupport)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports12BitsPerColor = TRUE;
    }
    if (pHdmiVsdbBlock->DC16BitSupport)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports16BitsPerColor = TRUE;
    }

    return;
}

/***************************************************************
 * @brief Ce Info Parser Get Supported BPC From Ce HFVSDB Block.
 *
 *  Method to get supported BPC from CE extension HF-VSDB block
 *      This block will indicate the BPC supported for YUV 420 modes.
 * @param pParseExtDataBlkArgs
 * @param pEdidData
 * @return void
 ***************************************************************/
void CeInfoParserGetSupportedBPCFromCeHFVSDBBlock(PARSE_EXTBLK_ARGS* pParseExtDataBlkArgs, EDID_DATA* pEdidData)
{
    HDMI_HFVSDB* pHdmiHFVsdbBlock = NULL;
    DDU32 IEEERegNum = 0;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU32 MaxTmdsCharRateCsc = 0;
    DD_BOOL HfVsdbBlockFound;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pParseExtDataBlkArgs);
    DDASSERT(pParseExtDataBlkArgs->pDataBlock);
    DDASSERT(pEdidData);

    // CeaExtendedTag is ignored as BlockIdOrTag is CEA_VENDOR_DATABLOCK
    CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_VENDOR_DATABLOCK, 0xFF, CEA_HDMI2_IEEE_REG_ID, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
    CeInfoParserGetCeaExtnBlockByID(&GetBlockById);

    HfVsdbBlockFound = TRUE; // Assume HF-VSDB Block found
    if (GetBlockById.pOutputBlock == NULL)
    {
        HfVsdbBlockFound = FALSE;
        // If HF-VSDB is not found, check for HF-SCDB as that has the same definition as this block
        CeInfoParserFillGetBlockByIdArgs(&GetBlockById, CEA_USE_EXTENDED_TAG, CEA_HF_SCDB_DATABLOCK, 0, 0, pParseExtDataBlkArgs->pDataBlock, pParseExtDataBlkArgs->LenDataBlock);
        CeInfoParserGetCeaExtnBlockByID(&GetBlockById);
        if (GetBlockById.pOutputBlock == NULL)
        {
            // HDMI HF-VSDB/HF-SCDB Data block not found, not a critical error
            DISP_FUNC_EXIT_W_STATUS(DDS_CE_EXTN_BLOCK_NOT_FOUND);
            return;
        }
    }

    pHdmiHFVsdbBlock = (HDMI_HFVSDB*)(GetBlockById.pOutputBlock);

    if (HfVsdbBlockFound)
    {
        // Double check we got the right Block; Both HDMI 2.0 & 2.1 support same IEEE Reg ID
        IEEERegNum = DisplayInfoParserConvertHexArrayToInt(pHdmiHFVsdbBlock->IEEERegID, 3);

        if (IEEERegNum != CEA_HDMI2_IEEE_REG_ID)
        {
            // Not a HF-VSDB block
            DISP_FUNC_EXIT_W_STATUS(DDS_HDMI_VSDB_BLOCK_NOT_FOUND);
            return;
        }
    }

    if (pHdmiHFVsdbBlock->CeDataBlockHdr.Length < 7)
    {
        // Not a valid HF-VSDB/HF-SCDB block
        DISP_FUNC_EXIT_W_STATUS(DDS_HDMI_VSDB_BLOCK_NOT_FOUND);
        return;
    }

    pEdidData->pData->HdmiCaps.HfVsdbInfo.IsHfVsdbInfoValid = TRUE;

    if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.ColorDepthMask == 0)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports8BitsPerColor = TRUE;
    }

    if (pHdmiHFVsdbBlock->DC10BitSupport)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports10BitsPerColor = TRUE;
    }
    if (pHdmiHFVsdbBlock->DC12BitSupport)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports12BitsPerColor = TRUE;
    }
    if (pHdmiHFVsdbBlock->DC16BitSupport)
    {
        pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports16BitsPerColor = TRUE;
    }

    MaxTmdsCharRateCsc = pHdmiHFVsdbBlock->MaxTMDSCharRateIn5Mhz * 5 * MEGA_HERTZ; // In Csc
    pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = DD_MAX(MaxTmdsCharRateCsc, pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz);
    CeParseHfVsdbFrlInfo(pHdmiHFVsdbBlock->MaxFrlRateIndex, pEdidData);

    return;
}

/***************************************************************
 * @brief Get EDID Extension Count from EDID Extension Override Block (EEODB).
 *
 * Description:
 * 1. HDMI Forum EDID Extension Override Data Block (HF-EEODB) is utilized by Sink Devices to provide an alternate
 *    method to indicate an EDID Extension Block count larger than 1.
 * 2. Sinks that present more than one E-EDID Extension Block should present a CTA Extension including the HF-EEODB to
 *    indicate the total number of E-EDID Extension Blocks present in the E-EDID.
 * 3. Sinks that include the HF-EEODB in their E-EDID shall include one and only one instance of the HF-EEODB in the E-EDID,
 *    occupying bytes 4 through 6 of Block 1 of the E-EDID.
 * 4. Sinks that include the HF-EEODB in their E-EDID shall set byte 126 of Block 0 (the Extension Flag) to 1.
 * 5. Sources shall ignore the Extension Flag if greater than 1 when an HF-EEODB is present.
 *
 * @param pEdidExtnBlock
 * @return DDU8
 ***************************************************************/
static DDU8 CeInfoParserGetExtensionCountFromHdmiEeodb(DDU8* pEdidExtnBlock)
{
    HDMI_HFEEODB* pHdmiEeodbBlock;

    pHdmiEeodbBlock = (HDMI_HFEEODB*)(((CE_EDID_EXTN_BLOCK*)pEdidExtnBlock)->CeBlockData);

    if (pHdmiEeodbBlock->ExtendedTagCode != CEA_HF_EEODB_DATABLOCK)
    {
        return INVALID_EDID_BLOCK_COUNT;
    }

    // Length of EEODB is currently fixed to 2 Bytes.
    if (pHdmiEeodbBlock->CeDataBlockHdr.Length != HF_EEODB_BLOCK_SIZE)
    {
        // Nothing more to parse
        return INVALID_EDID_BLOCK_COUNT;
    }

    return pHdmiEeodbBlock->EdidExtensionBlockCount;
}

/*
 *
 * CE Info parser related functions -- END
 *
 ***************************************************************/

 /*
  *
  * Display ID parser related functions -- START
  *
  ***************************************************************/

  /***************************************************************
   * @brief Method to return pointer corresponding to input Display ID Block.
   *
   * @param pGetBlockById
   * @return void
   ***************************************************************/
void DisplayIdParserFillGetBlockByIdArgs(GET_BLOCK_BY_ID_ARGS* pGetBlockById, DDU8* pEdidOrDisplayIDBuf, DDU8 BlockIdOrTag, DDU8 InstanceOfBlock)
{
    DDU32 InputBlockLength = 0;
    DDU8* pInputBuf = NULL;
    if (pEdidOrDisplayIDBuf)
    {
        pInputBuf = (DDU8*)(pEdidOrDisplayIDBuf + sizeof(DISPLAYID_HEADER));    // 1st Display ID block
        InputBlockLength = ((DISPLAYID_HEADER*)(pEdidOrDisplayIDBuf))->BytesInSection; // Exclude the last checksum byte

        pGetBlockById->DisplayIdVerAndRev = ((DISPLAYID_HEADER*)(pEdidOrDisplayIDBuf))->RevisionAndVersion;
        pGetBlockById->BlockIdOrTag = BlockIdOrTag;
        pGetBlockById->CeaExtendedTag = 0; // Valid only for CE extension block search and when BlockIdOrTag == CEA_USE_EXTENDED_TAG
        pGetBlockById->IEEERegNum = 0; // Valid only for CE extension block search and when BlockIdOrTag == CEA_VENDOR_DATABLOCK
        pGetBlockById->InstanceOfBlock = InstanceOfBlock;
        pGetBlockById->pInputBuf = pInputBuf;
        pGetBlockById->InputBlockLength = InputBlockLength;
        pGetBlockById->pOutputBlock = NULL;
    }
}

/***************************************************************
 * @brief Display Id Parser Verify Data Blocks.
 *
 *  This function verifies if incoming EDID block data is valid by verifying
 *       1. Buffer size passed.HDmi
 *       2. Checksum for each EDID/CE extension block.
 * @param pEdidOrDisplayIDBuf
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayIdParserVerifyDataBlocks(DDU8* pEdidOrDisplayIDBuf)
{
    DISPLAYID_HEADER DidExtnBlockHeader;

    if (((DISPLAYID_HEADER*)(pEdidOrDisplayIDBuf))->BytesInSection > MAX_SECTION_DATA_SIZE)
    {
        // Excluding header + Checksum, Total data bytes cannot exceed 251 bytes
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_DATA);
        return DDS_INVALID_DATA;
    }

    DidExtnBlockHeader.Value = ((DISPLAYID_HEADER*)(pEdidOrDisplayIDBuf))->Value;

    if (pEdidOrDisplayIDBuf[DidExtnBlockHeader.BytesInSection + sizeof(DISPLAYID_HEADER)] !=
        DisplayInfoRoutinesCalcChksum(pEdidOrDisplayIDBuf, DidExtnBlockHeader.BytesInSection + sizeof(DISPLAYID_HEADER) + 1))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_DATA);
        return DDS_INVALID_DATA;
    }

    // There is no checksum error, so return success
    return DDS_SUCCESS;
}

/***************************************************************
 * @brief  Method to return pointer corresponding to input Display ID Block.
 *
 * @param pGetBlockById
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayIdParserGetDisplayIDBlockByID(GET_BLOCK_BY_ID_ARGS* pGetBlockById)
{
    DDSTATUS Status;
    DISPLAYID_BLOCK* pDidBlockHeader = NULL;
    DDU32 DispIdBlockOffset;
    DDU8 InstanceOfBlock;
    DDU8 MaxValidBlockTag, CeaBlockTag;

    // Arg validation
    DDASSERT(pGetBlockById);
    DDASSERT(pGetBlockById->pInputBuf);

    switch (pGetBlockById->DisplayIdVerAndRev)
    {
    case DISPLAY_ID_VER_2_1:
    case DISPLAY_ID_VER_2_0:
        CeaBlockTag = DID2_DATA_BLOCK_CTA_DISPLAY_ID;
        MaxValidBlockTag = DID2_DATA_BLOCK_VENDOR_SPECIFIC;
        break;
    default:
        CeaBlockTag = DID_DATA_BLOCK_CEA_SPECIFIC;
        MaxValidBlockTag = DID_DATA_BLOCK_DETAILED_DESCRIPTOR_TIMING_6;
        break;
    }

    if ((pGetBlockById->BlockIdOrTag > MaxValidBlockTag) && (pGetBlockById->BlockIdOrTag != CeaBlockTag))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_INVALID_PARAM);
        return DDS_INVALID_PARAM;
    }

    // pGetBlockById->pInputBuf points to 1st Block of display ID

    DispIdBlockOffset = 0;
    InstanceOfBlock = 0;
    Status = DDS_UNSUCCESSFUL; // Assume block not found

    // Make sure that there are at least 3 bytes (for a possible DisplayID Data block header) remaining, so that we have valid data block.
    while (DispIdBlockOffset < (pGetBlockById->InputBlockLength - sizeof(DISPLAYID_BLOCK)))
    {
        pDidBlockHeader = (DISPLAYID_BLOCK*)(pGetBlockById->pInputBuf + DispIdBlockOffset);

        if ((pDidBlockHeader->BlockID == 0) && (pDidBlockHeader->RevAndOtherData == 0) && (pDidBlockHeader->NumberOfBytes == 0))
        {
            // This is the case of Padding when DisplayID is used as EDID extension, so no need to parse further
            break;
        }

        // Is this the Display ID data block we are looking for?
        if (pDidBlockHeader->BlockID == pGetBlockById->BlockIdOrTag)
        {
            if (pGetBlockById->InstanceOfBlock == InstanceOfBlock)
            {
                pGetBlockById->pOutputBlock = (DDU8*)(pDidBlockHeader);
                Status = DDS_SUCCESS;
                break;
            }

            // Caller asking to check if there is a 2nd/3rd... instance of the same block
            InstanceOfBlock++;
        }

        DispIdBlockOffset += (pDidBlockHeader->NumberOfBytes + sizeof(DISPLAYID_BLOCK));
    };
    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Display Id Parser Get Edid Modes.
 *
 *  Method to Get all DisplaY modes present
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *
 *      Note: Below calls has be in order of increasing priority.
 *            Lower priority modes are added 1st and higher priority modes are added later
 *
 *          1. DisplayIdGetSupportedBPC to get the Supported BPC.
 *          2. DisplayIdParserParseCeExtension to parse CE extension block within display ID
 *          3. DisplayIdParserParseCeaTiming to parse CE Timing block within Display ID
 *          4. DisplayIdParserParseType1Timing to get Type1 timings
 *          5. DisplayIdParserParseType2Timing to get Type2 timings
 *          6. DisplayIdParserParseType3Timing to get Type3 timings
 *          7. DisplayIdParserParseType4Timing to get Type4 timings
 *          8. DisplayIdParserParseStandardTiming to parse Standard timing block extension.
 * @param   pGetEdidModes
 * @param   pEdidData
 * @return  void
 ***************************************************************/
void DisplayIdParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU8* pOriginalEdidBuf;
    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pOriginalEdidBuf = pGetEdidModes->pEdidOrDisplayIDBuf; // Save original pointer
    // 1. Get the supported BPC. This is required for filling the Timing information.
    DisplayIdGetSupportedBPC(pGetEdidModes, pEdidData);

    // Get Tiled info caps, which will be used to add modes in later part of this function
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseTiledTopologyBlock(pGetEdidModes, pEdidData, DID_DATA_BLOCK_TILED_TOPOLOGY);

    // 2. Parse CE extenstion blocks that might be present within this Display ID Block
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseCeExtension(pGetEdidModes, pEdidData, DID_DATA_BLOCK_CEA_SPECIFIC);

    // 3. Parse CE Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseCeaTiming(pGetEdidModes, pEdidData);

    // 4. Parse Type I Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf;     // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseType1Timing(pGetEdidModes, pEdidData); //, DID_DATA_BLOCK_DETAILED_TIMING_1);

    // 5. Parse Type II Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseType2Timing(pGetEdidModes, pEdidData);

    // 6. Parse Type III Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseType3Timing(pGetEdidModes, pEdidData);

    // 7. Parse Type IV Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseType4Timing(pGetEdidModes, pEdidData);

    // 8. Parse Standard Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseStandardTiming(pGetEdidModes, pEdidData);

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Display Id Parser Get Edid Caps.
 *
 *  Method to Get all Display caps from Display ID block
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU8* pOriginalEdidBuf;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pOriginalEdidBuf = pGetEdidModes->pEdidOrDisplayIDBuf; // Save original pointer

    // Get the supported BPC. This is required for filling the timing info structure.
    DisplayIdGetSupportedBPC(pGetEdidModes, pEdidData);

    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserGetMonitorDetails(pGetEdidModes, pEdidData);

    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseTiledTopologyBlock(pGetEdidModes, pEdidData, DID_DATA_BLOCK_TILED_TOPOLOGY);

    // Parse CE Timing section, if found we set caps for CEExtn display
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseCeaTiming(pGetEdidModes, pEdidData);

    // Parse CE extenstion blocks that might be present within this Display ID Block
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseCeExtension(pGetEdidModes, pEdidData, DID_DATA_BLOCK_CEA_SPECIFIC);

    return;
}

/***************************************************************
 * @brief Display Id Parser Get Monitor Details.
 *
 *  Method to parse Monitor details from various Display ID blocks:
 *      1. Monitor Range limits
 *      2. Monitor Name
 *      3. Min/max RR and dotclock supported
 *      4. Monitor supported Gamma
 *      5. Monitor support for CVT/GTF modes
 *      6. Chromaticity info
 *      7. Monitor Interface supported - DP/HDMI/DVI.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserGetMonitorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU32 NumPrimaryWhitePoints;
    DDU16 ChromaX, ChromaY;
    DISPLAYID_DISPLAY_PARAMETERS* pDidDisplayParams = NULL;
    DISPLAYID_COLOR_CHARS* pColorCharInfo = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Note: Not checking for multiple instances of any of below blocks as they are assumed to be occurring only once.
    // If multiple instances of below blocks occur, only the first one will be parsed
    DisplayIdParserGetMonitorDescriptorDetails(pGetEdidModes, pEdidData);

    //   The 0x78 translates to a default gamma value of 2.2 (0x78 + 100 / 100)
    //   supported by the majority of LFP panels.
    pEdidData->pData->BasicDisplayCaps.DisplayGamma = 0x78;

    // Not checking for multiple instances of CE block yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_DISPLAY_PARAMS, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDidDisplayParams = (DISPLAYID_DISPLAY_PARAMETERS*)(GetBlockById.pOutputBlock);
        if (pDidDisplayParams->TransferChar != 0xFF)
        {
            pEdidData->pData->BasicDisplayCaps.DisplayGamma = pDidDisplayParams->TransferChar;
        }
    }
    // Keeping this as base data and convert EDID data to this
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_COLOR_CHARACTERISTICS, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pColorCharInfo = (DISPLAYID_COLOR_CHARS*)(GetBlockById.pOutputBlock);

        NumPrimaryWhitePoints = pColorCharInfo->ColorCharsInfo.NumOfPrimaries + pColorCharInfo->ColorCharsInfo.NumOfWhitePoints;
        if ((NumPrimaryWhitePoints == 0) || (NumPrimaryWhitePoints % DID_NUM_PRIMARY_WHITE_POINTS != 0))
        {
            // Nothing to be parsed further, improper DID data
            return;
        }

        // Conversion from 12 bits of Display ID Chromaticity to EDID 10 bits if needed has to be done by caller
        // We are just storing the data structure here as it is

        ChromaX = pColorCharInfo->ChromaData[0].ColorxOruLowBits | (pColorCharInfo->ChromaData[0].ColorxOruHighBits << 8);
        ChromaY = pColorCharInfo->ChromaData[0].ColoryOrvLowBits | (pColorCharInfo->ChromaData[0].ColoryOrvHighBits << 4);
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.RedX = ChromaX;
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.RedY = ChromaY;

        ChromaX = pColorCharInfo->ChromaData[1].ColorxOruLowBits | (pColorCharInfo->ChromaData[1].ColorxOruHighBits << 8);
        ChromaY = pColorCharInfo->ChromaData[1].ColoryOrvLowBits | (pColorCharInfo->ChromaData[1].ColoryOrvHighBits << 4);
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.GreenX = ChromaX;
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.GreenY = ChromaY;

        ChromaX = pColorCharInfo->ChromaData[2].ColorxOruLowBits | (pColorCharInfo->ChromaData[2].ColorxOruHighBits << 8);
        ChromaY = pColorCharInfo->ChromaData[2].ColoryOrvLowBits | (pColorCharInfo->ChromaData[2].ColoryOrvHighBits << 4);
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.BlueX = ChromaX;
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.BlueY = ChromaY;

        ChromaX = pColorCharInfo->ChromaData[3].ColorxOruLowBits | (pColorCharInfo->ChromaData[3].ColorxOruHighBits << 8);
        ChromaY = pColorCharInfo->ChromaData[3].ColoryOrvLowBits | (pColorCharInfo->ChromaData[3].ColoryOrvHighBits << 4);
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.WhiteX = ChromaX;
        pEdidData->pData->BasicDisplayCaps.ChromaLumaData.WhiteY = ChromaY;

        // Display ID can indicate multiple sets of primary + White point information
        // We parse only the 1st set
    }

    // All blocks here are optional, so not returning failure
    return;
}
/***************************************************************
 * @brief Method to parse Product ID Data struct common between DID1.3 & DID2.0.
 *
 * @param  pGetEdidModes
 * @param pEdidData
 * @param ProductIDBlockTag
 * @return void
 ***************************************************************/
void DisplayIdParserGetProductIDDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 ProductIDBlockTag)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_PNP_OUI_ID* pPnpOuiId = NULL;
    DDU8 MonNameLength;

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, ProductIDBlockTag, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status))
        return;

    pPnpOuiId = (DISPLAYID_PNP_OUI_ID*)(GetBlockById.pOutputBlock);
    // if Native DisplayId block is used
    if ((pEdidData->IsDisplayIdBaseBlock) && (pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[0] == 0) && (pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[1] == 0) &&
        (pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[2] == 0))
    {
        pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[0] = pPnpOuiId->ManufacturerID[0];
        pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[1] = pPnpOuiId->ManufacturerID[1];
        pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ManufacturerID[2] = pPnpOuiId->ManufacturerID[2];

        DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.ProductID, sizeof(pPnpOuiId->ProductID), pPnpOuiId->ProductID, sizeof(pPnpOuiId->ProductID));
        DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.SerialNumber, sizeof(pPnpOuiId->SerialNumber), pPnpOuiId->SerialNumber, sizeof(pPnpOuiId->SerialNumber));

        pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.WeekOfManufacture = pPnpOuiId->WeekOfManufacture;
        pEdidData->pData->BasicDisplayCaps.BaseBlkPnpID.YearOfManufacture = pPnpOuiId->YearOfManufacture;

        // Display ID Base block can consist of PnpID as 0s, marking any PnpID parsed from this block as valid
        pEdidData->pData->BasicDisplayCaps.IsBaseBlkPnpIDValid = TRUE;
    }

    // caching PnpID from DID blocks in ExtnBlkPnpID from first DID extension block only (Edid Base + DID Extn)
    if ((pEdidData->IsDisplayIdBaseBlock == FALSE) && (pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ManufacturerID[0] == 0) &&
        (pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ManufacturerID[1] == 0) && (pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ManufacturerID[2] == 0))
    {
        pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ManufacturerID[0] = pPnpOuiId->ManufacturerID[0];
        pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ManufacturerID[1] = pPnpOuiId->ManufacturerID[1];
        pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ManufacturerID[2] = pPnpOuiId->ManufacturerID[2];

        DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.ProductID, sizeof(pPnpOuiId->ProductID), pPnpOuiId->ProductID, sizeof(pPnpOuiId->ProductID));
        DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.SerialNumber, sizeof(pPnpOuiId->SerialNumber), pPnpOuiId->SerialNumber, sizeof(pPnpOuiId->SerialNumber));

        pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.WeekOfManufacture = pPnpOuiId->WeekOfManufacture;
        pEdidData->pData->BasicDisplayCaps.ExtnBlkPnpID.YearOfManufacture = pPnpOuiId->YearOfManufacture;

        // Display ID extension block is present, mark IsPnpIDValid as TRUE, as per DisplayID spec PnpID with 0s is valid, no Zero check required
        pEdidData->pData->BasicDisplayCaps.IsExtnBlkPnpIDValid = TRUE;
    }

    // Product ID string is variable length, we can copy 13 bytes which is length of Monitor Name, so either copy 13 bytes or copy whatever is the length available up to 13 bytes
    MonNameLength = DD_MIN(pPnpOuiId->SizeOfProductID, MONITOR_NAME_LENGTH);
    if (MonNameLength > 0)
    {
        DD_MEM_COPY_SAFE(pEdidData->pData->BasicDisplayCaps.MonitorName, MONITOR_NAME_LENGTH, (&(pPnpOuiId->ProductIDString[0]) + 0x0), MonNameLength);
        pEdidData->pData->BasicDisplayCaps.MonitorNameLength = MonNameLength;
    }
}
/***************************************************************
 * @brief Display Id Parser Get Monitor Descriptor Details.
 *
 *  Method to parse Monitor descriptor block details from various Display ID blocks:
 *      1. Monitor Range limits
 *      2. Monitor Name
 *      3. Min/max RR and dotclock supported.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserGetMonitorDescriptorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_VIDEO_RANGE* pDidVideoRange = NULL;
    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Note: Not checking for multiple instances of any of below blocks as they are assumed to be occurring only once.
    // If multiple instances of below blocks occur, only the first one will be parsed

    // Other fields in all of the blocks below isn't parsed as there are no equivalant fields in EDID. Needed?
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_VIDEO_RANGE_LIMITS, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDidVideoRange = (DISPLAYID_VIDEO_RANGE*)(GetBlockById.pOutputBlock);
        pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = DisplayInfoParserConvertHexArrayToInt(pDidVideoRange->MaximumPixelClock, 3);
        pEdidData->pData->FtrSupport.IsCVTRedBlankSupported = pDidVideoRange->VesaCvtReducedBlankingSupported;
        pEdidData->pData->FtrSupport.IsCVTSupported = pDidVideoRange->VesaCvtStandardBlankingSupported;
        pEdidData->pData->FtrSupport.IsContinuousFreqSupported = pDidVideoRange->DiscreteFreqDisplayDevice ? FALSE : TRUE;

        pEdidData->pData->FtrSupport.IsMRLBlockPresent = TRUE;
        if (pEdidData->pData->FtrSupport.IsContinuousFreqSupported)
        {
            pEdidData->pData->RrSwitchCaps.FixedRrCaps.IsFullRrRangeSupported = TRUE;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = pDidVideoRange->MinimumVerticalFreq * KILO_HERTZ;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = pDidVideoRange->MaximumVerticalFreq * KILO_HERTZ;
        }
    }

    DisplayIdParserGetProductIDDetails(pGetEdidModes, pEdidData, DID_DATA_BLOCK_PRODUCT_ID);

    // All blocks here are optional, so not returning failure
    return;
}

/***************************************************************
 * @brief Display Id Parser Parse Tiled Topology Block.
 *
 *  Method to parse Tiled topolgy Block from display ID
 *              Parses Tiled topology details from  DID_DATA_BLOCK_TILED_TOPOLOGY.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseTiledTopologyBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 TiledDisplayTag)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_TILED_DISPLAY_TOPOLOGY* pTiledTopologyBlock = NULL;
    DD_TILED_DISPLAY_INFO_BLOCK* pTiledInfo = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Note: Not checking for multiple instances of Tiled info block as they are assumed to be occurring only once.
    // If multiple instances of below blocks occur, only the first one will be parsed
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, TiledDisplayTag, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Display ID Tiled Block not found ");
        return;
    }

    pTiledTopologyBlock = (DISPLAYID_TILED_DISPLAY_TOPOLOGY*)(GetBlockById.pOutputBlock);

    pTiledInfo = &(pEdidData->pData->TiledDisplayInfo);

    // Assume Block is valid
    pTiledInfo->IsValidBlock = TRUE;

    if (pTiledTopologyBlock->BlockHeader.NumberOfBytes < MIN_SIZE_OF_TILED_DISPLAY_BLOCK)
    {
        // Don't parse the block if # of valid bytes is less than required (13 bytes).
        pTiledInfo->IsValidBlock = FALSE;
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid Display ID Tiled Block not found ");
        return;
    }
    else if (pTiledTopologyBlock->BlockHeader.NumberOfBytes < SIZE_OF_TILED_DISPLAY_BLOCK)
    {
        // If the # of bytes in block > min. required, but < Max (22 bytes), parse the block, but set Block Valid to FALSE
        pTiledInfo->IsValidBlock = FALSE;
    }

    pTiledInfo->InSinglePhysicalDisplayEnclosure = pTiledTopologyBlock->TileDisplayEnclosure;
    // pTiledTopologyBlock->TiledDisplayExtBehavior is don't case. See header for details
    if (pTiledTopologyBlock->TiledDisplayBehavior < SCALING_TYPE_MAX)
    {
        pTiledInfo->Scaling = (DD_TILED_SCALING)pTiledTopologyBlock->TiledDisplayBehavior;
    }

    pTiledInfo->IsBezelInfoAvailable = pTiledTopologyBlock->TileBezelInfoPresent;

    pTiledInfo->TotalNumberOfHTiles = (pTiledTopologyBlock->NumHTilesHighBits << 4) | pTiledTopologyBlock->NumHTilesLowBits;
    pTiledInfo->TotalNumberOfVTiles = (pTiledTopologyBlock->NumVTilesHighBits << 4) | pTiledTopologyBlock->NumVTilesLowBits;

    pTiledInfo->HTileLocation = (pTiledTopologyBlock->HTileLocationHighBits << 4) | pTiledTopologyBlock->HTileLocationLowBits;
    pTiledInfo->VTileLocation = (pTiledTopologyBlock->VTileLocationHighBits << 4) | pTiledTopologyBlock->VTileLocationLowBits;

    pTiledInfo->HTileSizeInPixels = pTiledTopologyBlock->TiledDisplayHorSize;
    pTiledInfo->VTileSizeInLines = pTiledTopologyBlock->TiledDisplayVerSize;

    pTiledInfo->PixelMultiplier = pTiledTopologyBlock->PixelMultilier;
    pTiledInfo->TileBezelInformation = pTiledTopologyBlock->TileBezelInformation;

    DD_MEM_COPY_SAFE(pTiledInfo->ManufacturerID, 3, pTiledTopologyBlock->ManufacturerID, 3);
    DD_MEM_COPY_SAFE(pTiledInfo->ProductID, 2, pTiledTopologyBlock->ProductID, 2);
    DD_MEM_COPY_SAFE(pTiledInfo->SerialNumber, 4, pTiledTopologyBlock->SerialNumber, 4);

    // Tiled block is optional, so not returning failure
    return;
}

/***************************************************************
 * @brief Display Id Parser Parse Type1 Timing.
 *
 *  Method to parse and add modes from Detailed Timing Type-I
 *              Parses Display ID Timing Block DID_DATA_BLOCK_DETAILED_TIMING_1
 *              Calls the common function "DisplayIdParserParseDTDTiming" for parsing.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseType1Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DisplayIdParserParseDTDTiming(pGetEdidModes, pEdidData, DID_DATA_BLOCK_DETAILED_TIMING_1, DID_1_3_DOT_CLOCK_SCALE);
}

/***************************************************************
 * @brief Display Id Parser Parse DTD Timing.
 *
 *  The common Method between DID1.3 & DID 2.0 to parse and add modes
 *              from Detailed Timing Data Block. The only difference is the Dot Clock
 *              Scale , which is 10 KHz for DID1.3 & 1KHz for DID2.0.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @param  DTDTimingTag
 * @param  DotClockScale :
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayIdParserParseDTDTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 DTDTimingTag, DDU32 DotClockScale)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU8 NumberofDtds;
    DISPLAYID_DTD_TIMING_1* pDidTiming = NULL;
    TIMINGINFO_1* pDidTimingInfo = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);
    DDASSERT(pGetEdidModes->pModeTable);

    for (DDU8 InstanceOfBlock = 0; InstanceOfBlock < MAX_DID_BLOCK_INSTANCES; InstanceOfBlock++)
    {
        DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DTDTimingTag, InstanceOfBlock);
        Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
        if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
        {
            DISP_FUNC_EXIT_W_STATUS(DDS_DISPLAY_ID_BLOCK_NOT_FOUND);
            return DDS_DISPLAY_ID_BLOCK_NOT_FOUND;
        }

        pDidTiming = (DISPLAYID_DTD_TIMING_1*)(GetBlockById.pOutputBlock);
        NumberofDtds = pDidTiming->BlockHeader.NumberOfBytes / sizeof(TIMINGINFO_1);
        pDidTimingInfo = &(pDidTiming->DidTimingInfo1);

        Status = DisplayInfoParserParseDTDTiming(pEdidData, DTDTimingTag, DotClockScale, pDidTimingInfo, NumberofDtds);

    } // for loop for Instance of Block ends

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Display Id Parser Parse Type2 Timing.
 *
 *  Method to parse and add modes from Detailed Timing Type-II
 *              Parses Display ID Timing Block DID_DATA_BLOCK_DETAILED_TIMING_2.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseType2Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU32 DtdCounter;
    DDU8 NumType2Dtds;
    DDU32 XRes, YRes, RRate;
    DDU64 PixelClock;
    DDU32 HTotal, VTotal, HBlank, VBlank, HSPW, VSPW, HSO, VSO;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DISPLAYID_DTD_TIMING_2* pDidTiming2 = NULL;
    DID_TIMINGINFO_2* pDidTimingInfo2 = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    for (DDU8 InstanceOfBlock = 0; InstanceOfBlock < MAX_DID_BLOCK_INSTANCES; InstanceOfBlock++)
    {
        DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_DETAILED_TIMING_2, InstanceOfBlock);
        Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
        if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
        {
            // Detailed timing block in Display ID not found, not a critical error, so just returning
            return;
        }

        pDidTiming2 = (DISPLAYID_DTD_TIMING_2*)(GetBlockById.pOutputBlock);

        NumType2Dtds = pDidTiming2->BlockHeader.NumberOfBytes / sizeof(DID_TIMINGINFO_2);
        pDidTimingInfo2 = &(pDidTiming2->DidTimingInfo2);

        for (DtdCounter = 0; DtdCounter < NumType2Dtds; DtdCounter++, pDidTimingInfo2++)
        {
            DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

            XRes = (pDidTimingInfo2->HActive + 1) * 8; // H. Active field is represented as (Pixels/8) - 1
            YRes = pDidTimingInfo2->VActive + 1;

            YRes = (pDidTimingInfo2->FrameScanType == SCAN_TYPE_INTERLACED) ? YRes * 2 : YRes;

            PixelClock = DisplayInfoParserConvertHexArrayToInt(pDidTimingInfo2->PixelClock, 3); // 3 bytes of pixel clock
            // Pixel clock is represented in units of 10,000
            PixelClock = (PixelClock + 1) * 10000;

            if ((PixelClock == 10000) || (pDidTimingInfo2->HActive == 0) || (pDidTimingInfo2->VActive == 0))
            {
                // Invalid DTD, parse next one
                DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid DTD in the Display ID");
                continue;
            }

            HBlank = (pDidTimingInfo2->HBlank + 1) * 8; // H. Blank field is represented as (Pixels/8) - 1
            HTotal = XRes + HBlank;
            VBlank = pDidTimingInfo2->VBlank + 1; // Old code has +1 and was commented. find out why??
            VBlank = (pDidTimingInfo2->FrameScanType == SCAN_TYPE_INTERLACED) ? VBlank * 2 : VBlank;

            VTotal = YRes + VBlank;
            if (HTotal > 0 && VTotal > 0)
            {
                RRate = (DDU32)(DD_ROUND_DIV((PixelClock), (HTotal * VTotal)));
            }
            else
            {
                RRate = 0;
                continue;
            }

            RRate = (pDidTimingInfo2->FrameScanType == SCAN_TYPE_INTERLACED) ? RRate * 2 : RRate;

            HSPW = (pDidTimingInfo2->HSyncWidth + 1) * 8; // H. SyncWidth field is represented as (Pixels/8) - 1
            VSPW = (pDidTimingInfo2->VSyncWidth + 1);
            VSPW = (pDidTimingInfo2->FrameScanType == SCAN_TYPE_INTERLACED) ? VSPW * 2 : VSPW;

            HSO = (pDidTimingInfo2->HSyncOffset + 1) * 8; // H. SyncOffset field is represented as (Pixels/8) - 1
            VSO = (pDidTimingInfo2->VSyncOffset + 1);
            VSO = (pDidTimingInfo2->FrameScanType == SCAN_TYPE_INTERLACED) ? VSO * 2 : VSO;

            if (((HSO + HSPW) > HBlank) || ((VSO + VSPW) > VBlank))
            {
                DISP_DBG_MSG(GFXDBG_CRITICAL, "Invalid DTD in the Display ID Type 2 timing");
                // Invalid DTD, parse next one
                continue;
            }

            TimingInfo.DotClockInHz = PixelClock;
            TimingInfo.HTotal = HTotal;
            TimingInfo.HActive = XRes;
            TimingInfo.HBlankStart = XRes;
            TimingInfo.HBlankEnd = HTotal - 1;
            TimingInfo.HSyncStart = XRes + HSO;
            TimingInfo.HSyncEnd = TimingInfo.HSyncStart + HSPW - 1;
            TimingInfo.HRefresh = (DDU32)(PixelClock / HTotal);

            TimingInfo.VTotal = VTotal;
            TimingInfo.VActive = YRes;
            TimingInfo.VBlankStart = YRes;
            TimingInfo.VBlankEnd = VTotal - 1;
            TimingInfo.VSyncStart = YRes + VSO;

            TimingInfo.VSyncEnd = TimingInfo.VSyncStart + VSPW - 1;
            TimingInfo.VRoundedRR = RRate;
            TimingInfo.IsInterlaced = pDidTimingInfo2->FrameScanType;
            TimingInfo.HSyncPolarity = (pDidTimingInfo2->HSyncPolarity) ? FALSE : TRUE;
            TimingInfo.VSyncPolarity = (pDidTimingInfo2->VSyncPolarity) ? FALSE : TRUE;
            if (pDidTimingInfo2->Preferedflag)
            {
                TimingInfo.Flags.PreferredMode = TRUE;
                AddModes.ForceAdd = TRUE;
            }
            else
            {
                AddModes.ForceAdd = FALSE;
            }

            // Add mode to the list
            TimingInfo.ModeSource = DID_DTD;
            TimingInfo.CeData.VicId[0] = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
            TimingInfo.SignalStandard = DD_SIGNAL_UNKNOWN;

            AddModes.pTimingInfo = &TimingInfo;

            DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        } // for loop for DTDCounter ends
    }     // for loop for Instance of Block ends

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Display Id Parser Parse Type3 Timing.
 *
 *  Method to parse and add modes from Short Timing Type-III
 *              Parses Display ID Timing Block DID_DATA_BLOCK_SHORT_TIMING_3.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseType3Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU32 DescrCounter;
    DDU32 NumType3Descriptors;
    DDU16 AspectRatioX;
    DDU16 AspectRatioY;
    DDU32 XRes, YRes, RRate;
    CREATE_CVT_TIMING_ARGS CreateCVTTimingArgs;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DISPLAYID_SHORT_TIMING_3* pDidTiming3 = NULL;
    DID_TIMINGINFO_3* pDidTimingInfo3 = NULL;
    DDU8 VicId;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    for (DDU8 InstanceOfBlock = 0; InstanceOfBlock < MAX_DID_BLOCK_INSTANCES; InstanceOfBlock++)
    {
        DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_SHORT_TIMING_3, InstanceOfBlock);
        Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
        if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
        {
            // Short timing block in Display ID not found, not a critical error, so just returning
            return;
        }

        pDidTiming3 = (DISPLAYID_SHORT_TIMING_3*)(GetBlockById.pOutputBlock);

        NumType3Descriptors = pDidTiming3->BlockHeader.NumberOfBytes / sizeof(DID_TIMINGINFO_3);
        pDidTimingInfo3 = &(pDidTiming3->DidTimingInfo3);

        for (DescrCounter = 0; DescrCounter < NumType3Descriptors; DescrCounter++, pDidTiming3++)
        {
            DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

            AspectRatioX = DidAspectRatioTable[pDidTimingInfo3->AspectRatio].XRatio;
            AspectRatioY = DidAspectRatioTable[pDidTimingInfo3->AspectRatio].YRatio;

            XRes = (pDidTimingInfo3->HActive + 1) * 8; // H. Active field is represented as (Pixels/8) - 1
            YRes = (XRes * AspectRatioY) / AspectRatioX;
            RRate = pDidTimingInfo3->TransferRate + 1; // Refresh rate filed is represesnted as [1 → 128HZ]

            CreateCVTTimingArgs.XRes = XRes;
            CreateCVTTimingArgs.YRes = YRes;
            CreateCVTTimingArgs.RRate = RRate;
            CreateCVTTimingArgs.IsInterLaced = FALSE;
            CreateCVTTimingArgs.IsMargin_Req = FALSE;

            if (pDidTimingInfo3->TimingFormula == 0)
            {
                CreateCVTTimingArgs.IsRed_Blank_Req = TRUE;
            }

            CreateCVTTimingArgs.pTimingInfo = &TimingInfo;
            Status = DisplayInfoRoutinesCreateCVTTiming(&CreateCVTTimingArgs);

            if (IS_DDSTATUS_ERROR(Status))
            {
                continue;
            }

            VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

            // Add mode to the list
            AddModes.pTimingInfo = &TimingInfo;
            TimingInfo.ModeSource = DID_DTD;
            TimingInfo.CeData.VicId[0] = VicId;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
            TimingInfo.Flags.PreferredMode = pDidTimingInfo3->Preferedflag ? TRUE : FALSE;
            TimingInfo.SignalStandard = DD_VESA_CVT;
            AddModes.ForceAdd = FALSE;

            DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        } // for loop for DescrCounter ends
    }     // for loop for Instance of Block ends

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Display Id Parser Parse Type4 Timing.
 *
 *  Method to parse and add modes from DMT ID code Timing Type-IV Timing
 *              Parses Display ID Timing Block DID_DATA_BLOCK_DMTID_CODE_TIMING_4.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseType4Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DisplayIdParserParseEnumeratedTimingBlock(pGetEdidModes, pEdidData, DID_DATA_BLOCK_DMTID_CODE_TIMING_4, DID_1_3_TIMING_CODE_SIZE_IN_BYTES);
}

/***************************************************************
 * @brief Display Id Parser Parse Enumerated Timing Block.
 *
 *  The common Method between DID1.3 and DID2.0 to parse and add modes
 *               from DMT /CTA/ HDMI VIC ID code Timing. This is Enumerated form.
 *               With Type8 (of DID2) "Timing Code Size in byte" is also added to be
 *               either 1 or 2. This was always as 1 in case of DID 1.3.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @param EnumeratedTimingTag
 * @param TimingCodeSizeInByte
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayIdParserParseEnumeratedTimingBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 EnumeratedTimingTag, DDU8 TimingCodeSizeInByte)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU8 NumType4Descriptors;
    DISPLAYID_DMTID_CODE_TIMING_4* pDidTiming4 = NULL;
    SHORT_TIMING_4_REVID Timing4HdrOverride;
    DDU8* pDidTimingInfo4 = NULL;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Not checking for multiple instances of Type4 timing yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, EnumeratedTimingTag, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_DISPLAY_ID_BLOCK_NOT_FOUND);
        return DDS_DISPLAY_ID_BLOCK_NOT_FOUND;
    }

    pDidTiming4 = (DISPLAYID_DMTID_CODE_TIMING_4*)(GetBlockById.pOutputBlock);

    NumType4Descriptors = (pDidTiming4->BlockHeader.NumberOfBytes) / (sizeof(DDU8) * TimingCodeSizeInByte);
    Timing4HdrOverride.RevAndOtherData = pDidTiming4->BlockHeader.RevAndOtherData;
    pDidTimingInfo4 = &(pDidTiming4->DidTimingInfo4);

    Status = DisplayInfoParserParseEnumTimings(&Timing4HdrOverride, TimingCodeSizeInByte, pEdidData, EnumeratedTimingTag, pDidTimingInfo4, NumType4Descriptors);

    DISP_FUNC_EXIT();
    return Status;
}

/***************************************************************
 * @brief Display Id Parser Parse Standard Timing.
 *
 *  Method to parse and add Standard Timing Modes in Display ID
 *              Parses Display ID Timing Block DID_DATA_BLOCK_VESA_TIMING_STD.
 * @param pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseStandardTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU32 TimingModeCtr;
    DDU32 DmtIndex;
    DDU8 DmtCounter;
    ADD_MODES AddModes;
    DD_TIMING_INFO TimingInfo;
    DISPLAYID_VESA_TIMING_STD* pDidVesaTimingStd = NULL;
    DID_VESA_STD_TIMING_INFO* pDidVesaStdTimingInfo = NULL;
    DDU8 VicId;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Not checking for multiple instances of Standard timing yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_VESA_TIMING_STD, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
    {
        // Standard timing block in Display ID not found, not a critical error, so just returning
        return;
    }

    pDidVesaTimingStd = (DISPLAYID_VESA_TIMING_STD*)(GetBlockById.pOutputBlock);

    if (pDidVesaTimingStd->BlockHeader.NumberOfBytes != VESA_STD_TIMING_BLOCK_LENGTH)
    {
        // Wrong length reported in Block header
        return;
    }

    pDidVesaStdTimingInfo = &(pDidVesaTimingStd->VesaStdTimingInfo);

    AddModes.pTimingInfo = &TimingInfo;
    AddModes.ForceAdd = FALSE;

    for (TimingModeCtr = 0; TimingModeCtr < VESA_STD_TIMING_BLOCK_LENGTH; TimingModeCtr++)
    {
        for (DmtCounter = 0; DmtCounter < BYTE_LENGTH; DmtCounter++)
        {
            if ((pDidVesaStdTimingInfo->TimingsModes[TimingModeCtr] & (1 << DmtCounter)) == 0)
            {
                // DMT Mode not supported
                continue;
            }

            DD_ZERO_MEM(&TimingInfo, sizeof(DD_TIMING_INFO));

            // Adding one as we added a 0th Entry into the table which is not represented in this block
            DmtIndex = (TimingModeCtr * BYTE_LENGTH) + DmtCounter + 1;

            if (DmtIndex >= g_ulTotalDisplayID_VESA_DMTmodes)
            {
                // DMT ID Index exceeded Max limit
                continue;
            }

            if (g_DisplayID_VESA_DMTModeTable[DmtIndex].pTimingInfo == NULL)
            {
                // No timings found for this DMT ID code
                continue;
            }

            VicId = (pEdidData->pData->FtrSupport.IsCeExtnDisplay) ? CeInfoParserIsCeMode(&TimingInfo) : VIC_UNDEFINED;

            TimingInfo = *g_DisplayID_VESA_DMTModeTable[DmtIndex].pTimingInfo;
            TimingInfo.ModeSource = STD_TIMING;
            TimingInfo.CeData.VicId[0] = VicId;
            TimingInfo.CeData.Par[0].Value = 0; // AVI_PAR_NODATA;
            TimingInfo.SignalStandard = DD_VESA_DMT;
            TimingInfo.Flags.PreferredMode = FALSE; // These are not preferred modes as per Display ID spec

            // Add mode to the list
            DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
        }
    }

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Display Id Parser Parse Cea Timing.
 *
 *  Method to parse and add CEA Timing Modes in Display ID
 *              Parses Display ID Timing Block DID_DATA_BLOCK_CEA_TIMING_STD.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseCeaTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU32 TimingModeCtr;
    DDU32 CeaIndex;
    DDU8 CeaCounter;
    DISPLAYID_CEA_TIMING_STD* pDidCeaTimingStd = NULL;
    DID_CEA_TIMING_INFO* pDidCeaTimingInfo = NULL;
    CE_MODE_LIST CeVicData;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    DD_ZERO_MEM(&CeVicData, sizeof(CE_MODE_LIST));

    // Not checking for multiple instances of Type4 timing yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_CEA_TIMING_STD, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
    {
        // CEA timing block in Display ID not found, not a critical error, so just returning
        return;
    }

    pEdidData->pData->FtrSupport.IsCeExtnDisplay = TRUE;

    // Breakout if this call was for just to get caps (ensure further from here, there are no caps that are parsed)
    if (pGetEdidModes->pModeTable == NULL)
    {
        return;
    }

    pDidCeaTimingStd = (DISPLAYID_CEA_TIMING_STD*)(GetBlockById.pOutputBlock);

    if (pDidCeaTimingStd->BlockHeader.NumberOfBytes != DID_CEA_TIMING_STD_SIZE)
    {
        // Wrong length reported in Block header
        return;
    }

    pDidCeaTimingInfo = &(pDidCeaTimingStd->CeaTimingInfo);

    for (TimingModeCtr = 0; TimingModeCtr < DID_CEA_TIMING_STD_SIZE; TimingModeCtr++)
    {
        for (CeaCounter = 0; CeaCounter < BYTE_LENGTH; CeaCounter++)
        {
            if ((pDidCeaTimingInfo->TimingsModes[TimingModeCtr] & (1 << CeaCounter)) == 0)
            {
                // CEA Mode not supported
                continue;
            }

            // Adding one as we added a 0th Entry into the table which is not represented in this block
            CeaIndex = (TimingModeCtr * BYTE_LENGTH) + CeaCounter + 1;

            if (CeaIndex >= g_ulTotalDisplayID_CEA_modes)
            {
                // CE VIC ID code exceeded Max limit
                continue;
            }

            if (g_DisplayID_CeStandardTiming[CeaIndex].pTimingInfo == NULL)
            {
                // No timings found for this DMT ID code
                continue;
            }

            // Add the VIC found to VIClist with all required params
            CeVicData.IsPreferred = FALSE;
            CeVicData.SignalStandard = DD_CEA_861B; // It is still monitor timings
            CeVicData.SamplingMode.Rgb = 1;
            // Since CEIndex here is <64, no need to convert it
            CeVicData.VicId[0] = g_DisplayID_CeStandardTiming[CeaIndex].CEIndex;
            CeInfoParserUpdateCeModeList(pEdidData, &CeVicData, CE_SVD);
        }
    }

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Display Id Parser Parse Ce Extension.
 *
 *  Method to parse Cea Data Block within Display ID block and add modes from it
 *              Parses CEA Block within Display ID data block:  DID_DATA_BLOCK_CEA_SPECIFIC.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdParserParseCeExtension(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData, DDU8 CEAExtTag)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_CEA_EXTN_BLOCK* pCeaExtnBlock = NULL;
    PARSE_EXTBLK_ARGS ParseExtDataBlkArgs;
    DDU8* pOriginalEdidBuf;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pOriginalEdidBuf = pGetEdidModes->pEdidOrDisplayIDBuf; // Save original pointer

    // Not checking for multiple instances of CE block yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, CEAExtTag, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
    {
        // CE Extension timing block in Display ID not found, not a critical error, so just returning
        return;
    }

    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue

    pEdidData->pData->FtrSupport.IsCeExtnDisplay = TRUE;
    pCeaExtnBlock = (DISPLAYID_CEA_EXTN_BLOCK*)(GetBlockById.pOutputBlock);

    ParseExtDataBlkArgs.LenDataBlock = (pCeaExtnBlock->BlockHeader.NumberOfBytes > 248) ? 248 : pCeaExtnBlock->BlockHeader.NumberOfBytes;
    ParseExtDataBlkArgs.pDataBlock = (DDU8*)(&((pCeaExtnBlock->CeaDataBlock)));
    ParseExtDataBlkArgs.pModeTable = pGetEdidModes->pModeTable;

    // Getting caps always, this will be helpful if someone needs to build modes based on caps
    CeInfoParserParseCeExtensionForCaps(&ParseExtDataBlkArgs, pEdidData);

    ParseExtDataBlkArgs.pDataBlock = (DDU8*)(&((pCeaExtnBlock->CeaDataBlock))); // Restore pointer to prevent tainted_scalar issue

    if (pGetEdidModes->pModeTable != NULL)
    {
        CeInfoParserParseCeExtensionForModes(&ParseExtDataBlkArgs, pEdidData);
    }

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Method to get supported BPC from Display ID Block.
 *
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdGetSupportedBPC(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_DISPLAY_DEVICE_DATA* pDisplayDeviceData = NULL;
    DISPLAYID_DISPLAY_INTERFACE* pDisplayInterface = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    DisplayIdParserGetMonitorDescriptorDetails(pGetEdidModes, pEdidData);

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_DISPLAY_DEVICE_DATA, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDisplayDeviceData = (DISPLAYID_DISPLAY_DEVICE_DATA*)(GetBlockById.pOutputBlock);

        if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask == 0)
        {
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes = DisplayInfoParserGetSupportedBpcMaskFromColorDepth(pDisplayDeviceData->ColorBitDepth);
        }
    }

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID_DATA_BLOCK_DISPLAY_INTERFACE, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDisplayInterface = (DISPLAYID_DISPLAY_INTERFACE*)(GetBlockById.pOutputBlock);

        if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask == 0)
        {
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask = (pDisplayInterface->ColorBitDepthRGBEncoding & 0x3F);
        }
    }
    return;
}

/***************************************************************
 * @brief Method to Remove the Serial Num From Display ID Block.
 *       Serial num Blocks are present in Tiled Display Block and
 *       Product Id Block for Display ID.
 *
 * @param pEdidDataBuffer
 * @param DataIndex
 * @return void
 ***************************************************************/
void DisplayIdParserRemoveSerialNumber(DD_BB_EDID_CONTAINER* pEdidDataBuffer, DDU32 DataIndex)
{
    GET_BLOCK_BY_ID_ARGS GetTileDisplayBlock, GetProductIdBlock;
    DISPLAYID_EDID_EXTN_HEADER* pDisplayIdExtHeader = NULL;
    DISPLAYID_HEADER* pDisplayIdHeader = NULL;
    DISPLAYID_TILED_DISPLAY_TOPOLOGY* pTiledTopologyBlock = NULL;
    DISPLAYID_PNP_OUI_ID* pProductIdBlock = NULL;
    DDSTATUS Status;

    DDASSERT(pEdidDataBuffer);

    pDisplayIdExtHeader = (DISPLAYID_EDID_EXTN_HEADER*)pEdidDataBuffer->EdidList[DataIndex].EDIDBlock;
    pDisplayIdHeader = (DISPLAYID_HEADER*)pEdidDataBuffer->EdidList[DataIndex].EDIDBlock;

    DD_ZERO_MEM(&GetTileDisplayBlock, sizeof(GET_BLOCK_BY_ID_ARGS));
    DD_ZERO_MEM(&GetProductIdBlock, sizeof(GET_BLOCK_BY_ID_ARGS));

    if (DID_EXT_TAG == pDisplayIdExtHeader->TagId)
    {
        // This is DisplayID Extension Block
        if ((DISPLAY_ID_VER_2_0 == pDisplayIdExtHeader->DisplayIdHeader.RevisionAndVersion) || (DISPLAY_ID_VER_2_1 == pDisplayIdExtHeader->DisplayIdHeader.RevisionAndVersion))
        {
            DisplayIdParserFillGetBlockByIdArgs(&GetTileDisplayBlock, (DDU8*)&pDisplayIdExtHeader->DisplayIdHeader, DID2_DATA_BLOCK_TILED_DISPLAY_TOPOLOGY, 0);
            DisplayIdParserFillGetBlockByIdArgs(&GetProductIdBlock, (DDU8*)&pDisplayIdExtHeader->DisplayIdHeader, DID2_DATA_BLOCK_PRODUCTID, 0);
        }
        else
        {
            DisplayIdParserFillGetBlockByIdArgs(&GetTileDisplayBlock, (DDU8*)&pDisplayIdExtHeader->DisplayIdHeader, DID_DATA_BLOCK_TILED_TOPOLOGY, 0);
            DisplayIdParserFillGetBlockByIdArgs(&GetProductIdBlock, (DDU8*)&pDisplayIdExtHeader->DisplayIdHeader, DID_DATA_BLOCK_PRODUCT_ID, 0);
        }
    }
    else if ((DISPLAY_ID_VER_2_1 == pDisplayIdExtHeader->DisplayIdHeader.RevisionAndVersion) || (DISPLAY_ID_VER_2_0 == pDisplayIdHeader->RevisionAndVersion) ||
        (DISPLAY_ID_VER_1_3 == pDisplayIdHeader->RevisionAndVersion))
    {
        // This is DisplayID Base Block, in DisplayID Base Block Serial Number may be present in ProductID Block and Tile Display Block
        if ((DISPLAY_ID_VER_2_0 == pDisplayIdHeader->RevisionAndVersion) || (DISPLAY_ID_VER_2_1 == pDisplayIdExtHeader->DisplayIdHeader.RevisionAndVersion))
        {
            DisplayIdParserFillGetBlockByIdArgs(&GetTileDisplayBlock, (DDU8*)pDisplayIdHeader, DID2_DATA_BLOCK_TILED_DISPLAY_TOPOLOGY, 0);
            DisplayIdParserFillGetBlockByIdArgs(&GetProductIdBlock, (DDU8*)pDisplayIdHeader, DID2_DATA_BLOCK_PRODUCTID, 0);
        }
        else
        {
            DisplayIdParserFillGetBlockByIdArgs(&GetTileDisplayBlock, (DDU8*)pDisplayIdHeader, DID_DATA_BLOCK_TILED_TOPOLOGY, 0);
            DisplayIdParserFillGetBlockByIdArgs(&GetProductIdBlock, (DDU8*)pDisplayIdHeader, DID_DATA_BLOCK_PRODUCT_ID, 0);
        }
    }
    else
    {
        // This is a CEA Extension Block, so just return with default values
        return;
    }

    Status = DisplayIdParserGetDisplayIDBlockByID(&GetTileDisplayBlock);
    if (IS_DDSTATUS_SUCCESS(Status) && (GetTileDisplayBlock.pOutputBlock))
    {
        // Tiled Block is found in this 128 byte block
        pTiledTopologyBlock = (DISPLAYID_TILED_DISPLAY_TOPOLOGY*)(GetTileDisplayBlock.pOutputBlock);
        DisplayInfoParserProcessSerialNumberBlock(pEdidDataBuffer, DataIndex, pTiledTopologyBlock->SerialNumber);
    }

    Status = DisplayIdParserGetDisplayIDBlockByID(&GetProductIdBlock);
    if (IS_DDSTATUS_SUCCESS(Status) && (GetProductIdBlock.pOutputBlock))
    {
        // ProductId Block is found in this 128 byte block
        pProductIdBlock = (DISPLAYID_PNP_OUI_ID*)(GetProductIdBlock.pOutputBlock);
        DisplayInfoParserProcessSerialNumberBlock(pEdidDataBuffer, DataIndex, pProductIdBlock->SerialNumber);
    }
}

/***************************************************************
 * @brief Method to return checksum from EDID block.
 *
 * @todo: VSDI-23694 - To be moved to early during Edid Caps Parsing.
 *
 * @param pEDIDBlock
 * @param NumEdidExtBlocks
 * @param pEdidChecksum
 * @return void
 ***************************************************************/
void DisplayInfoParserGetEdidExtBlockHash(DDU8* pEDIDBlock, DDU8 NumEdidExtBlocks, DDU8* pEdidChecksum)
{
    DDU8 SectionSize;

    if (pEDIDBlock)
    {
        for (DDU8 Index = 0; Index <= NumEdidExtBlocks; Index++)
        {
            // If the block is DisplayID Extension block identified by Byte0 TAG# 70h, that would indicate its an extension block to EDID. Cache the section checksum value.
            if (DID_EXTENSION_TO_EDID_TAG_VALUE_70H == pEDIDBlock[(Index * EDID_BLOCK_SIZE) + DID_EXTENSION_BLOCK_TAG_OFFSET])
            {
                SectionSize = pEDIDBlock[((Index * EDID_BLOCK_SIZE) + DID_EXTENSION_TO_EDID_SECTION_SIZE_OFFSET_02H)];

                pEdidChecksum[Index] = pEDIDBlock[((Index * EDID_BLOCK_SIZE) + DID_EXTENSION_TO_EDID_TAG_HEADER_SIZE_IN_BYTES + SectionSize)];
            }
            else
            {
                pEdidChecksum[Index] = pEDIDBlock[((Index * EDID_BLOCK_SIZE) + EDID_CHECKSUM_OFFSET)];
            }
        }
    }
}

/***************************************************************
 * @brief Method to get EDID Extension Override Block Value.
 *
 * @param pEdidOrDisplayIDBuf
 * @return DDU8
 ***************************************************************/
DDU8 DisplayInfoParserGetHdmiOverrideExtensionBlockCount(DDU8* pEdidOrDisplayIDBuf)
{
    DDASSERT(pEdidOrDisplayIDBuf);

    if (FALSE == DisplayInfoParserIsCeExtnDisplay((CE_EDID_EXTN_BLOCK*)pEdidOrDisplayIDBuf))
    {
        return INVALID_EDID_BLOCK_COUNT;
    }

    return CeInfoParserGetExtensionCountFromHdmiEeodb(pEdidOrDisplayIDBuf);
}

/***************************************************************
 * @brief Method to validate EDID/DID Header Data.
 *
 * Validates EDID/DID Base and Extension Block
 * For DID, checks if the header has valid TAG (Byte[0] != 0)
 * and checks is version is valid (1.3 or 2.0)
 * For EDID, checks if base/Extn block has valid header
 * and checks if version is valid (1.3 or 1.4).
 *
 * @param pEdidOrDisplayIDBuf
 * @param BlockSize
 * @param BlockNum
 * @param IsBaseBlockDid
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DisplayInfoParserValidateEdidDidBlock(DDU8* pEdidOrDisplayIDBuf, DDU32 BlockSize, DDU8 BlockNum, DD_BOOL IsBaseBlockDid)
{
    DDASSERT(pEdidOrDisplayIDBuf);

    if (IsBaseBlockDid)
    {
        return DisplayIdParserVerifyDataBlocks(pEdidOrDisplayIDBuf);
    }
    else
    {
        return EdidParserVerifyDataBlocks(pEdidOrDisplayIDBuf, BlockSize, BlockNum);
    }
}

//-----------------------------------------------------------------------------
//
// Display ID parser related functions -- END
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Display ID 2.0 parser related functions -- START
//
//-----------------------------------------------------------------------------

/***************************************************************
 * @brief Display IdV2 generate timings and add to mode table
 *
 *  Method to generate timings for Timing formula 9 & 10, Then add those timings to Mode table.
 *
 * @param  pCreateCVTTimingArgs
 * @param  TimingVersion
 * @param  pEdidData
 * @param  pModeTable
 * @return void
 ***************************************************************/
void DisplayIdV2GenerateAndAddCvtTimings(CREATE_CVT_TIMING_ARGS* pCreateCVTTimingArgs, CVT_RB_TIMING_FORMULA_VERSION TimingVersion, EDID_DATA* pEdidData, DD_TABLE* pModeTable)
{
    DDSTATUS Status;
    DD_TIMING_INFO TimingInfo;
    ADD_MODES AddModes;

    DD_ZERO_MEM(&TimingInfo, sizeof(TimingInfo));

    TimingInfo.SignalStandard = DD_VESA_CVT;

    pCreateCVTTimingArgs->pTimingInfo = &TimingInfo;

    // If Display Interface Features data block is available, supported color sampling mode will be available.
    TimingInfo.CeData.SamplingMode.Value = pEdidData->pData->BasicDisplayCaps.DidSamplingMode.Value;

    // If, DI features data block is not present, RGB mode needs to be marked as default
    if (TimingInfo.CeData.SamplingMode.Value == 0)
    {
        TimingInfo.CeData.SamplingMode.Rgb = 1;
    }

    // Cache Yuv 420 Support (Byte 0, bit 6) for Type 10 timings (this is timing specific), for Type 9, this bit will be 0 and Sampling mode will be parsed as per EDID
    TimingInfo.CeData.SamplingMode.Yuv420 |= pCreateCVTTimingArgs->IsYCbCr420Supported;

    switch (TimingVersion)
    {
    case VESA_CVT_VER2:
        Status = DisplayInfoRoutinesCreateCVTTiming(pCreateCVTTimingArgs);
        break;
    case VESA_CVT_REDUCED_BLANKING_VER1:
        Status = DisplayInfoRoutinesCreateCVTTiming(pCreateCVTTimingArgs);
        break;
    case VESA_CVT_REDUCED_BLANKING_VER2:
        Status = DisplayInfoRoutinesCreateCVT2Timing(pCreateCVTTimingArgs);
        break;
    case VESA_CVT_REDUCED_BLANKING_VER3:
        Status = DisplayInfoRoutinesCreateCVT2Timing(pCreateCVTTimingArgs);
        break;
    default:
        DDRLSASSERT_UNEXPECTED(0, "Unsupported Timing Formula!");
        Status = DDS_INVALID_PARAM;
        break;
    }

    if (IS_DDSTATUS_SUCCESS(Status))
    {
        AddModes.pTimingInfo = &TimingInfo;
        AddModes.ForceAdd = FALSE;
        AddModes.pTargetModeTable = pModeTable;

        Status = DisplayInfoParserAddEDIDModeToList(&AddModes, pEdidData);
    }
}

/***************************************************************
 * @brief Display IdV2 fill Cvt Timing Args for Formula9
 *
 *  Method to fill the input values for Cvt calculation timings for Timing formula 9.
 *
 * @param  pCreateCVTTimingArgs
 * @param  TimingVersion
 * @param  pTiming9
 * @return void
 ***************************************************************/
void DisplayIdV2FillFormula9Inputs(CREATE_CVT_TIMING_ARGS* pCreateCVTTimingArgs, DISPLAYID_2_0_TIMINGINFO_IX* pTiming9)
{
    pCreateCVTTimingArgs->XRes = pTiming9->HActive + 1;
    pCreateCVTTimingArgs->YRes = pTiming9->VActive + 1;
    pCreateCVTTimingArgs->RRate = pTiming9->RefreshRate + 1;
    pCreateCVTTimingArgs->IsInterLaced = FALSE;
    pCreateCVTTimingArgs->IsMargin_Req = FALSE;
    pCreateCVTTimingArgs->IsRed_Blank_Req = (pTiming9->TimingFormula > VESA_CVT_VER2) ? TRUE : FALSE;
    pCreateCVTTimingArgs->IsEarlyVsync = FALSE;
    if (pTiming9->TimingFormula == VESA_CVT_REDUCED_BLANKING_VER2)
    {
        pCreateCVTTimingArgs->RedBlankVersion = RED_BLANK_VER_2;
    }
}

/***************************************************************
 * @brief Display IdV2 fill Cvt Timing Args for Formula10
 *
 *  Method to fill the input values for Cvt calculation timings for Timing formula 10.
 *
 * @param  pCreateCVTTimingArgs
 * @param  TimingVersion
 * @param  pTiming10
 * @param  ExtraByteSupported
 * @return void
 ***************************************************************/
void DisplayIdV2FillFormula10Inputs(CREATE_CVT_TIMING_ARGS* pCreateCVTTimingArgs, TIMINGINFO_TYPE_X_7BYTE* pTiming10, DD_BOOL ExtraByteSupported)
{
    DDS8 DeltaHBlank;

    pCreateCVTTimingArgs->XRes = pTiming10->HActive + 1;
    pCreateCVTTimingArgs->YRes = pTiming10->VActive + 1;
    pCreateCVTTimingArgs->RRate = pTiming10->RefreshRateLow + 1;
    pCreateCVTTimingArgs->IsInterLaced = FALSE;
    pCreateCVTTimingArgs->IsMargin_Req = FALSE;
    pCreateCVTTimingArgs->IsRed_Blank_Req = (pTiming10->TimingFormula > VESA_CVT_VER2) ? TRUE : FALSE;
    pCreateCVTTimingArgs->IsEarlyVsync = FALSE;

    // TBD: BYTE0, BIT6:5 : 3D Sterio support ? what to set here ?
    // BYTE0, BIT7 : YCC420 support
    pCreateCVTTimingArgs->IsYCbCr420Supported = pTiming10->Ycc420Support;

    if (ExtraByteSupported)
    {
        // TBD: BYTE6, BIT1:0 : consider 2 higher bits for RR beyond 8 bits for RR > 256
    }
    if (pTiming10->TimingFormula == VESA_CVT_REDUCED_BLANKING_VER3)
    {
        pCreateCVTTimingArgs->RedBlankVersion = RED_BLANK_VER_3;
        pCreateCVTTimingArgs->IsEarlyVsync = pTiming10->EarlyVsync;
        if (pTiming10->HBlankOption == HBLANK_PIXEL_160)
        {
            pCreateCVTTimingArgs->ReqHblankPixels = RB_HBLANK_MIN_SIZE_160_PIXELS;
        }
        else
        {
            pCreateCVTTimingArgs->ReqHblankPixels = RB_HBLANK_MIN_SIZE_80_PIXELS;
        }
        pCreateCVTTimingArgs->ReqVblankTime = RB_VBLANK_MIN_SIZE_TIME;

        if (ExtraByteSupported)
        {
            DeltaHBlank = (pTiming10->DeltaHBlank > 5) ? (5 - pTiming10->DeltaHBlank) : pTiming10->DeltaHBlank;
            pCreateCVTTimingArgs->ReqHblankPixels = (DDU32)((DeltaHBlank * RB_HBLANK_ADDITIONAL_STEP_SIZE) + pCreateCVTTimingArgs->ReqHblankPixels);
            pCreateCVTTimingArgs->ReqVblankTime = pTiming10->AddVBlankTime * 35 + RB_VBLANK_MIN_SIZE_TIME;
        }
    }
    else if (pTiming10->TimingFormula == VESA_CVT_REDUCED_BLANKING_VER2)
    {
        pCreateCVTTimingArgs->RedBlankVersion = RED_BLANK_VER_2;
    }
}

/***************************************************************
 * @brief Parser function to parse Type 10 timing
 *
 * Fucntion to parse Type 10 timing & this will inturn call various subfunctions below which inturn will add modes to the mode-table.
 *
 * @param  pDidFormula10 (structure pointer when called through Display ID Extension block)
 * @param  pCtaFormula10 (structure pointer when called through CTA Extension block)
 * @param  pModeTable
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayInfoParserParseTypeXTiming(DISPLAYID_2_0_FORMULA_TIMING_X* pDidFormula10, CEA_EXT_FORMULA_TIMING_X* pCtaFormula10, DD_TABLE* pModeTable, EDID_DATA* pEdidData,
    DD_BOOL IsTimingAddedInCtaBlk)
{
    DD_BOOL ExtraByteSupported;
    DDU8 NoOfEntries, Count, SizePerEntry;
    CREATE_CVT_TIMING_ARGS CreateCVTTimingArgs;
    TIMINGINFO_TYPE_X_7BYTE* pXtimingFormula;

    // Addition of Display ID Type 10 timing through CTA Extension block & Display ID Extension has minor differences in meta data packing, this information needs to be parsed individually,
    // timing information is common for both
    if (IsTimingAddedInCtaBlk == TRUE)
    {
        if (pCtaFormula10)
        {
            ExtraByteSupported = (pCtaFormula10->PayloadBytesPerDescriptor == 1);
            SizePerEntry = (ExtraByteSupported == FALSE) ? TIMINGINFO_X_NORMAL_SIZE : TIMINGINFO_X_EXTRABYTE_SIZE;
            NoOfEntries = (pCtaFormula10->CeExtHeader.CeHdr.Length - 2) / SizePerEntry; // the first 2 bytes of data block are used for blobk ID Tag (0x2A) & Version & Other Data
            pXtimingFormula = (TIMINGINFO_TYPE_X_7BYTE*)&pCtaFormula10->DidV2Timing10;
        }
        else
        {
            DDRLSASSERT_UNEXPECTED(0, "");
            DISP_DBG_MSG(GFXDBG_CRITICAL, "Either Timing is not available or not parsed correctly");
            return;
        }
    }
    else
    {
        if (pDidFormula10)
        {
            ExtraByteSupported = (pDidFormula10->BlockHeader.PayloadBytesPerDescriptor == 1);
            SizePerEntry = (ExtraByteSupported == FALSE) ? TIMINGINFO_X_NORMAL_SIZE : TIMINGINFO_X_EXTRABYTE_SIZE;
            NoOfEntries = pDidFormula10->BlockHeader.NumberOfBytes / SizePerEntry;
            pXtimingFormula = &pDidFormula10->DidV2Timing10;
        }
        else
        {
            DDRLSASSERT_UNEXPECTED(0, "");
            DISP_DBG_MSG(GFXDBG_CRITICAL, "Either Timing is not available or not parsed correctly");
            return;
        }
    }

    for (Count = 0; Count < NoOfEntries; Count++)
    {
        DD_ZERO_MEM(&CreateCVTTimingArgs, sizeof(CreateCVTTimingArgs));

        DisplayIdV2FillFormula10Inputs(&CreateCVTTimingArgs, pXtimingFormula, ExtraByteSupported);
        DisplayIdV2GenerateAndAddCvtTimings(&CreateCVTTimingArgs, (CVT_RB_TIMING_FORMULA_VERSION)pXtimingFormula->TimingFormula, pEdidData, pModeTable);

        if (ExtraByteSupported)
        {
            pXtimingFormula++;
        }
        else
        {
            // we have to move upward only by 6 bytes, so for now cast to the 6byte struct and use ++
            // alternative is to cast to void and add 6 bytes.
            DID_TIMINGINFO_TYPE_X* pTemp = (DID_TIMINGINFO_TYPE_X*)pXtimingFormula;
            pXtimingFormula = (TIMINGINFO_TYPE_X_7BYTE*)++pTemp;
        }
    }
}

/***************************************************************
 * @brief Display IdV2 Parser ParseFourmulaBasedTiming
 *
 *  Method to Get all Display caps from Display ID V2 block
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table.
 *
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserParseFormulaBasedTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_2_0_FORMULA_TIMING_X* pFormula10;
    DISPLAYID_2_0_FORMULA_TIMING_IX* pFormula9;
    DDU8 NoOfEntries, Count;

    DISPLAYID_2_0_TIMINGINFO_IX* pTiming9;
    CREATE_CVT_TIMING_ARGS CreateCVTTimingArgs;

    // Parse Type 9 timing, if any
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_FORMULA_TIMING_IX, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);

    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pFormula9 = (DISPLAYID_2_0_FORMULA_TIMING_IX*)GetBlockById.pOutputBlock;
        NoOfEntries = pFormula9->BlockHeader.NumberOfBytes / TIMINGINFO_IX_SIZE;

        pTiming9 = &pFormula9->DidV2Timing9;
        for (Count = 0; Count < NoOfEntries; Count++)
        {
            DD_ZERO_MEM(&CreateCVTTimingArgs, sizeof(CreateCVTTimingArgs));
            DisplayIdV2FillFormula9Inputs(&CreateCVTTimingArgs, pTiming9);
            DisplayIdV2GenerateAndAddCvtTimings(&CreateCVTTimingArgs, (CVT_RB_TIMING_FORMULA_VERSION)pTiming9->TimingFormula, pEdidData, pGetEdidModes->pModeTable);
            pTiming9++;
        }
    }

    // Parse Type 10 timing, if any
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_FORMULA_TIMING_X, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);

    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pFormula10 = (DISPLAYID_2_0_FORMULA_TIMING_X*)GetBlockById.pOutputBlock;
        DisplayInfoParserParseTypeXTiming(pFormula10, NULL, pGetEdidModes->pModeTable, pEdidData, FALSE);
    }
}

/***************************************************************
 * @brief Display IdV2 Parser Get Edid Modes.
 *
 *  Method to Get all Display modes present
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table
 *
 *      Note: Below calls has be in order of increasing priority.
 *            Lower priority modes are added 1st and higher priority modes are added later
 *
 *          1. DisplayIdV2GetSupportedBPC to get the Supported BPC.
 *          2. Reuse DisplayIdParserParseCeExtension to parse CE extension block within Display ID-2.0
 *          3. DisplayIdV2ParserParseType7Timing to get DTD VII timings
 *          4. DisplayIdV2ParserParseType8EnumeratedTiming to get Enumerated VIII timings
 *          5. DisplayIdV2ParserParseFormulaBasedTiming to get FormulaBased IX timings.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserGetEdidModes(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU8* pOriginalEdidBuf;
    DISP_FUNC_WO_STATUS_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pOriginalEdidBuf = pGetEdidModes->pEdidOrDisplayIDBuf; // Save original pointer

    // 1. Get the supported BPC. This is required for filling the Timing information.
    DisplayIdV2GetSupportedBPC(pGetEdidModes, pEdidData);

    // Get Tiled info caps, which will be used to add modes in later part of this function
    // Reuse DisplayIdParserParseTiledTopologyBlock with DID2 specific "Tiled" Block Tag - 0x28
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseTiledTopologyBlock(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_TILED_DISPLAY_TOPOLOGY);

    // 2. Parse CE extenstion blocks that might be present within this Display ID-2.0 Block
    // Reuse DisplayIdParserParseCeExtension with DID2 specific "CTA" Block Tag - 0x81
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseCeExtension(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_CTA_DISPLAY_ID);

    // 3. Parse Type VII Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdV2ParserParseType7Timing(pGetEdidModes, pEdidData);

    // 4. Parse Type VIII Enumerated Timing section
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdV2ParserParseType8EnumeratedTiming(pGetEdidModes, pEdidData);

    // 5. Parse Formula Based Timing section
    // Its not prioritized and implemented yet. Similar to Type5 Short Timings of DID1.3

    DisplayIdV2ParserParseFormulaBasedTiming(pGetEdidModes, pEdidData);

    DISP_FUNC_WO_STATUS_EXIT();
    return;
}

/***************************************************************
 * @brief Display IdV2 Parser Get Edid Caps.
 *
 *  Method to Get all Display caps from Display ID V2 block
 *         This will inturn call various subfunctions below which inturn will add modes to the mode-table.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDU8* pOriginalEdidBuf;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    pOriginalEdidBuf = pGetEdidModes->pEdidOrDisplayIDBuf; // Save original pointer

    // Get the supported BPC. This is required for filling the timing info structure.
    DisplayIdV2GetSupportedBPC(pGetEdidModes, pEdidData);

    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdV2ParserGetMonitorDetails(pGetEdidModes, pEdidData);

    // Reuse DisplayIdParserParseTiledTopologyBlock with DID2 specific "Tiled" Block Tag - 0x28
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseTiledTopologyBlock(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_TILED_DISPLAY_TOPOLOGY);

    // Parse CE extenstion blocks that might be present within this Display ID Block
    // Reuse DisplayIdParserParseCeExtension with DID2 specific "CTA" Block Tag - 0x81
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdParserParseCeExtension(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_CTA_DISPLAY_ID);

    // Parse HDR Caps through display Params (0x21) & Display Interface (0x26) blocks
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdV2ParserParseHdrCapDataBlock(pGetEdidModes, pEdidData);

    // Parse Mso Caps from DID2 VSDB block
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdV2ParserParseMsoCapsFromVSDBBlock(pGetEdidModes, pEdidData);

    // Get Adaptive Sync Data
    pGetEdidModes->pEdidOrDisplayIDBuf = pOriginalEdidBuf; // Restore the saved pointer back to prevert tainter_Scalar coverity issue
    DisplayIdV21ParserGetAdaptiveSyncDetails(pGetEdidModes, pEdidData);

    return;
}

/***************************************************************
 * @brief Display Id V2.1 Parser Parse Brightness Luminance Range Data Block.
 *
 * Function: DisplayIdV21ParserParseBrightnessLuminanceRangeDataBlock:
 * Parsing 0x2E block to get Brightness Luminance Ranges which will be reported
 * to OS if present.
 *
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV21ParserParseBrightnessLuminanceRangeDataBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAY_ID_2_1_BRIGHTNESS_LUMINANCE_RANGE_BLOCK* pDid21BrightnessLuminanceRange;

    DISP_FUNC_ENTRY();

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID21_DATA_BLOCK_BRIGHTNESS_LUMINANCE_RANGE, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        // Read the Luminance Caps
        pDid21BrightnessLuminanceRange = (DISPLAY_ID_2_1_BRIGHTNESS_LUMINANCE_RANGE_BLOCK*)(GetBlockById.pOutputBlock);
        pEdidData->pData->BlcDisplayCaps.BrightnessLuminanceData.MinLuminanceFull = pDid21BrightnessLuminanceRange->MinLuminanceFull;
        pEdidData->pData->BlcDisplayCaps.BrightnessLuminanceData.MaxLuminanceFull = pDid21BrightnessLuminanceRange->MaxLuminanceFull;
        pEdidData->pData->BlcDisplayCaps.BrightnessLuminanceData.MaxLuminanceBoost = pDid21BrightnessLuminanceRange->MaxLuminanceBoost;
    }

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief DID2 Convert Size.
 *
 *  Method to convert size of the attached device into EDID (cm format)
 *              Convert Size to cm. 1 cm = 10 mm. If Final val in CM is > 255
 *              then return 255 else return the size being calculated.
 * @param SizeInMM
 * @param ImageSizePrecision
 * @return DDU8
 ***************************************************************/
DDU8 DID2ConvertSize(DDU16 SizeInMM, DDU8 ImageSizePrecision)
{
    DDU8 SizeInCM = 0;
    DDU8 ImageSizeMultiplier = 1;
    ImageSizeMultiplier = (ImageSizePrecision ? 10 : 1);
    SizeInCM = (SizeInMM * ImageSizeMultiplier) / 10;
    return DD_MIN(SizeInCM, 255);
}

/***************************************************************
 * @brief Display Id V2 Parser Get Monitor Details.
 *
 *  Method to parse Monitor details from various Display ID V2 blocks:
 *      1. Monitor Range limits
 *      2. Monitor Name
 *      3. Min/max RR and dotclock supported
 *      4. Monitor supported Gamma
 *      5. Chromaticity info.
 * @param  pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserGetMonitorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU16 ChromaX, ChromaY;
    DISPLAYID_2_0_DISPLAY_PARAMETERS* pDid2DisplayParams = NULL;
    DISPLAYID_2_0_DISPLAY_PARAMS_REVID DisplayParamRevID;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Note: Not checking for multiple instances of any of below blocks as they are assumed to be occurring only once.
    // If multiple instances of below blocks occur, only the first one will be parsed
    DisplayIdV2ParserGetMonitorDescriptorDetails(pGetEdidModes, pEdidData);

    //   The 0x78 translates to a default gamma value of 2.2 (0x78 + 100 / 100)
    //   supported by the majority of LFP panels.
    pEdidData->pData->BasicDisplayCaps.DisplayGamma = 0x78;

    // Not checking for multiple instances of CE block yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DISPLAY_PARAMS, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status))
        return;

    pDid2DisplayParams = (DISPLAYID_2_0_DISPLAY_PARAMETERS*)(GetBlockById.pOutputBlock);
    if (pDid2DisplayParams->GammaEOTF != 0xFF)
    {
        pEdidData->pData->BasicDisplayCaps.DisplayGamma = pDid2DisplayParams->GammaEOTF;
    }
    // If Revision Data shows Precision bit being set means scale it with 1 else with 0.1
    DisplayParamRevID.RevAndOtherData = pDid2DisplayParams->BlockHeader.RevAndOtherData;

    // The HImageSize & VImageSize are in mm while EDID BasicDisplayCaps has it in cm.
    // If Final HImage Size = HImageSize * Image size Multiplier (ImagePrecision) is >255 then set it to 255
    pEdidData->pData->BasicDisplayCaps.HSize = DID2ConvertSize(pDid2DisplayParams->HImageSize, DisplayParamRevID.ImagePrecision);
    pEdidData->pData->BasicDisplayCaps.VSize = DID2ConvertSize(pDid2DisplayParams->VImageSize, DisplayParamRevID.ImagePrecision);

    // Conversion from 12 bits of Display ID Chromaticity to EDID 10 bits if needed has to be done by caller
    // We are just storing the data structure here as it is

    ChromaX = pDid2DisplayParams->RedChroma.ColorxOruLowBits | (pDid2DisplayParams->RedChroma.ColorxOruHighBits << 8);
    ChromaY = pDid2DisplayParams->RedChroma.ColoryOrvLowBits | (pDid2DisplayParams->RedChroma.ColoryOrvHighBits << 4);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.RedX = ChromaX;
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.RedY = ChromaY;

    ChromaX = pDid2DisplayParams->GreenChroma.ColorxOruLowBits | (pDid2DisplayParams->GreenChroma.ColorxOruHighBits << 8);
    ChromaY = pDid2DisplayParams->GreenChroma.ColoryOrvLowBits | (pDid2DisplayParams->GreenChroma.ColoryOrvHighBits << 4);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.GreenX = ChromaX;
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.GreenY = ChromaY;

    ChromaX = pDid2DisplayParams->BlueChroma.ColorxOruLowBits | (pDid2DisplayParams->BlueChroma.ColorxOruHighBits << 8);
    ChromaY = pDid2DisplayParams->BlueChroma.ColoryOrvLowBits | (pDid2DisplayParams->BlueChroma.ColoryOrvHighBits << 4);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.BlueX = ChromaX;
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.BlueY = ChromaY;

    ChromaX = pDid2DisplayParams->WhitePointChroma.ColorxOruLowBits | (pDid2DisplayParams->WhitePointChroma.ColorxOruHighBits << 8);
    ChromaY = pDid2DisplayParams->WhitePointChroma.ColoryOrvLowBits | (pDid2DisplayParams->WhitePointChroma.ColoryOrvHighBits << 4);
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.WhiteX = ChromaX;
    pEdidData->pData->BasicDisplayCaps.ChromaLumaData.WhiteY = ChromaY;

    // All blocks here are optional, so not returning failure
    return;
}

/***************************************************************
 * @brief Display Id V2 Parser Get Monitor Descriptor Details.
 *
 *  Method to parse Monitor descriptor block details from various Display ID blocks:
 *      1. Monitor Range limits
 *      2. Monitor Name
 *      3. Min/max RR and dotclock supported.
 * @param   pGetEdidModes
 * @param   pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserGetMonitorDescriptorDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_2_0_VIDEO_RANGE* pDid2VideoRange = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Note: Not checking for multiple instances of any of below blocks as they are assumed to be occurring only once.
    // If multiple instances of below blocks occur, only the first one will be parsed

    // Other fields in all of the blocks below isn't parsed as there are no equivalant fields in EDID. Needed?
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DYNAMIC_VIDEO_TIMING, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDid2VideoRange = (DISPLAYID_2_0_VIDEO_RANGE*)(GetBlockById.pOutputBlock);

        pEdidData->pData->BasicDisplayCaps.MaxDotClockSupportedInHz = DisplayInfoParserConvertHexArrayToInt(pDid2VideoRange->MaximumPixelClock, 3);
        pEdidData->pData->FtrSupport.IsContinuousFreqSupported = pDid2VideoRange->SeamlessDynamicVideoTiming ? TRUE : FALSE;

        pEdidData->pData->FtrSupport.IsMRLBlockPresent = TRUE;
        if (pEdidData->pData->FtrSupport.IsContinuousFreqSupported)
        {
            pEdidData->pData->RrSwitchCaps.FixedRrCaps.IsFullRrRangeSupported = TRUE;
            // Max RR is 8 bit in DID2.0 Dynamic Video Timing Rev 0 and 10bit in Rev 1, Rev 0 will have zeros in Msb 2 bits
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = pDid2VideoRange->MinimumVerticalRr * KILO_HERTZ;
            pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = ((pDid2VideoRange->MaximumVerticalRrMsb << 8) + pDid2VideoRange->MaximumVerticalRrLsb) * KILO_HERTZ;
        }
    }

    DisplayIdParserGetProductIDDetails(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_PRODUCTID);
    // All blocks here are optional, so not returning failure
    return;
}

/***************************************************************
 * @brief Display Id V2 Parser Parse Type7 Timing.
 *
 *  Method to parse and add modes from Detailed Timing Type-VII
 *              Parses Display ID V2 Timing Block DID2_DATA_BLOCK_DETAIL_TIMING_VII.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserParseType7Timing(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DisplayIdParserParseDTDTiming(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_DETAIL_TIMING_VII, DID_2_0_DOT_CLOCK_SCALE);
}
/***************************************************************
 * @brief Display Id V2 Parser Parse Type8 Enumerated Timing.
 *
 *  Method to parse and add modes from Enumerated Codes based on
 *              DMT/CTA/HDMI VICs. Parses Display ID V2 Data Block
 *              DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII.
 *              The "Timing Code Size in Byte" (1 or 2) is detemined from RevID.
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserParseType8EnumeratedTiming(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    // The struct is same between DID1.3 & DID2.0
    DISPLAYID_DMTID_CODE_TIMING_4* pDid2Timing8 = NULL;
    DID_TIMINGINFO_TYPE_VIII DID2EnumeratedRevID;

    DISP_FUNC_ENTRY();

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Not checking for multiple instances of Type8 timing yet. Will do on need basis
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status) || (GetBlockById.pOutputBlock == NULL))
    {
        // Enumerated timing block in Display ID not found, not a critical error, so just returning
        return;
    }
    // Let's extract Revision Data Byte (Byte-2)
    pDid2Timing8 = (DISPLAYID_DMTID_CODE_TIMING_4*)(GetBlockById.pOutputBlock);

    DID2EnumeratedRevID.RevAndOtherData = pDid2Timing8->BlockHeader.RevAndOtherData;
    DisplayIdParserParseEnumeratedTimingBlock(pGetEdidModes, pEdidData, DID2_DATA_BLOCK_ENUMERATED_TIMING_VIII,
        DID2EnumeratedRevID.TimingCodesize + 1); // Incrementing by 1 as TimingCodesize value 0 = 1 Byte Descriptor Timing Code, 1 = 2 Byte Descriptor Timing Code

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Display IDV2 Parser Get Supported Bpc Mask From Color Depth.
 *
 *  Returns supported BPC mask from color depth
 *              As 14BPC is not supported, separate function is defined for DID2.0.
 * @param ColorBitDepth
 * @return DD_BPC_SUPPORTED
 ***************************************************************/
static DD_BPC_SUPPORTED DisplayIDV2ParserGetSupportedBpcMaskFromColorDepth(DDU32 ColorBitDepth)
{
    DD_BPC_SUPPORTED SupportedBpc;

    SupportedBpc.ColorDepthMask = 0;
    switch (ColorBitDepth)
    {
    case DID2_COLOR_DEPTH_16_BPC:
        SupportedBpc.Supports16BitsPerColor = 1;
    case DID2_COLOR_DEPTH_12_BPC:
        SupportedBpc.Supports12BitsPerColor = 1;
    case DID2_COLOR_DEPTH_10_BPC:
        SupportedBpc.Supports10BitsPerColor = 1;
    case DID2_COLOR_DEPTH_8_BPC:
        SupportedBpc.Supports8BitsPerColor = 1;
    case DID2_COLOR_DEPTH_6_BPC:
        SupportedBpc.Supports6BitsPerColor = 1;
        break;
    case DID2_COLOR_DEPTH_NOT_DEFINED:
    default:
        SupportedBpc.ColorDepthMask = 0;
        break;
    };

    return SupportedBpc;
}
/***************************************************************
 * @brief  Method to extract Color Model for DID 2.0.
 *
 * @param  pGetEdidModes
 * @return DD_COLOR_MODEL
 ***************************************************************/
static DD_COLOR_MODEL DisplayIDV2ConvertedColorModel(DD_GET_EDID_MODES* pGetEdidModes)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_2_0_DISPLAY_INTERFACE* pDid2DisplayInterface = NULL;

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DISPLAY_IF_FEATURES, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDid2DisplayInterface = (DISPLAYID_2_0_DISPLAY_INTERFACE*)(GetBlockById.pOutputBlock);
        if ((pDid2DisplayInterface->BT2020ClrSMPTE2084EOTF) || (pDid2DisplayInterface->BT2020ColorEOTF))
        {
            return DD_COLOR_MODEL_YCBCR_2020;
        }
        else if (pDid2DisplayInterface->BT601ColorEOTF)
        {
            return DD_COLOR_MODEL_YCBCR_601;
        }
        else if (pDid2DisplayInterface->BT709ColorBT1886EOTF)
        {
            return DD_COLOR_MODEL_YCBCR_709;
        }
        else if ((pDid2DisplayInterface->AdobeRGBColorEOTF) || (pDid2DisplayInterface->DCIP3ColorEOTF))
        {
            return DD_COLOR_MODEL_SCRGB;
        }
        else if (pDid2DisplayInterface->SRGBColorEOTF)
        {
            return DD_COLOR_MODEL_RGB;
        }
        else
            return DD_COLOR_MODEL_CUSTOM;
    }
    else
    {
        return DD_COLOR_MODEL_RGB;
    }
}
/***************************************************************
 * @brief Method to get supported BPC from Display ID-2.0 Block.
 *
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2GetSupportedBPC(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_2_0_DISPLAY_PARAMETERS* pDisplayParams = NULL;
    DISPLAYID_2_0_DISPLAY_INTERFACE* pDisplayInterface = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    DisplayIdV2ParserGetMonitorDescriptorDetails(pGetEdidModes, pEdidData);

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DISPLAY_PARAMS, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDisplayParams = (DISPLAYID_2_0_DISPLAY_PARAMETERS*)(GetBlockById.pOutputBlock);

        if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask == 0)
        {
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes = DisplayIDV2ParserGetSupportedBpcMaskFromColorDepth(pDisplayParams->ColorBitDepth);
        }
    }

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DISPLAY_IF_FEATURES, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pDisplayInterface = (DISPLAYID_2_0_DISPLAY_INTERFACE*)(GetBlockById.pOutputBlock);

        if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.ColorDepthMask == 0)
        {
            // Update supported BPCs for RGB modes
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports6BitsPerColor = pDisplayInterface->Support6bitPerPrimaryColorRGB;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports8BitsPerColor = pDisplayInterface->Support8bitPerPrimaryColorRGB;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports10BitsPerColor = pDisplayInterface->Support10bitPerPrimaryColorRGB;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports12BitsPerColor = pDisplayInterface->Support12bitPerPrimaryColorRGB;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports14BitsPerColor = pDisplayInterface->Support14bitPerPrimaryColorRGB;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedForAllModes.Supports16BitsPerColor = pDisplayInterface->Support16bitPerPrimaryColorRGB;
        }

        if (pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.ColorDepthMask == 0)
        {
            // Update supported BPCs for YCbCr420 modes
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports8BitsPerColor = pDisplayInterface->Support8bitPerPrimaryColorYCbCr420;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports10BitsPerColor = pDisplayInterface->Support10bitPerPrimaryColorYCbCr420;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports12BitsPerColor = pDisplayInterface->Support12bitPerPrimaryColorYCbCr420;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports14BitsPerColor = pDisplayInterface->Support14bitPerPrimaryColorYCbCr420;
            pEdidData->pData->BasicDisplayCaps.BpcsSupportedFor420Modes.Supports16BitsPerColor = pDisplayInterface->Support16bitPerPrimaryColorYCbCr420;
        }
        pEdidData->pData->BasicDisplayCaps.ColorModel = DisplayIDV2ConvertedColorModel(pGetEdidModes);

        // Update Sampling mode
        pEdidData->pData->BasicDisplayCaps.DidSamplingMode.Rgb = (pDisplayInterface->SupportedColorDepthRGBencoding & 0x3F) ? 1 : 0;
        pEdidData->pData->BasicDisplayCaps.DidSamplingMode.Yuv420 = (pDisplayInterface->SupportedColorDepthYCbCr420encoding & 0x1F) ? 1 : 0;

        // Minimum pixel rate support for YCbCr420 modes from DID Display Interface v2 block
        // 74.25MP/s x field value
        pEdidData->pData->BasicDisplayCaps.DidMinPixelRateForYCbCr420Modes = pDisplayInterface->MinPixelRateYCbCr420Encoding;
    }
    return;
}

/***************************************************************
 * @brief DisplayIdV2ParserParseStandardEotfAndGamutCaps
 *
 * Function: DisplayIdV2ParserParseStandardEotfAndGamutCaps:
 *  Parsing GamutEotfCombination value to Gamut and EOTF values.
 * @param GamutEotfCombination
 * @param pGamut
 * @param pEotf
 * @return void
 ***************************************************************/
static void DisplayIdV2ParserParseStandardEotfAndGamutCaps(DDU8 GamutEotfCombination, DD_COLOR_GAMUT* pGamut, DD_CE_HDR_EOTF_TYPE* pEotf)
{
    // Initialize the value of the structure
    pEotf->Value = 0;

    // Get the right EOTF and Gamut based on the Spec Enum
    // Bit 6 : BT2020 CS & ST-2084 EOTF
    // Bit 1 : BT601  CS & EOTF
    // Bit 0 : sRGB   CS & EOTF

    // Take the largest CS first
    if (DD_IS_BIT_SET(GamutEotfCombination, 6))
    {
        pEotf->SmpteST2084 = TRUE;
        *pGamut = DD_COLOR_GAMUT_2020;
    }
    else if (DD_IS_BIT_SET(GamutEotfCombination, 0))
    {
        pEotf->TraditionalGammaSDR = TRUE;
        *pGamut = DD_COLOR_GAMUT_709;
    }
    else if (DD_IS_BIT_SET(GamutEotfCombination, 1))
    {
        pEotf->TraditionalGammaSDR = TRUE;
        *pGamut = DD_COLOR_GAMUT_601;
    }
    else
    {
        // Default Value
        pEotf->TraditionalGammaSDR = TRUE;
        *pGamut = DD_COLOR_GAMUT_709;
    }
}

/***************************************************************
 * @brief Display Id V2 Parser Parse Hdr Cap DataBlock.
 *
 * Function: DisplayIdV2ParserParseHdrCapDataBlock:
 *  Parsing DISPLAYID 2_0 DISPLAY_PARAMETERS (0x21) Block and DID2_DATA_BLOCK_DISPLAY_IF_FEATURES
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserParseHdrCapDataBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_2_0_DISPLAY_PARAMETERS* pDid2DisplayParams = NULL;
    DISPLAYID_2_0_DISPLAY_INTERFACE* pDid2DisplayInterface = NULL;
    DISP_FUNC_ENTRY();

    // Interface Params block for the EOTF and Gamut Caps
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DISPLAY_IF_FEATURES, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        // Parse the EOTF Support
        pDid2DisplayInterface = (DISPLAYID_2_0_DISPLAY_INTERFACE*)(GetBlockById.pOutputBlock);
        DisplayIdV2ParserParseStandardEotfAndGamutCaps(pDid2DisplayInterface->ColorSpaceEOTFCombination1, &pEdidData->pData->HdrCaps.DidData.GamutSupported,
            &pEdidData->pData->HdrCaps.DidData.EOTFSupported);
    }

    // Display Params block for the Luma Caps
    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_DISPLAY_PARAMS, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_ERROR(Status))
    {
        DISP_FUNC_EXIT();
        return;
    }

    // Parse the Luminance Caps
    pDid2DisplayParams = (DISPLAYID_2_0_DISPLAY_PARAMETERS*)(GetBlockById.pOutputBlock);
    pEdidData->pData->HdrCaps.DidData.MaxCLL = pDid2DisplayParams->MaxLuminance10Prcnt;
    pEdidData->pData->HdrCaps.DidData.MaxFALL = pDid2DisplayParams->MaxLuminanceFull;
    pEdidData->pData->HdrCaps.DidData.MinCLL = pDid2DisplayParams->MinLuminanceFull;

    // ETW Events to be added in later phase
    DISP_DBG_MSG(GFXDBG_CRITICAL, "DID2 HDR DISPLAY LUMA PARAMS BLOCK FOUND: %d %d %d", pDid2DisplayParams->MaxLuminance10Prcnt, pDid2DisplayParams->MaxLuminanceFull,
        pDid2DisplayParams->MinLuminanceFull);

    DISP_FUNC_EXIT();
    return;
}

/***************************************************************
 * @brief Display Id V2 Parser Parse Mso Caps From VSDB Block.
 *
 * Function: DisplayIdV2ParserParseMsoCapsFromVSDBBlock:
 *  Parsing DISPLAYID 2_0 DISPLAY_PARAMETERS (0x7E) Block.
 * @param pGetEdidModes
 * @param pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV2ParserParseMsoCapsFromVSDBBlock(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DISPLAYID_2_0_VENDOR_SPECIFIC* pVsdbData = NULL;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID2_DATA_BLOCK_VENDOR_SPECIFIC, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pVsdbData = (DISPLAYID_2_0_VENDOR_SPECIFIC*)(GetBlockById.pOutputBlock);

        switch (pVsdbData->MultiSstOperation)
        {
        case DID2_MSO_TWO_STREAMS:
            pEdidData->pData->MsoDisplayCaps.NumOfLinks = EDP_MSO_2_LINK;
            break;
        case DID2_MSO_FOUR_STREAMS:
            pEdidData->pData->MsoDisplayCaps.NumOfLinks = EDP_MSO_4_LINK;
            break;
        default:
            // Single SST link
            pEdidData->pData->MsoDisplayCaps.NumOfLinks = EDP_SINGLE_LINK;
            break;
        }
        // Pixel Overlap count ranges from 0 to 8
        pEdidData->pData->MsoDisplayCaps.OverlapPixelCount = DD_MIN(pVsdbData->OverlapPixelCount, 8);
        if (pEdidData->pData->MsoDisplayCaps.NumOfLinks >= EDP_MSO_2_LINK)
        {
            pEdidData->pData->MsoDisplayCaps.IsMsoCapable = TRUE;
        }
    }

    return;
}
//-----------------------------------------------------------------------------
//
// Display ID 2.0 parser related functions -- END
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Display ID 2.1 parser related functions -- START
//
//-----------------------------------------------------------------------------

/***************************************************************
 * @brief Display IdV2.1 Parser Get EDID Caps.
 *
 *  Get all Display caps from Display ID V2.1 block
 *
 * @param  pGetEdidModes
 * @param  pEdidData
 * @return void
 ***************************************************************/
void DisplayIdV21ParserGetEdidCaps(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDASSERT(pEdidData)
        DDASSERT(pEdidData->pData)
        DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf)

        // Reuse DID v2.0 function to parse v2 blocks.
        DisplayIdV2ParserGetEdidCaps(pGetEdidModes, pEdidData);

    // Parse DID v2.1 specific blocks below
    DisplayIdV21ParserParseBrightnessLuminanceRangeDataBlock(pGetEdidModes, pEdidData);
}

/***************************************************************
 * @brief Display Id V2.1 Parser Get Adaptive Sync Details.
 *
 * @param   pGetEdidModes
 * @param   pEdidData
 * @return  void
 ***************************************************************/
void DisplayIdV21ParserGetAdaptiveSyncDetails(DD_GET_EDID_MODES* pGetEdidModes, EDID_DATA* pEdidData)
{
    DDSTATUS Status;
    GET_BLOCK_BY_ID_ARGS GetBlockById;
    DDU8 DescriptorSize, NumberOfDescriptors;
    DDU32 DescriptorByteOffset;
    DISPLAY_ID_2_1_ADAPTIVE_SYNC_BLOCK_HEADER* pAdaptiveSyncBlockHeader;
    ADAPTIVE_SYNC_OPERATION_MODE_RANGE_DESCRIPTOR* pAdaptiveSyncOperationModeRangeDescriptor;
    DD_DID_AS_DESCRIPTOR* pDidAsDescriptor;

    // Arg validation
    DDASSERT(pEdidData);
    DDASSERT(pEdidData->pData);
    DDASSERT(pGetEdidModes->pEdidOrDisplayIDBuf);

    // Note: Not checking for multiple instances of any of below blocks as they are assumed to be occurring only once.
    // If multiple instances of below blocks occur, only the first one will be parsed

    DisplayIdParserFillGetBlockByIdArgs(&GetBlockById, pGetEdidModes->pEdidOrDisplayIDBuf, DID21_DATA_BLOCK_ADAPTIVE_SYNC, 0);
    Status = DisplayIdParserGetDisplayIDBlockByID(&GetBlockById);
    if (IS_DDSTATUS_SUCCESS(Status))
    {
        pAdaptiveSyncBlockHeader = (DISPLAY_ID_2_1_ADAPTIVE_SYNC_BLOCK_HEADER*)(GetBlockById.pOutputBlock);
        DescriptorSize = DD_GET_DID_AS_DESCRIPTOR_SIZE(pAdaptiveSyncBlockHeader->PayloadBytesInDescriptor);
        NumberOfDescriptors = pAdaptiveSyncBlockHeader->PayloadBytesInBlock / DescriptorSize;

        pEdidData->pData->FtrSupport.IsContinuousFreqSupported = TRUE;
        pEdidData->pData->FtrSupport.IsMRLBlockPresent = TRUE;
        pEdidData->pData->FtrSupport.IsDidAsDataBlockPresent = TRUE;
        pEdidData->pData->RrSwitchCaps.FixedRrCaps.IsFullRrRangeSupported = TRUE;
        pEdidData->pData->RrSwitchCaps.IsFlickerParamsValid = TRUE;

        for (DDU8 DescriptorIndex = 0; DescriptorIndex < NumberOfDescriptors; DescriptorIndex++)
        {
            if (DescriptorIndex >= MAX_DID_AS_DESCRIPTORS)
            {
                DDRLSASSERT_UNEXPECTED(DD_DIAG_FEATURE_EDID_DID, "DID AS descriptor count exceeded the max allowed (5)");
                break;
            }

            DescriptorByteOffset = sizeof(DISPLAY_ID_2_1_ADAPTIVE_SYNC_BLOCK_HEADER) + (DescriptorIndex * DescriptorSize);
            pAdaptiveSyncOperationModeRangeDescriptor = (ADAPTIVE_SYNC_OPERATION_MODE_RANGE_DESCRIPTOR*)(GetBlockById.pOutputBlock + DescriptorByteOffset);
            pDidAsDescriptor = &pEdidData->pData->RrSwitchCaps.DidAsDescriptors[DescriptorIndex];

            // FAVT is by default supported if AS SDP block is present
            pDidAsDescriptor->IsAvtSupported = (ADAPTIVE_SYNC_MODE_ALL == pAdaptiveSyncOperationModeRangeDescriptor->SupportedAdaptiveSyncModes);
            pDidAsDescriptor->MinRr1000 = pAdaptiveSyncOperationModeRangeDescriptor->MinRefreshRate * KILO_HERTZ;

            // As per DID 2.1 spec, MaxRR is calculated as below
            // 0h = 1 * 1.00035 Hz
            // 1h = 2 * 1.00035 Hz
            // ...
            // 3FFh = 1024 * 1.00035 Hz
            pDidAsDescriptor->MaxRr1000 = ((pAdaptiveSyncOperationModeRangeDescriptor->MaxRefreshRateMsb << 8) + pAdaptiveSyncOperationModeRangeDescriptor->MaxRefreshRateLsb + 1) * KILO_HERTZ;

            pDidAsDescriptor->SfditInUs = 0;
            if (SUCCESSIVE_FRAME_DURATION_TOLERANCE_LIMITED == pAdaptiveSyncOperationModeRangeDescriptor->SuccessiveFrameDurationIncreaseTolerance)
            {
                // Integer part is in ms. Convert to micro seconds.
                // Fractional part is a multiple of 0.25 ms. Convert to micro seconds.
                pDidAsDescriptor->SfditInUs = pAdaptiveSyncOperationModeRangeDescriptor->MaxSingleFrameDurationIncreaseIntegerBits * DD_MILLI_TO_MICRO_SEC;
                pDidAsDescriptor->SfditInUs += pAdaptiveSyncOperationModeRangeDescriptor->MaxSingleFrameDurationIncreaseFractionalBits * FORMAT_6_2_FRACTIONAL_MULTIPLIER_IN_US;
            }

            pDidAsDescriptor->SfddtInUs = 0;
            if (SUCCESSIVE_FRAME_DURATION_TOLERANCE_LIMITED == pAdaptiveSyncOperationModeRangeDescriptor->SuccessiveFrameDurationDecreaseTolerance)
            {
                pDidAsDescriptor->SfddtInUs = pAdaptiveSyncOperationModeRangeDescriptor->MaxSingleFrameDurationDecreaseIntegerBits * DD_MILLI_TO_MICRO_SEC;
                pDidAsDescriptor->SfddtInUs += pAdaptiveSyncOperationModeRangeDescriptor->MaxSingleFrameDurationDecreaseFractionalBits * FORMAT_6_2_FRACTIONAL_MULTIPLIER_IN_US;
            }

            // Consider the highest RR by default for fixed RR range.
            if (pDidAsDescriptor->MaxRr1000 >= pEdidData->pData->RrSwitchCaps.FixedRrCaps.MaxRr1000)
            {
                pEdidData->pData->RrSwitchCaps.FixedRrCaps.MinRr1000 = pDidAsDescriptor->MinRr1000;
                pEdidData->pData->RrSwitchCaps.FixedRrCaps.MaxRr1000 = pDidAsDescriptor->MaxRr1000;
            }

            // Consider the highest RR by default for variable RR range.
            if (pDidAsDescriptor->IsAvtSupported && (pDidAsDescriptor->MaxRr1000 >= pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000))
            {
                pEdidData->pData->RrSwitchCaps.VariableRrCaps.MinRr1000 = pDidAsDescriptor->MinRr1000;
                pEdidData->pData->RrSwitchCaps.VariableRrCaps.MaxRr1000 = pDidAsDescriptor->MaxRr1000;
                pEdidData->pData->RrSwitchCaps.SfditInUs = pDidAsDescriptor->SfditInUs;
                pEdidData->pData->RrSwitchCaps.SfddtInUs = pDidAsDescriptor->SfddtInUs;
            }

            pEdidData->pData->RrSwitchCaps.NumberOfAsDescriptors++;
        }
    }
}

//-----------------------------------------------------------------------------
//
// Display ID 2.1 parser related functions -- END
//
//-----------------------------------------------------------------------------
