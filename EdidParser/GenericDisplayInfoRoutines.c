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
 * @file  GenericDisplayInfoRoutines.c
 * @ingroup dpl
 * @brief This file contains all generic display info routines like CVT/GTF timning created, VDSC calculations etc...
 *
 */

#include "GenericDisplayInfoRoutines.h"
#include "DisplayLogging.h"

#define OFFSET_FRACTIONAL_BITS 11
#define MAX_LINKM_LINKN_VALUE (DDU32)((1 << 24) - 1) // LinkM/LinkN are 24bit values(2^24-1)

 //
 // DSC related data entries.
 //

 /*
 RC buffer threshold values as per VESA DSC model.
 **/
static DDU32 RC_BUF_THRESH[] = { 896, 1792, 2688, 3584, 4480, 5376, 6272, 6720, 7168, 7616, 7744, 7872, 8000, 8064 };

/*
RC Range parameters from .cfg files supplied along with VESA DSC model.
**/
// clang-format off
static RC_PARAMETERS RcParams[][MAX_COLUMN_INDEX] = {
    {
        // 6BPP/8BPC
        { 768, 15, 6144, 3, 13, 11, 11,
          { { 0, 4, 0 },
            { 1, 6, DD_TWOS_COMPLEMENT(2) },
            { 3, 8, DD_TWOS_COMPLEMENT(2) },
            { 4, 8, DD_TWOS_COMPLEMENT(4) },
            { 5, 9, DD_TWOS_COMPLEMENT(6) },
            { 5, 9, DD_TWOS_COMPLEMENT(6) },
            { 6, 9, DD_TWOS_COMPLEMENT(6) },
            { 6, 10, DD_TWOS_COMPLEMENT(8) },
            { 7, 11, DD_TWOS_COMPLEMENT(8) },
            { 8, 12, DD_TWOS_COMPLEMENT(10) },
            { 9, 12, DD_TWOS_COMPLEMENT(10) },
            { 10, 12, DD_TWOS_COMPLEMENT(12) },
            { 10, 12, DD_TWOS_COMPLEMENT(12) },
            { 11, 12, DD_TWOS_COMPLEMENT(12) },
            { 13, 14, DD_TWOS_COMPLEMENT(12) } } },

            // 6BPP/10BPC
            { 768, 15, 6144, 7, 17, 15, 15,
              { { 0, 8, 0 },
                { 3, 10, DD_TWOS_COMPLEMENT(2) },
                { 7, 12, DD_TWOS_COMPLEMENT(2) },
                { 8, 12, DD_TWOS_COMPLEMENT(4) },
                { 9, 13, DD_TWOS_COMPLEMENT(6) },
                { 9, 13, DD_TWOS_COMPLEMENT(6) },
                { 10, 13, DD_TWOS_COMPLEMENT(6) },
                { 10, 14, DD_TWOS_COMPLEMENT(8) },
                { 11, 15, DD_TWOS_COMPLEMENT(8) },
                { 12, 16, DD_TWOS_COMPLEMENT(10) },
                { 13, 16, DD_TWOS_COMPLEMENT(10) },
                { 14, 16, DD_TWOS_COMPLEMENT(12) },
                { 14, 16, DD_TWOS_COMPLEMENT(12) },
                { 15, 16, DD_TWOS_COMPLEMENT(12) },
                { 17, 18, DD_TWOS_COMPLEMENT(12) } } },

                // 6BPP/12BPC
                { 768, 15, 6144, 11, 21, 19, 19,
                  { { 0, 12, 0 },
                  { 5, 14, DD_TWOS_COMPLEMENT(2) },
                  { 11, 16, DD_TWOS_COMPLEMENT(2) },
                  { 12, 16, DD_TWOS_COMPLEMENT(4) },
                  { 13, 17, DD_TWOS_COMPLEMENT(6) },
                  { 13, 17, DD_TWOS_COMPLEMENT(6) },
                  { 14, 17, DD_TWOS_COMPLEMENT(6) },
                  { 14, 18, DD_TWOS_COMPLEMENT(8) },
                  { 15, 19, DD_TWOS_COMPLEMENT(8) },
                  { 16, 20, DD_TWOS_COMPLEMENT(10) },
                  { 17, 20, DD_TWOS_COMPLEMENT(10) },
                  { 18, 20, DD_TWOS_COMPLEMENT(12) },
                  { 18, 20, DD_TWOS_COMPLEMENT(12) },
                  { 19, 20, DD_TWOS_COMPLEMENT(12) },
                  { 21, 22, DD_TWOS_COMPLEMENT(12) } } },

                  // 6BPP/14BPC
                  { 768, 15, 6144, 15, 25, 23, 27,
                    { { 0, 16, 0 },
                      { 7, 18, DD_TWOS_COMPLEMENT(2) },
                      { 15, 20, DD_TWOS_COMPLEMENT(2) },
                      { 16, 20, DD_TWOS_COMPLEMENT(4) },
                      { 17, 21, DD_TWOS_COMPLEMENT(6) },
                      { 17, 21, DD_TWOS_COMPLEMENT(6) },
                      { 18, 21, DD_TWOS_COMPLEMENT(6) },
                      { 18, 22, DD_TWOS_COMPLEMENT(8) },
                      { 19, 23, DD_TWOS_COMPLEMENT(8) },
                      { 20, 24, DD_TWOS_COMPLEMENT(10) },
                      { 21, 24, DD_TWOS_COMPLEMENT(10) },
                      { 22, 24, DD_TWOS_COMPLEMENT(12) },
                      { 22, 24, DD_TWOS_COMPLEMENT(12) },
                      { 23, 24, DD_TWOS_COMPLEMENT(12) },
                      { 25, 26, DD_TWOS_COMPLEMENT(12) } } },

                      // 6BPP/16BPC
                      { 768, 15, 6144, 19, 29, 27, 27,
                        { { 0, 20, 0 },
                          { 9, 22, DD_TWOS_COMPLEMENT(2) },
                          { 19, 24, DD_TWOS_COMPLEMENT(2) },
                          { 20, 24, DD_TWOS_COMPLEMENT(4) },
                          { 21, 25, DD_TWOS_COMPLEMENT(6) },
                          { 21, 25, DD_TWOS_COMPLEMENT(6) },
                          { 22, 25, DD_TWOS_COMPLEMENT(6) },
                          { 22, 26, DD_TWOS_COMPLEMENT(8) },
                          { 23, 27, DD_TWOS_COMPLEMENT(8) },
                          { 24, 28, DD_TWOS_COMPLEMENT(10) },
                          { 25, 28, DD_TWOS_COMPLEMENT(10) },
                          { 26, 28, DD_TWOS_COMPLEMENT(12) },
                          { 26, 28, DD_TWOS_COMPLEMENT(12) },
                          { 27, 28, DD_TWOS_COMPLEMENT(12) },
                          { 29, 30, DD_TWOS_COMPLEMENT(12) } } },
                  },
                  {
                      // 8BPP/8BPC
                      { 512, 12, 6144, 3, 12, 11, 11,
                        { { 0, 4, 2 },
                          { 0, 4, 0 },
                          { 1, 5, 0 },
                          { 1, 6, DD_TWOS_COMPLEMENT(2) },
                          { 3, 7, DD_TWOS_COMPLEMENT(4) },
                          { 3, 7, DD_TWOS_COMPLEMENT(6) },
                          { 3, 7, DD_TWOS_COMPLEMENT(8) },
                          { 3, 8, DD_TWOS_COMPLEMENT(8) },
                          { 3, 9, DD_TWOS_COMPLEMENT(8) },
                          { 3, 10, DD_TWOS_COMPLEMENT(10) },
                          { 5, 11, DD_TWOS_COMPLEMENT(10) },
                          { 5, 12, DD_TWOS_COMPLEMENT(12) },
                          { 5, 13, DD_TWOS_COMPLEMENT(12) },
                          { 7, 13, DD_TWOS_COMPLEMENT(12) },
                          { 13, 15, DD_TWOS_COMPLEMENT(12) } } },

                          // 8BPP/10BPC
                          { 512, 12, 6144, 7, 16, 15, 15,
                            { { 0, 4, 2 },
                              { 4, 8, 0 },
                              { 5, 9, 0 },
                              { 5, 10, DD_TWOS_COMPLEMENT(2) },
                              { 7, 11, DD_TWOS_COMPLEMENT(4) },
                              { 7, 11, DD_TWOS_COMPLEMENT(6) },
                              { 7, 11, DD_TWOS_COMPLEMENT(8) },
                              { 7, 12, DD_TWOS_COMPLEMENT(8) },
                              { 7, 13, DD_TWOS_COMPLEMENT(8) },
                              { 7, 14, DD_TWOS_COMPLEMENT(10) },
                              { 9, 15, DD_TWOS_COMPLEMENT(10) },
                              { 9, 16, DD_TWOS_COMPLEMENT(12) },
                              { 9, 17, DD_TWOS_COMPLEMENT(12) },
                              { 11, 17, DD_TWOS_COMPLEMENT(12) },
                              { 17, 19, DD_TWOS_COMPLEMENT(12) } } },

                              // 8BPP/12BPC
                              { 512, 12, 6144, 11, 20, 19, 19,
                                { { 0, 12, 2 },
                                  { 4, 12, 0 },
                                  { 9, 13, 0 },
                                  { 9, 14, DD_TWOS_COMPLEMENT(2) },
                                  { 11, 15, DD_TWOS_COMPLEMENT(4) },
                                  { 11, 15, DD_TWOS_COMPLEMENT(6) },
                                  { 11, 15, DD_TWOS_COMPLEMENT(8) },
                                  { 11, 16, DD_TWOS_COMPLEMENT(8) },
                                  { 11, 17, DD_TWOS_COMPLEMENT(8) },
                                  { 11, 18, DD_TWOS_COMPLEMENT(10) },
                                  { 13, 19, DD_TWOS_COMPLEMENT(10) },
                                  { 13, 20, DD_TWOS_COMPLEMENT(12) },
                                  { 13, 21, DD_TWOS_COMPLEMENT(12) },
                                  { 15, 21, DD_TWOS_COMPLEMENT(12) },
                                  { 21, 23, DD_TWOS_COMPLEMENT(12) } } },

                                  // 8BPP/14BPC
                                  { 512, 12, 6144, 15, 24, 23, 23,
                                    { { 0, 12, 0 },
                                      { 5, 13, 0 },
                                      { 11, 15, 0 },
                                      { 12, 17, DD_TWOS_COMPLEMENT(2) },
                                      { 15, 19, DD_TWOS_COMPLEMENT(4) },
                                      { 15, 19, DD_TWOS_COMPLEMENT(6) },
                                      { 15, 19, DD_TWOS_COMPLEMENT(8) },
                                      { 15, 20, DD_TWOS_COMPLEMENT(8) },
                                      { 15, 21, DD_TWOS_COMPLEMENT(8) },
                                      { 15, 22, DD_TWOS_COMPLEMENT(10) },
                                      { 17, 22, DD_TWOS_COMPLEMENT(10) },
                                      { 17, 23, DD_TWOS_COMPLEMENT(12) },
                                      { 17, 23, DD_TWOS_COMPLEMENT(12) },
                                      { 21, 24, DD_TWOS_COMPLEMENT(12) },
                                      { 24, 25, DD_TWOS_COMPLEMENT(12) } } },

                                      // 8BPP/16BPC
                                      { 512, 12, 6144, 19, 28, 27, 27,
                                        { { 0, 12, 2 },
                                          { 6, 14, 0 },
                                          { 13, 17, 0 },
                                          { 15, 20, DD_TWOS_COMPLEMENT(2) },
                                          { 19, 23, DD_TWOS_COMPLEMENT(4) },
                                          { 19, 23, DD_TWOS_COMPLEMENT(6) },
                                          { 19, 23, DD_TWOS_COMPLEMENT(8) },
                                          { 19, 24, DD_TWOS_COMPLEMENT(8) },
                                          { 19, 25, DD_TWOS_COMPLEMENT(8) },
                                          { 19, 26, DD_TWOS_COMPLEMENT(10) },
                                          { 21, 26, DD_TWOS_COMPLEMENT(10) },
                                          { 21, 27, DD_TWOS_COMPLEMENT(12) },
                                          { 21, 27, DD_TWOS_COMPLEMENT(12) },
                                          { 25, 28, DD_TWOS_COMPLEMENT(12) },
                                          { 28, 29, DD_TWOS_COMPLEMENT(12) } } },
                                  },
                                  {
                                      // 10BPP/8BPC
                                      {  410, 15, 5632, 3, 12, 11, 11,
                                          { { 0, 3, 2 },
                                            { 0, 4, 0 },
                                            { 1, 5, 0 },
                                            { 2, 6, DD_TWOS_COMPLEMENT(2) },
                                            { 3, 7, DD_TWOS_COMPLEMENT(4) },
                                            { 3, 7, DD_TWOS_COMPLEMENT(6) },
                                            { 3, 7, DD_TWOS_COMPLEMENT(8) },
                                            { 3, 8, DD_TWOS_COMPLEMENT(8) },
                                            { 3, 9, DD_TWOS_COMPLEMENT(8) },
                                            { 3, 9, DD_TWOS_COMPLEMENT(10) },
                                            { 5, 10, DD_TWOS_COMPLEMENT(10) },
                                            { 5, 10, DD_TWOS_COMPLEMENT(10) },
                                            { 5, 11, DD_TWOS_COMPLEMENT(12) },
                                            { 7, 11, DD_TWOS_COMPLEMENT(12) },
                                            { 11, 12, DD_TWOS_COMPLEMENT(12) } } },

                                            // 10BPP/10BPC
                                            {   410, 15, 5632, 7, 16, 15, 15,
                                                { { 0, 7, 2 },
                                                  { 4, 8, 0 },
                                                  { 5, 9, 0 },
                                                  { 6, 10, DD_TWOS_COMPLEMENT(2) },
                                                  { 7, 11, DD_TWOS_COMPLEMENT(4) },
                                                  { 7, 11, DD_TWOS_COMPLEMENT(6) },
                                                  { 7, 11, DD_TWOS_COMPLEMENT(8) },
                                                  { 7, 12, DD_TWOS_COMPLEMENT(8) },
                                                  { 7, 13, DD_TWOS_COMPLEMENT(8) },
                                                  { 7, 13, DD_TWOS_COMPLEMENT(10) },
                                                  { 9, 14, DD_TWOS_COMPLEMENT(10) },
                                                  { 9, 14, DD_TWOS_COMPLEMENT(10) },
                                                  { 9, 15, DD_TWOS_COMPLEMENT(12) },
                                                  { 11, 15, DD_TWOS_COMPLEMENT(12) },
                                                  { 15, 16, DD_TWOS_COMPLEMENT(12) } } },

                                                  // 10BPP/12BPC
                                                  {   410, 15, 5632, 11, 20, 19, 19,
                                                      { { 0, 11, 2 },
                                                        { 4, 12, 0 },
                                                        { 9, 13, 0 },
                                                        { 10, 14, DD_TWOS_COMPLEMENT(2) },
                                                        { 11, 15, DD_TWOS_COMPLEMENT(4) },
                                                        { 11, 15, DD_TWOS_COMPLEMENT(6) },
                                                        { 11, 15, DD_TWOS_COMPLEMENT(8) },
                                                        { 11, 16, DD_TWOS_COMPLEMENT(8) },
                                                        { 11, 17, DD_TWOS_COMPLEMENT(8) },
                                                        { 11, 17, DD_TWOS_COMPLEMENT(10) },
                                                        { 13, 18, DD_TWOS_COMPLEMENT(10) },
                                                        { 13, 18, DD_TWOS_COMPLEMENT(10) },
                                                        { 13, 19, DD_TWOS_COMPLEMENT(12) },
                                                        { 15, 19, DD_TWOS_COMPLEMENT(12) },
                                                        { 19, 20, DD_TWOS_COMPLEMENT(12) } } },

                                                        // 10BPP/14BPC
                                                        {   410, 15, 5632, 15, 24, 23, 23,
                                                            { { 0, 11, 2 },
                                                              { 5, 13, 0 },
                                                              { 11, 15, 0 },
                                                              { 13, 18, DD_TWOS_COMPLEMENT(2) },
                                                              { 15, 19, DD_TWOS_COMPLEMENT(4) },
                                                              { 15, 19, DD_TWOS_COMPLEMENT(6) },
                                                              { 15, 19, DD_TWOS_COMPLEMENT(8) },
                                                              { 15, 20, DD_TWOS_COMPLEMENT(8) },
                                                              { 15, 21, DD_TWOS_COMPLEMENT(8) },
                                                              { 15, 21, DD_TWOS_COMPLEMENT(10) },
                                                              { 17, 22, DD_TWOS_COMPLEMENT(10) },
                                                              { 17, 22, DD_TWOS_COMPLEMENT(10) },
                                                              { 17, 23, DD_TWOS_COMPLEMENT(12) },
                                                              { 19, 23, DD_TWOS_COMPLEMENT(12) },
                                                              { 23, 24, DD_TWOS_COMPLEMENT(12) } } },

                                                              // 10BPP/16BPC
                                                              {   410, 15, 5632, 19, 28, 27, 27,
                                                                  { { 0, 11, 2 },
                                                                    { 6, 14, 0 },
                                                                    { 13, 17, 0 },
                                                                    { 16, 20, DD_TWOS_COMPLEMENT(2) },
                                                                    { 19, 23, DD_TWOS_COMPLEMENT(4) },
                                                                    { 19, 23, DD_TWOS_COMPLEMENT(6) },
                                                                    { 19, 23, DD_TWOS_COMPLEMENT(8) },
                                                                    { 19, 24, DD_TWOS_COMPLEMENT(8) },
                                                                    { 19, 25, DD_TWOS_COMPLEMENT(8) },
                                                                    { 19, 25, DD_TWOS_COMPLEMENT(10) },
                                                                    { 21, 26, DD_TWOS_COMPLEMENT(10) },
                                                                    { 21, 26, DD_TWOS_COMPLEMENT(10) },
                                                                    { 21, 27, DD_TWOS_COMPLEMENT(12) },
                                                                    { 23, 27, DD_TWOS_COMPLEMENT(12) },
                                                                    { 27, 28, DD_TWOS_COMPLEMENT(12) } } },

                                                              },
                                                          {
                                                              // 12BPP/8BPC
                                                              { 341, 15, 2048, 3, 12, 11, 11,
                                                                { { 0, 2, 2 },
                                                                  { 0, 4, 0 },
                                                                  { 1, 5, 0 },
                                                                  { 1, 6, DD_TWOS_COMPLEMENT(2) },
                                                                  { 3, 7, DD_TWOS_COMPLEMENT(4) },
                                                                  { 3, 7, DD_TWOS_COMPLEMENT(6) },
                                                                  { 3, 7, DD_TWOS_COMPLEMENT(8) },
                                                                  { 3, 8, DD_TWOS_COMPLEMENT(8) },
                                                                  { 3, 9, DD_TWOS_COMPLEMENT(8) },
                                                                  { 3, 10, DD_TWOS_COMPLEMENT(10) },
                                                                  { 5, 11, DD_TWOS_COMPLEMENT(10) },
                                                                  { 5, 12, DD_TWOS_COMPLEMENT(12) },
                                                                  { 5, 13, DD_TWOS_COMPLEMENT(12) },
                                                                  { 7, 13, DD_TWOS_COMPLEMENT(12) },
                                                                  { 13, 15, DD_TWOS_COMPLEMENT(12) } } },

                                                                  // 12BPP/10BPC
                                                                  { 341, 15, 2048, 7, 16, 15, 15,
                                                                    { { 0, 2, 2 },
                                                                      { 2, 5, 0 },
                                                                      { 3, 7, 0 },
                                                                      { 4, 8, DD_TWOS_COMPLEMENT(2) },
                                                                      { 6, 9, DD_TWOS_COMPLEMENT(4) },
                                                                      { 7, 10, DD_TWOS_COMPLEMENT(6) },
                                                                      { 7, 11, DD_TWOS_COMPLEMENT(8) },
                                                                      { 7, 12, DD_TWOS_COMPLEMENT(8) },
                                                                      { 7, 13, DD_TWOS_COMPLEMENT(8) },
                                                                      { 7, 14, DD_TWOS_COMPLEMENT(10) },
                                                                      { 9, 15, DD_TWOS_COMPLEMENT(10) },
                                                                      { 9, 16, DD_TWOS_COMPLEMENT(12) },
                                                                      { 9, 17, DD_TWOS_COMPLEMENT(12) },
                                                                      { 11, 17, DD_TWOS_COMPLEMENT(12) },
                                                                      { 17, 19, DD_TWOS_COMPLEMENT(12) } } },

                                                                      // 12BPP/12BPC
                                                                      { 341, 15, 2048, 11, 20, 19, 19,
                                                                        { { 0, 6, 2 },
                                                                          { 4, 9, 0 },
                                                                          { 7, 11, 0 },
                                                                          { 8, 12, DD_TWOS_COMPLEMENT(2) },
                                                                          { 10, 13, DD_TWOS_COMPLEMENT(4) },
                                                                          { 11, 14, DD_TWOS_COMPLEMENT(6) },
                                                                          { 11, 15, DD_TWOS_COMPLEMENT(8) },
                                                                          { 11, 16, DD_TWOS_COMPLEMENT(8) },
                                                                          { 11, 17, DD_TWOS_COMPLEMENT(8) },
                                                                          { 11, 18, DD_TWOS_COMPLEMENT(10) },
                                                                          { 13, 19, DD_TWOS_COMPLEMENT(10) },
                                                                          { 13, 20, DD_TWOS_COMPLEMENT(12) },
                                                                          { 13, 21, DD_TWOS_COMPLEMENT(12) },
                                                                          { 15, 21, DD_TWOS_COMPLEMENT(12) },
                                                                          { 21, 23, DD_TWOS_COMPLEMENT(12) } } },

                                                                          // 12BPP/14BPC
                                                                          { 341, 15, 2048, 15, 24, 23, 23,
                                                                            { { 0, 6, 2 },
                                                                              { 7, 10, 0 },
                                                                              { 9, 13, 0 },
                                                                              { 11, 16, DD_TWOS_COMPLEMENT(2) },
                                                                              { 14, 17, DD_TWOS_COMPLEMENT(4) },
                                                                              { 15, 18, DD_TWOS_COMPLEMENT(6) },
                                                                              { 15, 19,DD_TWOS_COMPLEMENT(8) },
                                                                              { 15, 20, DD_TWOS_COMPLEMENT(8) },
                                                                              { 15, 20, DD_TWOS_COMPLEMENT(8) },
                                                                              { 15, 21, DD_TWOS_COMPLEMENT(10) },
                                                                              { 17, 21, DD_TWOS_COMPLEMENT(10) },
                                                                              { 17, 21, DD_TWOS_COMPLEMENT(12) },
                                                                              { 17, 21, DD_TWOS_COMPLEMENT(12) },
                                                                              { 19, 22, DD_TWOS_COMPLEMENT(12) },
                                                                              { 22, 23, DD_TWOS_COMPLEMENT(12) } } },

                                                                              // 12BPP/16BPC
                                                                              { 341, 15, 2048, 19, 28, 27, 27,
                                                                                { { 0, 6, 2 },
                                                                                  { 6, 11, 0 },
                                                                                  { 11, 15, 0 },
                                                                                  { 14, 18, DD_TWOS_COMPLEMENT(2) },
                                                                                  { 18, 21, DD_TWOS_COMPLEMENT(4) },
                                                                                  { 19, 22, DD_TWOS_COMPLEMENT(6) },
                                                                                  { 19, 23, DD_TWOS_COMPLEMENT(8) },
                                                                                  { 19, 24, DD_TWOS_COMPLEMENT(8) },
                                                                                  { 19, 24, DD_TWOS_COMPLEMENT(8) },
                                                                                  { 19, 25, DD_TWOS_COMPLEMENT(10) },
                                                                                  { 21, 25, DD_TWOS_COMPLEMENT(10) },
                                                                                  { 21, 25, DD_TWOS_COMPLEMENT(12) },
                                                                                  { 21, 25, DD_TWOS_COMPLEMENT(12) },
                                                                                  { 23, 26, DD_TWOS_COMPLEMENT(12) },
                                                                                  { 26, 27, DD_TWOS_COMPLEMENT(12) } } },
                                                                          },
                                                                          {
                                                                              // 15BPP/8BPC
                                                                              { 273, 15, 2048, 3, 12, 11, 11,
                                                                                { { 0, 0, 10 },
                                                                                  { 0, 1, 8 },
                                                                                  { 0, 1, 6 },
                                                                                  { 0, 2, 4 },
                                                                                  { 1, 2, 2 },
                                                                                  { 1, 3, 0 },
                                                                                  { 1, 3, DD_TWOS_COMPLEMENT(2) },
                                                                                  { 2, 4, DD_TWOS_COMPLEMENT(4) },
                                                                                  { 2, 5, DD_TWOS_COMPLEMENT(6) },
                                                                                  { 3, 5, DD_TWOS_COMPLEMENT(8) },
                                                                                  { 4, 6, DD_TWOS_COMPLEMENT(10) },
                                                                                  { 4, 7, DD_TWOS_COMPLEMENT(10) },
                                                                                  { 5, 7, DD_TWOS_COMPLEMENT(12) },
                                                                                  { 7, 8, DD_TWOS_COMPLEMENT(12) },
                                                                                  { 8, 9, DD_TWOS_COMPLEMENT(12) } } },

                                                                                  // 15BPP/10BPC
                                                                                  { 273, 15, 2048, 7, 16, 15, 15,
                                                                                    { { 0, 2, 10 },
                                                                                      { 2, 5, 8 },
                                                                                      { 3, 5, 6 },
                                                                                      { 4, 6, 4 },
                                                                                      { 5, 6, 2 },
                                                                                      { 5, 7, 0 },
                                                                                      { 5, 7, DD_TWOS_COMPLEMENT(2) },
                                                                                      { 6, 8, DD_TWOS_COMPLEMENT(4) },
                                                                                      { 6, 9, DD_TWOS_COMPLEMENT(6) },
                                                                                      { 7, 9, DD_TWOS_COMPLEMENT(8) },
                                                                                      { 8, 10, DD_TWOS_COMPLEMENT(10) },
                                                                                      { 8, 11, DD_TWOS_COMPLEMENT(10) },
                                                                                      { 9, 11, DD_TWOS_COMPLEMENT(12) },
                                                                                      { 11, 12, DD_TWOS_COMPLEMENT(12) },
                                                                                      { 12, 13, DD_TWOS_COMPLEMENT(12) } } },

                                                                                      // 15BPP/12BPC
                                                                                      { 273, 15, 2048, 11, 20, 19, 19,
                                                                                        { { 0, 4, 10 },
                                                                                          { 2, 7, 8 },
                                                                                          { 4, 9, 6 },
                                                                                          { 6, 11, 4 },
                                                                                          { 9, 11, 2 },
                                                                                          { 9, 11, 0 },
                                                                                          { 9, 12, DD_TWOS_COMPLEMENT(2) },
                                                                                          { 10, 12, DD_TWOS_COMPLEMENT(4) },
                                                                                          { 11, 13, DD_TWOS_COMPLEMENT(6) },
                                                                                          { 11, 13, DD_TWOS_COMPLEMENT(8) },
                                                                                          { 12, 14, DD_TWOS_COMPLEMENT(10) },
                                                                                          { 13, 15, DD_TWOS_COMPLEMENT(10) },
                                                                                          { 13, 15, DD_TWOS_COMPLEMENT(12) },
                                                                                          { 15, 16, DD_TWOS_COMPLEMENT(12) },
                                                                                          { 16, 17, DD_TWOS_COMPLEMENT(12) } } },

                                                                                          // 15BPP/14BPC
                                                                                          { 273, 15, 2048, 15, 24, 23, 23,
                                                                                            { { 0, 4, 10 },
                                                                                              { 3, 8, 8 },
                                                                                              { 6, 11, 6 },
                                                                                              { 9, 14, 4 },
                                                                                              { 13, 15, 2 },
                                                                                              { 13, 15, 0 },
                                                                                              { 13, 16, DD_TWOS_COMPLEMENT(2) },
                                                                                              { 14, 16, DD_TWOS_COMPLEMENT(4) },
                                                                                              { 15, 17, DD_TWOS_COMPLEMENT(6) },
                                                                                              { 15, 17, DD_TWOS_COMPLEMENT(8) },
                                                                                              { 16, 18, DD_TWOS_COMPLEMENT(10) },
                                                                                              { 17, 19, DD_TWOS_COMPLEMENT(10) },
                                                                                              { 17, 19, DD_TWOS_COMPLEMENT(12) },
                                                                                              { 19, 20, DD_TWOS_COMPLEMENT(12) },
                                                                                              { 20, 21, DD_TWOS_COMPLEMENT(12) } } },

                                                                                              // 15BPP/16BPC
                                                                                              { 273, 15, 2048, 19, 28, 27, 27,
                                                                                                { { 0, 4, 10 },
                                                                                                  { 4, 9, 8 },
                                                                                                  { 8, 13, 6 },
                                                                                                  { 12, 17, 4 },
                                                                                                  { 17, 19, 2 },
                                                                                                  { 17, 20, 0 },
                                                                                                  { 17, 20, DD_TWOS_COMPLEMENT(2) },
                                                                                                  { 18, 20, DD_TWOS_COMPLEMENT(4) },
                                                                                                  { 19, 21, DD_TWOS_COMPLEMENT(6) },
                                                                                                  { 19, 21, DD_TWOS_COMPLEMENT(8) },
                                                                                                  { 20, 22, DD_TWOS_COMPLEMENT(10) },
                                                                                                  { 21, 23, DD_TWOS_COMPLEMENT(10) },
                                                                                                  { 21, 23, DD_TWOS_COMPLEMENT(12) },
                                                                                                  { 23, 24, DD_TWOS_COMPLEMENT(12) },
                                                                                                  { 24, 25, DD_TWOS_COMPLEMENT(12) } } }
                                                                                          }
};
// clang-format on

/*
rc_range_minqp, rc_range_maxqp parameters for computation as per DSCParameterValuesVESA V1-2 spreadsheet
maximum column-index for 444= ((3*BPC - 6)*2 + 1)
**/
// clang-format off
DDU8 RangeMinQp444_8bpc[][37] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 3,2,2,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 3,3,2,2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 5,4,4,3,3,3,3,3,3,3,3,3,3,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 },
    { 5,4,4,4,3,3,3,3,3,3,3,3,3,3,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0 },
    { 5,5,4,4,3,3,3,3,3,3,3,3,3,3,3,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0 },
    { 5,5,4,4,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0 },
    { 5,5,4,4,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0,0,0 },
    { 6,5,5,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,1,1,1,1,1,1,0,0,0 },
    { 6,6,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,2,2,1,1,1,1,1,1,1,1,0 },
    { 6,6,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,2,2,2,2,1,1,1,1,0 },
    { 6,6,6,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,3,3,2,2,2,2,1,1,1,1,0 },
    { 9,9,9,9,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,5,5,5,5,4,4,3,3,3,3,2,2,1,1,1 },
    { 14,14,13,13,12,12,12,12,11,11,10,10,10,10,9,9,9,8,8,8,7,7,7,7,6,6,5,5,5,5,4,4,4,3,3,3,3 }
};

DDU8 RangeMaxQp444_8bpc[][37] = {
    { 4,4,4,4,4,4,3,3,3,3,2,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 6,6,5,5,4,4,4,4,4,4,4,4,3,3,3,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
    { 8,7,7,6,5,5,5,5,5,5,5,5,4,4,4,3,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },
    { 8,8,7,7,6,6,6,6,6,6,6,6,5,5,5,4,3,3,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0 },
    { 9,8,8,7,7,7,7,7,7,7,7,7,6,6,5,4,4,3,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,0 },
    { 9,8,8,8,7,7,7,7,7,7,7,7,6,6,6,5,4,4,3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,1,1,1 },
    { 9,9,8,8,7,7,7,7,7,7,7,7,7,6,6,5,4,4,3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,1,1,1 },
    { 10,10,9,9,8,8,8,8,8,8,8,8,8,7,7,6,5,5,4,4,4,4,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1 },
    { 11,11,10,10,9,9,9,9,9,9,8,8,8,7,7,6,6,5,5,5,5,5,4,4,4,4,3,3,2,2,2,2,2,2,1,1,1 },
    { 12,11,11,10,10,10,9,9,9,9,9,9,9,8,8,7,6,6,5,5,5,5,4,4,4,4,3,3,2,2,2,2,2,2,1,1,1 },
    { 12,12,11,11,10,10,10,10,10,10,9,9,9,8,8,7,7,6,6,6,5,5,4,4,4,4,3,3,2,2,2,2,2,2,2,2,1 },
    { 12,12,12,11,11,11,10,10,10,10,9,9,9,9,8,8,8,7,7,7,6,6,5,5,5,5,4,4,3,3,3,3,2,2,2,2,1 },
    { 12,12,12,12,11,11,11,11,11,10,10,9,9,9,8,8,8,7,7,7,6,6,5,5,5,5,4,4,3,3,3,3,2,2,2,2,1 },
    { 13,13,13,13,12,12,11,11,11,11,10,10,10,10,9,9,8,8,8,8,7,7,6,6,6,6,5,5,4,4,4,4,3,3,2,2,2 },
    { 15,15,14,14,13,13,13,13,12,12,11,11,11,11,10,10,10,9,9,9,8,8,8,8,7,7,6,6,6,6,5,5,5,4,4,4,4 }
};

DDU8 RangeMinQp444_10bpc[][49] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 4,4,4,4,4,4,4,4,4,4,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 7,6,6,6,5,5,5,5,5,5,5,5,4,4,4,3,3,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 7,7,6,6,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0 },
    { 9,8,8,7,7,7,7,7,7,7,7,7,6,6,5,5,5,5,4,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0,0 },
    { 9,8,8,8,7,7,7,7,7,7,7,7,6,6,6,6,5,5,5,5,5,5,4,4,4,4,3,3,3,3,3,2,2,2,2,2,2,2,2,2,1,1,1,0,0,0,0,0,0 },
    { 9,9,8,8,7,7,7,7,7,7,7,7,7,7,7,6,6,6,5,5,5,5,5,4,4,4,3,3,3,3,3,3,3,3,3,3,3,2,2,2,1,1,1,1,0,0,0,0,0 },
    { 9,9,8,8,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,3,2,2,2,2,1,1,1,0,0,0,0 },
    { 9,9,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,5,5,5,5,5,5,5,4,4,4,4,4,3,3,3,3,3,3,2,2,2,1,1,1,1,0,0 },
    { 10,9,9,8,8,8,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,5,5,4,4,4,4,3,3,3,3,3,3,3,2,2,2,1,1,1,1,0 },
    { 10,10,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,7,7,7,7,7,6,6,6,6,6,6,5,5,5,5,5,5,5,5,4,4,4,3,3,3,2,2,2,1,1,1,1 },
    { 10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,7,7,7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,3,2,2,1,1,1 },
    { 10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,7,7,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,1 },
    { 12,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11,11,11,10,10,9,9,9,9,8,8,7,7,7,7,6,6,5,5,5,5,4,4,3,3,3,3,2,2,2,2,1 },
    { 18,18,17,17,16,16,16,16,15,15,14,14,14,14,13,13,13,12,12,12,11,11,11,11,10,10,9,9,9,9,9,8,8,7,7,7,7,7,6,6,5,5,5,5,4,4,3,3,3 }
};

DDU8 RangeMaxQp444_10bpc[][49] = {
    { 8,8,8,8,8,8,7,7,7,6,5,5,4,4,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 10,10,9,9,8,8,8,8,8,8,7,7,6,6,6,5,5,4,4,4,4,3,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0 },
    { 12,11,11,10,9,9,9,9,9,9,9,9,8,8,8,7,6,6,5,5,5,4,4,4,4,4,3,3,3,3,3,3,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0 },
    { 12,12,11,11,10,10,10,10,10,10,10,10,9,9,9,8,7,7,6,6,6,5,5,5,5,5,4,4,3,3,3,3,3,3,3,3,2,2,1,1,1,1,1,0,0,0,0,0,0 },
    { 13,12,12,11,11,11,11,11,11,11,11,11,10,10,9,8,8,7,6,6,6,6,5,5,5,5,5,5,4,4,4,3,3,3,3,3,2,2,2,2,1,1,1,1,0,0,0,0,0 },
    { 13,12,12,12,11,11,11,11,11,11,11,11,10,10,10,9,8,8,7,7,7,7,6,6,6,6,5,5,5,5,4,3,3,3,3,3,3,3,3,3,2,2,2,1,1,1,1,0,0 },
    { 13,13,12,12,11,11,11,11,11,11,11,11,11,10,10,9,8,8,7,7,7,7,7,6,6,6,5,5,5,5,4,4,4,4,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1 },
    { 14,14,13,13,12,12,12,12,12,12,12,12,12,11,11,10,9,9,8,8,8,8,7,7,7,7,6,6,6,5,5,5,4,4,4,4,4,4,3,3,3,3,2,2,2,1,1,1,1 },
    { 15,15,14,14,13,13,13,13,13,13,12,12,12,11,11,10,10,9,9,9,9,9,8,8,8,8,7,7,6,5,5,5,5,5,4,4,4,4,4,4,3,3,3,2,2,2,2,1,1 },
    { 16,15,15,14,14,14,13,13,13,13,13,13,13,12,12,11,10,10,9,9,9,9,8,8,8,8,7,7,6,6,5,5,5,5,4,4,4,4,4,4,4,3,3,3,2,2,2,2,1 },
    { 16,16,15,15,14,14,14,14,14,14,13,13,13,12,12,11,11,10,10,10,9,9,8,8,8,8,7,7,6,6,6,6,6,6,6,6,5,5,5,4,4,4,3,3,3,2,2,2,2 },
    { 16,16,16,15,15,15,14,14,14,14,13,13,13,13,12,12,12,11,11,11,10,10,9,9,9,9,8,8,7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,3,3,2,2,2 },
    { 16,16,16,16,15,15,15,15,15,14,14,13,13,13,12,12,12,11,11,11,10,10,9,9,9,9,8,8,7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,3,2 },
    { 17,17,17,17,16,16,15,15,15,15,14,14,14,14,13,13,12,12,12,12,11,11,10,10,10,10,9,9,8,8,8,8,7,7,6,6,6,6,5,5,4,4,4,4,3,3,3,3,2 },
    { 19,19,18,18,17,17,17,17,16,16,15,15,15,15,14,14,14,13,13,13,12,12,12,12,11,11,10,10,10,10,10,9,9,8,8,8,8,8,7,7,6,6,6,6,5,5,4,4,4 }
};

DDU8 RangeMinQp444_12bpc[][61] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 5,5,5,5,4,4,4,4,4,4,4,4,4,4,4,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 11,10,10,9,8,8,8,8,8,8,8,8,7,7,7,6,5,5,4,4,4,3,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 11,11,10,10,9,9,9,9,9,9,9,9,8,8,8,7,6,6,6,6,6,5,5,5,5,5,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 13,12,12,11,11,11,11,11,11,11,11,11,10,10,9,9,9,8,7,7,7,7,5,5,5,5,5,5,4,4,4,3,3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 13,12,12,12,11,11,11,11,11,11,11,11,11,11,11,10,9,9,8,8,8,8,6,6,6,6,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0 },
    { 13,13,12,12,11,11,11,11,11,11,11,11,11,11,11,10,9,9,9,9,9,9,9,8,8,8,7,7,7,6,5,5,5,5,5,5,5,4,4,4,4,4,4,4,3,3,3,2,2,1,1,1,1,1,1,1,0,0,0,0,0 },
    { 13,13,12,12,11,11,11,11,11,11,11,11,11,11,11,11,10,10,10,10,10,10,9,9,9,9,8,8,8,7,7,7,6,6,6,6,6,6,5,5,5,5,4,4,4,3,3,2,2,2,2,2,2,1,1,1,1,0,0,0,0 },
    { 13,13,12,12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,10,10,10,10,9,9,8,7,7,7,7,7,6,6,6,6,6,6,5,5,5,4,4,4,4,3,3,2,2,2,2,1,1,1,1,1,1,0,0 },
    { 14,13,13,12,12,12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,10,10,10,10,9,9,8,8,8,8,8,8,7,7,7,7,7,6,6,5,5,5,4,4,4,4,3,3,3,3,3,2,2,2,2,2,1,1,0 },
    { 14,14,13,13,13,13,13,13,13,13,13,13,13,13,13,12,12,12,12,12,11,11,11,11,11,11,10,10,9,9,9,9,9,9,9,9,8,8,8,7,7,7,6,6,6,5,5,5,5,4,4,3,3,3,2,2,2,2,1,1,1 },
    { 14,14,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,12,11,11,11,11,11,11,10,10,10,10,9,9,9,9,8,8,8,8,7,7,7,7,6,6,5,5,5,4,4,4,3,3,3,3,2,2,1,1,1 },
    { 14,14,14,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,12,12,12,12,12,11,11,10,10,10,10,9,9,9,9,8,8,8,8,7,7,7,7,6,6,6,6,5,4,4,4,3,3,3,3,2,2,1,1,1 },
    { 17,17,17,17,16,16,15,15,15,15,15,15,15,15,15,15,15,15,15,15,14,14,13,13,13,13,12,12,11,11,11,11,10,10,9,9,9,9,8,8,7,7,7,7,7,6,6,6,5,5,5,5,4,4,3,3,3,3,2,2,1 },
    { 22,22,21,21,20,20,20,20,19,19,18,18,18,18,17,17,17,16,16,16,15,15,15,15,14,14,13,13,13,13,13,12,12,11,11,11,11,11,10,10,9,9,9,9,9,8,8,7,7,7,7,7,6,6,5,5,5,5,4,4,3 }
};

DDU8 RangeMaxQp444_12bpc[][61] = {
    { 12,12,12,12,12,12,11,11,11,10,9,9,6,6,5,5,5,4,4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 14,14,13,13,12,12,12,12,12,12,11,11,9,9,9,8,8,7,7,7,7,5,5,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 16,15,15,14,13,13,13,13,13,13,13,13,12,12,12,11,10,10,9,9,9,7,7,7,7,7,6,6,5,5,5,5,4,4,4,3,3,3,3,3,3,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 16,16,15,15,14,14,14,14,14,14,14,14,13,13,13,12,11,11,10,10,10,8,8,8,8,8,7,7,6,5,5,5,5,5,5,5,4,4,3,3,3,3,3,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0 },
    { 17,16,16,15,15,15,15,15,15,15,15,15,14,14,13,12,12,11,10,10,10,10,8,8,8,8,8,8,7,7,7,6,6,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0,0 },
    { 17,16,16,16,15,15,15,15,15,15,15,15,14,14,14,13,12,12,11,11,11,11,9,9,9,9,8,8,8,8,7,6,6,6,6,6,5,5,5,5,4,4,4,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,0 },
    { 17,17,16,16,15,15,15,15,15,15,15,15,15,14,14,13,12,12,11,11,11,11,11,10,10,10,9,9,9,8,7,7,7,7,7,7,7,6,6,6,5,5,5,5,4,4,4,3,3,2,2,2,2,2,2,2,1,1,1,1,0 },
    { 18,18,17,17,16,16,16,16,16,16,16,16,16,15,15,14,13,13,12,12,12,12,11,11,11,11,10,10,10,8,8,8,7,7,7,7,7,7,6,6,6,6,5,5,5,4,4,3,3,3,3,3,3,2,2,2,2,1,1,1,1 },
    { 19,19,18,18,17,17,17,17,17,17,16,16,16,15,15,14,14,13,13,13,13,13,12,12,12,12,11,11,10,9,8,8,8,8,7,7,7,7,7,7,6,6,6,5,5,5,5,4,4,3,3,3,3,2,2,2,2,2,2,1,1 },
    { 20,19,19,18,18,18,17,17,17,17,17,17,17,16,16,15,14,14,13,13,13,13,12,12,12,12,11,11,10,10,9,9,9,9,8,8,8,8,8,7,7,6,6,6,5,5,5,5,4,4,4,4,4,3,3,3,3,3,2,2,1 },
    { 20,20,19,19,18,18,18,18,18,18,17,17,17,16,16,15,15,14,14,14,13,13,12,12,12,12,11,11,10,10,10,10,10,10,10,10,9,9,9,8,8,8,7,7,7,6,6,6,6,5,5,4,4,4,3,3,3,3,2,2,2 },
    { 20,20,20,19,19,19,18,18,18,18,17,17,17,17,16,16,16,15,15,15,14,14,13,13,13,13,12,12,11,11,11,11,10,10,10,10,9,9,9,9,8,8,8,8,7,7,6,6,6,5,5,5,4,4,4,4,3,3,2,2,2 },
    { 20,20,20,20,19,19,19,19,19,18,18,17,17,17,16,16,16,15,15,15,14,14,13,13,13,13,12,12,11,11,11,11,10,10,10,10,9,9,9,9,8,8,8,8,7,7,7,7,6,5,5,5,4,4,4,4,3,3,2,2,2 },
    { 21,21,21,21,20,20,19,19,19,19,18,18,18,18,17,17,16,16,16,16,15,15,14,14,14,14,13,13,12,12,12,12,11,11,10,10,10,10,9,9,8,8,8,8,8,7,7,7,6,6,6,6,5,5,4,4,4,4,3,3,2 },
    { 23,23,22,22,21,21,21,21,20,20,19,19,19,19,18,18,18,17,17,17,16,16,16,16,15,15,14,14,14,14,14,13,13,12,12,12,12,12,11,11,10,10,10,10,10,9,9,8,8,8,8,8,7,7,6,6,6,6,5,5,4 }
};

// maximum column-index for 422 = ((2*BPC - 6)*2 + 1)
DDU8 RangeMinQp422_8bpc[][21] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
    {3,3,3,3,3,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
    {3,3,3,3,3,2,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,0,0},
    {3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,1,1,1},
    {5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,2,2,1,1,1},
    {5,5,5,5,5,5,5,4,4,4,4,4,4,4,3,3,3,2,2,1,1},
    {5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1,1},
    {8,8,7,7,7,7,7,7,7,7,6,6,5,5,4,4,3,3,2,2,2},
    {12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,4,3,3}
};

DDU8 RangeMaxQp422_8bpc[][21] = {
    {4,4,3,3,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,4,4,4,4,3,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {5,5,5,5,5,4,3,2,1,1,1,1,1,1,1,1,1,0,0,0,0},
    {6,6,6,6,6,5,4,3,2,2,2,2,2,1,1,1,1,1,1,0,0},
    {7,7,7,7,7,6,5,3,2,2,2,2,2,2,2,1,1,1,1,1,1},
    {7,7,7,7,7,6,5,4,3,3,3,2,2,2,2,2,2,1,1,1,1},
    {7,7,7,7,7,6,5,4,3,3,3,3,3,2,2,2,2,2,1,1,1},
    {8,8,8,8,8,7,6,5,4,4,4,3,3,3,3,2,2,2,2,1,1},
    {9,9,9,8,8,7,6,6,5,5,5,4,4,3,3,3,3,2,2,2,2},
    {10,10,9,9,9,8,7,6,5,5,5,5,4,4,4,3,3,3,2,2,2},
    {10,10,10,9,9,8,7,7,6,6,6,5,5,4,4,4,3,3,2,2,2},
    {11,11,10,10,9,9,8,7,7,7,6,6,5,5,4,4,4,3,3,2,2},
    {11,11,11,10,9,9,8,8,7,7,7,6,6,5,5,4,4,3,3,2,2},
    {12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,3},
    {13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,5,4,4}
};

DDU8 RangeMinQp422_10bpc[][29] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,4,4,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {5,5,5,4,3,3,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0},
    {6,6,6,6,5,4,4,4,3,3,3,3,3,2,2,2,2,2,2,1,1,1,0,0,0,0,0,0,0},
    {6,6,6,6,5,5,5,4,4,4,4,4,4,4,4,3,3,3,2,2,2,1,1,1,0,0,0,0,0},
    {6,6,6,6,6,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,2,1,1,0,0,0,0,0},
    {6,6,6,6,6,5,5,5,5,5,5,5,5,4,4,4,4,4,3,3,3,2,2,1,1,1,0,0,0},
    {7,7,7,7,7,6,6,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,2,2,1,1,1,1,1},
    {7,7,7,7,7,6,6,6,6,6,6,6,6,5,5,5,4,4,4,4,4,3,2,2,1,1,1,1,1},
    {8,8,7,7,7,7,7,7,7,7,7,7,6,6,6,6,5,5,4,4,4,3,3,2,2,1,1,1,1},
    {9,9,9,8,8,8,8,8,8,8,8,7,7,6,6,6,5,5,5,5,5,3,3,2,2,2,1,1,1},
    {9,9,9,9,8,8,8,8,8,8,8,8,7,7,6,6,6,6,6,5,5,4,3,3,2,2,1,1,1},
    {9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,7,7,6,6,5,5,4,3,3,3,2,2,1,1},
    {12,12,11,11,11,11,11,11,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1},
    {16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,8,7,7,6,6,5,5,5,4,4,3}
};

DDU8 RangeMaxQp422_10bpc[][29] = {
    {8,8,7,5,4,4,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {8,8,8,6,6,5,4,4,3,3,3,3,3,2,2,2,2,2,2,1,1,1,0,0,0,0,0,0,0},
    {9,9,9,8,7,6,5,4,3,3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,0,0,0,0,0},
    {10,10,10,10,9,8,7,6,5,5,5,5,5,4,4,3,3,3,3,2,2,2,1,1,1,1,1,1,1},
    {11,11,11,11,10,9,8,6,5,5,5,5,5,5,5,4,4,4,3,3,3,2,2,2,1,1,1,1,1},
    {11,11,11,11,11,10,9,8,7,6,6,5,5,5,5,5,5,4,4,4,4,3,2,2,1,1,1,1,1},
    {11,11,11,11,11,10,9,8,7,7,7,7,7,6,6,6,5,5,4,4,4,3,3,2,2,2,1,1,1},
    {12,12,12,12,12,11,10,9,8,8,8,7,7,7,7,6,5,5,5,4,4,4,3,3,2,2,2,2,2},
    {13,13,13,12,12,11,10,10,9,9,9,8,8,7,7,7,6,5,5,5,5,4,3,3,2,2,2,2,2},
    {14,14,13,13,13,12,11,10,9,9,9,9,8,8,8,7,6,6,5,5,5,4,4,3,3,2,2,2,2},
    {14,14,14,13,13,12,11,11,10,10,10,9,9,8,8,8,7,7,6,6,6,4,4,3,3,3,2,2,2},
    {15,15,14,14,13,13,12,11,11,11,10,10,9,9,8,8,8,7,7,6,6,5,4,4,3,3,2,2,2},
    {15,15,15,14,13,13,12,12,11,11,11,10,10,9,9,8,8,7,7,6,6,5,4,4,4,3,3,2,2},
    {16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2},
    {17,17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,9,8,8,7,7,6,6,6,5,5,4}
};

DDU8 RangeMinQp422_12bpc[][37] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,4,4,4,4,3,3,3,2,2,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {9,9,9,8,7,6,5,5,4,4,4,4,4,4,3,3,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {10,10,10,10,8,8,8,7,6,6,6,6,6,5,4,3,3,3,3,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {11,11,11,11,10,9,9,8,7,7,7,7,6,6,5,4,4,4,3,3,3,2,2,2,2,2,2,1,1,1,0,0,0,0,0,0,0},
    {11,11,11,11,11,10,10,9,9,8,8,7,6,6,5,5,5,4,4,4,4,3,2,2,2,2,2,1,1,1,1,0,0,0,0,0,0},
    {11,11,11,11,11,10,10,10,9,9,9,9,8,7,7,7,6,6,5,5,5,4,4,3,3,3,2,2,2,2,2,1,1,0,0,0,0},
    {11,11,11,11,11,11,10,10,10,10,10,9,8,8,8,7,6,6,6,5,5,5,4,4,3,3,3,3,3,2,2,2,1,1,0,0,0},
    {11,11,11,11,11,11,11,11,11,11,11,10,9,8,8,8,7,6,6,6,6,5,4,4,3,3,3,3,3,3,3,2,2,1,0,0,0},
    {11,11,11,11,11,11,11,11,11,11,11,11,9,9,9,8,7,7,6,6,6,5,5,4,4,3,3,3,3,3,3,2,2,1,1,0,0},
    {13,13,13,13,13,12,12,12,12,12,12,11,11,10,10,10,9,9,8,8,8,6,6,5,5,5,4,4,4,4,3,3,2,2,1,1,1},
    {13,13,13,13,13,13,13,13,13,13,12,12,11,11,10,10,10,9,9,8,8,7,6,6,5,5,4,4,4,4,4,3,3,2,2,1,1},
    {13,13,13,13,13,13,13,13,13,13,13,12,12,11,11,10,10,9,9,8,8,7,6,6,6,5,5,4,4,4,4,3,3,2,2,1,1},
    {16,16,15,15,15,15,15,15,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,4,3,3,2,2},
    {20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,12,11,11,10,10,9,9,9,8,8,7,7,6,6,6,5,5,4,4}
};

DDU8 RangeMaxQp422_12bpc[][37] = {
    {12,12,11,9,6,6,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {12,12,12,10,9,8,7,7,6,6,5,5,5,4,4,4,3,3,3,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {13,13,13,12,10,9,8,7,6,6,6,6,6,6,5,5,4,3,3,3,3,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {14,14,14,14,12,11,10,9,8,8,8,8,8,7,6,5,5,5,5,4,4,4,3,3,2,2,2,2,2,1,1,0,0,0,0,0,0},
    {15,15,15,15,14,13,12,10,9,9,9,9,8,8,7,6,6,6,5,5,5,4,4,4,3,3,3,2,2,2,1,1,1,1,0,0,0},
    {15,15,15,15,15,14,13,12,11,10,10,9,8,8,7,7,7,6,6,6,6,5,4,4,3,3,3,2,2,2,2,1,1,1,1,1,1},
    {15,15,15,15,15,14,13,12,11,11,11,11,10,9,9,9,8,8,7,7,7,6,6,5,5,5,4,4,4,3,3,2,2,1,1,1,1},
    {16,16,16,16,16,15,14,13,12,12,12,11,10,10,10,9,8,8,8,7,7,7,6,6,5,5,5,5,5,3,3,3,2,2,1,1,1},
    {17,17,17,16,16,15,14,14,13,13,13,12,11,10,10,10,9,8,8,8,8,7,6,6,5,5,5,5,5,4,4,3,3,2,1,1,1},
    {18,18,17,17,17,16,15,14,13,13,13,13,11,11,11,10,9,9,8,8,8,7,7,6,6,5,5,5,5,4,4,3,3,2,2,1,1},
    {18,18,18,17,17,16,15,15,14,14,14,13,13,12,12,12,11,11,10,10,10,8,8,7,7,7,6,6,6,5,4,4,3,3,2,2,2},
    {19,19,18,18,17,17,16,15,15,15,14,14,13,13,12,12,12,11,11,10,10,9,8,8,7,7,6,6,6,5,5,4,4,3,3,2,2},
    {19,19,19,18,17,17,16,16,15,15,15,14,14,13,13,12,12,11,11,10,10,9,8,8,8,7,7,6,6,5,5,4,4,3,3,2,2},
    {20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,5,4,4,3,3},
    {21,21,20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,13,12,12,11,11,10,10,10,9,9,8,8,7,7,7,6,6,5,5}
};

// maximum column-index for 420= ((3*BPC/2 - 4)*2 + 1) = ((3*BPC - 8) + 1)
DDU8 RangeMinQp420_8bpc[][17] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,2,1,1,1,1,1,1,1,0,0,0,0},
    {3,3,3,3,3,2,2,1,1,1,1,1,1,1,0,0,0},
    {3,3,3,3,3,3,2,2,1,1,1,1,1,1,1,0,0},
    {3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,0},
    {3,3,3,3,3,3,2,2,2,2,2,2,2,2,1,1,0},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,1,1},
    {5,5,5,5,5,4,4,4,4,4,3,3,3,3,2,1,1},
    {5,5,5,5,5,5,5,4,4,4,4,4,4,3,2,2,1},
    {5,5,5,5,5,5,5,5,5,5,5,4,4,3,3,2,1},
    {9,8,8,7,7,7,7,7,7,6,5,5,4,3,3,3,2},
    {13,12,12,11,10,10,9,8,8,7,6,6,5,5,4,4,3}
};

DDU8 RangeMaxQp420_8bpc[][17] = {
    {4,4,3,3,2,2,1,1,0,0,0,0,0,0,0,0,0},
    {4,4,4,4,4,3,2,2,1,1,1,1,0,0,0,0,0},
    {5,5,5,5,5,4,3,2,1,1,1,1,1,1,0,0,0},
    {6,6,6,6,6,5,4,3,2,2,2,1,1,1,1,0,0},
    {7,7,7,7,7,5,4,3,2,2,2,2,2,1,1,1,0},
    {7,7,7,7,7,6,5,4,3,3,3,2,2,2,1,1,0},
    {7,7,7,7,7,6,5,4,3,3,3,3,2,2,2,1,1},
    {8,8,8,8,8,7,6,5,4,4,4,3,3,2,2,2,1},
    {9,9,9,8,8,7,6,6,5,5,4,4,3,3,2,2,1},
    {10,10,9,9,9,8,7,6,5,5,5,4,4,3,3,2,2},
    {10,10,10,9,9,8,8,7,6,6,5,5,4,4,3,2,2},
    {11,11,10,10,9,9,8,7,7,6,6,5,5,4,3,3,2},
    {11,11,11,10,9,9,9,8,7,7,6,5,5,4,4,3,2},
    {13,12,12,11,10,10,9,8,8,7,6,6,5,4,4,4,3},
    {14,13,13,12,11,11,10,9,9,8,7,7,6,6,5,5,4}
};

DDU8 RangeMinQp420_10bpc[][23] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,4,4,3,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
    {4,4,4,3,3,3,3,3,3,2,2,2,2,2,1,0,0,0,0,0,0,0,0},
    {5,5,5,4,4,4,4,4,4,3,3,2,2,2,2,1,0,0,0,0,0,0,0},
    {7,7,7,6,6,5,5,4,4,3,3,3,3,2,2,2,1,1,1,0,0,0,0},
    {7,7,7,7,7,6,5,5,5,5,5,4,3,3,2,2,1,1,1,1,1,0,0},
    {7,7,7,7,7,6,6,5,5,5,5,4,4,4,3,2,2,2,2,1,1,1,0},
    {7,7,7,7,7,7,6,6,6,6,6,5,4,4,4,3,2,2,2,1,1,1,0},
    {7,7,7,7,7,7,7,7,6,6,6,6,5,5,4,4,3,3,2,2,2,1,1},
    {7,7,7,7,7,7,7,7,7,7,7,6,6,5,5,4,4,3,3,2,2,1,1},
    {9,9,9,9,9,8,8,8,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1},
    {9,9,9,9,9,9,8,8,8,8,8,8,8,7,6,6,5,4,4,3,3,2,1},
    {9,9,9,9,9,9,9,9,9,9,9,8,8,7,7,6,5,4,4,3,3,2,1},
    {13,12,12,11,11,11,11,11,11,10,9,9,8,7,7,6,5,5,4,3,3,2,2},
    {17,16,16,15,14,14,13,12,12,11,10,10,10,9,8,8,7,6,6,5,5,4,4}
};

DDU8 RangeMaxQp420_10bpc[][23] = {
    {8,8,7,6,4,4,3,3,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {8,8,8,7,6,5,4,4,3,3,3,3,2,1,1,1,0,0,0,0,0,0,0},
    {9,9,9,8,8,7,6,5,4,3,3,3,3,3,2,1,1,1,0,0,0,0,0},
    {10,10,10,9,9,8,7,6,5,4,4,3,3,3,3,2,1,1,1,1,1,0,0},
    {11,11,11,10,10,8,7,6,5,4,4,4,4,3,3,3,2,2,2,1,1,1,0},
    {11,11,11,10,10,9,8,7,6,6,6,5,4,4,3,3,2,2,2,2,2,1,1},
    {11,11,11,11,11,10,9,8,7,7,7,6,5,5,4,3,3,3,3,2,2,2,1},
    {12,12,12,12,12,11,10,9,8,8,8,7,6,5,5,4,3,3,3,2,2,2,1},
    {13,13,13,12,12,11,10,10,9,9,8,8,7,7,6,5,4,4,3,3,3,2,2},
    {14,14,13,13,13,12,11,10,9,9,9,8,8,7,7,6,5,4,4,3,3,2,2},
    {14,14,14,13,13,12,12,11,10,10,9,9,8,8,7,6,5,5,4,4,3,3,2},
    {15,15,14,14,13,13,12,11,11,10,10,9,9,8,7,7,6,5,5,4,4,3,2},
    {15,15,15,14,13,13,13,12,11,11,10,9,9,8,8,7,6,5,5,4,4,3,2},
    {17,16,16,15,14,14,13,12,12,11,10,10,9,8,8,7,6,6,5,4,4,3,3},
    {18,17,17,16,15,15,14,13,13,12,11,11,11,10,9,9,8,7,7,6,6,5,5}
};

DDU8 RangeMinQp420_12bpc[][29] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {4,4,4,4,4,4,3,3,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {9,8,8,7,7,6,5,5,4,4,4,4,3,3,3,2,2,1,0,0,0,0,0,0,0,0,0,0,0},
    {10,9,9,8,8,8,7,7,6,6,6,5,5,4,4,3,2,2,1,1,1,0,0,0,0,0,0,0,0},
    {11,10,10,10,10,9,9,8,7,6,6,6,6,5,5,4,3,3,3,2,2,1,0,0,0,0,0,0,0},
    {11,11,11,11,11,10,10,9,9,9,9,8,7,6,5,5,4,4,3,3,3,2,1,1,0,0,0,0,0},
    {11,11,11,11,11,11,10,10,9,9,9,8,8,7,6,5,5,5,5,4,3,3,2,1,1,1,1,1,0},
    {11,11,11,11,11,11,11,10,10,10,10,9,8,8,8,7,6,6,5,4,4,3,2,2,1,1,1,1,1},
    {11,11,11,11,11,11,11,11,11,11,10,10,9,9,8,8,7,7,6,5,5,4,4,2,2,1,1,1,1},
    {11,11,11,11,11,11,11,11,11,11,11,10,10,9,9,8,8,7,7,6,5,4,4,3,2,2,1,1,1},
    {13,13,13,13,13,13,13,12,12,12,11,11,10,10,9,9,8,8,7,7,6,5,4,3,3,2,2,1,1},
    {13,13,13,13,13,13,13,13,13,12,12,12,12,11,10,10,9,8,8,7,7,6,5,4,3,3,2,2,1},
    {13,13,13,13,13,13,13,13,13,13,13,12,12,11,11,10,9,8,8,7,7,6,5,4,4,3,2,2,1},
    {15,15,15,15,15,15,15,15,15,14,13,13,12,11,11,10,9,9,8,8,7,6,6,5,4,4,3,3,2},
    {21,20,20,19,18,18,17,16,16,15,14,14,14,13,12,12,11,10,10,10,9,8,8,7,6,6,5,5,4}
};

DDU8 RangeMaxQp420_12bpc[][29] = {
    {11,10,9,8,6,6,5,5,4,3,3,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    {12,11,11,10,9,8,7,7,6,6,5,5,4,3,3,2,1,1,1,1,1,1,1,0,0,0,0,0,0},
    {13,12,12,11,11,10,9,8,7,6,6,6,5,5,4,3,3,2,1,1,1,1,1,0,0,0,0,0,0},
    {14,13,13,12,12,11,10,9,8,7,7,6,6,5,5,4,3,3,2,2,2,1,1,1,0,0,0,0,0},
    {15,14,14,13,13,11,10,9,8,7,7,7,7,6,6,5,4,4,4,3,3,2,1,1,1,0,0,0,0},
    {15,15,15,14,14,13,12,11,10,10,10,9,8,7,6,6,5,5,4,4,4,3,2,2,1,1,0,0,0},
    {15,15,15,15,15,14,13,12,11,11,11,10,9,8,7,6,6,6,6,5,4,4,3,2,2,2,1,1,0},
    {16,16,16,16,16,15,14,13,12,12,12,11,10,9,9,8,7,7,6,5,5,4,3,3,2,2,2,1,1},
    {17,17,17,16,16,15,14,14,13,13,12,12,11,11,10,9,8,8,7,6,6,5,5,3,3,2,2,1,1},
    {18,18,17,17,17,16,15,14,13,13,13,12,12,11,11,10,9,8,8,7,6,5,5,4,3,3,2,2,1},
    {18,18,18,17,17,16,16,15,14,14,13,13,12,12,11,10,9,9,8,8,7,6,5,4,4,3,3,2,2},
    {19,19,18,18,17,17,16,15,15,14,14,13,13,12,11,11,10,9,9,8,8,7,6,5,4,4,3,3,2},
    {19,19,19,18,17,17,17,16,15,15,14,13,13,12,12,11,10,9,9,8,8,7,6,5,5,4,3,3,2},
    {21,20,20,19,18,18,17,16,16,15,14,14,13,12,12,11,10,10,9,9,8,7,7,6,5,5,4,4,3},
    {22,21,21,20,19,19,18,17,17,16,15,15,15,14,13,13,12,11,11,11,10,9,9,8,7,7,6,6,5}
};


// Note: Don't leave color characteristics offsets [0x19-0x22] as 0. We use a sample
// color characteristic values from VESA EDID example. These values are not actually
// used anywhere. Even if they are, using 0 values are just as bad as incorrect values.
static DDU8 FakeEdidChroma[] = { 0x00, 0x00, 0xa0, 0x57, 0x49, 0x9b, 0x26, 0x10, 0x48, 0x4f };
static DDU8 FakeEdidPNPId[10] = { 0x36, 0x7F, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0c };

// clang-format on

// Represents Max HDMI 2.1 Per Lane Link BW in Mbps
// 0 - FRL not supported
// 1 - 3 Gbps with 3 Lanes
// 2 - 3 Gbps with 3 Lanes + 6 Gbps with 3 lanes
// 3 - above "2" + 6 Gbps with 4 lanes
// 4 - above "3" + 8 Gbps with 4 lanes
// 5 - above "4" + 10 Gbps with 4 lanes
// 6 - above "5" + 12 Gbps with 4 lanes
static const DDU32 Hdmi2_1_FrlLinkBwMbps[] = { 0, 3000, 6000, 6000, 8000, 10000, 12000 };
/**
 * @brief Method to get HDMI FRL Link bandwidth in Mbps
 *          This is called from Display info parser as well as DPCD parser code.
 *
 * @param MaxFrlRateIndex
 * @return FRL Link bandwidth in Mbps
 */
DDU32 DisplayInfoRoutinesGetHdmiLinkBwInMbps(DDU8 MaxFrlRateIndex)
{
    if (MaxFrlRateIndex < NUM_FRL_LINK_RATES)
    {
        return Hdmi2_1_FrlLinkBwMbps[MaxFrlRateIndex];
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Method to get TIMING_INFO from DTD.
 *
 * @param pDTD
 * @param pTimingInfo
 * @return DDSTATUS Returns an empty timing info if failed
 */
DDSTATUS DisplayInfoRoutinesGetTimingFromDTD(EDID_DTD_TIMING* pDTD, DD_TIMING_INFO* pTimingInfo)
{
    DDU32 XRes, YRes, RRate;
    DDU32 HTotal, VTotal, PixelClock, HBlank, VBlank, HSPW, VSPW;
    DDU32 HSO, VSO;

    DD_ZERO_MEM(pTimingInfo, sizeof(DD_TIMING_INFO));

    XRes = ((pDTD->Timing.HActiveHighBits << 8) | pDTD->Timing.HActiveLowBits);
    YRes = ((pDTD->Timing.VActiveHighBits << 8) | pDTD->Timing.VActiveLowBits);

    PixelClock = pDTD->Timing.PixelClock * 10000;

    if ((PixelClock == 0) || (XRes == 0) || (YRes == 0))
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid DTD in the Edid");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    //    Calculate refresh rate
    HBlank = (pDTD->Timing.HBlankHighBits << 8) + pDTD->Timing.HBlanklowBits;
    HTotal = XRes + HBlank;
    VBlank = (pDTD->Timing.VBlankHighBits << 8) + pDTD->Timing.VBlankLowBits;
    if (pDTD->IsInterlaced)
    {
        YRes = YRes * 2;
        VBlank = VBlank * 2;
    }
    VTotal = YRes + VBlank;
    RRate = (PixelClock + (HTotal * VTotal / 2)) / (HTotal * VTotal);

    if (pDTD->IsInterlaced)
    {
        RRate = RRate * 2;
    }

    //  Calculate sync pulse width
    HSPW = (pDTD->Timing.HSyncPulseWidthHighBits << 8) | pDTD->Timing.HSyncPulseWidthLowBits;
    VSPW = (pDTD->Timing.VSyncPulseWidthHighBits << 4) | pDTD->Timing.VSyncPulseWidthLowBits;

    // Fix for 1940035: For interlaced mode, sync pulse width is doubled.
    if (pDTD->IsInterlaced)
        VSPW = VSPW * 2;

    //    Calculate sync offset
    HSO = (pDTD->Timing.HSyncOffsetHighBits << 8) | pDTD->Timing.HSyncOffsetLowBits;
    VSO = (pDTD->Timing.VSyncOffsetHighBits << 4) | pDTD->Timing.VSyncOffsetLowBits;
    if (pDTD->IsInterlaced)
        VSO = VSO * 2;

    if ((HSO + HSPW) > HBlank)
    {
        // DISP_DBG_MSG(GFXDBG_CRITICAL,"::Critical::%s::Invalid DTD ((Horizontal Sync Offset+ Horizontal Sync Pulse Width)> Horizontal Blanking Interval)",__FUNCTION__);
        // break;
        // Even though this DTD is wrong, add this timing as there are many LVDS panels with this kind of wrong timing (working with this wrong timing)
        // Non-LVDS panels blank-out with this kind of timing, so if Sync Offset+Sync Pulse exceeds Blanking, confine Sync Pulse to within Blanking
        // i.e., ulHSPW = ulHBlank - ulHSO
        HSPW = HBlank - HSO;
    }

    if ((VSO + VSPW) > VBlank)
    {
        // DISP_DBG_MSG(GFXDBG_CRITICAL,"::Critical::%s::Invalid DTD ((Vertical Sync Offset+ Vertical Sync Pulse Width)> Vertical Blanking Interval)",__FUNCTION__);
        // break;
        // Even though this DTD is wrong, add this timing as there are many LVDS panels with this kind of wrong timing (working with this wrong timing)
        // Non-LVDS panels blank-out with this kind of timing, so if Sync Offset+Sync Pulse exceeds Blanking, confine Sync Pulse to within Blanking
        // i.e., ulVSPW = ulVBlank - ulVSO
        VSPW = VBlank - VSO;
    }
    //    Update mode timing information
    pTimingInfo->DotClockInHz = PixelClock;
    pTimingInfo->HTotal = HTotal;
    pTimingInfo->HActive = XRes;
    pTimingInfo->HBlankStart = XRes + pDTD->HBorder;
    pTimingInfo->HBlankEnd = HTotal - pDTD->HBorder - 1;
    pTimingInfo->HSyncStart = XRes + HSO;
    pTimingInfo->HSyncEnd = pTimingInfo->HSyncStart + HSPW - 1;
    pTimingInfo->HRefresh = PixelClock / HTotal;

    pTimingInfo->VTotal = VTotal;
    pTimingInfo->VActive = YRes;
    pTimingInfo->VBlankStart = YRes + pDTD->VBorder;
    pTimingInfo->VBlankEnd = VTotal - pDTD->VBorder - 1;
    pTimingInfo->VSyncStart = YRes + VSO;

    pTimingInfo->VSyncEnd = pTimingInfo->VSyncStart + VSPW - 1;
    pTimingInfo->VRoundedRR = RRate;

    pTimingInfo->IsInterlaced = pDTD->IsInterlaced;

    // For Interlaced modes, pTimingInfo->VTotal is 1 less than actual number of lines. So add 1 here.
    if (pTimingInfo->IsInterlaced)
    {
        pTimingInfo->VTotal++;
        pTimingInfo->VBlankEnd++;
    }

    //    The sync polarity definition of TIMING_INFO is reversed from that of DTD's
    if (pDTD->HSyncPolarity == FALSE)
    {
        pTimingInfo->HSyncPolarity = TRUE;
    }
    if (pDTD->VSyncPolarity == FALSE)
    {
        pTimingInfo->VSyncPolarity = TRUE;
    }

    return DDS_SUCCESS;
}
/**
 * @brief Method to get TIMING_INFO from GDTD.
 *
 * @param pGenericDisplayTimingData
 * @param pTimingInfo
 * @return DDSTATUS Returns an empty timing info if failed
 */
DDSTATUS DisplayInfoRoutinesGetTimingFromGenericDTD(GENERIC_DISPLAY_TIMING_DATA* pGenericDisplayTimingData, DD_TIMING_INFO* pTimingInfo)
{
    DDU32 HActive, VActive, RRate;
    DDU32 HTotal, VTotal, PixelClockInHz, HBlank, VBlank;

    DD_ZERO_MEM(pTimingInfo, sizeof(DD_TIMING_INFO));

    HActive = pGenericDisplayTimingData->HActive;
    VActive = pGenericDisplayTimingData->VActive;

    PixelClockInHz = pGenericDisplayTimingData->PixelClockInKhz * 1000;

    if ((PixelClockInHz == 0) || (HActive == 0) || (VActive == 0))
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid DTD in the Edid");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    // Calculate refresh rate
    HBlank = pGenericDisplayTimingData->HBlank;
    HTotal = HActive + HBlank;
    VBlank = pGenericDisplayTimingData->VBlank;
    VTotal = VActive + VBlank;
    RRate = (PixelClockInHz + (HTotal * VTotal / 2)) / (HTotal * VTotal);

    // Update mode timing information
    pTimingInfo->DotClockInHz = PixelClockInHz;
    pTimingInfo->HTotal = HTotal;
    pTimingInfo->HActive = HActive;
    pTimingInfo->HBlankStart = HActive;
    pTimingInfo->HBlankEnd = HTotal - 1; // -1 as the value is stored as 0 based index
    pTimingInfo->HSyncStart = pGenericDisplayTimingData->HActive + pGenericDisplayTimingData->HFrontPorch;
    pTimingInfo->HSyncEnd = pTimingInfo->HSyncStart + pGenericDisplayTimingData->HSync - 1; // // -1 as the value is stored as 0 based index
    pTimingInfo->HRefresh = PixelClockInHz / HTotal;

    pTimingInfo->VTotal = VTotal;
    pTimingInfo->VActive = VActive;
    pTimingInfo->VBlankStart = VActive;
    pTimingInfo->VBlankEnd = VTotal - 1; // -1 as the value is stored as 0 based index
    pTimingInfo->VSyncStart = pGenericDisplayTimingData->VActive + pGenericDisplayTimingData->VFrontPorch;
    pTimingInfo->VSyncEnd = pTimingInfo->VSyncStart + pGenericDisplayTimingData->VSync - 1; // -1 as the value is stored as 0 based index
    pTimingInfo->VRoundedRR = RRate;

    // The sync polarity definition of TIMING_INFO is reversed from that of DTD's
    if (pGenericDisplayTimingData->HSyncPolarity == FALSE)
    {
        pTimingInfo->HSyncPolarity = TRUE;
    }
    if (pGenericDisplayTimingData->VSyncPolarity == FALSE)
    {
        pTimingInfo->VSyncPolarity = TRUE;
    }

    return DDS_SUCCESS;
}

/**
 * @brief Method to get  DTD from TIMING_INFO. Returns FALSE if failed
 * Source: TranslateTiming2DTD() method in UAIM.
 *
 * @param pTimingInfo
 * @param pDTD
 * @return DDSTATUS
 */
DDSTATUS DisplayInfoRoutinesGetDTDFromTimingInfo(DD_TIMING_INFO* pTimingInfo, EDID_DTD_TIMING* pDTD)
{
    DDU32 HActive = 0;
    DDU32 HBlank = 0;
    DDU32 VActive = 0;
    DDU32 VBlank = 0;
    DDU32 HSyncOffset = 0;
    DDU32 HSyncPulseWidth = 0;
    DDU32 VSyncOffset = 0;
    DDU32 VSyncPulseWidth = 0;

    DDASSERT(pTimingInfo);
    DDASSERT(pDTD);

    DD_ZERO_MEM(pDTD, sizeof(EDID_DTD_TIMING));

    if (pTimingInfo->HActive == 0 || pTimingInfo->VActive == 0 || pTimingInfo->DotClockInHz == 0)
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid inputs!");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    HActive = pTimingInfo->HActive;
    HBlank = pTimingInfo->HTotal - pTimingInfo->HActive;
    VActive = pTimingInfo->VActive;
    if (pTimingInfo->IsInterlaced)
    {
        VActive = (VActive + 1) >> 1; // +1 to take care of any
    }
    VBlank = pTimingInfo->VTotal - pTimingInfo->VActive;

    // For Interlaced modes, pTimingInfo->VTotal is 1 less than actual number of lines. So not adding 1 here while converting
    if (pTimingInfo->IsInterlaced)
    {
        VBlank = (VBlank) >> 1; // Not adding 1 as we assume +1 was already done for Vblank when we got TimingInfo.
    }

    HSyncOffset = (pTimingInfo->HSyncStart - pTimingInfo->HActive);
    HSyncPulseWidth = (pTimingInfo->HSyncEnd - pTimingInfo->HSyncStart + 1);
    VSyncOffset = (pTimingInfo->VSyncStart - pTimingInfo->VActive);
    if (VSyncOffset > 0x3F)
    {
        // this condition is new! DTD cannot take more than 6bits (0x3F) for VSO. Timing_Info might be bad
        VSyncOffset = 0x3F; // set to the max!
    }
    if (pTimingInfo->IsInterlaced)
    {
        VSyncOffset = (VSyncOffset + 1) >> 1;
    }

    VSyncPulseWidth = (pTimingInfo->VSyncEnd - pTimingInfo->VSyncStart + 1);

    if (pTimingInfo->IsInterlaced)
    {
        VSyncPulseWidth = (VSyncPulseWidth + 1) >> 1;
    }

    // Fill out the DTD part #1
    pDTD->Timing.PixelClock = (DDU16)(pTimingInfo->DotClockInHz / 10000); // Hz to 10KHz

    pDTD->Timing.HActiveLowBits = (DDU8)(HActive & 0xFF);
    pDTD->Timing.HBlanklowBits = (DDU8)(HBlank & 0xFF);
    pDTD->Timing.HActiveHBlankHighBits = (DDU8)(((HActive >> 8) << 4) | (HBlank >> 8));
    pDTD->Timing.VActiveLowBits = (DDU8)(VActive & 0xFF);
    pDTD->Timing.VBlankLowBits = (DDU8)(VBlank & 0xFF);
    pDTD->Timing.VActiveVBlankHighBits = (DDU8)(((VActive >> 8) << 4) | (VBlank >> 8));

    // Fill out the DTD part #1
    pDTD->Timing.HSyncOffsetLowBits = (DDU8)(HSyncOffset & 0xFF);
    pDTD->Timing.HSyncPulseWidthLowBits = (DDU8)(HSyncPulseWidth & 0xFF);

    pDTD->Timing.VSyncOffsetVSyncPulseWidthLowBits = (DDU8)(((VSyncOffset & 0x0F) << 4) | (VSyncPulseWidth & 0x0F));

    pDTD->Timing.VSyncPulseWidthHighBits = (DDU8)((VSyncPulseWidth >> 4) & 0x03);
    pDTD->Timing.VSyncOffsetHighBits = (DDU8)((VSyncOffset >> 4) & 0x03);
    pDTD->Timing.HSyncPulseWidthHighBits = (DDU8)((HSyncPulseWidth >> 8) & 0x03);
    pDTD->Timing.HSyncOffsetHighBits = (DDU8)((HSyncOffset >> 8) & 0x03);

    // Tibet 1549938 - The polarity of H/Vsync is fixed and not effected by resolution on DVI monitor
    // Fix - Setup the interlace and polarity flags in DTD using TimingInfo structure
    if (pTimingInfo->IsInterlaced)
    {
        pDTD->IsInterlaced = 1;
    }

    // Let bits[6:5] = [0:0] = normal display, no stereo
    pDTD->Stereo1 = 0;
    pDTD->Stereo2 = 0;

    pDTD->Flags |= BIT4 | BIT3; // [4:3] = [1:1] = digital separate

    if (pTimingInfo->VSyncPolarity == FALSE)
    {
        pDTD->Flags |= BIT2; // Set + VSync Polarity
    }

    if (pTimingInfo->HSyncPolarity == FALSE)
    {
        pDTD->Flags |= BIT1; // Set + HSync Polarity
    }

    // Get border as well
    pDTD->HBorder = (DDU8)(pTimingInfo->HBlankStart - pTimingInfo->HActive);
    // pDTD->ucVBorder = (DDU8) (pTimingInfo->VBlankStart - pTimingInfo->VActive);
    pDTD->VBorder = (DDU8)(pTimingInfo->VTotal - pTimingInfo->VBlankEnd - 1);

    return DDS_SUCCESS;
}

/**
 * @brief Method to check if a Timing Info can be fit into EDID base block
 * Source: MipiDsiCreateFakeEDID.
 *
 * @param pTimingInfo
 * @return boolean
 */
DD_BOOL DisplayInfoRoutinesCanTimingBeFitInEdid(DD_TIMING_INFO* pTimingInfo)
{
    DDU32 HBlank;
    DDU32 HSync;
    DDU32 HFP;
    DDU32 VBlank;
    DDU32 VSync;
    DDU32 VFP;

    HBlank = pTimingInfo->HTotal - pTimingInfo->HActive;
    HSync = pTimingInfo->HSyncEnd - pTimingInfo->HSyncStart + 1;
    HFP = pTimingInfo->HSyncStart - pTimingInfo->HActive;

    VBlank = pTimingInfo->VTotal - pTimingInfo->VActive;
    VSync = pTimingInfo->VSyncEnd - pTimingInfo->VSyncStart + 1;
    VFP = pTimingInfo->VSyncStart - pTimingInfo->VActive;

    if (pTimingInfo->DotClockInHz > EDID_MAX_SUPPORTED_PIXEL_CLOCK_HZ)
    {
        return FALSE;
    }

    if ((pTimingInfo->HActive > EDID_MAX_SUPPORTED_HACTIVE) || (pTimingInfo->VActive > EDID_MAX_SUPPORTED_VACTIVE))
    {
        return FALSE;
    }

    if ((HBlank > EDID_MAX_SUPPORTED_HBLANK) || (VBlank > EDID_MAX_SUPPORTED_VBLANK))
    {
        return FALSE;
    }

    if ((HSync > EDID_MAX_SUPPORTED_HSYNC) || (VSync > EDID_MAX_SUPPORTED_VSYNC))
    {
        return FALSE;
    }

    if ((HFP > EDID_MAX_SUPPORTED_HFRONTPORCH) || (VFP > EDID_MAX_SUPPORTED_VFRONTPORCH))
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Method to update Custom Timing Info for reporting Fake EDID with reduced active region to support
 *        Screen misalignment adjustment in Dual LFP Formfactor devices (VSRI -4335)
 * Source: MipiDsiCreateFakeEDID.
 *
 * @param pTimingInfo
 * @param VDelta
 * @param HDelta
 * @return void
 */
void DisplayInfoRoutinesUpdateTimingforCustomAlignment(DD_TIMING_INFO* pTimingInfo, DDU32 VDelta, DDU32 HDelta)
{
    // Total Border should be less than Active Region. Asserting if VBT data says otherwise
    DDASSERT(VDelta < DD_MAX_BORDER_FOR_CUSTOM_ALIGNMENT);

    // For these modified timings, the below adjustments are done such that VFP, HFP / VSync, HSync remain the same.
    // While calculating Fake Edid to report to OS, To keep the VTotal & HTotal constant, VBP is increased, so that(VBlank + VActive) & (HBlank + HActive) remains the same as VTotal & HTotal.
    // We have to keep VTotal & HTotal constant so that pixel clock doesn't change b/w old and new timings and RR remains the same
    pTimingInfo->VActive -= VDelta;
    pTimingInfo->VBlankStart -= VDelta;
    pTimingInfo->VSyncStart -= VDelta;
    pTimingInfo->VSyncEnd -= VDelta;

    DDASSERT(HDelta < DD_MAX_BORDER_FOR_CUSTOM_ALIGNMENT);

    pTimingInfo->HActive -= HDelta;
    pTimingInfo->HBlankStart -= HDelta;
    pTimingInfo->HSyncStart -= HDelta;
    pTimingInfo->HSyncEnd -= HDelta;

    return;
}

/**
 * @brief Method to get  DTD from TIMING_INFO. Returns FALSE if failed
 * Source: TranslateTiming2DTD() method in UAIM.
 *
 * @param pCreateGTFTimingArgs
 * @return DDSTATUS
 */
DDSTATUS DisplayInfoRoutinesCreateGTFTiming(CREATE_GTF_TIMING_ARGS* pCreateGTFTimingArgs)
{
    DDASSERT(pCreateGTFTimingArgs);

    if ((pCreateGTFTimingArgs->RRate == 0) || (pCreateGTFTimingArgs->XRes == 0) || (pCreateGTFTimingArgs->YRes == 0))
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid inputs!");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    // fixed defines as per VESA spec
    // double MarginPerct = 1.80;// size of top and bottom overscan margin as percentage of active vertical image
    double CellGran = 8.0;   // cell granularity
    DDU32 MinPorch = 1;     // 1 line/char cell
    DDU32 VSyncRqd = 3;     // width of vsync in lines
    float HSynchPerct = 8.0;   // width of hsync as a percentage of total line period
    float Min_Vsync_BP = 550.0; // Minimum time of vertical sync + back porch interval (us).
    double BlankingGradient_M = 600.0; // The blanking formula gradient
    double BlankingOffset_C = 40.0;  // The blanking formula offset
    double BlankingScaling_K = 128.0; // The blanking formula scaling factor
    double BlankingScalWeighing_J = 20.0;  // The blanking formula scaling factor weighting
    // Spec defination ends here

    // Calculation of C',M'
    // C' = Basic offset constant
    // M' = Basic gradient constant
    double CPrime = (BlankingOffset_C - BlankingScalWeighing_J) * (BlankingScaling_K) / 256.0 + BlankingScalWeighing_J;
    double MPrime = BlankingScaling_K / 256 * BlankingGradient_M;

    DD_BOOL IsInterLaced = pCreateGTFTimingArgs->IsProgressiveMode ? FALSE : TRUE;

    // calculation of timing paramters
    // Step 1: Round the Horizontal Resolution to nearest 8 pixel
    DDU32 HPixels = pCreateGTFTimingArgs->XRes;
    DDU32 HPixelsRnd = (DDU32)(DD_ROUNDTONEARESTINT(HPixels / CellGran) * CellGran);

    // Step 2: Calculate Vertical line rounded to nearest integer
    float VLines = (float)pCreateGTFTimingArgs->YRes;
    DDU32 VLinesRnd = (DDU32)DD_ROUNDTONEARESTINT(IsInterLaced ? VLines / 2 : VLines);

    // Step 3: Find the field rate required (only useful for interlaced)
    float VFieldRateRqd = (float)(IsInterLaced ? pCreateGTFTimingArgs->RRate * 2 : pCreateGTFTimingArgs->RRate);

    // Step 4 and 5: Calculate top and bottom margins, we assumed zero for now
    // assumption top/bottom margins are unused, if a requirement comes for use of
    // margin then it has to added as function input parameter
    DDU32 TopMargin = 0;
    DDU32 BottomMargin = 0;

    // Step 6: If Interlaced set this value which is used in the other calculations
    float InterLaced = (float)(IsInterLaced ? 0.5 : 0);

    // Step 7: Estimate the Horizontal period in usec per line
    float HPeriodEst = ((1 / VFieldRateRqd) - (Min_Vsync_BP / 1000000)) / (VLinesRnd + 2 * TopMargin + MinPorch + InterLaced) * 1000000;

    // Step 8: Find the number of lines in V sync + back porch
    DDU32 VSync_BP = (DDU32)DD_ROUNDTONEARESTINT(Min_Vsync_BP / HPeriodEst);

    // Step 9: Find the number of lines in V back porch alone
    DDU32 VBackPorch = VSync_BP - VSyncRqd;

    // Step 10: Find the total number of lines in vertical field
    float FlTotalVLines = VLinesRnd + TopMargin + BottomMargin + VSync_BP + InterLaced + MinPorch;

    // Step 11: Estimate the vertical field frequency
    float VFieldRateEst = 1 / HPeriodEst / FlTotalVLines * 1000000;

    // Step 12: Find actual horizontal period
    float HPeriod = HPeriodEst / (VFieldRateRqd / VFieldRateEst);

    // Step 13: Find the actual vertical field frequency
    float VFieldRate = (1 / HPeriod / FlTotalVLines) * 1000000;

    // Step 14: Find the actual vertical frame frequency
    float VFrameRate = IsInterLaced ? VFieldRate / 2 : VFieldRate;

    // Step 15,16: Find the number of pixels in the left, right margins, we assume they are zero
    DDU32 LeftMargin = 0, RightMargin = 0;

    // Step 17: Find total number of active pixels in one line plus the margins
    DDU32 TotalActivePixels = HPixelsRnd + RightMargin + LeftMargin;

    // Step 18: Find the ideal blanking duty cycle form blanking duty cycle equation
    float IdealDutyCycle = (float)(CPrime - (MPrime * HPeriod / 1000));

    // Step 19: Find the number of pixels in the blanking time to the nearest double charactr cell
    DDU32 HBlankPixels = (DDU32)(DD_ROUNDTONEARESTINT(TotalActivePixels * IdealDutyCycle / (100 - IdealDutyCycle) / (2 * CellGran)) * (2 * CellGran));

    // Step 20: Find total number of pixels in one line
    DDU32 TotalPixels = TotalActivePixels + HBlankPixels;

    // Step 21: Find pixel clock frequency
    // currently we are taking value till 3 places after decimal
    // If the precision need to be increased to 4 places of decimal replace the
    // PRECISION3DEC by PRECISION4DEC
    DDU32 DecPrecisonPoint = DD_PRECISION3DEC;
    // Get the pixel clcok till 3 places of decimals
    DDU32 PixelClock = (DDU32)DD_ROUNDTONEARESTINT((TotalPixels / HPeriod) * DecPrecisonPoint);

    // Step 22:  Get the horizontal frequency
    float HFreq = (1000 / HPeriod) * 1000;

    DDU32 HSyncPixles = (DDU32)(DD_ROUNDTONEARESTINT((TotalPixels / CellGran) * (HSynchPerct / 100)) * CellGran);
    DDU32 HSyncStart = TotalActivePixels + (HBlankPixels / 2) - HSyncPixles;
    DDU32 HSyncEnd = TotalActivePixels + (HBlankPixels / 2) - 1;
    // Gtf calculations ends here

    // This is the per frame total no of vertical lines
    DDU32 TotalVLines = (DDU32)DD_ROUNDTONEARESTINT(IsInterLaced ? 2 * FlTotalVLines : FlTotalVLines);

    // This is done to get the pixel clock in Hz
    // for Interlaced modes, SB needs Dot Clock to be halved..
    pCreateGTFTimingArgs->pTimingInfo->DotClockInHz = IsInterLaced ? (PixelClock * (1000000 / DecPrecisonPoint)) / 2 : (PixelClock * (1000000 / DecPrecisonPoint)); // from step 21
    pCreateGTFTimingArgs->pTimingInfo->HTotal = TotalPixels;                                                                                                  // from step 20
    pCreateGTFTimingArgs->pTimingInfo->HActive = TotalActivePixels;                                                                                            // from step 17

    pCreateGTFTimingArgs->pTimingInfo->HBlankStart = TotalActivePixels;
    pCreateGTFTimingArgs->pTimingInfo->HBlankEnd = TotalPixels - 1;

    pCreateGTFTimingArgs->pTimingInfo->HSyncStart = HSyncStart; // from step 23
    pCreateGTFTimingArgs->pTimingInfo->HSyncEnd = HSyncEnd;
    // for Interlaced modes, SB needs H Refresh Rate to be halved..
    pCreateGTFTimingArgs->pTimingInfo->HRefresh = (DDU32)(IsInterLaced ? DD_ROUNDTONEARESTINT(HFreq) / 2 : DD_ROUNDTONEARESTINT(HFreq)); // from step 22

    // calculate in case of interlaced the frame based parameters
    // instead of per field basis
    pCreateGTFTimingArgs->pTimingInfo->VTotal = TotalVLines;                              // from step 10
    pCreateGTFTimingArgs->pTimingInfo->VActive = IsInterLaced ? VLinesRnd * 2 : VLinesRnd; // from step 2

    pCreateGTFTimingArgs->pTimingInfo->VBlankStart = IsInterLaced ? VLinesRnd * 2 : VLinesRnd;
    pCreateGTFTimingArgs->pTimingInfo->VBlankEnd = TotalVLines - 1;

    // The interlaced paramter is also taken in to account to calculate the
    // sync start and end if the timings are to be generated for interlaced
    pCreateGTFTimingArgs->pTimingInfo->VSyncStart = (IsInterLaced ? VLinesRnd * 2 : VLinesRnd) + MinPorch + (DDU32)DD_ROUNDTONEARESTINT(InterLaced);
    pCreateGTFTimingArgs->pTimingInfo->VSyncEnd = TotalVLines - (IsInterLaced ? 2 * VBackPorch : VBackPorch) - 3 * (DDU32)DD_ROUNDTONEARESTINT(InterLaced) - 1;
    pCreateGTFTimingArgs->pTimingInfo->VRoundedRR = (DDU32)DD_ROUNDTONEARESTINT(VFrameRate); // from step 14

    // Interlaced calculations are FIELD based instead of FRAME based
    // field related calculation are not done currently in the code. In future if
    // this function is to be used for calculating the interlaced timings following
    // parameters need to be added and pipe registers should be programmed bsaed upon
    // the even field parameters.Parameters dependent on this are
    // 1.Vertical Blank Time
    // 2.Vertical Sync Time
    // 3.Vertical Back Porch
    // vertical field period = FlTotalVLines*HPeriod/1000
    // odd  vertical blanking lines = VSync_BP+MinPorch
    // odd  vertical blanking time = (VSync_BP+MinPorch)*HPeriod/1000
    // even vertical blanking lines = VSync_BP+MinPorch + 2*InterLaced
    // odd vertical back porch time = VBackPorch+InterLaced
    // even vertical back porch time = VBackPorch
    // odd  vertical front porch lines = (MinPorch+InterLaced)
    // even vertical front porch lines = (MinPorch)

    // Flags info
    pCreateGTFTimingArgs->pTimingInfo->HSyncPolarity = 1; // Negative
    pCreateGTFTimingArgs->pTimingInfo->VSyncPolarity = 0; // Positive
    pCreateGTFTimingArgs->pTimingInfo->IsInterlaced = IsInterLaced;

    if (pCreateGTFTimingArgs->pTimingInfo->HSyncStart < pCreateGTFTimingArgs->pTimingInfo->HActive)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    return DDS_SUCCESS;
}

/**
 * @brief Method to create CVT timing
 * Reference documents used are   CVT Standard (version 1.1) pdf file & Spread Sheet provided by VESA for CVT timing calculation.
 * In current version of CVT, there is no way of specifying values for input parameters
 * bMargin_Req & IsInterLaced. So currently, these should be assigned FALSE. Check in future versions of CVT.
 * @param pCreateCvtTimingArgs
 * @return DDSTATUS
 */
DDSTATUS DisplayInfoRoutinesCreateCVTTiming(CREATE_CVT_TIMING_ARGS* pCreateCvtTimingArgs)
{
    float FlActHFreq;
    float FlInterLaced;
    float FlActVFrameRate;

    DDASSERT(pCreateCvtTimingArgs);

    pCreateCvtTimingArgs->IsMargin_Req = FALSE; // Not currently supported in CVT descriptor, so forcefully making it zero.

    if ((pCreateCvtTimingArgs->RRate == 0) || (pCreateCvtTimingArgs->XRes == 0) || (pCreateCvtTimingArgs->YRes == 0))
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid inputs!");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    // fixed definations as per VESA spec

    // Constants which are same for Both Type of blankings
    const float MarginPerct = 1.8f;  // margin percentage for all sides.
    const DDU32 CellGran = 8;     // cell granularity
    const DDU32 Min_V_FPorch = 3;     // Variable for Both type of Blankings. Minimum V_Front Porch (in lines) given by spec
    const float ClockStep = 0.250; // Pixel clock freq should be integer multiple of this step.

    // Constants related to Normal-Blanking Calculations
    const DDU32 Min_V_BPorch = 6;     // minimum default back porch(in lines)
    const DDU32 HSynchPerct = 8;     // width of hsync as a percentage of total line period
    const float Min_Vsync_BP = 550.0; // Minimum time of vertical sync + back porch interval (us).
    const float BlankingGradient_M = 600.0; // The blanking formula gradient
    const float BlankingOffset_C = 40.0;  // The blanking formula offset
    const float BlankingScaling_K = 128.0; // The blanking formula scaling factor
    const float BlankingScalWeighing_J = 20;    // The blanking formula scaling factor weighting

    // For Reduced Blanking (RB)
    float RB_Min_VBlank = 460.0; // In RB, minimum V Blanking period is 460 uSec.
    DDU32 RB_Min_V_BPorch = 6;     // Minimum V_Back Porch (in lines) given by spec

    // Common Variables in Both Type of Blankings.
    DDU32 VSyncRqd = 0;
    DDU32 HBlankPixels = 0; // In RB, Horizontal Blanking is fixed to 160 pixel clocks.
    float HPeriodEst = 0, TotalVLines = 0;
    DDU32 Act_VB_Lines = 0, TotalPixels = 0;
    float Act_Pixel_Freq = 0;
    float Act_H_Freq = 0, Act_VFieldRate = 0, Act_VFrameRate = 0;
    DDU32 HSyncPixels = 0; // In RB, H sync width is fixed to 32 pixel clocks.

    // For Normal Blanking
    DDU32 VSync_BP_Est = 0, VSync_BP = 0, VBackPorch = 0;
    float IdealDutyCycle = 0;

    // For Reduced Blanking
    DDU32 RB_VB_Lines = 0, RB_Min_VB_Lines = 0, RB_BackPorch = 0;

    // Spec defination ends here

    // Calculation of C',M'
    // C' = Basic offset constant
    // M' = Basic gradient constant

    float CPrime = (BlankingOffset_C - BlankingScalWeighing_J) * (BlankingScaling_K) / 256 + BlankingScalWeighing_J;
    float MPrime = BlankingScaling_K / 256 * BlankingGradient_M;

    //===========================================================================================
    // calculation of Common timing paramters
    //===========================================================================================

    // Step 1: Find the field rate required (only useful for interlaced)
    float VFieldRateRqd = (float)(pCreateCvtTimingArgs->IsInterLaced ? (pCreateCvtTimingArgs->RRate * 2) : pCreateCvtTimingArgs->RRate);

    // Step 2: Round the Horizontal Resolution to nearest 8 pixel--character cell multiple
    DDU32 HPixels = pCreateCvtTimingArgs->XRes;
    DDU32 HPixelsRnd = (DDU32)(DD_ROUNDTONEARESTINT((double)HPixels / CellGran) * CellGran);

    // Step 3: Calculate side Margins.Implement when required.
    DDU32 RightMargin = (DDU32)(pCreateCvtTimingArgs->IsMargin_Req ? (((HPixelsRnd * MarginPerct / 100) / CellGran) * CellGran) : 0);
    DDU32 LeftMargin = RightMargin;

    // Step 4: Find total number of active pixels in one line plus the margins
    DDU32 ActivePixels = (HPixelsRnd + RightMargin + LeftMargin);

    // Step 5: Calculate Vertical line rounded to nearest integer
    float VLines = (float)(pCreateCvtTimingArgs->YRes);
    DDU32 VLinesRnd = (DDU32)DD_ROUNDTONEARESTINT(pCreateCvtTimingArgs->IsInterLaced ? VLines / 2 : VLines);

    DDU32 Vpixels = (DDU32)DD_ROUNDTONEARESTINT(pCreateCvtTimingArgs->IsInterLaced ? VLinesRnd * 2 : VLinesRnd);

    // Step 6: Calculate top and bottom margins,
    DDU32 BottomMargin = pCreateCvtTimingArgs->IsMargin_Req ? (DDU32)(VLinesRnd * MarginPerct / 100) : 0;
    DDU32 TopMargin = BottomMargin;

    // Step 7: If Interlaced set this value which is used in the other calculations
    double InterLaced = pCreateCvtTimingArgs->IsInterLaced ? 0.5 : 0;

    // Calculation of VSync width
    if (HPixelsRnd == DD_CHECK_ASPECTRATIO((4 / 3), CellGran, Vpixels))
        VSyncRqd = 4;

    else if (HPixelsRnd == DD_CHECK_ASPECTRATIO((16 / 9), CellGran, Vpixels))
        VSyncRqd = 5;

    else if (HPixelsRnd == DD_CHECK_ASPECTRATIO((16 / 10), CellGran, Vpixels))
        VSyncRqd = 6;

    else if (HPixelsRnd == DD_CHECK_ASPECTRATIO((5 / 4), CellGran, Vpixels))
        VSyncRqd = 7;

    else if (HPixelsRnd == DD_CHECK_ASPECTRATIO((15 / 9), CellGran, Vpixels))
        VSyncRqd = 7;
    else
        VSyncRqd = 10; // This is the case for custom Aspect Ratios

    //=============================================================================================
    // Calculation of CVT--CRT Timings
    //==============================================================================================

    if (FALSE == pCreateCvtTimingArgs->IsRed_Blank_Req)
    {
        // Step 8: Estimate the Horizontal period in usec per line
        HPeriodEst = (float)(((1 / VFieldRateRqd) - (Min_Vsync_BP / 1000000)) / (VLinesRnd + 2 * TopMargin + Min_V_FPorch + InterLaced) * 1000000);

        // Step 9: Find the number of lines in V sync + back porch
        VSync_BP_Est = ((DDU32)(Min_Vsync_BP / HPeriodEst)) + 1;
        VSync_BP = VSync_BP_Est < (VSyncRqd + Min_V_BPorch) ? (VSyncRqd + Min_V_BPorch) : VSync_BP_Est;

        // Step 10: Find the number of lines in V back porch alone
        VBackPorch = VSync_BP - VSyncRqd;
        VBackPorch = (VBackPorch < 7) ? 7 : VBackPorch; // VBackPorch should always be >=7.
        VSync_BP = VSyncRqd + VBackPorch;

        // For Step 11: Find the total number of lines in vertical field
        TotalVLines = (float)(VLinesRnd + TopMargin * 2 + InterLaced + VSync_BP + Min_V_FPorch);

        // Step 12: Find the ideal blanking duty cycle form blanking duty cycle equation
        IdealDutyCycle = (CPrime - (MPrime * HPeriodEst / 1000));

        // Step 13: Find the number of pixels in the blanking time to the nearest float character cell
        if (IdealDutyCycle < 20)
            IdealDutyCycle = 20;

        HBlankPixels = (DDU32)(((DDU32)(ActivePixels * IdealDutyCycle / (100 - IdealDutyCycle) / (2 * CellGran))) * (2 * CellGran));

        // Step 14  Find total number of pixels in one line
        TotalPixels = ActivePixels + HBlankPixels;

        // Step 15: Find Pixel clock freq.
        Act_Pixel_Freq = (ClockStep * ((DDU32)((TotalPixels / HPeriodEst) / ClockStep)));

        // Calculation of Horizontal Sync width

        HSyncPixels = (DDU32)(DD_ROUNDTONEARESTINT((double)TotalPixels / CellGran * HSynchPerct / 100) * CellGran);

        // Flags info
        pCreateCvtTimingArgs->pTimingInfo->HSyncPolarity = 1; // Negative
        pCreateCvtTimingArgs->pTimingInfo->VSyncPolarity = 0; // Positive
    }
    else
    {
        //=========================================================================================
        // CVT-Reduced Blanking Timing Calculation
        //=========================================================================================

        HBlankPixels = 160;
        HSyncPixels = 32;

        // Step 8:Estimate Horizontal Period

        HPeriodEst = (float)(((1000000 / VFieldRateRqd) - RB_Min_VBlank) / (VLinesRnd + 2 * TopMargin));

        // Step 9:Find No. of Lines in Vertical Blanking
        RB_VB_Lines = ((DDU32)(RB_Min_VBlank / HPeriodEst)) + 1;

        // Step 10:Check Whether Vertical Blanking is sufficient.
        RB_Min_VB_Lines = VSyncRqd + Min_V_FPorch + RB_Min_V_BPorch;
        Act_VB_Lines = (RB_VB_Lines < RB_Min_VB_Lines) ? RB_Min_VB_Lines : RB_VB_Lines;
        // with Min_V_FPorch = 3, RB_Min_V_BPorch = 6, RB_Min_VB_Lines = 13 to 19 when VSyncRqd is 4 to 10
        // 1615: TRUE:  Act_VB_Lines can be 13 to 19, RB_VB_Lines - 12 to 18 -> 19 - 10 - 3 = 6 or 13 - 4 - 3 = 6
        // 1615: FALSE: Act_VB_Lines can be 14 to N,  RB_VB_Lines - 14 to > 19 -> 14 - 4 - 3 = 7, 19 - 10 - 3 = 6, 20 - 10 - 3 = 7 etc...
        // RB_BackPorch < 6 cannot be true

        // check on V_BackPorch ==6...in VESA CVT pdf file the limit is 7...but in Excel file it is 6.
        RB_BackPorch = Act_VB_Lines - VSyncRqd - Min_V_FPorch;
        // RB_BackPorch = (RB_BackPorch < 6) ? 6 : RB_BackPorch;

        Act_VB_Lines = VSyncRqd + Min_V_FPorch + RB_BackPorch;

        // Step 11 for Both : Find the total number of lines in vertical field
        TotalVLines = (float)(VLinesRnd + TopMargin * 2 + InterLaced + Act_VB_Lines);

        // Step  12 : Find total number of pixels in one line
        TotalPixels = ActivePixels + HBlankPixels;

        // Step 13:Find pixel clock freq.
        Act_Pixel_Freq = (ClockStep * ((DDU32)((VFieldRateRqd * TotalVLines * TotalPixels / 1000000) / ClockStep)));

        // Flags info
        pCreateCvtTimingArgs->pTimingInfo->HSyncPolarity = 0; // Positive
        pCreateCvtTimingArgs->pTimingInfo->VSyncPolarity = 1; // Negative
    }

    // Common equations for both type of timings.

    // Step 16 for NB & 14 ofr RB: Find actual horizontal Freq.
    // H_Freq calculated here is in KHz.
    Act_H_Freq = (1000 * Act_Pixel_Freq / TotalPixels);

    // Step 17 for NB & 15 RB: Find the actual vertical field frequency
    Act_VFieldRate = (1000 * Act_H_Freq / TotalVLines);

    // Step 18 for NB & 16 for RB: Find the actual vertical frame frequency
    Act_VFrameRate = (pCreateCvtTimingArgs->IsInterLaced ? Act_VFieldRate / 2 : Act_VFieldRate);

    // for Interlaced modes, DD needs dot clock to be halved.
    // To avoid the floating point error multiplying by 1000, two times instead of 1000000
    pCreateCvtTimingArgs->pTimingInfo->DotClockInHz =
        (DDU32)(pCreateCvtTimingArgs->IsInterLaced ? (DD_ROUNDTO3DECIMALS(Act_Pixel_Freq) * (1000)) / 2 : (DD_ROUNDTO3DECIMALS(Act_Pixel_Freq) * (1000))); // from step 15-NB & 13- RB
    pCreateCvtTimingArgs->pTimingInfo->DotClockInHz *= 1000;
    pCreateCvtTimingArgs->pTimingInfo->HTotal = TotalPixels;  // from step 14-NB & 12-Rb
    pCreateCvtTimingArgs->pTimingInfo->HActive = ActivePixels; // from step 4 in Both

    pCreateCvtTimingArgs->pTimingInfo->HBlankStart = ActivePixels;
    pCreateCvtTimingArgs->pTimingInfo->HBlankEnd = TotalPixels - 1;

    pCreateCvtTimingArgs->pTimingInfo->HSyncStart = (ActivePixels + (HBlankPixels / 2) - HSyncPixels); // from 23 in RB
    pCreateCvtTimingArgs->pTimingInfo->HSyncEnd = (ActivePixels + (HBlankPixels / 2) - 1);
    // for Interlaced modes, SB needs H Refresh Rate to be halved..

    // from step 15-NB & 14-RB ... Here we need to multiply by 1000 to get H-RR in Hz.
    FlActHFreq = DD_ROUNDTO3DECIMALS(Act_H_Freq);
    pCreateCvtTimingArgs->pTimingInfo->HRefresh = (DDU32)(pCreateCvtTimingArgs->IsInterLaced ? (DD_ROUNDTONEARESTINT(FlActHFreq * 1000)) / 2 : (DD_ROUNDTONEARESTINT(FlActHFreq * 1000)));

    // calculate in case of interlaced the frame based parameters
    // instead of per field basis
    // For intelaced mode, the VTotal calculated by CVT will be per field. but SB needs it per frame.

    // from step 11 in Both
    pCreateCvtTimingArgs->pTimingInfo->VTotal = (DDU32)(pCreateCvtTimingArgs->IsInterLaced ? (DD_ROUNDTO3DECIMALS(TotalVLines * 2)) : (DD_ROUNDTO3DECIMALS(TotalVLines)));
    pCreateCvtTimingArgs->pTimingInfo->VActive = Vpixels; // from step 5 in Both

    pCreateCvtTimingArgs->pTimingInfo->VBlankStart = Vpixels;
    pCreateCvtTimingArgs->pTimingInfo->VBlankEnd = pCreateCvtTimingArgs->pTimingInfo->VTotal - 1;

    // The interlaced paramter is also taken in to account to calculate the
    // sync start and end if the timings are to be generated for interlaced
    FlInterLaced = DD_ROUNDTO3DECIMALS(InterLaced);
    pCreateCvtTimingArgs->pTimingInfo->VSyncStart = Vpixels + Min_V_FPorch + (DDU32)DD_ROUNDTONEARESTINT(FlInterLaced);
    pCreateCvtTimingArgs->pTimingInfo->VSyncEnd = Vpixels + Min_V_FPorch + (DDU32)DD_ROUNDTONEARESTINT(FlInterLaced) + VSyncRqd - 1;
    FlActVFrameRate = DD_ROUNDTO3DECIMALS(Act_VFrameRate);
    pCreateCvtTimingArgs->pTimingInfo->VRoundedRR = (DDU32)DD_ROUNDTONEARESTINT(FlActVFrameRate); // from step 18 in NB & 16 in RB

    pCreateCvtTimingArgs->pTimingInfo->IsInterlaced = pCreateCvtTimingArgs->IsInterLaced;

    // Set double wide mode flag
    // stTimingInfo.Flags.bDoubleWideMode = MODESMANAGER_IsDoubleWideMode(&stTimingInfo);

    return DDS_SUCCESS;
}
/**
 * @brief Method to verify if two pixel clocks are same.
 *
 * +/-0.5 % is the pixel clock deviation allowed as per VESA spec.
 * After multipying by 100000 (which is done to avoid floating point calculation),
 *  acceptable deviation range is +/-500.
 * @param pPixelClkCompareArgs
 * @return DD_BOOL
 */
DD_BOOL DisplayInfoRoutinesIsSamePixelClock(PIXELCLK_COMPARE_ARGS* pPixelClkCompareArgs)
{
    DDU64 Deviation = 0;

    if (pPixelClkCompareArgs->PixelClock1 != 0)
    {
        if (pPixelClkCompareArgs->PixelClock1 >= pPixelClkCompareArgs->PixelClock2)
        {
            Deviation = ((DDU64)(pPixelClkCompareArgs->PixelClock1 - pPixelClkCompareArgs->PixelClock2) * 100000) / pPixelClkCompareArgs->PixelClock1;
        }
        else
        {
            Deviation = ((DDU64)(pPixelClkCompareArgs->PixelClock2 - pPixelClkCompareArgs->PixelClock1) * 100000) / pPixelClkCompareArgs->PixelClock1;
        }

        if (Deviation <= 500)
        {
            return TRUE;
        }
    }

    return FALSE;
}
/**
 * @brief  Display Info Routines Get Timing From PRTiming For PRMask.
 *
 * @param pPRTimingInfo
 * @param pOutputTimingInfo
 * @param PRMask
 * @return DDSTATUS
 */
DDSTATUS DisplayInfoRoutinesGetTimingFromPRTimingForPRMask(DD_TIMING_INFO* pPRTimingInfo, DD_TIMING_INFO* pOutputTimingInfo, DDU8 PRMask)
{
    DDU32 HSO = 0;
    DDU32 HSPW = 0;
    DDU32 HBlank = 0;

    DDASSERT(pPRTimingInfo);
    DDASSERT(pOutputTimingInfo);
    if (PRMask == 0)
    {
        // Should never happen
        DDRLSASSERT_UNEXPECTED(0, "");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    DD_MEM_COPY_SAFE(pOutputTimingInfo, sizeof(DD_TIMING_INFO), pPRTimingInfo, sizeof(DD_TIMING_INFO));

    if (PRMask > 1) // For PRMask == 1, return same timing
    {
        HBlank = pPRTimingInfo->HBlankEnd - pPRTimingInfo->HBlankStart + 1;
        HSO = pPRTimingInfo->HSyncStart - pPRTimingInfo->HActive;
        HSPW = pPRTimingInfo->HSyncEnd - pPRTimingInfo->HSyncStart + 1;

        pOutputTimingInfo->DotClockInHz = pPRTimingInfo->DotClockInHz / PRMask;
        pOutputTimingInfo->HTotal = pPRTimingInfo->HTotal / PRMask;
        pOutputTimingInfo->HActive = pPRTimingInfo->HActive / PRMask;

        pOutputTimingInfo->HBlankStart = pOutputTimingInfo->HActive;
        pOutputTimingInfo->HBlankEnd = pOutputTimingInfo->HBlankStart + (HBlank / PRMask) - 1;
        pOutputTimingInfo->HSyncStart = pOutputTimingInfo->HActive + (HSO / PRMask);
        pOutputTimingInfo->HSyncEnd = pOutputTimingInfo->HSyncStart + (HSPW / PRMask) - 1;
        // pOutputTimingInfo->dwHRefresh = pOutputTimingInfo->dwDotClock / pOutputTimingInfo->dwHTotal;
    }

    pOutputTimingInfo->CeData.PixelReplication = PRMask - 1; // since in stTimingInfo, ucPixelReplication = 0 represents no replication

    // SB_CSL_ETW_LOGGING(&stTimingInfo, "TIMING_INFO", sizeof(DD_TIMING_INFO));

    return DDS_SUCCESS;
}

/**
 * @brief  This function calculates checksum.
 *
 * @param pBuffer
 * @param BufferLen
 * @return DDU8 Checksum
 */
DDU8 DisplayInfoRoutinesCalcChksum(DDU8* pBuffer, DDU32 BufferLen)
{
    DDU32 Counter = 0;
    DDU8 Chksum = 0;

    for (Counter = 0; Counter < BufferLen - 1; Counter++)
    {
        Chksum += pBuffer[Counter];
    }

    Chksum = 256 - Chksum;

    return Chksum;
}
/**
 * @brief This function.
 *
 * @param pSourceTimings
 * @param pTargetTimings
 * @return DD_BOOL
 */
DD_BOOL DisplayInfoRoutinesIsSameTiming(DD_TIMING_INFO* pSourceTimings, DD_TIMING_INFO* pTargetTimings)
{
    if ((pSourceTimings->HActive == pTargetTimings->HActive) && (pSourceTimings->HTotal == pTargetTimings->HTotal) && (pSourceTimings->VActive == pTargetTimings->VActive) &&
        (pSourceTimings->VTotal == pTargetTimings->VTotal) && (pSourceTimings->DotClockInHz == pTargetTimings->DotClockInHz) && (pSourceTimings->IsInterlaced == pTargetTimings->IsInterlaced) &&
        (pSourceTimings->S3DFormatMask == pTargetTimings->S3DFormatMask))
    {
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief Compute Offset Value at a specific group.
 *
 * Refer to DSCv1.2a C-Model for more information.
 *
 * @param pDscCfg
 * @param PixelsPerGroup
 * @param GroupsPerLine
 * @param GroupCount
 * @return DDS32
 * @retval Offset Value
 */
static DDS32 DisplayInfoRoutinesComputeOffset(DSC_CFG* pDscCfg, DDU32 PixelsPerGroup, DDU32 GroupsPerLine, DDU32 GroupCount)
{
    DDS32 Offset;
    DDU32 GroupCountId;

    GroupCountId = DD_ROUND_UP_DIV(pDscCfg->InitialXmitDelay, PixelsPerGroup);
    if (GroupCount <= GroupCountId)
    {
        Offset = DD_ROUND_UP_DIV(GroupCount * PixelsPerGroup * pDscCfg->BitsPerPixel, 16);
    }
    else
    {
        Offset = DD_ROUND_UP_DIV(GroupCountId * PixelsPerGroup * pDscCfg->BitsPerPixel, 16) - (((GroupCount - GroupCountId) * pDscCfg->SliceBpgOffset) >> OFFSET_FRACTIONAL_BITS);
    }

    if (GroupCount <= GroupsPerLine)
    {
        Offset += GroupCount * pDscCfg->FirstLineBpgOfs;
    }
    else
    {
        Offset += GroupsPerLine * pDscCfg->FirstLineBpgOfs - (((GroupCount - GroupsPerLine) * pDscCfg->NflBpgOffset) >> OFFSET_FRACTIONAL_BITS);
    }

    if (pDscCfg->Native420)
    {
        if (GroupCount <= GroupsPerLine)
        {
            Offset -= (GroupCount * pDscCfg->NslBpgOffset) >> OFFSET_FRACTIONAL_BITS;
        }
        else if (GroupCount <= 2 * GroupsPerLine)
        {
            Offset += (GroupCount - GroupsPerLine) * pDscCfg->SecondLineBpgOfs - ((GroupsPerLine * pDscCfg->NslBpgOffset) >> OFFSET_FRACTIONAL_BITS);
        }
        else
        {
            Offset += (GroupCount - GroupsPerLine) * pDscCfg->SecondLineBpgOfs - (((GroupCount - GroupsPerLine) * pDscCfg->NslBpgOffset) >> OFFSET_FRACTIONAL_BITS);
        }
    }

    return Offset;
}

/**
 * @brief Compute RC parameters for PPS (Picture Parameter Set).
 *
 * Refer to DSCv1.1 C-Model for more information.
 *
 * @param pDscCfg
 * @return DD_BOOL
 */
static DD_BOOL DisplayInfoRoutinesComputeRcParameters(DSC_CFG* pDscCfg)
{
    DDU32 GroupsPerLine = 0;
    DDU32 GroupsTotal = 0;
    DDU32 NumExtraMuxBits = 0;
    DDU32 SliceBits = 0;
    DDU32 HrdDelay = 0;
    DDU32 FinalScale = 0;
    DDU32 RbsMin = 0;
    DDU32 SliceWidth = 0;
    DDU32 NumSsps = 0;
    DDU32 PixelsPerGroup = 0;
    DDS32 MaxOffset = 0;
    DDS32 DscOffset;

    if (pDscCfg->Native422)
    {
        NumSsps = 4;
        PixelsPerGroup = 3;
    }
    else
    {
        NumSsps = 3;
        PixelsPerGroup = 3;
    }

    SliceWidth = ((pDscCfg->Native422 || pDscCfg->Native420) ? (pDscCfg->SliceWidth / 2) : pDscCfg->SliceWidth);

    // Number of groups used to code each line of a slice = ceil(slicewidth/PixelsPerGroup)
    GroupsPerLine = DD_ROUND_UP_DIV(SliceWidth, PixelsPerGroup);

    // chunksize = ceil(slicewidth*bitsperpixel/8) in Bytes
    pDscCfg->ChunkSize = DD_ROUND_UP_DIV(SliceWidth * pDscCfg->BitsPerPixel, (8 * 16));

    if (pDscCfg->ConvertRgb)
    {
        NumExtraMuxBits = NumSsps * (pDscCfg->MuxWordSize + (4 * pDscCfg->BitsPerComponent + 4) - 2);
    }
    else
    {
        // YCbCr
        if (TRUE == pDscCfg->Native422)
        {
            NumExtraMuxBits = NumSsps * pDscCfg->MuxWordSize + (4 * pDscCfg->BitsPerComponent + 4) + 3 * (4 * pDscCfg->BitsPerComponent) - 2;
        }
        else
        {
            NumExtraMuxBits = NumSsps * pDscCfg->MuxWordSize + (4 * pDscCfg->BitsPerComponent + 4) + 2 * (4 * pDscCfg->BitsPerComponent) - 2;
        }
    }

    // Number of bits in one Slice
    SliceBits = 8 * pDscCfg->ChunkSize * pDscCfg->SliceHeight;
    while ((NumExtraMuxBits > 0) && ((SliceBits - NumExtraMuxBits) % pDscCfg->MuxWordSize))
    {
        NumExtraMuxBits--;
    }

    // InitialScaleValue is a 6 bit value with 3 fractional bits (U3.3)
    // In order to preserve the fractional part multiply numerator by 2^3
    pDscCfg->InitialScaleValue = (8 * pDscCfg->RcModelSize) / (pDscCfg->RcModelSize - pDscCfg->InitialOffset);
    if (GroupsPerLine < pDscCfg->InitialScaleValue - 8)
    {
        pDscCfg->InitialScaleValue = GroupsPerLine + 8;
    }
    if (pDscCfg->InitialScaleValue > 8)
    {
        pDscCfg->ScaleDecrementInterval = GroupsPerLine / (pDscCfg->InitialScaleValue - 8);
    }
    else
    {
        pDscCfg->ScaleDecrementInterval = 4095;
    }

    pDscCfg->FinalOffset = pDscCfg->RcModelSize - (pDscCfg->InitialXmitDelay * pDscCfg->BitsPerPixel + 8) / 16 + NumExtraMuxBits;
    if (pDscCfg->FinalOffset >= pDscCfg->RcModelSize)
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "The FinalOffset must be less than the RcModelSize. Try increasing InitialXmitDelay");
        return FALSE;
    }

    // FinalScale, multiply by 8 to preserve 3 fractional bits
    FinalScale = (8 * pDscCfg->RcModelSize) / (pDscCfg->RcModelSize - pDscCfg->FinalOffset);
    if (FinalScale > 63)
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "A final scale value > than 63 / 8 may have undefined behavior on some implementations.Try increasing initial_xmit_delay.");
        return FALSE;
    }

    if (pDscCfg->SliceHeight > 1)
    {
        // NflBpgOffset is 16 bit value with 11 fractional bits
        // hence we multiply by 2^11 for preserving the fractional part
        pDscCfg->NflBpgOffset = DD_ROUND_UP_DIV((pDscCfg->FirstLineBpgOfs << OFFSET_FRACTIONAL_BITS), (pDscCfg->SliceHeight - 1));
    }
    else
    {
        pDscCfg->NflBpgOffset = 0;
    }
    if (pDscCfg->NflBpgOffset > DDMAXU16)
    {
        // 2^16 -1
        DISP_DBG_MSG(GFXDBG_CRITICAL, "NflBpgOffset is too large for this slice height");
        return FALSE;
    }

    if (pDscCfg->SliceHeight > 2)
    {
        // NslBpgOffset is 16 bit value with 11 fractional bits
        // hence we multiply by 2^11 for preserving the fractional part
        pDscCfg->NslBpgOffset = DD_ROUND_UP_DIV((pDscCfg->SecondLineBpgOfs << OFFSET_FRACTIONAL_BITS), (pDscCfg->SliceHeight - 1));
    }
    else
    {
        pDscCfg->NslBpgOffset = 0;
    }
    if (pDscCfg->NslBpgOffset > DDMAXU16)
    {
        // 2^16 -1
        DISP_DBG_MSG(GFXDBG_CRITICAL, "NslBpgOffset is too large for this slice height");
        return FALSE;
    }

    // Number of groups used to code the entire slice
    GroupsTotal = GroupsPerLine * pDscCfg->SliceHeight;

    // SliceBpgOffset is 16 bit value with 11 fractional bits
    // hence we multiply by 2^11 for preserving the fractional part
    pDscCfg->SliceBpgOffset = DD_ROUND_UP_DIV(((pDscCfg->RcModelSize - pDscCfg->InitialOffset + NumExtraMuxBits) << OFFSET_FRACTIONAL_BITS), GroupsTotal);

    if (pDscCfg->DscVersionMinor == 2)
    {
        if (pDscCfg->SliceHeight == 1 && pDscCfg->FirstLineBpgOfs > 0)
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "For slice_height == 1, the FIRST_LINE_BPG_OFFSET must be 0");
            return FALSE;
        }
        else if ((PixelsPerGroup * pDscCfg->BitsPerPixel) - ((pDscCfg->SliceBpgOffset + pDscCfg->NflBpgOffset) * 16 / 1 << OFFSET_FRACTIONAL_BITS) < (1 + 5 * PixelsPerGroup))
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "The bits/pixel allocation for non-first lines is too low (<5.33bpp) Consider decreasing FIRST_LINE_BPG_OFFSET");
            return FALSE;
        }
    }

    if (FinalScale > 9)
    {
        // ScaleIncrementInterval = finaloffset/((NflBpgOffset + SliceBpgOffset + NslBpgOffset)*8(finalscale - 1.125))
        // as (NflBpgOffset + SliceBpgOffset) has 11 bit fractional value we need divide by 2^11 from pDscCfg values
        // ScaleIncrementInterval = finaloffset/((NflBpgOffset + SliceBpgOffset + NslBpgOffset)/2^11*8(finalscale - 1.125))
        // ScaleIncrementInterval = finaloffset*2^11/((NflBpgOffset + SliceBpgOffset + NslBpgOffset)*8(finalscale - 1.125))
        // ScaleIncrementInterval = finaloffset*2^11/((NflBpgOffset + SliceBpgOffset + NslBpgOffset)*(8*finalscale - 9))
        // as finalscale has 3 fractional bits stored we need to divide by 8.
        // ScaleIncrementInterval = finaloffset*2^11/((NflBpgOffset + SliceBpgOffset + NslBpgOffset)*(finalscale - 9))
        pDscCfg->ScaleIncrementInterval = (pDscCfg->FinalOffset * (1 << OFFSET_FRACTIONAL_BITS)) / ((pDscCfg->NflBpgOffset + pDscCfg->SliceBpgOffset + pDscCfg->NslBpgOffset) * (FinalScale - 9));
    }
    else
    {
        // If finalScaleValue is less than or equal to 9, a value of 0 should be used to disable the
        // scale increment at the end of the slice
        pDscCfg->ScaleIncrementInterval = 0;
    }

    if (pDscCfg->ScaleIncrementInterval > DDMAXU16)
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "ScaleIncrementInterval is too large for this slice height. A smaller slice height should be used.");
        return FALSE;
    }

    // Minimum rate buffer size, in units of bits. The real physical rate buffer must be slightly
    // larger than this to account for subtle differences between the model and hardware group times
    if ((pDscCfg->DscVersionMinor == 2) && (pDscCfg->Native422 || pDscCfg->Native420))
    {
        // For Native 4:2:2 and 4:2:0 modes, the C model implements a tighter bound by finding the maximum offset(maxOffset)
        // After Initial Delay
        MaxOffset = DisplayInfoRoutinesComputeOffset(pDscCfg, PixelsPerGroup, GroupsPerLine, DD_ROUND_UP_DIV(pDscCfg->InitialXmitDelay, PixelsPerGroup));
        // After First Line
        DscOffset = DisplayInfoRoutinesComputeOffset(pDscCfg, PixelsPerGroup, GroupsPerLine, GroupsPerLine);
        MaxOffset = DD_MAX(MaxOffset, DscOffset);
        DscOffset = DisplayInfoRoutinesComputeOffset(pDscCfg, PixelsPerGroup, GroupsPerLine, 2 * GroupsPerLine);
        MaxOffset = DD_MAX(MaxOffset, DscOffset);
        RbsMin = pDscCfg->RcModelSize - pDscCfg->InitialOffset + MaxOffset;
    }
    else
    {
        RbsMin = pDscCfg->RcModelSize - pDscCfg->InitialOffset + DD_ROUND_UP_DIV((pDscCfg->InitialXmitDelay * pDscCfg->BitsPerPixel), 16) + GroupsPerLine * pDscCfg->FirstLineBpgOfs;
    }

    HrdDelay = DD_ROUND_UP_DIV((RbsMin * 16), pDscCfg->BitsPerPixel);
    pDscCfg->RcbBits = DD_ROUND_UP_DIV((HrdDelay * pDscCfg->BitsPerPixel), 16);
    pDscCfg->InitialDecDelay = HrdDelay - pDscCfg->InitialXmitDelay;

    return TRUE;
}

/**
 * @brief Get DSC PPS Initial Fullness Offset value.
 *
 * @param pDscCfg
 * @return DD_BOOL
 */
static DD_BOOL DisplayInfoRoutinesUpdateDscPpsInitialOffsetValue(DSC_CFG* pDscCfg)
{
    if (pDscCfg->Native422)
    {
        // =IF(CompressBpp >= 8, 2048, IF(CompressBpp <=  7, 5632, 5632 - ROUND((CompressBpp - 7) * (3584), 0)))
        // For Native422/Native420 CompressBpp value is doubled, so the above conditions are modified as follows:
        // =IF(CompressBpp >= 16, 2048, IF(CompressBpp <=  14, 5632, 5632 - ROUND((CompressBpp - 14) * (1792), 0)))
        if (pDscCfg->BitsPerPixel >= DSC_BPP_16)
        {
            pDscCfg->InitialOffset = 2048;
        }
        else if (pDscCfg->BitsPerPixel >= DSC_BPP_14)
        {
            pDscCfg->InitialOffset = 5632 - ((pDscCfg->BitsPerPixel - DSC_BPP_14) * 112);
        }
        else if (pDscCfg->BitsPerPixel >= DSC_BPP_12)
        {
            pDscCfg->InitialOffset = 5632;
        }
        else
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "BitsPerPixel < 6 in Native 4:2:2");
            return FALSE;
        }
    }
    else // RGB or YCbCr444 or Simple422 or Native420
    {
        if (pDscCfg->BitsPerPixel >= DSC_BPP_12)
        {
            pDscCfg->InitialOffset = 2048;
        }
        else if (pDscCfg->BitsPerPixel >= DSC_BPP_10)
        {
            pDscCfg->InitialOffset = 5632 - ((pDscCfg->BitsPerPixel - DSC_BPP_10) * 112); // 5632 - ROUND((CompressBpp - 10)*1792))
        }
        else if (pDscCfg->BitsPerPixel >= DSC_BPP_8)
        {
            pDscCfg->InitialOffset = 6144 - ((pDscCfg->BitsPerPixel - DSC_BPP_8) * 16); // 6144 - ROUND((CompressBpp - 8)*256))
        }
        else if (pDscCfg->BitsPerPixel >= DSC_BPP_6)
        {
            pDscCfg->InitialOffset = 6144;
        }
        else
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "BitsPerPixel < 6 in 4:4:4 or BitsPerPixel < 3 in Native 4:2:0");
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * @brief Get DSC PPS FirstLineBpgOffset and SecondLineBpgOffset.
 *
 * @param pDscCfg
 * @return DD_BOOL
 */
static void DisplayInfoRoutinesUpdateDscPpsLineBpgOffsetValues(DSC_CFG* pDscCfg)
{
    DDU32 YuvInput;
    DDU32 UncompressedBpgRate;

    // ConvertRgb == 1, when output color space is RGB
    YuvInput = pDscCfg->ConvertRgb == 1 ? 0 : 1;

    if (TRUE == pDscCfg->Native422)
    {
        UncompressedBpgRate = 3 * pDscCfg->BitsPerComponent * 4;
    }
    else
    {
        UncompressedBpgRate = (3 * pDscCfg->BitsPerComponent + (YuvInput ? 0 : 2)) * 3;
    }

    // 1. As per DSC 1.1 Standard  12 Bpg for 8Bpp and 15Bpg for 12 Bpp.
    // 2. As per Dsc C Model FirstLineBpgOffset is obtained from slice height as given below.
    /* if (dsc_cfg->slice_height >= 8)
        firstLineBpgOfs = 12 + ((int)(0.09 * MIN(34, dsc_cfg->slice_height - 8)));
    else
        firstLineBpgOfs = 2 * (dsc_cfg->slice_height - 1);
    firstLineBpgOfs = CLAMP(firstLineBpgOfs, 0, (int)(uncompressedBpgRate - 3 * bitsPerPixel));*/
    // DSC Spec Calls out DSC Cmodel takes precedence over the DSC Standard.

    if (pDscCfg->SliceHeight >= 8)
    {
        pDscCfg->FirstLineBpgOfs = 12 + ((9 * DD_MIN(34, (pDscCfg->SliceHeight - 8))) / 100);
    }
    else
    {
        pDscCfg->FirstLineBpgOfs = 2 * (pDscCfg->SliceHeight - 1);
    }
    pDscCfg->FirstLineBpgOfs = DD_MIN(pDscCfg->FirstLineBpgOfs, (((UncompressedBpgRate * 16) - (3 * pDscCfg->BitsPerPixel)) / 16));

    if (TRUE == pDscCfg->Native420)
    {
        pDscCfg->SecondLineBpgOfs = 12;
        pDscCfg->SecondLineBpgOfs = DD_MIN(pDscCfg->SecondLineBpgOfs, (((UncompressedBpgRate * 16) - (3 * pDscCfg->BitsPerPixel)) / 16));
    }
    else
    {
        pDscCfg->SecondLineBpgOfs = 0;
    }
}

/**
 * @brief Generate RC parameters for PPS (Picture Parameter Set).
 *
 * DSC v1.2a Standard (DSC v1.2a) and related support tools provide several methods that list or
 * deterministically generate valid rate control (RC) parameters for use within a DSC Picture Parameter
 * Set (PPS). The PPS structure is normative; which method is chosen to generate the RC values is
 * implementation-specific. Any of the following methods are valid and have been shown to provide
 * visually lossless quality:
 *
 * 1. Annex E, "Derivation of Rate Control Parameters (Informative),"of any in-force DSC Standard
 * 2. .cfg files included with the in-force DSC C model
 * 3. DSCParameterValuesVESA V1-2 spreadsheet
 * 4. VESA DSC 1.2a DSC Tools Application Note
 * (provides the same PPS values as DSCParameterValuesVESA V1-2 spreadsheet)
 *
 * The PPS values that are generated using any of these methods are typically the same for a
 * given configuration. Methods 1 and 2 provide empirically determined parameters for each supported
 * configuration. Methods 3 and 4 use a common formula to derive values for any combination of DSC
 * variables. The formula approach may yield slight differences in the derived PPS parameters from the
 * original parameter sets. These differences are not consequential to the coding performance because
 * all parameter sets have been shown to produce visually lossless quality
 *
 * Methods 1 and 2 doesn't provide RC parameters for all ranges of BPP. Hence we shall be
 * generating RC parameters using Method-3/4.
 *
 * @param pDscCfg
 * @return DD_BOOL
 */
static DD_BOOL DisplayInfoRoutinesGenerateRcParamerters(DSC_CFG* pDscCfg)
{
    DDU32 QpBpcModifier, SliceWidth, ColumnIndex, YuvInput;
    DDS32 Diff, Offset, PaddingPixels;

    // ConvertRgb == 1, when output color space is RGB
    YuvInput = pDscCfg->ConvertRgb == 1 ? 0 : 1;

    Diff = ((1 == pDscCfg->DscVersionMinor) && YuvInput) ? 1 : 0;
    QpBpcModifier = (pDscCfg->BitsPerComponent - 8) * 2 - Diff;

    pDscCfg->RcQuantIncrLimit0 = 11 + QpBpcModifier;
    pDscCfg->RcQuantIncrLimit1 = 11 + QpBpcModifier;

    if (FALSE == DisplayInfoRoutinesUpdateDscPpsInitialOffsetValue(pDscCfg))
    {
        return FALSE;
    }

    // InitialXmitDelay = rc_model_size/2/CompressBpp; rc_model_size = 8192
    // For Native422/Native420 CompressBpp value is doubled.
    pDscCfg->InitialXmitDelay = (DDU32)DD_ROUND((4096 * 16), pDscCfg->BitsPerPixel);

    DisplayInfoRoutinesUpdateDscPpsLineBpgOffsetValues(pDscCfg);

    // SliceWidth is halved for Native420/Native422 mode
    SliceWidth = ((pDscCfg->Native422 || pDscCfg->Native420) ? (pDscCfg->SliceWidth / 2) : pDscCfg->SliceWidth);
    PaddingPixels = ((SliceWidth % 3) ? (3 - (SliceWidth % 3)) : 0) * (pDscCfg->InitialXmitDelay / SliceWidth);
    if (((3 * pDscCfg->BitsPerPixel) >= (16 * ((pDscCfg->InitialXmitDelay + 2) / 3) * (pDscCfg->Native422 ? 4 : 3))) && (((pDscCfg->InitialXmitDelay + PaddingPixels) % 3) == 1))
    {
        pDscCfg->InitialXmitDelay++;
    }

    pDscCfg->FlatnessMinQp = 3 + QpBpcModifier;
    pDscCfg->FlatnessMaxQp = 12 + QpBpcModifier;
    pDscCfg->FlatnessDetThresh = 2 << (pDscCfg->BitsPerComponent - 8);
    pDscCfg->SecondLineOfsAdj = (TRUE == pDscCfg->Native420) ? 512 : 0;

    for (DDU32 Index = 0; Index < DD_NUM_BUF_RANGES; Index++)
    {
        DDU32 AbsOffset;

        if (TRUE == pDscCfg->Native420)
        {
            DDS32 OffsetUnder4Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -12, -12, -12, -12 };
            DDS32 OffsetUnder5Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -10, -12, -12, -12 };
            DDS32 OffsetUnder6Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -10, -12, -12, -12 };
            DDS32 OffsetUnder8Bpp[] = { 10, 8, 6, 4, 2, 0, -2, -4, -6, -8, -10, -10, -12, -12, -12 };

            ColumnIndex = (pDscCfg->BitsPerPixel - DSC_BPP_8) / 16;

            switch (pDscCfg->BitsPerComponent)
            {
            case 8:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp420_8bpc[Index][ColumnIndex]);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp420_8bpc[Index][ColumnIndex]);
                break;
            case 10:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp420_10bpc[Index][ColumnIndex]);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp420_10bpc[Index][ColumnIndex]);
                break;
            case 12:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp420_12bpc[Index][ColumnIndex]);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp420_12bpc[Index][ColumnIndex]);
                break;
            default:
                break;
            }

            if (pDscCfg->BitsPerPixel <= DSC_BPP_8)
            {
                Offset = OffsetUnder4Bpp[Index];
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_10)
            {
                Offset = OffsetUnder4Bpp[Index] + (DDS32)DD_ROUND((pDscCfg->BitsPerPixel - DSC_BPP_8) * (OffsetUnder5Bpp[Index] - OffsetUnder4Bpp[Index]), 32);
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_12)
            {
                Offset = OffsetUnder5Bpp[Index] + (DDS32)DD_ROUND((pDscCfg->BitsPerPixel - DSC_BPP_10) * (OffsetUnder6Bpp[Index] - OffsetUnder5Bpp[Index]), 32);
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_16)
            {
                Offset = OffsetUnder6Bpp[Index] + (DDS32)DD_ROUND((pDscCfg->BitsPerPixel - DSC_BPP_12) * (OffsetUnder8Bpp[Index] - OffsetUnder6Bpp[Index]), 64);
            }
            else
            {
                Offset = OffsetUnder8Bpp[Index];
            }
        }
        else if (TRUE == pDscCfg->Native422)
        {
            DDS32 OffsetUnder6Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -12, -12, -12, -12 };
            DDS32 OffsetUnder7Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -10, -12, -12, -12 };
            DDS32 OffsetUnder10Bpp[] = { 10, 8, 6, 4, 2, 0, -2, -4, -6, -8, -10, -10, -12, -12, -12 };

            ColumnIndex = (pDscCfg->BitsPerPixel - DSC_BPP_12) / 16;

            switch (pDscCfg->BitsPerComponent)
            {
            case 8:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp422_8bpc[Index][ColumnIndex]);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp422_8bpc[Index][ColumnIndex]);
                break;
            case 10:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp422_10bpc[Index][ColumnIndex]);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp422_10bpc[Index][ColumnIndex]);
                break;
            case 12:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp422_12bpc[Index][ColumnIndex]);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp422_12bpc[Index][ColumnIndex]);
                break;
            default:
                break;
            }

            if (pDscCfg->BitsPerPixel <= DSC_BPP_12)
            {
                Offset = OffsetUnder6Bpp[Index];
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_14)
            {
                Offset = OffsetUnder6Bpp[Index] + (DDS32)DD_ROUND((pDscCfg->BitsPerPixel - DSC_BPP_12) * (OffsetUnder7Bpp[Index] - OffsetUnder6Bpp[Index]), 32);
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_16)
            {
                Offset = OffsetUnder7Bpp[Index];
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_20)
            {
                Offset = OffsetUnder7Bpp[Index] + (DDS32)DD_ROUND((pDscCfg->BitsPerPixel - DSC_BPP_16) * (OffsetUnder10Bpp[Index] - OffsetUnder7Bpp[Index]), 64);
            }
            else
            {
                Offset = OffsetUnder10Bpp[Index];
            }
        }
        else
        {
            DDS32 OffsetUnder6Bpp[] = { 0, -2, -2, -4, -6, -6, -8, -8, -8, -10, -10, -12, -12, -12, -12 };
            DDS32 OffsetUnder8Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -10, -12, -12, -12 };
            DDS32 OffsetUnder12Bpp[] = { 2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -10, -12, -12, -12 };
            DDS32 OffsetUnder15Bpp[] = { 10, 8, 6, 4, 2, 0, -2, -4, -6, -8, -10, -10, -12, -12, -12 };

            ColumnIndex = (2 * (pDscCfg->BitsPerPixel - DSC_BPP_6)) / 16;

            // For DSC 1.1 AND (4:4:4 YCbCr OR 4:2:2 Simple modes): subtract 1 from each QP chart value.
            // Any negative values should be set to 0 (lower limit)
            switch (pDscCfg->BitsPerComponent)
            {
            case 8:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp444_8bpc[Index][ColumnIndex] - Diff);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp444_8bpc[Index][ColumnIndex] - Diff);
                break;
            case 10:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp444_10bpc[Index][ColumnIndex] - Diff);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp444_10bpc[Index][ColumnIndex] - Diff);
                break;
            case 12:
                pDscCfg->RcRangeParameters[Index].RangeMinQp = DD_MAX(0, RangeMinQp444_12bpc[Index][ColumnIndex] - Diff);
                pDscCfg->RcRangeParameters[Index].RangeMaxQp = DD_MAX(0, RangeMaxQp444_12bpc[Index][ColumnIndex] - Diff);
                break;
            default:
                break;
            }

            if (pDscCfg->BitsPerPixel <= DSC_BPP_6)
            {
                Offset = OffsetUnder6Bpp[Index];
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_8)
            {
                Offset = OffsetUnder6Bpp[Index] + (DDS32)DD_ROUND(((pDscCfg->BitsPerPixel - DSC_BPP_6) * (OffsetUnder8Bpp[Index] - OffsetUnder6Bpp[Index])), 32);
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_12)
            {
                Offset = OffsetUnder8Bpp[Index];
            }
            else if (pDscCfg->BitsPerPixel <= DSC_BPP_15)
            {
                Offset = OffsetUnder12Bpp[Index] + (DDS32)DD_ROUND(((pDscCfg->BitsPerPixel - DSC_BPP_12) * (OffsetUnder15Bpp[Index] - OffsetUnder12Bpp[Index])), 48);
            }
            else
            {
                Offset = OffsetUnder15Bpp[Index];
            }
        }

        AbsOffset = DD_ABS(Offset);
        pDscCfg->RcRangeParameters[Index].RangeBpgOffset = (Offset < 0) ? DD_TWOS_COMPLEMENT(AbsOffset) : Offset;

        // For DSC 1.1 AND (4:4:4 YCbCr OR 4:2:2 Simple modes): QPMax[14] may NOT be larger than 12 + 2 * (bpc - 8).
        // Therefore bpc values < 8.0 can not be supported without risk of buffer underflow (these are not supported visually lossless cases)
        if ((1 == pDscCfg->DscVersionMinor) && YuvInput && (pDscCfg->RcRangeParameters[Index].RangeMaxQp > 12 + 2 * (pDscCfg->BitsPerComponent - 8)))
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "In DSC 1.1 mode with YCbCr, RangeMaxQp[%d] must be less than %d", Index, 12 + 2 * (pDscCfg->BitsPerComponent - 8));
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * @brief Get RcParameter from cfg.
 *
 * @param pDscCfg:
 * @return void
 */
static DD_BOOL DisplayInfoRoutinesGetRcParamertersFromCfg(DSC_CFG* pDscCfg)
{
    // Generation of RC parameters is not designed when Pixels/Slice < 15000
    // So we shall be using the RC parameters from the supplied .cfg files as part of VESA C-Model.
    DDU8 RcParamRowIndex = 0;
    DDU8 RcParamColumnIndex = 0;
    DDU32 BitsPerPixel = 0;

    BitsPerPixel = pDscCfg->BitsPerPixel >> COMPRESSION_BPP_FRACTIONAL_BITS;

    switch (BitsPerPixel)
    {
    case 6:
        RcParamRowIndex = ROW_INDEX_6BPP;
        break;
    case 8:
        RcParamRowIndex = ROW_INDEX_8BPP;
        break;
    case 10:
        RcParamRowIndex = ROW_INDEX_10BPP;
        break;
    case 12:
        RcParamRowIndex = ROW_INDEX_12BPP;
        break;
    case 15:
        RcParamRowIndex = ROW_INDEX_15BPP;
        break;
    default:
        RcParamRowIndex = ROW_INDEX_INVALID;
        DISP_DBG_MSG(GFXDBG_CRITICAL, "Unsupported BPP");
        return FALSE;
    }

    switch (pDscCfg->BitsPerComponent)
    {
    case 8:
        RcParamColumnIndex = COLUMN_INDEX_8BPC;
        break;
    case 10:
        RcParamColumnIndex = COLUMN_INDEX_10BPC;
        break;
    case 12:
        RcParamColumnIndex = COLUMN_INDEX_12BPC;
        break;
    case 14:
        RcParamColumnIndex = COLUMN_INDEX_14BPC;
        break;
    case 16:
        RcParamColumnIndex = COLUMN_INDEX_16BPC;
        break;
    default:
        RcParamColumnIndex = COLUMN_INDEX_INVALID;
        DISP_DBG_MSG(GFXDBG_CRITICAL, "Unsupported BPC");
        return FALSE;
    }

    /* if (dsc_cfg->slice_height >= 8)
            firstLineBpgOfs = 12 + ((int)(0.09 * MIN(34, dsc_cfg->slice_height - 8)));
        else
            firstLineBpgOfs = 2 * (dsc_cfg->slice_height - 1);
        firstLineBpgOfs = CLAMP(firstLineBpgOfs, 0, (int)(uncompressedBpgRate - 3 * bitsPerPixel));*/
    if (pDscCfg->DscVersionMinor == 2)
    {
        if (pDscCfg->SliceHeight >= 8)
        {
            pDscCfg->FirstLineBpgOfs = 12 + ((9 * DD_MIN(34, (pDscCfg->SliceHeight - 8))) / 100);
        }
        else
        {
            pDscCfg->FirstLineBpgOfs = 2 * (pDscCfg->SliceHeight - 1);
        }
    }
    else
    {
        pDscCfg->FirstLineBpgOfs = RcParams[RcParamRowIndex][RcParamColumnIndex].FirstLineBpgOffset;
    }
    pDscCfg->InitialXmitDelay = RcParams[RcParamRowIndex][RcParamColumnIndex].InitialXmitDelay;
    pDscCfg->InitialOffset = RcParams[RcParamRowIndex][RcParamColumnIndex].InitialOffset;
    pDscCfg->FlatnessMinQp = RcParams[RcParamRowIndex][RcParamColumnIndex].FlatnessMinQp;
    pDscCfg->FlatnessMaxQp = RcParams[RcParamRowIndex][RcParamColumnIndex].FlatnessMaxQp;
    pDscCfg->RcQuantIncrLimit0 = RcParams[RcParamRowIndex][RcParamColumnIndex].RcQuantIncrLimit0;
    pDscCfg->RcQuantIncrLimit1 = RcParams[RcParamRowIndex][RcParamColumnIndex].RcQuantIncrLimit1;
    for (DDU8 Index = 0; Index < DD_NUM_BUF_RANGES; Index++)
    {
        pDscCfg->RcRangeParameters[Index].RangeMinQp = RcParams[RcParamRowIndex][RcParamColumnIndex].RcRangeParams[Index].RangeMinQp;
        pDscCfg->RcRangeParameters[Index].RangeMaxQp = RcParams[RcParamRowIndex][RcParamColumnIndex].RcRangeParams[Index].RangeMaxQp;
        pDscCfg->RcRangeParameters[Index].RangeBpgOffset = RcParams[RcParamRowIndex][RcParamColumnIndex].RcRangeParams[Index].RangeBpgOffset;
    }
    return TRUE;
}
/**
 * @brief Compute DSC PPS(Picture Parameter Set)
 * Refer to DSCv1.1 C-Model for more information.
 *
 * @param  pDscCfg [in, out]   Pointer to the ::DSC_CFG object.
 * @return DD_BOOL
 * @retval TRUE    If DSC PPS parameters are successfully computed.
 * @retval FALSE   If any of PPS parameters are invalid.
 **/
static DD_BOOL DisplayInfoRoutinesComputeDscParameters(DSC_CFG* pDscCfg, DD_BOOL IsMipiDsiDisplay)
{
    DDU8 Index = 0;

    // If native_422 or native_420 is set to 1, bits_per_pixel value shall be programmed to double the target bits per pixel rate
    if (pDscCfg->Native422 || pDscCfg->Native420)
    {
        if (pDscCfg->Native422 && pDscCfg->Native420)
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "Both Native422 and Native420 cannot be set");
            return FALSE;
        }

        // for DscVersionMinor == 1, Native422/Native420 is not supported
        if (1 == pDscCfg->DscVersionMinor)
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "For DscVersionMinor == 1, Native422/Native420 is not supported");
            return FALSE;
        }

        // If Native422 or Native420 is set to 1, BitsPerPixel value shall be programmed to double the target bits per pixel rate
        pDscCfg->BitsPerPixel *= 2;
    }

    pDscCfg->RcModelSize = 8192;
    pDscCfg->RcEdgeFactor = 6; // U3.1 format   (actual value 3.0)
    pDscCfg->RcTgtOffsetHi = 3;
    pDscCfg->RcTgtOffsetLo = 3;

    for (Index = 0; Index < DD_NUM_BUF_RANGES - 1; Index++)
    {
        // six 0s are appended to the lsb of each threshold value internally in h/w
        // Only 8 bits are allowed for programming RcBufThreshold, so we divide RcBufThreshold by 2^6
        pDscCfg->RcBufThresh[Index] = RC_BUF_THRESH[Index] / 64;
    }
    // For 6bpp, RC Buffer threshold 12 and 13 need a different value.
    if (DSC_BPP_6 == pDscCfg->BitsPerPixel)
    {
        pDscCfg->RcBufThresh[12] = 0x7C;
        pDscCfg->RcBufThresh[13] = 0x7D;
    }

    // Pixels/Slice = (pDscCfg->SliceHeight * pDscCfg->SliceWidth)
    // Selection b/w calculated RcRangeParameters or RcRangeParameters from .cfg files. (part of VESA C-Model) is as follows:
    //      if (Pixels/Slice >= 15000) {
    //          Choose calculated RcRangeParameters
    //      } else {
    //          Choose RcRangeParameters from .cfg files
    //      }
    // When RcRangeParameters are from .cfg files, they are defined for limited BPPs only.
    // Hence output DscBpp is limited to one of {8,10,12,15} BPPs.

    if ((IsMipiDsiDisplay) || (pDscCfg->SliceHeight * pDscCfg->SliceWidth < 15000))
    {
        DD_BOOL RetVal = DisplayInfoRoutinesGetRcParamertersFromCfg(pDscCfg);
        if (FALSE == RetVal)
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "DisplayInfoRoutinesGetRcParamertersFromCfg Failed");
            return FALSE;
        }
    }
    else
    {
        DD_BOOL RetVal = DisplayInfoRoutinesGenerateRcParamerters(pDscCfg);
        if (FALSE == RetVal)
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "DisplayInfoRoutinesGenerateRcParamerters Failed");
            return FALSE;
        }
    }

    if (pDscCfg->InitialOffset >= pDscCfg->RcModelSize)
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "The InitialOffset is greater than the RcModelSize");
        return FALSE;
    }

    // The muxWordSize is determined by the BitsPerComponent value:
    // When BitsPerComponent is 12bpc, muxWordSize shall be equal to 64 bits
    // When BitsPerComponent is 8 or 10bpc, muxWordSize shall be equal to 48 bits
    if (pDscCfg->BitsPerComponent <= 10)
    {
        pDscCfg->MuxWordSize = 48;
    }
    else
    {
        pDscCfg->MuxWordSize = 64;
    }

    while (pDscCfg->SliceHeight != 0)
    {
        if (DisplayInfoRoutinesComputeRcParameters(pDscCfg))
        {
            break;
        }
        else
        {
            DISP_DBG_MSG(GFXDBG_CRITICAL, "Compute RC Parameters failed ");
            DISP_DBG_MSG(GFXDBG_CRITICAL, "Reducing Slice Height by half ");
            pDscCfg->SliceHeight = pDscCfg->SliceHeight / 2;
            if (pDscCfg->SliceHeight == 0)
            {
                DISP_DBG_MSG(GFXDBG_CRITICAL, "Couldn't find valid Slice Height value ");
                DDRLSASSERT_UNEXPECTED(0, "");
                return FALSE;
            }
        }
    }

    return TRUE;
}

/***************************************************************
 * @brief Create DSC config struct.
 *
 * This function will prepare the DSC_CFG structure for the mode set arguments based on the sink DPCD values.
 * It will populate the structure and then also compute the DSC parameters required for generating the PPS packet.
 * Assumption: Caller will create the original DSC_CFG structure and initialize with the sink inputs and picture width/height as per resolution.
 * @param      pSinkDscDecoderCaps[in]    Pointer to RX DSC capability.
 * @param      pDscCfg           Pointer to the structure for populating ::DSC Config data.
 * @param      OptimizeSliceHeight
 * @return     DD_BOOL
 * @retval     TRUE    If DSC parameters are computed for the input mode set arguments.
 * @retval     FALSE   If no valid set of DSC parameters could be found.
 ***************************************************************/
DD_BOOL DisplayInfoRoutinesCreateDscCfg(DD_RX_DSC_CAPS* pSinkDscDecoderCaps, DD_IN_OUT DSC_CFG* pDscCfg, DD_BOOL OptimizeSliceHeight)
{
    DDU16 MinPixelsPerSlice;

    DDASSERT(pSinkDscDecoderCaps);
    DDASSERT(pDscCfg);

    pDscCfg->DscVersionMajor = pSinkDscDecoderCaps->DscMajorVersion;
    pDscCfg->DscVersionMinor = pSinkDscDecoderCaps->DscMinorVersion;
    pDscCfg->LineBufDepth = pSinkDscDecoderCaps->LineBufferDepth;
    pDscCfg->BlockPredEnable = pSinkDscDecoderCaps->IsBlockPredictionSupported;
    pDscCfg->SliceCount = pSinkDscDecoderCaps->SlicesPerLine;
    pDscCfg->SliceWidth = DD_ROUND_UP_DIV(pDscCfg->PicWidth, pDscCfg->SliceCount);

    if (FALSE == ((8 == pDscCfg->BitsPerComponent) || (10 == pDscCfg->BitsPerComponent) || (12 == pDscCfg->BitsPerComponent)))
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "Unsupported bits_per_component");
        return FALSE;
    }

    MinPixelsPerSlice = MIN_PIXELS_PER_DSC_SLICE_444;
    if (pDscCfg->Native422 || pDscCfg->Native420)
    {
        MinPixelsPerSlice = MIN_PIXELS_PER_DSC_SLICE_420_422;
    }

    // In case VBT gives slice height as zero, we will take slice height as (picture height / 2).
    // In case VBT gives slice height as non-zero and picture height is not evenly divisble by dsc sliceheight (from VBT)
    // we will take slice height as (picture height / 2).
    if ((pSinkDscDecoderCaps->DscSliceHeight != 0) && (pDscCfg->PicHeight % pSinkDscDecoderCaps->DscSliceHeight == 0))
    {
        pDscCfg->SliceHeight = pSinkDscDecoderCaps->DscSliceHeight;
    }
    else
    {
        if (TRUE == OptimizeSliceHeight)
        {
            // Slice Height determination
            // Sources may specify slice_height explicitly or utilize the C - Model to automatically generate Slice Height.
            // For DSC 1.2a, we choose AUTO_SLICE_HEIGHT_ALGORITHM == 1 (refer to VESA_DSC_1.2a_DSC_Tools_AN_v1.0.pdf)
            // 1 = Select smallest slice_height >= 96 that results in a valid PPS and minimizes the number of padding lines required for the final slice.
            // We also assume VActive is even
            DDASSERT(pDscCfg->PicHeight % 2 == 0);

            pDscCfg->SliceHeight = 0; // Assume Slice height to be 0
            // To generate RC parameters, it is recommended that Pixels/Slice >= ((Native420 || Native422) : 30000 ? 15000)
            for (DDU16 SliceHeight = 96; SliceHeight <= pDscCfg->PicHeight; SliceHeight += 2)
            {
                if ((0 == (pDscCfg->PicHeight % SliceHeight)) && ((pDscCfg->SliceWidth * SliceHeight) >= MinPixelsPerSlice))
                {
                    pDscCfg->SliceHeight = SliceHeight;
                    break;
                }
            }
        }
        else
        {
            pDscCfg->SliceHeight = pDscCfg->PicHeight;
        }
    }

    if (0 == pDscCfg->SliceHeight)
    {
        DISP_DBG_MSG(GFXDBG_CRITICAL, "Couldn't find valid slice height");
        return FALSE;
    }

    // Pixels/Slice = (pDscCfg->SliceHeight * pDscCfg->SliceWidth)
    // Selection b/w calculated RcRangeParameters or RcRangeParameters from .cfg files. (part of VESA C-Model) is as follows:
    //      if (Pixels/Slice >= 15000) {
    //          Choose calculated RcRangeParameters
    //      } else {
    //          Choose RcRangeParameters from .cfg files
    //      }
    // When RcRangeParameters are from .cfg files, they are defined for limited BPPs only.
    // Hence we need to limit output DscBpp to one of {8,10,12,15} BPPs.

    if ((TRUE == pSinkDscDecoderCaps->IsMipiDsiDisplay) || ((pDscCfg->SliceHeight * pDscCfg->SliceWidth) < MIN_PIXELS_PER_DSC_SLICE_444))
    {
        DDU16 Valid_Bpp[] = { 0x80, 0xA0, 0xC0, 0xF0 }; // BPP stored in U6.4 notation
        DDU8 Index;

        for (Index = 0; Index < (sizeof(Valid_Bpp) / sizeof(Valid_Bpp[0])); Index++)
        {
            if (Index != (sizeof(Valid_Bpp) / sizeof(Valid_Bpp[0])) - 1)
            {
                if ((pSinkDscDecoderCaps->MaxDscBitsPerPixel.Bppx16 >= Valid_Bpp[Index]) && (pSinkDscDecoderCaps->MaxDscBitsPerPixel.Bppx16 < Valid_Bpp[Index + 1]))
                {
                    pSinkDscDecoderCaps->MaxDscBitsPerPixel.Bppx16 = Valid_Bpp[Index];
                    break;
                }
            }
            else
            {
                // Required BPP is greater than last element. Hence taking the maximum value.
                pSinkDscDecoderCaps->MaxDscBitsPerPixel.Bppx16 = Valid_Bpp[Index];
            }
        }
    }

    pDscCfg->BitsPerPixel = pSinkDscDecoderCaps->MaxDscBitsPerPixel.Bppx16;

    // If slice count is only 1 then we can enable only 1 engine.
    if (1 == pDscCfg->SliceCount)
    {
        pDscCfg->NumVdscInstances = 1;
    }

    pDscCfg->Simple422 = pSinkDscDecoderCaps->IsSimpleYCbCr422Supported;
    pDscCfg->Native422 = pSinkDscDecoderCaps->IsYCbCrNative422Supported;
    pDscCfg->Native420 = pSinkDscDecoderCaps->IsYCbCrNative420Supported;
    pDscCfg->ConvertRgb = pSinkDscDecoderCaps->ConvertRgb;

    return DisplayInfoRoutinesComputeDscParameters(pDscCfg, pSinkDscDecoderCaps->IsMipiDsiDisplay);
}

/**
 * @brief For DP_SST calculate LinkM/N and DataM/N values.
 *
 * LinkM/LinkN = PixelClock/LinkSymbolClock
 * Link M,Link N are Mvid and Nvid data fields of MSA which are sent during vertical blanking period for
 * regenerating pixel clock. (used for main video stream clock regeneration)
 *
 * In case of eDP MSO, strm_clk is pertaining to single SST Link. So, we should be dividing pixel clock (including all SST segments in the panel)
 * by number of SST links. (caller is already doing that..hence we needn't do it here)
 *
 * DataM/DataN = Payload/Capacity = InputThroughPut/AvailableLinkBandwidth
 * In case of DP-SST, Data M/N programming is used by h/w for stuffing of dummy symbols (0 before scrambling).
 * TU is the Transfer Unit used in SST. TU Size is set to 64 link symbols.
 * (DataM/DataN)*100 gives the percentage of valid symbols present in 1 TU.
 *
 * In case of DSC, bytes per pixel is the output compressed bytes per pixel. When DSC is enabled, caller of this function already passes output compressed bytes per pixel as part of input
 * arguments. When FEC is enabled, FEC overhead must be considered while calculating Data M/N.
 *
 * Data M/N remains unchanged between MSO and non-MSO because the reduction in the number of lanes is compensated by the reduction in stream clock.
 *
 * NOTE: This function should not be called from any functions other than DpProtocolComputeDPTimings().
 *
 * @param pDpLinkData
 * @return DDSTATUS
 */
DDSTATUS DpProtocolSstComputeMNTu(DD_IN_OUT DP_LINK_DATA* pDpLinkData)
{
    DDU64 PixelClock100Hz = 0;
    DDU32 LinkSymbolClock100Hz = 0;
    DDU64 FecAdjustedLinkSymbolClock100Hz = 0;
    DDU64 IntermediateLinkMResult = 0;
    DDU64 IntermediateDataMResult = 0;
    DDU64 PixelClockHz;
    DDU32 LinkRateMbps;
    DDU64 DpDataBandwidthEfficiencyx10000;

    DDASSERT(pDpLinkData);

    // LinkSymbolClock in MHz => LinkRateMbps/10
    // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
    // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
    // LinkSymbolClock100Hz => LinkRateMbps * 10^3
    LinkRateMbps = pDpLinkData->LinkBwData.LinkRateMbps;
    LinkSymbolClock100Hz = (LinkRateMbps * DD_1K);

    PixelClockHz = pDpLinkData->LinkBwData.DotClockInHz;

    if (pDpLinkData->LinkBwData.IsMsoEnabled)
    {
        // DotClock for M,N,Tu calculations in case of MSO enabled port is DotClock/NumLinks
        PixelClockHz = PixelClockHz / pDpLinkData->LinkBwData.NumOfLinks;
    }

    if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
    {
        // SSC for Display Port spec 1.1 is defined as 0.5%
        // With SSC overhead there will be reduction in average link rate by half of SSC overhead percentage which
        // amounts to 0.25%. Rather than decreasing the link rate, raising the pixel clock by same percentage.
        // Average LinkRate = LinkRateMbps * (1 - 0.0025)
        // Average LinkRate = LinkRateMbps * 0.9975
        // 1/0.9975 approx is 1.002506
        // PixelClockHz = PixelClockHz * (1.002506)
        // PixelClockHz = (PixelClockHz * 1002506)/1000000
        PixelClockHz = ((PixelClockHz * pDpLinkData->LinkBwData.DpSscOverheadx1e6) / DD_MILLION);
    }

    // 8b/10b channel encoding
    if (pDpLinkData->LinkBwData.EnableFec)
    {
        // FEC Overhead: 2.4%
        // FEC and DSC Overhead: 2.774%
        DpDataBandwidthEfficiencyx10000 = (pDpLinkData->DscCapable) ? DP_DATA_BW_EFFICIENCY_SST_DSC_PER_10000 : DP_DATA_BW_EFFICIENCY_SST_FEC_PER_10000;
    }
    else
    {
        DpDataBandwidthEfficiencyx10000 = DP_DATA_BW_EFFICIENCY_SST_PER_10000;
    }

    // AvailableLinkBandwidth in Mbps = LinkRateMbps * LaneCount * DataBandwidthEfficiency
    // AvailableLinkBandwidth in Mbps = (LinkRateMbps * LaneCount * DataBandwidthEfficiencyx10000)/10^4
    // RequiredPixelBandwidth in bps = PixelClockHz * BitsPerPixel
    // RequiredPixelBandwidth in Mbps = (PixelClockHz * BitsPerPixel)/10^6
    // If AvailableLinkBandwidth is less than RequiredPixelBandwidth we should return FALSE else TRUE
    // Evaluation of the expression:
    // (AvailableLinkBandwidth (in Mbps) < RequiredPixelBandwidth (in Mbps))
    // (((LinkRateMbps * LaneCount * DataBandwidthEfficiencyx10000)/10^4) < ((PixelClockHz * BitsPerPixel)/10^6))
    // (((LinkRateMbps * LaneCount * DataBandwidthEfficiencyx10000)) < ((PixelClockHz * BitsPerPixel)/10^2))
    // (((LinkRateMbps * LaneCount * DataBandwidthEfficiencyx10000 * 10^2)) < ((PixelClockHz * BitsPerPixel)))

    // BitsPerPixel is in U6.4 format, to get the actual real value we have to divide the integer by 2^4.
    // typecast DataBandwidthEfficienyx10000 to DDU64 to prevent overflow
    if ((LinkRateMbps * pDpLinkData->LinkBwData.DpLaneWidthSelection * (DDU64)DpDataBandwidthEfficiencyx10000 * DD_CENT * FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR) <
        (PixelClockHz * pDpLinkData->LinkBwData.BitsPerPixel.Bppx16))
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_DPL_DP_INSUFFICIENT_LINK_BW);
        return DDS_DPL_DP_INSUFFICIENT_LINK_BW;
    }

    PixelClock100Hz = PixelClockHz / 100;

    // link M and N
    // M / N = desired pixel clock / link symbol clock
    // Don't want to use floats for this computation, so using DDU64 and precision of 10^8
    LinkSymbolClock100Hz = LinkSymbolClock100Hz / 10000;

    // Enabling FEC results in bandwidth overhead of approximately 2.774%
    // (i.e, an FEC-enabled DP link has 2.774% less link bandwidth available for transport of stream(s)).
    // Hence, FEC overhead for 1MHz clock would result in multiplier of 972661
    // Reference Bspec link: [Transcoder MN Value] https://gfxspecs.intel.com/Predator/Home/Index/4285
    FecAdjustedLinkSymbolClock100Hz = (DDU64)LinkSymbolClock100Hz;
    if (pDpLinkData->LinkBwData.EnableFec)
    {
        FecAdjustedLinkSymbolClock100Hz = ((DDU64)LinkSymbolClock100Hz * FEC_OVERHEAD_PER_MEGAHERTZ) / MEGA_HERTZ;
    }

    pDpLinkData->LinkBwData.MNTUData.LinkN = 0x80000;
    // Storing into intermediate DDU64 variable cause multiplication result may not fit into DDU32
    IntermediateLinkMResult = (DDU64)PixelClock100Hz;
    IntermediateLinkMResult *= (DDU64)pDpLinkData->LinkBwData.MNTUData.LinkN;
    IntermediateLinkMResult *= (DDU64)10000;
    IntermediateLinkMResult = (DDU64)(IntermediateLinkMResult / LinkSymbolClock100Hz);

    // Using precision of 10^8
    pDpLinkData->LinkBwData.MNTUData.LinkM = (DDU32)(IntermediateLinkMResult / 100000000);
    DDASSERT(pDpLinkData->LinkBwData.MNTUData.LinkM <= MAX_LINKM_LINKN_VALUE);

    // Data M and N =
    // Active/TU = (desired pixel clock * Bpp) / (link symbol clock * #lanes)
    pDpLinkData->LinkBwData.MNTUData.DataN = 0x800000;
    IntermediateDataMResult = (DDU64)PixelClock100Hz;
    // pDpLinkData->LinkBwData.BitsPerPixel is in U6.4 format, to get the actual real value we have to divide the integer by 2^4.
    IntermediateDataMResult = (IntermediateDataMResult * (DDU64)pDpLinkData->LinkBwData.BitsPerPixel.Bppx16) / FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR;
    IntermediateDataMResult *= (DDU64)(pDpLinkData->LinkBwData.MNTUData.DataN / (8 * pDpLinkData->LinkBwData.DpLaneWidthSelection));
    IntermediateDataMResult *= (DDU64)10000;
    IntermediateDataMResult = (DDU64)(IntermediateDataMResult / (DDU64)FecAdjustedLinkSymbolClock100Hz);

    if (pDpLinkData->LinkBwData.IsMsoEnabled)
    {
        // We have to multiply IntermediateDataM value with number of SST links. (such that Data M/N doesn't change b/w MSO and non-MSO cases)
        IntermediateDataMResult *= pDpLinkData->LinkBwData.NumOfLinks;
    }

    // Using precision of 10^8
    pDpLinkData->LinkBwData.MNTUData.DataM = (DDU32)(IntermediateDataMResult / 100000000);

    pDpLinkData->LinkBwData.MNTUData.DataTU = TU_HIGH_END;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief return the available link PBN based on link rate and lane count.
 *
 * For link rates < UHBR it is always 8b/10b encoding. 78.75 % is the 8b/10b MST data bandwidth efficiency.
 * For UHBR+ link rates it is always 128b/132b encoding. 96.71 % is the data bandwidth efficiency for 128b/132b encoding.
 *
 * AvailableLinkBandwidth in Mbps = LinkRateMbps * LaneCount * DataBandwidthEfficiency
 * AvailableLinkBandwidth in MBps = (LinkRateMbps * LaneCount * DataBandwidthEfficiency) /8
 * AvailableLinkBandwidth in PBN = (LinkRateMbps * LaneCount * DataBandwidthEfficiency) /8/(54/64)
 * AvailableLinkBandwidth in PBN = (LinkRateMbps * LaneCount * DataBandwidthEfficiency * 64) /(8 * 54)
 * AvailableLinkBandwidth in PBN = (LinkRateMbps * LaneCount * DataBandwidthEfficiency * 8) /54
 * AvailableLinkBandwidth in PBN = (LinkRateMbps * LaneCount * DataBandwidthEfficiencyx10000 * 8) /(54*10000)
 * AvailableLinkBandwidth in PBN = (LinkRateMbps * LaneCount * DataBandwidthEfficiencyx10000) /(67500).
 * @param LaneCount:
 * @param LinkRateMbps:
 * @return DDU32
 ***************************************************************/
DDU32 DpProtocolMstGetAvailableLinkPbn(DDU32 LaneCount, DDU32 LinkRateMbps)
{
    DDU32 AvailableLinkPBN;
    DDU32 DataBandwidthEfficiencyx10000;

    DataBandwidthEfficiencyx10000 = (CH_CODING_8B_10B == GET_DP_CHANNEL_CODING(LinkRateMbps)) ? DP_DATA_BW_EFFICIENCY_MST_PER_10000 : DP_DATA_BW_EFFICIENCY_128B_132B_PER_10000;
    // typecast to DDU64 to prevent overflow
    AvailableLinkPBN = ((DDU64)((DDU64)LinkRateMbps * LaneCount) * DataBandwidthEfficiencyx10000) / (67500);

    return AvailableLinkPBN;
}

/***************************************************************
 * @brief return PBNPerSlot (multiplied by 100) based on link rate and lane count.
 *
 * There are 64 time slots in 1 MTP.
 * a) In case of 128b/132b all 64 time slots of the MTP are available for VC Payloads allocation. (there's no MTP-Header in case of 128b/132b)
 * b) In case of 8b/10b timeslot-0 is used as MTP header, and the remaining 63 time slots are available for VC Payloads allocation.
 *
 * PbnPerTimeSlot/MTP = AvailableLinkPBN/AvailableMtpTimeSlots.
 * @param LaneCount:
 * @param LinkRateMbps:
 * @return DDU32
 ***************************************************************/
static DDU32 DpProtocolGetPbnPerSlotx100(DDU32 LaneCount, DDU32 LinkRateMbps)
{
    DDU32 PbnPerTimeSlotx100;
    DDU8 AvailableMtpTimeSlots;

    AvailableMtpTimeSlots = (CH_CODING_128B_132B == GET_DP_CHANNEL_CODING(LinkRateMbps)) ? AVAILABLE_MTP_TIMESLOTS_128B_132B : AVAILABLE_MTP_TIMESLOTS_8B_10B;

    PbnPerTimeSlotx100 = (DpProtocolMstGetAvailableLinkPbn(LaneCount, LinkRateMbps) * DD_CENT) / AvailableMtpTimeSlots;

    return PbnPerTimeSlotx100;
}

/***************************************************************
 * @brief For DP-MST calculate LinkM/N and DataM/N values, VC Payload time slot (TU), PBN.
 *
 * LinkM/LinkN = PixelClock/LinkSymbolClock
 * Link M,Link N are Mvid and Nvid data fields of MSA which are sent during vertical blanking period for
 * regenerating pixel clock. (used for main video stream clock regeneration)
 *
 * DataM/DataN = Payload/Capacity = InputThroughPut/AvailableLinkBandwidth
 *
 * In case of DP-MST, there is no stuffing of symbols as in DP-SST.
 * In case of DP-MST, Data M/N programming is used by h/w for Rate Governing as mentioned in section
 * "2.6.4.3 VC Payload Size Determination by a Source Payload Bandwidth Manager" of DP 1.4a spec.
 * For Rate Governing in DP Source, we needn't consider 0.6% margin for stream bandwidth. It should be considered while calculating PBN value needed for transmitting
 * pixel stream. (0.6% is the margin that accounts for the deviation of the link rate driven by downstream DP Branch devices)
 * While calculating number of time-slots allocated it is rounded-up to the nearest integer. Hence to maintain accurate data rate h/w needs Data M/N ratio.
 * For eg. #ofTimeSlots from calculations is 40.61, and VC time-slots allocated is 41. Out of 4 MTPs, 3 MTPs shall have 41 slots with data and
 * 1 MTP shall have 40 data slots and 1 VCPF so that average data rate matches 40.61%. To get the actual
 *
 * Some points to consider:
 * 1) PBN value that is sent as part of the ALLOCATE_PAYLOAD sideband message to DPRX shall always include down-spread overhead of 0.6%
 *    irrespective of whether the first hop has SSC enabled or not. This is because there might be SSC enabled downstream of the branch device.
 * 2) For VC Allocation in DP Source, will include down-spread overhead of 0.6% only when DP Source enables SSC.
 * 3) If SSC is enabled, then the SSC overhead should be facgitored into the DataM/N and LinkM/N calculations.
 *
 * In case of DSC, bytes per pixel is the output compressed bytes per pixel. When DSC is enabled, caller of this function already passes output
 * compressed bytes per pixel as part of input arguments. When FEC is enabled, FEC overhead must be considered while calculating Data M/N.
 *
 * Input Throughtput (MBps) = (PixelClockMHz * BitsPerPixel)/8
 * Available Link Bandwidth (MBps) = (LinkRateMbps * DataBandwidthEfficiency * LaneCount)/8
 * For 128b/132b, DataBandwidthEfficiency is 96.71%. (FEC is integral of the channel coding...the overhead is already considered)
 * For 8b/10b MST DataBandwidthEfficiency is considered as 80.00% (as the hardware internally multiplies programmed Data M/N with "64 time-slots" for rate-governing
 *                                                                 instead of 63 hence we are not considering actual bandwidth efficiency of 78.75%)
 *
 * DataM/DataN = (PixelClockMHz * BitsPerPixel)/(LinkRateMbps * DataBandwidthEfficiency * LaneCount)
 * Pixel Clock and Link Rate must be in same Unit notation. When compression is enabled, BitsPerPixel is the output compressed BPP.
 *
 * NOTE: This function should not be called from any functions other than DpProtocolComputeDPTimings().
 * @param pDpLinkData:
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DpProtocolMstComputeMNTuPbn(DD_IN_OUT DP_LINK_DATA* pDpLinkData)
{
    DDU64 GCD;
    DDU64 PixelClockHz, PixelClock100Hz;
    DDU64 LinkRateMbps, LinkSymbolClock100Hz;
    DDU32 Multiplier, Remainder, PBNPerSlotx100, LaneCount, DataBandwidthEfficiencyx10000;
    DDU32 MaxValue = (16 * 1024 * 1024) - 1; // max size in the register 24 bits.
    DP_CHANNEL_CODING_TYPE DpChannelCodingType;
    DDU64 N1, N2, DataM, DataN, LinkM, LinkN;

    // arg validation
    DDASSERT(pDpLinkData);
    DDASSERT(pDpLinkData->MstBwData.EnableMSTMode);

    LinkRateMbps = pDpLinkData->LinkBwData.LinkRateMbps;
    LaneCount = pDpLinkData->LinkBwData.DpLaneWidthSelection;
    PixelClockHz = pDpLinkData->LinkBwData.DotClockInHz;

    if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
    {
        // SSC for Display Port spec 1.1 is defined as 0.5%
        // With SSC overhead there will be reduction in average link rate by half of SSC overhead percentage which
        // amounts to 0.25%. Rather than decreasing the link rate, raising the pixel clock by same percentage.
        // Average LinkRate = LinkRateMbps * (1 - 0.0025)
        // Average LinkRate = LinkRateMbps * 0.9975
        // 1/0.9975 approx is 1.002506
        // PixelClockHz = PixelClockHz * (1.002506)
        // PixelClockHz = (PixelClockHz * 1002506)/1000000
        PixelClockHz = ((PixelClockHz * pDpLinkData->LinkBwData.DpSscOverheadx1e6) / DD_MILLION);
    }

    DpChannelCodingType = GET_DP_CHANNEL_CODING(LinkRateMbps);
    if (CH_CODING_128B_132B == DpChannelCodingType)
    {
        DataBandwidthEfficiencyx10000 = DP_DATA_BW_EFFICIENCY_128B_132B_PER_10000;
    }
    else
    {
        // For 8b / 10b MST DataBandwidthEfficiency is considered as 80.00% instead of actual 78.75%.
        // (as the hardware internally multiplies programmed Data M/N with "64 time-slots" for rate-governing
        // instead of 63 hence we are not considering actual bandwidth efficiency of 78.75%)
        if (TRUE == pDpLinkData->LinkBwData.EnableFec)
        {
            // FEC Overhead: 2.4%
            // FEC and End-Of-Chunks(EOC) Overhead: 2.774%
            DataBandwidthEfficiencyx10000 = (TRUE == pDpLinkData->DscCapable) ? DP_DATA_BW_EFFICIENCY_SST_DSC_PER_10000 : DP_DATA_BW_EFFICIENCY_SST_FEC_PER_10000;
        }
        else
        {
            DataBandwidthEfficiencyx10000 = DP_DATA_BW_EFFICIENCY_SST_PER_10000;
        }
    }
    PixelClock100Hz = PixelClockHz / 100;

    // Link Symbol Clock is the Clock with which Main-Link symbols are transferred within PHY and Link Layer circuits
    // of either a DPTX or DPRX for Main-Link transport.
    if (CH_CODING_128B_132B == DpChannelCodingType)
    {
        // Main-Link Symbols:
        //  a) Data link symbols and control link symbols are both 32 bits in size.
        // LinkSymbolClock in MHz => LinkRateMbps/32
        // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
        // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
        // LinkSymbolClock100Hz => (LinkRateMbps * 10^4)/32
        // LinkSymbolClock100Hz => (LinkRateMbps * 625)/2
        LinkSymbolClock100Hz = (LinkRateMbps * 625) / 2;
    }
    else // 8b/10b encoding
    {
        // Main-Link Symbols:
        //  a) Data symbols contain 8 bits of data and are encoded into 10-bit data characters by way of channel coding.
        //  b) Control link symbols are encoded into 10-bit special characters of 8b/10b (referred to as "K-codes")
        // LinkSymbolClock in MHz => LinkRateMbps/10
        // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
        // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
        // LinkSymbolClock100Hz => LinkRateMbps * 10^3
        LinkSymbolClock100Hz = (LinkRateMbps * DD_1K);
    }

    // LinkM/LinkN = PixelClock/LinkSymbolClock
    // For 8b/10b channel encoding:
    // LinkM/LinkN values programmed are transmitted as Mvid [23:0] and Nvid [23:0] fields as part of MSA during VBlank periods.
    // DP Sink regenerates pixel clock from the link symbol clock, using time stamp values Mvid and Nvid.
    // In our h/w the link only operates in Synchronous Clock mode i.e, the link symbol clock and pixel clock are synchronous
    // and the link M and N values stay constant for a given pixel rate.
    // For 128b/132b channel encoding:
    // The Mvid[23:0] and Nvid[23:0] fields used in 8b/10b Link Layer are replaced with the VFREQ[47:0] field.
    // But HW internally uses LinkM/LinkN programmed values (for both 8b/10b and 128b/132b). Hence these must be programmed for 128b/132b channel encoding also.

    GCD = DD_FindGCD(LinkSymbolClock100Hz, PixelClock100Hz);
    LinkM = (PixelClock100Hz / GCD);
    LinkN = (LinkSymbolClock100Hz / GCD);

    if (LinkM > MaxValue || LinkN > MaxValue)
    {
        DDU64 TempLinkM;
        // When values of LinkM or LinkN exceeds (2^24-1), fix LinkN to 0x8000 and calculate the LinkM value from the ratio already calculated.
        // Assume X1/X2 are 64 bit ratio values for LinkM/LinkN using GCD method. Worst case, when GCD == 1, X1 equals PixelClock100Hz value. LinkM = (X1 * 0x8000)/X2 Choosing worst case pixel clock
        // of 5940MHz, it can occupy 33 bits.(X1 * 0x8000) -> won't exceed 64 bit.
        TempLinkM = (LinkM * 0x8000) / LinkN;
        LinkM = TempLinkM;
        LinkN = 0x8000;
    }

    // This check is if we have a low M or N value it results into panel blank-out some times.
    // So just multiplying the values by a common factor to make M greater than 100 and that should hold good.
    if ((LinkM > 0) && (LinkM < 1000))
    {
        Multiplier = (DDU32)(1000 / LinkM) + 1;
        if ((LinkN * Multiplier) < MaxValue)
        {
            LinkM *= Multiplier;
            LinkN *= Multiplier;
        }
    }

    pDpLinkData->LinkBwData.MNTUData.LinkM = (DDU32)(LinkM);
    pDpLinkData->LinkBwData.MNTUData.LinkN = (DDU32)(LinkN);

    // The Mvid[23:0] and Nvid[23:0] fields used in 8b/10b Link Layer are replaced with the VFREQ[47:0] field.
    pDpLinkData->LinkBwData.MNTUData.VFreqHz = (CH_CODING_128B_132B == DpChannelCodingType) ? pDpLinkData->LinkBwData.DotClockInHz : 0;

    // Data M / N = (PixelClockHz * BitsPerPixel)/(LinkRateBps * DataBandwidthEfficiency * LaneCount)
    // Data M / N = (PixelClockHz * BitsPerPixel)/((LinkRateBps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
    // Pixel Clock and Link Rate must be in same Unit notation.
    // Data M / N = (PixelClock100Hz * BitsPerPixel)/((LinkRate100bps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
    // LinkRate100bps/10000 => LinkRateMbps
    // Data M / N = (PixelClock100Hz * BitsPerPixel)/(LinkRateMbps * DataBandwidthEfficienyx10000 * LaneCount)
    // LinkBwData.BitsPerPixel is in U6.4 format, to get the actual real value we have to divide the integer by 2^4.
    N1 = (PixelClock100Hz * pDpLinkData->LinkBwData.BitsPerPixel.Bppx16) / FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR;
    N2 = (LinkRateMbps * LaneCount) * (DDU64)DataBandwidthEfficiencyx10000;

    // Check whether Required Pixel Bandwidth is greater than the Available Link Bandwidth
    if (N1 > N2)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_DPL_DP_INSUFFICIENT_LINK_BW);
        return DDS_DPL_DP_INSUFFICIENT_LINK_BW;
    }

    GCD = DD_FindGCD(N1, N2);
    DataM = (N1 / GCD);
    DataN = (N2 / GCD);

    if (DataM > MaxValue || DataN > MaxValue)
    {
        // These cases can happen only when GCD is very less and it's very rare condition
        // We will use different approach to compute Data M & Data N Values
        // Data N will be fixed to 80000. The reason this number has been selected is because
        // it is divisible both by 8 & 10000
        DataN = 80000;
        // Data M  = Data N  * Data Ratio
        //         = (80000 * PixelClockHz * BitsPerPixel) / (LinkRateBps * DataBandwidthEfficiency * LaneCount)
        // Pixel Clock and Link Rate must be in same Unit notation.
        //         = (80000 * PixelClock100Hz * BitsPerPixel) / (LinkRate100bps * DataBandwidthEfficiency * LaneCount)
        //         = (80000 * PixelClock100Hz * BitsPerPixel) / ((LinkRate100bps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
        // LinkRate100bps/10000 => LinkRateMbps
        //         = (80000 * PixelClock100Hz * BitsPerPixel) / (LinkRateMbps * DataBandwidthEfficienyx10000 * LaneCount)
        // LinkBwData.BitsPerPixel is in U6.4 format, to get the actual real value we have to divide the integer by 2^4.
        DataM = (N1 * 80000) / N2;
    }

    if ((DataM > 0) && (DataM < 1000))
    {
        Multiplier = (DDU32)(1000 / DataM) + 1;
        if ((DataN * Multiplier) < MaxValue)
        {
            DataM *= Multiplier;
            DataN *= Multiplier;
        }
    }

    PBNPerSlotx100 = DpProtocolGetPbnPerSlotx100(LaneCount, (DDU32)LinkRateMbps);
    if (PBNPerSlotx100 == 0)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    // For determining VC Payload Size in DP Source, we needn't consider 0.6% margin for stream bandwidth when spread is not enabled.
    // (refer to Section 2.6.4.3 VC Payload Size Determination by a Source Payload Bandwidth Manager of DP1.4a spec)
    // DataM/DataN ratio should always be less than "1", Ceil(DataM * 64/DataN) will give number of allocated VC-timeslots.
    pDpLinkData->LinkBwData.MNTUData.DataTU = (DDU32)DD_ROUND_UP_DIV(DataM * 64, DataN);

    // Allocated VC time-slot for a pixel stream can be filled with either stream symbol or VCPF.
    // When there are no stream symbols to transmit while the link is enabled DP Source shall transmit VCPFs.

    // In case of MST, the link layer always operates on 4 lane count irrespective of the physical lane count value.
    // Downstream Sink devices needs to know the 4-lane boundary to properly the recover the stream.
    // Downstream Sink devices shall use only the VCPF control link symbol sequence to detect the phase of the 4 - symbol sequence unit mapping
    // to Main - Link lanes in the 2 - and 1 -lane cases.

    // For the below given configuration:
    // Link Rate: 5400 Mbps, Lane Count: 2
    // PixelClock : 146.25MHz, RR: 60Hz
    // HActive: 1680 pixels, HTotal: 2240 pixels
    // VActive: 1050 lines, VTotal: 1089 lines
    // BPP: 24
    // Ceil(DataM * 64, DataN) == Floor(DataM * 64, DataN) == 26
    // (numerator % denominator == 0)
    // which does not give opportunity for DP Source to insert VCPF during allocated VC time-slots for the pixel stream.
    // (where there is no symbol left to transmit VCPFs during the allocated VC time-slots)
    // As there are no VCPFs, downstream sink devices are unable to decode the link symbols properly.

    // It's a limitation from spec which doesn't mention what needs to be done in this case.
    // As a workaround, DP Source will increase VC time-slots by 1 when DD_ROUND_UP_DIV(DataM * 64, DataN) == DD_ROUND_DOWN_DIV(DataM * 64, DataN)
    if (DD_ROUND_UP_DIV(DataM * 64, DataN) == DD_ROUND_DOWN_DIV(DataM * 64, DataN))
    {
        pDpLinkData->LinkBwData.MNTUData.DataTU += 1;
    }

    // Restrictions on the Virtual Channel(VC) payload size in DisplayPort MST mode
    // In a x1 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 4.
    // In a x2 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 2.
    // In a x4 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 1.
    Remainder = ((pDpLinkData->LinkBwData.MNTUData.DataTU * LaneCount) % 4);
    if (Remainder != 0)
    {
        pDpLinkData->LinkBwData.MNTUData.DataTU += ((4 - Remainder) / LaneCount);
    }

    // Pixel Bandwidth required in Mbps considering 0.6% margin = (PixelClockHz * BitsPerPixel * 1.006)/1000000
    // 0.6% margin account for down-spreading overhead of 0.5% and MST Branch device link reference clock difference of up to 0.06%.
    // While calculating required pixel bandwidth we should be rounding up to the closest integer.
    //
    // Required Pixel Bandwidth in MBps = Ceil((PixelClockHz * BitsPerPixel * 1.006)/(8 * 10^6))
    // Required Pixel Bandwidth in PBN  = Ceil(((PixelClock100Hz * BitsPerPixel * 1.006) / (8 * 10^6)) / (54 / 64))
    // PixelPBN is send as part of ALLOCATE_PAYLOAD sideband message.
    // When FEC is enabled, PixelPBN should consider FEC overhead. And when DSC is enabled, PixelPBN needs to consider both FEC and EOC overhead.
    // As DataM/N considers both FEC and EOC overhead..we can easily derive PixelPBN as below:
    // Required Pixel Bandwidth in PBN = Ceil((DataM/N Ratio) * 64 * PBNPerSlot * 1.006)
    // It is independent of the number of VC Slots.
    // Actual PBN indicates how much Pixel PBN will actually be needed and is the value used in ALLOCATE_PAYLOAD sideband message
    if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
    {
        // When down-spread is enabled...DataM/N factored in the 0.25% overhead in pixel clock. So need to add extra 0.35% overhead while calculating Pixel PBN.
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlot * (1 + (1.006 - 1.025)))
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * (1 + (1.006 - 1.025)))/100
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(100*1000000)
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(100*1000000/64)
        // 1000000/64 => 15625
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(1562500)
        DDU64 Overhead = (DD_MILLION + (1006000 - pDpLinkData->LinkBwData.DpSscOverheadx1e6));
        pDpLinkData->MstBwData.ActualPBN = (DDU32)DD_ROUND_UP_DIV(((DDU64)DataM * (PBNPerSlotx100 * Overhead)), ((DDU64)DataN * 1562500));
    }
    else
    {
        // When down-spread is not enabled...DataM/N didn't factor in the 0.6% overhead. So need to consider it while calculating Pixel PBN.
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlot * 1.006)
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * 1.006)/100
        // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * 1006)/(100*1000)

        pDpLinkData->MstBwData.ActualPBN = (DDU32)DD_ROUND_UP_DIV(((DDU64)DataM * (64 * PBNPerSlotx100 * 1006)), ((DDU64)DataN * (DD_1K * DD_CENT)));
    }

    // AllocatedPBN indicates the PBN allocation within the DP Source, which is determined based on the number of slots, link rate, number of lanes
    // and HW restrictions
    pDpLinkData->MstBwData.AllocatedPBN = DD_ROUND_UP_DIV(pDpLinkData->LinkBwData.MNTUData.DataTU * PBNPerSlotx100, DD_CENT);

    // the num slot is updated with TU number
    pDpLinkData->MstBwData.VcPayloadTableNumSlots = (DDU8)pDpLinkData->LinkBwData.MNTUData.DataTU;

    pDpLinkData->LinkBwData.MNTUData.DataM = (DDU32)DataM;
    pDpLinkData->LinkBwData.MNTUData.DataN = (DDU32)DataN;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief For DP-SST/DP-MST calculate LinkM/N and DataM/N values, VC Payload time slot (TU), PBN.
 *
 * LinkM/LinkN = PixelClock/LinkSymbolClock
 * Link M,Link N are Mvid and Nvid data fields of MSA which are sent during vertical blanking period for
 * regenerating pixel clock. (used for main video stream clock regeneration)
 *
 * DataM/DataN = Payload/Capacity = InputThroughPut/AvailableLinkBandwidth
 *
 * In case of eDP MSO, strm_clk is pertaining to single SST Link. So, we should be dividing pixel clock (including all SST segments in the panel)
 * by number of SST links. (caller is already doing that..hence we needn't do it here)
 *
 * In case of DP-SST, Data M/N programming is used by h/w for stuffing of dummy symbols (0 before scrambling).
 * TU is the Transfer Unit used in SST. TU Size is set to 64 link symbols.
 * (DataM/DataN)*100 gives the percentage of valid symbols present in 1 TU.
 *
 * Data M/N remains unchanged between MSO and non-MSO because the reduction in the number of lanes is compensated by the reduction in stream clock.
 * In case of eDP MSO, as the caller of this function is passing stream clock pertaining to single SST Link as input arguments, we have to multiply IntermediateDataM value
 * with number of SST links. (such that Data M/N doesn't change b/w MSO and non-MSO cases)
 *
 * In case of DP-MST, there is no stuffing of symbols as in DP-SST.
 * In case of DP-MST, Data M/N programming is used by h/w for Rate Governing as mentioned in section
 * "2.6.4.3 VC Payload Size Determination by a Source Payload Bandwidth Manager" of DP 1.4a spec.
 * For Rate Governing in DP Source, we needn't consider 0.6% margin for stream bandwidth. It should be considered while calculating PBN value needed for transmitting
 * pixel stream. (0.6% is the margin that accounts for the deviation of the link rate driven by downstream DP Branch devices)
 * While calculating number of time-slots allocated it is rounded-up to the nearest integer. Hence to maintain accurate data rate h/w needs Data M/N ratio.
 * For eg. #ofTimeSlots from calculations is 40.61, and VC time-slots allocated is 41. Out of 4 MTPs, 3 MTPs shall have 41 slots with data and
 * 1 MTP shall have 40 data slots and 1 VCPF so that average data rate matches 40.61%. To get the actual
 *
 * Some points to consider:
 * 1) PBN value that is sent as part of the ALLOCATE_PAYLOAD sideband message to DPRX shall always include down-spread overhead of 0.6%
 *    irrespective of whether the first hop has SSC enabled or not. This is because there might be SSC enabled downstream of the branch device.
 * 2) For VC Allocation in DP Source, will include down-spread overhead of 0.6% only when DP Source enables SSC.
 * 3) If SSC is enabled, then the SSC overhead should be factored into the DataM/N and LinkM/N calculations.
 *
 * In case of DSC, bytes per pixel is the output compressed bytes per pixel. When DSC is enabled, caller of this function already passes output
 * compressed bytes per pixel as part of input arguments. When FEC is enabled, FEC overhead must be considered while calculating Data M/N.
 *
 * Input Throughtput (MBps) = (PixelClockMHz * BitsPerPixel)/8
 * Available Link Bandwidth (MBps) = (LinkRateMbps * DataBandwidthEfficiency * LaneCount)/8
 * For 128b/132b, DataBandwidthEfficiency is 96.71%. (FEC is integral of the channel coding...the overhead is already considered)
 * For 8b/10b MST DataBandwidthEfficiency is considered as 80.00% (as the hardware internally multiplies programmed Data M/N with "64 time-slots" for rate-governing
 *                                                                 instead of 63 hence we are not considering actual bandwidth efficiency of 78.75%)
 *
 * DataM/DataN = (PixelClockMHz * BitsPerPixel)/(LinkRateMbps * DataBandwidthEfficiency * LaneCount)
 * Pixel Clock and Link Rate must be in same Unit notation. When compression is enabled, BitsPerPixel is the output compressed BPP.
 *
 * NOTE: This function should not be called from any functions other than DpProtocolComputeDPTimings().
 * @param pDpLinkData:
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DpProtocolComputeMNTuPbnConsideringEoc(DD_IN_OUT DP_LINK_DATA* pDpLinkData)
{
    DDU64 GCD, N1, N2, DataM, DataN, LinkM, LinkN;
    DDU64 PixelClockHz, PixelClock100Hz, PixelClockKHz, LinkRateMbps, LinkSymbolClock100Hz, LinkSymbolClockKHz;
    DDU32 Multiplier, Remainder, PBNPerSlotx100, LaneCount, DataBandwidthEfficiencyx10000, SliceWidth;
    DDU32 MaxValue = (16 * 1024 * 1024) - 1; // max size in the register 24 bits.
    DP_CHANNEL_CODING_TYPE DpChannelCodingType;
    DDU32 ChunkSize, PaddingBytesPerChunk, BytesPerClock, EoCBSExtraBitsPerScanline, NumOfBitsPerSymbolClock, HBlankIncLinkClocks, K;
    DD_BITS_PER_PIXEL BitsPerPixel;
    DDU16 TuHighEnd;

    // arg validation
    DDASSERT(pDpLinkData);

    LinkRateMbps = pDpLinkData->LinkBwData.LinkRateMbps;
    LaneCount = pDpLinkData->LinkBwData.DpLaneWidthSelection;
    DpChannelCodingType = GET_DP_CHANNEL_CODING(pDpLinkData->LinkBwData.LinkRateMbps);
    PixelClockHz = pDpLinkData->LinkBwData.DotClockInHz;

    if (pDpLinkData->LinkBwData.IsMsoEnabled)
    {
        // DotClock for M,N,Tu calculations in case of MSO enabled port is DotClock/NumLinks
        PixelClockHz = PixelClockHz / pDpLinkData->LinkBwData.NumOfLinks;
    }

    if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
    {
        // SSC for Display Port spec 1.1 is defined as 0.5%
        // With SSC overhead there will be reduction in average link rate by half of SSC overhead percentage which
        // amounts to 0.25%. Rather than decreasing the link rate, raising the pixel clock by same percentage.
        // Average LinkRate = LinkRateMbps * (1 - 0.0025)
        // Average LinkRate = LinkRateMbps * 0.9975
        // 1/0.9975 approx is 1.002506
        // PixelClockHz = PixelClockHz * (1.002506)
        // PixelClockHz = (PixelClockHz * 1002506)/1000000

        // On MTL+ C20 PHY, SSC overhead is 0.45% compared to typical 0.5%.
        // With SSC overhead there will be reduction in average link rate by 0.225%.
        // Rather than decreasing the link rate, raising the pixel clock by same percentage.
        // Average LinkRate = LinkRateMbps * (1 - 0.00225)
        // Average LinkRate = LinkRateMbps * 0.99775
        // 1/0.99775 approx is 1.002255
        // PixelClockHz = PixelClockHz * (1.002255)
        // PixelClockHz = (PixelClockHz * 1002255)/1000000
        PixelClockHz = ((PixelClockHz * pDpLinkData->LinkBwData.DpSscOverheadx1e6) / DD_MILLION);
    }

    PixelClock100Hz = PixelClockHz / DD_CENT;
    PixelClockKHz = PixelClockHz / DD_1K;
    BitsPerPixel.Bppx16 = pDpLinkData->LinkBwData.BitsPerPixel.Bppx16;

    // Link Symbol Clock is the Clock with which Main-Link symbols are transferred within PHY and Link Layer circuits
    // of either a DPTX or DPRX for Main-Link transport.
    if (CH_CODING_128B_132B == DpChannelCodingType)
    {
        DataBandwidthEfficiencyx10000 = DP_DATA_BW_EFFICIENCY_128B_132B_PER_10000;
        NumOfBitsPerSymbolClock = 32;
        // Main-Link Symbols:
        //  a) Data link symbols and control link symbols are both 32 bits in size.
        // LinkSymbolClock in MHz => LinkRateMbps/32
        // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
        // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
        // LinkSymbolClock100Hz => (LinkRateMbps * 10^4)/32
        // LinkSymbolClock100Hz => (LinkRateMbps * 625)/2
        LinkSymbolClock100Hz = (LinkRateMbps * 625) / 2;
    }
    else // 8b/10b encoding
    {
        // For 8b / 10b MST DataBandwidthEfficiency is considered as 80.00% instead of actual 78.75%.
        // (as the hardware internally multiplies programmed Data M/N with "64 time-slots" for rate-governing
        // instead of 63 hence we are not considering actual bandwidth efficiency of 78.75%)
        // FEC Overhead: 2.4%

        DataBandwidthEfficiencyx10000 = (TRUE == pDpLinkData->LinkBwData.EnableFec) ? DP_DATA_BW_EFFICIENCY_SST_FEC_PER_10000 : DP_DATA_BW_EFFICIENCY_SST_PER_10000;

        NumOfBitsPerSymbolClock = 8;
        // Main-Link Symbols:
        //  a) Data symbols contain 8 bits of data and are encoded into 10-bit data characters by way of channel coding.
        //  b) Control link symbols are encoded into 10-bit special characters of 8b/10b (referred to as "K-codes")
        // LinkSymbolClock in MHz => LinkRateMbps/10
        // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
        // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
        // LinkSymbolClock100Hz => LinkRateMbps * 10^3
        LinkSymbolClock100Hz = (LinkRateMbps * DD_1K);
    }

    LinkSymbolClockKHz = LinkSymbolClock100Hz / 10;

    // LinkM/LinkN = PixelClock/LinkSymbolClock
    // For 8b/10b channel encoding:
    // LinkM/LinkN values programmed are transmitted as Mvid [23:0] and Nvid [23:0] fields as part of MSA during VBlank periods.
    // DP Sink regenerates pixel clock from the link symbol clock, using time stamp values Mvid and Nvid.
    // In our h/w the link only operates in Synchronous Clock mode i.e, the link symbol clock and pixel clock are synchronous
    // and the link M and N values stay constant for a given pixel rate.
    // For 128b/132b channel encoding:
    // The Mvid[23:0] and Nvid[23:0] fields used in 8b/10b Link Layer are replaced with the VFREQ[47:0] field.
    // But HW internally uses LinkM/LinkN programmed values (for both 8b/10b and 128b/132b). Hence these must be programmed for 128b/132b channel encoding also.

    GCD = DD_FindGCD(LinkSymbolClock100Hz, PixelClock100Hz);
    LinkM = (PixelClock100Hz / GCD);
    LinkN = (LinkSymbolClock100Hz / GCD);

    if (LinkM > MaxValue || LinkN > MaxValue)
    {
        DDU64 TempLinkM;
        // When values of LinkM or LinkN exceeds (2^24-1), fix LinkN to 0x8000 and calculate the LinkM value from the ratio already calculated.
        // Assume X1/X2 are 64 bit ratio values for LinkM/LinkN using GCD method. Worst case, when GCD == 1, X1 equals PixelClock100Hz value.
        // LinkM = (X1 * 0x8000)/X2 Choosing worst case pixel clock 5940MHz, it can occupy 33 bits.(X1 * 0x8000) -> won't exceed 64 bit.
        TempLinkM = (LinkM * 0x8000) / LinkN;
        LinkM = TempLinkM;
        LinkN = 0x8000;
    }

    // This check is if we have a low M or N value it results into panel blank-out some times.
    // So just multiplying the values by a common factor to make M greater than 100 and that should hold good.
    if ((LinkM > 0) && (LinkM < 1000))
    {
        Multiplier = (DDU32)(1000 / LinkM) + 1;
        if ((LinkN * Multiplier) < MaxValue)
        {
            LinkM *= Multiplier;
            LinkN *= Multiplier;
        }
    }

    pDpLinkData->LinkBwData.MNTUData.LinkM = (DDU32)(LinkM);
    pDpLinkData->LinkBwData.MNTUData.LinkN = (DDU32)(LinkN);

    // The Mvid[23:0] and Nvid[23:0] fields used in 8b/10b Link Layer are replaced with the VFREQ[47:0] field.
    pDpLinkData->LinkBwData.MNTUData.VFreqHz = (CH_CODING_128B_132B == DpChannelCodingType) ? pDpLinkData->LinkBwData.DotClockInHz : 0;

    // For MST 1 EOC is 4 LS because for both 128b/132b and 8b/10b MST, DP Link Layer always operates on four Main-Link Lanes irrespective
    // of the underlying Physical Lane Count.
    // 8b/10b channel encoding 1 LS occupies 8 bits. For each slice need to send extra 8*4 bits
    // 128b/132b channel encoding 1 LS occupies 32 bits. For each slice need to send extra 32*4 bits
    // EOCOverhead per horizontal scanline = NumOfSlices * (32 or 128)
    // EOC is send during VActive region..so extra pixel bandwidth is  approx EOC*VActive*RR
    if (TRUE == pDpLinkData->DscCapable && pDpLinkData->DscSlicesPerScanline != 0 && pDpLinkData->HActive != 0)
    {
        // slicewidth = ceil(HActive/NumOfSlicesPerScanline)
        SliceWidth = DD_ROUND_UP_DIV(pDpLinkData->HActive, pDpLinkData->DscSlicesPerScanline);
        // chunksize = ceil(slicewidth*bitsperpixel/8) in Bytes
        ChunkSize = DD_ROUND_UP_DIV(SliceWidth * BitsPerPixel.Bppx16, 8 * FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR);
        // Bytes transferred per link symbol clock.
        // 8b/10b channel encoding (1 LS = 8 bits = 1 byte)
        // 128b/132b channel encoding (1 LS = 32 bits = 4 bytes)
        if (CH_CODING_128B_132B == DpChannelCodingType)
        {
            BytesPerClock = 16;
        }
        else
        {
            BytesPerClock = (TRUE == pDpLinkData->MstBwData.EnableMSTMode) ? 4 : LaneCount;
        }
        // link symbol clocks per slice = ChunkSize/BytesPerClock
        // PaddingBytesPerChunk = ((CEILING(ChunkSize / BytesPerClock) * Bytes Per Clock) - ChunkSize)
        PaddingBytesPerChunk = ((DD_ROUND_UP_DIV(ChunkSize, BytesPerClock) * BytesPerClock) - ChunkSize);
        // For MST 1 End-Of-Chunk(EOC) is 4 LS. For each slice need to send extra (32 or 8) * 4 + Padding bits
        // 8b/10b channel encoding: EOC/BS Overhead per horizontal scanline = (NumOfSlicesPerScanline + 1) * 32 + NumOfSlicesPerScanline * PaddingBytesPerChunk * 8
        // 128b/132b channel encoding: EOC/BS Overhead per horizontal scanline = (NumOfSlicesPerScanline + 1) * 128 + NumOfSlicesPerScanline * PaddingBytesPerChunk * 8
        // Extra bits due to EOC overhead are send during Active region of the frame
        EoCBSExtraBitsPerScanline = ((pDpLinkData->DscSlicesPerScanline + 1) * NumOfBitsPerSymbolClock * 4 + pDpLinkData->DscSlicesPerScanline * PaddingBytesPerChunk * 8);

        HBlankIncLinkClocks = pDpLinkData->HBlankIncLinkSymbolClocks;

        // Data M / N = (PixelClockHz * BitsPerPixel)/(LinkRateBps * DataBandwidthEfficiency * LaneCount)
        // Data M / N = (PixelClockHz * BitsPerPixel)/((LinkRateBps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
        // Pixel Clock and Link Rate must be in same Unit notation.
        // Data M / N = (PixelClockKHz * BitsPerPixel)/((LinkRateKbps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
        // LinkRateKbps = (LinkRateMbps * 1000)
        // Data M / N = (PixelClockKHz * BitsPerPixel)/((LinkRateMbps * 1000 * DataBandwidthEfficienyx10000 * LaneCount)/10000)
        // Data M / N = (PixelClockKHz * BitsPerPixel)/((LinkRateMbps * DataBandwidthEfficienyx10000 * LaneCount)/10)

        // AdjBitsPerPixel = ((ChunkSize * 8 * NumOfSlicesPerScanline) + X)/HA
        // where X is the extra bits that are sent within HActive like EOC etc, HA is Horizontal Active
        // ChunkSize is in Bytes, so need to multiply by 8 to convert into bits
        // AdjPixelClockKHz = PixelClockKHz * HA/ReducedHA
        // ReducedHA = HA - reduction in HActive
        // DataM = AdjPixelClockKHz * AdjBitsPerPixel
        // DataM = PixelClockKHz * HActive * ((ChunkSize * 8* NumOfSlicesPerScanline) + EoCBSExtraBitsPerScanline)/(ReducedHActive * HActive)
        // DataM = PixelClockKHz * ((ChunkSize * 8 * NumOfSlicesPerScanline) + EoCBSExtraBitsPerScanline)/(ReducedHActive)
        // DataN = (LinkRateMbps * DataBandwidthEfficiencyx10000 * LaneCount)/10

        // To avoid division, you can multiply ReducedHA to the DataN
        // For simplification...consider numerator/denominator as N1 and N2.
        // N1 = PixelClockKHz * ((ChunkSize * 8 * NumOfSlicesPerScanline) + EoCBSExtraBitsPerScanline)
        // N2 = (ReducedHA)*(LinkRateMbps * DataBandwidthEfficiencyx10000 * LaneCount)/10
        // ReducedHA = HActive - ((HBlankIncLinkClocks * PixelClockKHz)/LinkSymbolClockKHz)
        // ReducedHA = (HActive * LinkSymbolClockKHz - HBlankIncLinkClocks * PixelClockKHz)/LinkSymbolClockKHz
        // N2 = (ReducedHA)*(LinkRateMbps * DataBandwidthEfficiencyx10000 * LaneCount)/10
        // N2 = ((HActive * LinkSymbolClockKHz - HBlankIncLinkClocks * PixelClockKHz)/LinkSymbolClockKHz)*(LinkRateMbps * DataBandwidthEfficiencyx10000 * LaneCount)/10
        // N2 = ((HActive * LinkSymbolClockKHz - HBlankIncLinkClocks * PixelClockKHz) * DataBandwidthEfficiencyx10000 * LaneCount * (LinkRateMbps/LinkSymbolClockKHz))/10

        if (CH_CODING_128B_132B == DpChannelCodingType)
        {
            // LinkSymbolClockKHz = (LinkRateMbps * 1000)/ 32
            // LinkSymbolClockKHz = (LinkRateMbps * 125)/ 4
            // LinkRateMbps = (LinkSymbolClockKHz * 4)/ 125
            // N1 = (PixelClockKHz * ((ChunkSize * 8 * NumOfSlicesPerScanline) + EoCBSExtraBitsPerScanline))
            // N2 = ((HActive * LinkSymbolClockKHz - HBlankIncLinkClocks * PixelClockKHz) * DataBandwidthEfficiencyx10000 * LaneCount * 4)/1250
            N1 = ((DDU64)PixelClockKHz * (((DDU64)ChunkSize * (8 * pDpLinkData->DscSlicesPerScanline)) + EoCBSExtraBitsPerScanline));
            N2 = ((DDU64)((DDU64)((DDU64)pDpLinkData->HActive * LinkSymbolClockKHz - (DDU64)PixelClockKHz * HBlankIncLinkClocks) * LaneCount * DataBandwidthEfficiencyx10000 * 4)) / 1250;
        }
        else
        {
            // LinkRateMbps = LinkSymbolClockKHz / 100
            // N1 = (PixelClockKHz * ((ChunkSize * 8 * NumOfSlicesPerScanline) + EoCBSExtraBitsPerScanline))
            // N2 = ((HActive * LinkSymbolClockKHz - HBlankIncLinkClocks * PixelClockKHz) * DataBandwidthEfficiencyx10000 * LaneCount)/DD_1K
            if (LaneCount == 1)
                K = 1000;
            else if (LaneCount == 2)
                K = 500;
            else
                K = 250;

            N1 = ((DDU64)PixelClockKHz * (((DDU64)ChunkSize * (8 * pDpLinkData->DscSlicesPerScanline)) + EoCBSExtraBitsPerScanline));
            N2 = ((DDU64)((DDU64)((DDU64)pDpLinkData->HActive * LinkSymbolClockKHz - (DDU64)PixelClockKHz * HBlankIncLinkClocks) * DataBandwidthEfficiencyx10000)) / K;
        }
    }
    else
    {
        // Data M / N = (PixelClockHz * BitsPerPixel)/(LinkRateBps * DataBandwidthEfficiency * LaneCount)
        // Data M / N = (PixelClockHz * BitsPerPixel)/((LinkRateBps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
        // Pixel Clock and Link Rate must be in same Unit notation.
        // Data M / N = (PixelClock100Hz * BitsPerPixel)/((LinkRate100bps * DataBandwidthEfficienyx10000 * LaneCount)/10000)
        // LinkRate100bps/10000 => LinkRateMbps
        // Data M / N = (PixelClock100Hz * BitsPerPixel)/(LinkRateMbps * DataBandwidthEfficienyx10000 * LaneCount)
        // BitsPerPixel is in U6.4 format, to get the actual real value we have to divide the integer by 2^4.
        N1 = (PixelClock100Hz * BitsPerPixel.Bppx16) / FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR;
        N2 = (LinkRateMbps * LaneCount) * (DDU64)DataBandwidthEfficiencyx10000;
    }

    // Check whether Required Pixel Bandwidth is greater than the Available Link Bandwidth
    if (N1 > N2)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_DPL_DP_INSUFFICIENT_LINK_BW);
        return DDS_DPL_DP_INSUFFICIENT_LINK_BW;
    }

    GCD = DD_FindGCD(N1, N2);
    DataM = (N1 / GCD);
    DataN = (N2 / GCD);

    if (DataM > MaxValue || DataN > MaxValue)
    {
        DDU64 Temp;
        // These cases can happen only when GCD is very less and we shall use different approach to compute Data M & Data N Values
        // Data N will be fixed to 80000. The reason this number has been selected is because it is divisible by both 8 & 10000
        // Data M  = Data N  * Data Ratio
        Temp = DD_ROUND_UP_DIV(DataM * 80000, DataN);
        DataM = Temp;
        DataN = 80000;
    }

    if ((DataM > 0) && (DataM < 1000))
    {
        Multiplier = (DDU32)(1000 / DataM) + 1;
        if ((DataN * Multiplier) < MaxValue)
        {
            DataM *= Multiplier;
            DataN *= Multiplier;
        }
    }

    // For determining VC Payload Size in DP Source, we needn't consider 0.6% margin for stream bandwidth when spread is not enabled.
    // (refer to Section 2.6.4.3 VC Payload Size Determination by a Source Payload Bandwidth Manager of DP1.4a spec)
    // DataM/DataN ratio should always be less than "1", Ceil(DataM * 64/DataN) will give number of allocated VC-timeslots.
    pDpLinkData->LinkBwData.MNTUData.DataTU = (DDU32)DD_ROUND_UP_DIV(DataM * 64, DataN);

    // Allocated VC time-slot for a pixel stream can be filled with either stream symbol or VCPF.
    // When there are no stream symbols to transmit while the link is enabled DP Source shall transmit VCPFs.

    // In case of MST, the link layer always operates on 4 lane count irrespective of the physical lane count value.
    // Downstream Sink devices needs to know the 4-lane boundary to properly the recover the stream.
    // Downstream Sink devices shall use only the VCPF control link symbol sequence to detect the phase of the 4 - symbol sequence unit mapping
    // to Main - Link lanes in the 2 - and 1 -lane cases.

    // For the below given configuration:
    // Link Rate: 5400 Mbps, Lane Count: 2
    // PixelClock : 146.25MHz, RR: 60Hz
    // HActive: 1680 pixels, HTotal: 2240 pixels
    // VActive: 1050 lines, VTotal: 1089 lines
    // BPP: 24
    // Ceil(DataM * 64, DataN) == Floor(DataM * 64, DataN) == 26
    // (numerator % denominator == 0)
    // which does not give opportunity for DP Source to insert VCPF during allocated VC time-slots for the pixel stream.
    // (where there is no symbol left to transmit VCPFs during the allocated VC time-slots)
    // As there are no VCPFs, downstream sink devices are unable to decode the link symbols properly.

    // It's a limitation from spec which doesn't mention what needs to be done in this case.
    // As a workaround, DP Source will increase VC time-slots by 1 when DD_ROUND_UP_DIV(DataM * 64, DataN) == DD_ROUND_DOWN_DIV(DataM * 64, DataN)
    if (DD_ROUND_UP_DIV(DataM * 64, DataN) == DD_ROUND_DOWN_DIV(DataM * 64, DataN))
    {
        pDpLinkData->LinkBwData.MNTUData.DataTU += 1;
    }

    if (CH_CODING_8B_10B == DpChannelCodingType)
    {
        // Restrictions on the Virtual Channel(VC) payload size in DisplayPort MST mode (applicable only for 8b/10b channel encoding)
        // In a x1 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 4.
        // In a x2 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 2.
        // In a x4 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 1.
        Remainder = ((pDpLinkData->LinkBwData.MNTUData.DataTU * LaneCount) % 4);
        if (Remainder != 0)
        {
            pDpLinkData->LinkBwData.MNTUData.DataTU += ((4 - Remainder) / LaneCount);
        }
    }

    // a) 8b/10b DPRX Device or 128b/132b DPRX Device supporting multiple streams (MST_CAP(21h[0] == 1))
    // b) 8b/10b DPRX Device or 128b/132b DPRX Device supporting single stream sideband message (MST_CAP(21h[0] == 0) && SS_SB_MSG_SUP(21h[1] == 1))
    // PixelPBN is send as part of ALLOCATE_PAYLOAD sideband message to downstream DP Branch devices.
    if ((TRUE == pDpLinkData->MstBwData.EnableMSTMode) || (TRUE == pDpLinkData->IsSsSbmSupported))
    {
        PBNPerSlotx100 = DpProtocolGetPbnPerSlotx100(LaneCount, (DDU32)LinkRateMbps);
        if (PBNPerSlotx100 == 0)
        {
            DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
            return DDS_UNSUCCESSFUL;
        }

        // Pixel Bandwidth required in Mbps considering 0.6% spread margin = (PixelClockHz * BitsPerPixel * 1.006)/1000000
        // 0.6% margin account for down-spreading overhead of 0.5%, and DP Branch device link reference clock difference of up to 0.06%.
        // While calculating required pixel bandwidth we should be rounding up to the closest integer.
        // Required Pixel Bandwidth in MBps = Ceil((PixelClockHz * BitsPerPixel * 1.006)/(8 * 10^6))
        // Required Pixel Bandwidth in PBN  = Ceil(((PixelClock100Hz * BitsPerPixel * 1.006) / (8 * 10^6)) / (54 / 64))

        // When FEC is enabled, PixelPBN should consider FEC overhead. And when DSC is enabled, PixelPBN needs to consider EOC overhead.
        // As DataM/N considers both FEC/EOC overhead..we can easily determine PixelPBN as below:
        // Required Pixel Bandwidth in PBN = Ceil((DataM/N Ratio) * 64 * PBNPerSlot * 1.006)
        // It is independent of the number of VC Slots.
        // Actual PBN indicates how much Pixel PBN will actually be needed and is the value used in ALLOCATE_PAYLOAD sideband message
        if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
        {
            // When down-spread is enabled...DataM/N factored in the 0.25% overhead in pixel clock. So need to add extra 0.35% overhead while calculating Pixel PBN.
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlot * (1 + (1.006 - 1.025)))
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * (1 + (1.006 - 1.025)))/100
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(100*1000000)
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(100*1000000/64)
            // 1000000/64 => 15625
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(1562500)
            DDU64 Overhead = (DD_MILLION + (1006000 - pDpLinkData->LinkBwData.DpSscOverheadx1e6));
            pDpLinkData->MstBwData.ActualPBN = (DDU32)DD_ROUND_UP_DIV(((DDU64)DataM * (PBNPerSlotx100 * Overhead)), ((DDU64)DataN * 1562500));
        }
        else
        {
            // When down-spread is not enabled...DataM/N didn't factor in the spread overhead. So need to consider it while calculating Pixel PBN.
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlot * 1.006)
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * 1.006)/100
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * 1006)/(100*1000)
            pDpLinkData->MstBwData.ActualPBN = (DDU32)DD_ROUND_UP_DIV(((DDU64)DataM * (64 * PBNPerSlotx100 * 1006)), ((DDU64)DataN * (DD_1K * DD_CENT)));
        }

        // AllocatedPBN indicates the PBN allocation within the DP Source, which is determined based on the number of slots, link rate, number of lanes
        // and HW restrictions
        pDpLinkData->MstBwData.AllocatedPBN = DD_ROUND_UP_DIV(pDpLinkData->LinkBwData.MNTUData.DataTU * PBNPerSlotx100, DD_CENT);

        // the num slot is updated with TU number
        pDpLinkData->MstBwData.VcPayloadTableNumSlots = (DDU8)pDpLinkData->LinkBwData.MNTUData.DataTU;

        // As ActualPBN contains the rounded-up Pixel PBN value, need to check whether the final rounded value
        // is within the Time-slot limits.
        TuHighEnd = (CH_CODING_128B_132B == DpChannelCodingType) ? TU_HIGH_END : (TU_HIGH_END - 1);
        if ((pDpLinkData->MstBwData.ActualPBN * DD_CENT) > (TuHighEnd * PBNPerSlotx100))
        {
            DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
            return DDS_UNSUCCESSFUL;
        }
    }
    else
    {
        // For DP-SST, 8b/10b Channel Encoding, Size of TU is 64
        // For 128b/132b Channel Encoding, TU must be programmed to actual number of time-slots occupied by the pixel data.
        if (CH_CODING_8B_10B == DpChannelCodingType)
        {
            pDpLinkData->LinkBwData.MNTUData.DataTU = TU_HIGH_END;
        }

        if (pDpLinkData->LinkBwData.IsMsoEnabled)
        {
            // We have to multiply DataM value with number of SST links. (such that Data M/N doesn't change b/w MSO and non-MSO cases)
            DataM *= pDpLinkData->LinkBwData.NumOfLinks;
        }
    }

    // DataM and DataN values shall always be less than (2^24-1), so we can safely typecast
    pDpLinkData->LinkBwData.MNTUData.DataM = (DDU32)DataM;
    pDpLinkData->LinkBwData.MNTUData.DataN = (DDU32)DataN;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief For DP-SST/DP-MST calculate LinkM/N and DataM/N values, VC Payload time slot (TU), PBN.
 *
 * LinkM/LinkN = PixelClock/LinkSymbolClock
 * Link M,Link N are Mvid and Nvid data fields of MSA which are sent during vertical blanking period for
 * regenerating pixel clock. (used for main video stream clock regeneration)
 *
 * DataM/DataN = Payload/Capacity = InputThroughPut/AvailableLinkBandwidth
 *
 * In case of eDP MSO, strm_clk is pertaining to single SST Link. So, we should be dividing pixel clock (including all SST segments in the panel)
 * by number of SST links. (caller is already doing that..hence we needn't do it here)
 *
 * In case of DP-SST, Data M/N programming is used by h/w for stuffing of dummy symbols (0 before scrambling).
 * TU is the Transfer Unit used in SST. TU Size is set to 64 link symbols.
 * (DataM/DataN)*100 gives the percentage of valid symbols present in 1 TU.
 *
 * Data M/N remains unchanged between MSO and non-MSO because the reduction in the number of lanes is compensated by the reduction in stream clock.
 * In case of eDP MSO, as the caller of this function is passing stream clock pertaining to single SST Link as input arguments, we have to multiply IntermediateDataM value
 * with number of SST links. (such that Data M/N doesn't change b/w MSO and non-MSO cases)
 *
 * In case of DP-MST, there is no stuffing of symbols as in DP-SST.
 * In case of DP-MST, Data M/N programming is used by h/w for Rate Governing as mentioned in section
 * "2.6.4.3 VC Payload Size Determination by a Source Payload Bandwidth Manager" of DP 1.4a spec.
 * For Rate Governing in DP Source, we needn't consider 0.6% margin for stream bandwidth. It should be considered while calculating PBN value needed for transmitting
 * pixel stream. (0.6% is the margin that accounts for the deviation of the link rate driven by downstream DP Branch devices)
 * While calculating number of time-slots allocated it is rounded-up to the nearest integer. Hence to maintain accurate data rate h/w needs Data M/N ratio.
 * For eg. #ofTimeSlots from calculations is 40.61, and VC time-slots allocated is 41. Out of 4 MTPs, 3 MTPs shall have 41 slots with data and
 * 1 MTP shall have 40 data slots and 1 VCPF so that average data rate matches 40.61%. To get the actual
 *
 * Some points to consider:
 * 1) PBN value that is sent as part of the ALLOCATE_PAYLOAD sideband message to DPRX shall always include down-spread overhead of 0.6%
 *    irrespective of whether the first hop has SSC enabled or not. This is because there might be SSC enabled downstream of the branch device.
 * 2) For VC Allocation in DP Source, will include down-spread overhead of 0.6% only when DP Source enables SSC.
 * 3) If SSC is enabled, then the SSC overhead should be factored into the DataM/N and LinkM/N calculations.
 *
 * In case of DSC, bytes per pixel is the output compressed bytes per pixel. When DSC is enabled, caller of this function already passes output
 * compressed bytes per pixel as part of input arguments. When FEC is enabled, FEC overhead must be considered while calculating Data M/N.
 *
 * Input Throughtput (MBps) = (PixelClockMHz * BitsPerPixel)/8
 * Available Link Bandwidth (MBps) = (LinkRateMbps * DataBandwidthEfficiency * LaneCount)/8
 * For 128b/132b, DataBandwidthEfficiency is 96.71%. (FEC is integral of the channel coding...the overhead is already considered)
 * For 8b/10b MST DataBandwidthEfficiency is considered as 80.00% (as the hardware internally multiplies programmed Data M/N with "64 time-slots" for rate-governing
 *                                                                 instead of 63 hence we are not considering actual bandwidth efficiency of 78.75%)
 *
 * DataM/DataN = (PixelClockMHz * BitsPerPixel)/(LinkRateMbps * DataBandwidthEfficiency * LaneCount)
 * Pixel Clock and Link Rate must be in same Unit notation. When compression is enabled, BitsPerPixel is the output compressed BPP.
 *
 * NOTE: This function should not be called from any functions other than DpProtocolComputeDPTimings().
 * @param pDpLinkData:
 * @return DDSTATUS
 ***************************************************************/
DDSTATUS DpProtocolComputeMNTuPbnLinkLayerSymbolCount(DD_IN_OUT DP_LINK_DATA* pDpLinkData)
{
    DDU64 GCD, N1, N2, DataM, DataN, LinkM, LinkN;
    DDU64 PixelClockHz, PixelClock100Hz, LinkRateMbps, LinkSymbolClock100Hz, DataBandwidthEfficiencyx10000;
    DDU32 Multiplier, Remainder, PBNPerSlotx100, LaneCount, SliceWidth, ChunkSize, NumOfBitsPerSymbolClock;
    DDU32 MaxValue = (16 * 1024 * 1024) - 1; // max size in the register 24 bits.
    DP_CHANNEL_CODING_TYPE DpChannelCodingType;
    DD_BITS_PER_PIXEL BitsPerPixel;
    DDU16 TuHighEnd;
    DDU64 HActiveLLSymCycCnt, HBlankLLSymCycCnt, HTotalLLSymCycCnt;

    // arg validation
    DDASSERT(pDpLinkData);

    LinkRateMbps = pDpLinkData->LinkBwData.LinkRateMbps;
    LaneCount = pDpLinkData->LinkBwData.DpLaneWidthSelection;
    DpChannelCodingType = GET_DP_CHANNEL_CODING(pDpLinkData->LinkBwData.LinkRateMbps);
    PixelClockHz = pDpLinkData->LinkBwData.DotClockInHz;

    if (pDpLinkData->LinkBwData.IsMsoEnabled)
    {
        // DotClock for M,N,Tu calculations in case of MSO enabled port is DotClock/NumLinks
        PixelClockHz = PixelClockHz / pDpLinkData->LinkBwData.NumOfLinks;
    }

    if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
    {
        // SSC for Display Port spec 1.1 is defined as 0.5%
        // With SSC overhead there will be reduction in average link rate by half of SSC overhead percentage which
        // amounts to 0.25%. Rather than decreasing the link rate, raising the pixel clock by same percentage.
        // Average LinkRate = LinkRateMbps * (1 - 0.0025)
        // Average LinkRate = LinkRateMbps * 0.9975
        // 1/0.9975 approx is 1.002506
        // PixelClockHz = PixelClockHz * (1.002506)
        // PixelClockHz = (PixelClockHz * 1002506)/1000000

        // On MTL+ C20 PHY, SSC overhead is 0.45% compared to typical 0.5%.
        // With SSC overhead there will be reduction in average link rate by 0.225%.
        // Rather than decreasing the link rate, raising the pixel clock by same percentage.
        // Average LinkRate = LinkRateMbps * (1 - 0.00225)
        // Average LinkRate = LinkRateMbps * 0.99775
        // 1/0.99775 approx is 1.002255
        // PixelClockHz = PixelClockHz * (1.002255)
        // PixelClockHz = (PixelClockHz * 1002255)/1000000
        PixelClockHz = ((PixelClockHz * pDpLinkData->LinkBwData.DpSscOverheadx1e6) / DD_MILLION);
    }

    PixelClock100Hz = PixelClockHz / DD_CENT;
    BitsPerPixel.Bppx16 = pDpLinkData->LinkBwData.BitsPerPixel.Bppx16;

    // Link Symbol Clock is the Clock with which Main-Link symbols are transferred within PHY and Link Layer circuits
    // of either a DPTX or DPRX for Main-Link transport.
    if (CH_CODING_128B_132B == DpChannelCodingType)
    {
        DataBandwidthEfficiencyx10000 = DP_DATA_BW_EFFICIENCY_128B_132B_PER_10000;
        NumOfBitsPerSymbolClock = 32;
        // Main-Link Symbols:
        //  a) Data link symbols and control link symbols are both 32 bits in size.
        // LinkSymbolClock in MHz => LinkRateMbps/32
        // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
        // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
        // LinkSymbolClock100Hz => (LinkRateMbps * 10^4)/32
        // LinkSymbolClock100Hz => (LinkRateMbps * 625)/2
        LinkSymbolClock100Hz = (LinkRateMbps * 625) / 2;
    }
    else // 8b/10b encoding
    {
        // For 8b / 10b MST DataBandwidthEfficiency is considered as 80.00% instead of actual 78.75%.
        // (as the hardware internally multiplies programmed Data M/N with "64 time-slots" for rate-governing
        // instead of 63 hence we are not considering actual bandwidth efficiency of 78.75%)
        // FEC Overhead: 2.4%
        DataBandwidthEfficiencyx10000 = (TRUE == pDpLinkData->LinkBwData.EnableFec) ? DP_DATA_BW_EFFICIENCY_SST_FEC_PER_10000 : DP_DATA_BW_EFFICIENCY_SST_PER_10000;
        NumOfBitsPerSymbolClock = 8;
        // Main-Link Symbols:
        //  a) Data symbols contain 8 bits of data and are encoded into 10-bit data characters by way of channel coding.
        //  b) Control link symbols are encoded into 10-bit special characters of 8b/10b (referred to as "K-codes")
        // LinkSymbolClock in MHz => LinkRateMbps/10
        // LinkSymbolClock in 100Hz => (LinkSymbolClock in MHz) * 10^6/100;
        // LinkSymbolClock100Hz => (LinkSymbolClock in MHz) * 10^4;
        // LinkSymbolClock100Hz => LinkRateMbps * 10^3
        LinkSymbolClock100Hz = (LinkRateMbps * DD_1K);
    }

    // LinkM/LinkN = PixelClock/LinkSymbolClock
    // For 8b/10b channel encoding:
    // LinkM/LinkN values programmed are transmitted as Mvid [23:0] and Nvid [23:0] fields as part of MSA during VBlank periods.
    // DP Sink regenerates pixel clock from the link symbol clock, using time stamp values Mvid and Nvid.
    // In our h/w the link only operates in Synchronous Clock mode i.e, the link symbol clock and pixel clock are synchronous
    // and the link M and N values stay constant for a given pixel rate.
    // For 128b/132b channel encoding:
    // The Mvid[23:0] and Nvid[23:0] fields used in 8b/10b Link Layer are replaced with the VFREQ[47:0] field.
    // But HW internally uses LinkM/LinkN programmed values (for both 8b/10b and 128b/132b). Hence these must be programmed for 128b/132b channel encoding also.

    GCD = DD_FindGCD(LinkSymbolClock100Hz, PixelClock100Hz);
    LinkM = (PixelClock100Hz / GCD);
    LinkN = (LinkSymbolClock100Hz / GCD);

    if (LinkM > MaxValue || LinkN > MaxValue)
    {
        DDU64 TempLinkM;
        // When values of LinkM or LinkN exceeds (2^24-1), fix LinkN to 0x8000 and calculate the LinkM value from the ratio already calculated.
        // Assume X1/X2 are 64 bit ratio values for LinkM/LinkN using GCD method. Worst case, when GCD == 1, X1 equals PixelClock100Hz value. LinkM = (X1 * 0x8000)/X2 Choosing worst case pixel clock
        // of 5940MHz, it can occupy 33 bits.(X1 * 0x8000) -> won't exceed 64 bit.
        TempLinkM = (LinkM * 0x8000) / LinkN;
        LinkM = TempLinkM;
        LinkN = 0x8000;
    }

    // This check is if we have a low M or N value it results into panel blank-out some times.
    // So just multiplying the values by a common factor to make M greater than 100 and that should hold good.
    if ((LinkM > 0) && (LinkM < 1000))
    {
        Multiplier = (DDU32)(1000 / LinkM) + 1;
        if ((LinkN * Multiplier) < MaxValue)
        {
            LinkM *= Multiplier;
            LinkN *= Multiplier;
        }
    }

    pDpLinkData->LinkBwData.MNTUData.LinkM = (DDU32)(LinkM);
    pDpLinkData->LinkBwData.MNTUData.LinkN = (DDU32)(LinkN);

    // The Mvid[23:0] and Nvid[23:0] fields used in 8b/10b Link Layer are replaced with the VFREQ[47:0] field.
    pDpLinkData->LinkBwData.MNTUData.VFreqHz = (CH_CODING_128B_132B == DpChannelCodingType) ? pDpLinkData->LinkBwData.DotClockInHz : 0;

    // With uncompressed pixel stream:
    // HACT_LL_SYM_CYC_CNT = CEIL(CEIL(HACT_WIDTH / 4) × PIX_BPP / SYMBOL_SIZE)
    HActiveLLSymCycCnt = DD_ROUND_UP_DIV((DD_ROUND_UP_DIV(pDpLinkData->HActive, 4) * BitsPerPixel.Bppx16), (FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR * NumOfBitsPerSymbolClock));

    // With uncompressed or compressed pixel stream:
    // HBLANK_LL_SYM_CYC_CNT =  CEIL(CEIL(HBLNK_WIDTH / 4) × PIX_BPP / SYMBOL_SIZE)
    HBlankLLSymCycCnt =
        DD_ROUND_UP_DIV((DD_ROUND_UP_DIV((pDpLinkData->HTotal - pDpLinkData->HActive), 4) * BitsPerPixel.Bppx16), (FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR * NumOfBitsPerSymbolClock));

    // For MST 1 EOC is 4 LS because for both 128b/132b and 8b/10b MST, DP Link Layer always operates on four Main-Link Lanes irrespective
    // of the underlying Physical Lane Count.
    // 8b/10b channel encoding 1 LS occupies 8 bits. For each slice need to send extra 8*4 bits
    // 128b/132b channel encoding 1 LS occupies 32 bits. For each slice need to send extra 32*4 bits
    if (TRUE == pDpLinkData->DscCapable && pDpLinkData->DscSlicesPerScanline != 0)
    {
        // slicewidth = ceil(HActive/NumOfSlicesPerScanline)
        SliceWidth = DD_ROUND_UP_DIV(pDpLinkData->HActive, pDpLinkData->DscSlicesPerScanline);
        // chunksize = ceil(slicewidth*bitsperpixel/8) in Bytes
        ChunkSize = DD_ROUND_UP_DIV(SliceWidth * BitsPerPixel.Bppx16, 8 * FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR);

        // HACT_LL_SYM_CYC_CNT = HSLICE_CNT × (CEIL(CHUNK_SIZE × 8 / (4 × SYMBOL_SIZE)) + 1)
        // CHUNK_SIZE is in units of byte count. The CHUNK_SIZE × 8 value is the number of bits per DSC chunk
        // "+1" accounts for a symbol cycle that carries an EOC control link symbol per DSC horizontal slice(HSlice)
        // HSLICE_CNT is the number of DSC HSlices per scanline
        HActiveLLSymCycCnt = (pDpLinkData->DscSlicesPerScanline * (DD_ROUND_UP_DIV((ChunkSize * 8), (4 * NumOfBitsPerSymbolClock)) + 1));
    }

    // HTOTAL_LL_SYM_CYC_CNT = HACT_LL_SYM_CYC_CNT + HBLNK_LL_SYM_CYC_CNT
    HTotalLLSymCycCnt = (HActiveLLSymCycCnt + HBlankLLSymCycCnt);

    // Data M / N = (PixelClockHz * BitsPerPixel)/(LinkRateBps * DataBandwidthEfficiency * LaneCount)

    // EFF_PIX_BPP = (EFF_HTOTAL_LL_SYM_CYC_CNT × SYMBOL_SIZE × 4) / HTOTAL_WIDTH
    // N1 = (PixeClockHz * EFF_PIX_BPP)
    // N2 = (LinkRateBps * DataBandwidthEfficiency * LaneCount)
    //
    // N1 = (PixelClockHz * (EFF_HTOTAL_LL_SYM_CYC_CNT × SYMBOL_SIZE × 4)) / HTOTAL_WIDTH
    // N2 = (LinkRateBps * DataBandwidthEfficiency * LaneCount)
    //
    // N1 = (PixelClockHz * (EFF_HTOTAL_LL_SYM_CYC_CNT × SYMBOL_SIZE × 4))
    // N2 = (LinkRateBps * DataBandwidthEfficiency * LaneCount * HTOTAL_WIDTH)
    // LinkRateBps = (LinkRateMbps * 10^6)
    // N2 = (LinkRateMbps * 10^6 * DataBandwidthEfficiencyx10000 * LaneCount * HTOTAL_WIDTH)/10000
    // N2 = (LinkRateMbps * 10^2 * DataBandwidthEfficiencyx10000 * LaneCount * HTOTAL_WIDTH)

    N1 = (PixelClockHz * HTotalLLSymCycCnt * NumOfBitsPerSymbolClock * 4);
    N2 = (LinkRateMbps * DD_CENT * DataBandwidthEfficiencyx10000 * LaneCount * pDpLinkData->HTotal);

    // Check whether Required Pixel Bandwidth is greater than the Available Link Bandwidth
    if (N1 > N2)
    {
        DISP_FUNC_EXIT_W_STATUS(DDS_DPL_DP_INSUFFICIENT_LINK_BW);
        return DDS_DPL_DP_INSUFFICIENT_LINK_BW;
    }

    GCD = DD_FindGCD(N1, N2);
    DataM = (N1 / GCD);
    DataN = (N2 / GCD);

    if ((DataM > 0) && (DataM < 1000))
    {
        Multiplier = (DDU32)(1000 / DataM) + 1;
        if ((DataN * Multiplier) < MaxValue)
        {
            DataM *= Multiplier;
            DataN *= Multiplier;
        }
    }

    // For determining VC Payload Size in DP Source, we needn't consider 0.6% margin for stream bandwidth when spread is not enabled.
    // (refer to Section 2.6.4.3 VC Payload Size Determination by a Source Payload Bandwidth Manager of DP1.4a spec)
    // DataM/DataN ratio should always be less than "1", Ceil(DataM * 64/DataN) will give number of allocated VC-timeslots.
    pDpLinkData->LinkBwData.MNTUData.DataTU = (DDU32)DD_ROUND_UP_DIV(DataM * 64, DataN);

    // Allocated VC time-slot for a pixel stream can be filled with either stream symbol or VCPF.
    // When there are no stream symbols to transmit while the link is enabled DP Source shall transmit VCPFs.

    // In case of MST, the link layer always operates on 4 lane count irrespective of the physical lane count value.
    // Downstream Sink devices needs to know the 4-lane boundary to properly the recover the stream.
    // Downstream Sink devices shall use only the VCPF control link symbol sequence to detect the phase of the 4 - symbol sequence unit mapping
    // to Main - Link lanes in the 2 - and 1 -lane cases.

    // For the below given configuration:
    // Link Rate: 5400 Mbps, Lane Count: 2
    // PixelClock : 146.25MHz, RR: 60Hz
    // HActive: 1680 pixels, HTotal: 2240 pixels
    // VActive: 1050 lines, VTotal: 1089 lines
    // BPP: 24
    // Ceil(DataM * 64, DataN) == Floor(DataM * 64, DataN) == 26
    // (numerator % denominator == 0)
    // which does not give opportunity for DP Source to insert VCPF during allocated VC time-slots for the pixel stream.
    // (where there is no symbol left to transmit VCPFs during the allocated VC time-slots)
    // As there are no VCPFs, downstream sink devices are unable to decode the link symbols properly.

    // It's a limitation from spec which doesn't mention what needs to be done in this case.
    // As a workaround, DP Source will increase VC time-slots by 1 when DD_ROUND_UP_DIV(DataM * 64, DataN) == DD_ROUND_DOWN_DIV(DataM * 64, DataN)
    if (DD_ROUND_UP_DIV(DataM * 64, DataN) == DD_ROUND_DOWN_DIV(DataM * 64, DataN))
    {
        pDpLinkData->LinkBwData.MNTUData.DataTU += 1;
    }

    if (CH_CODING_8B_10B == DpChannelCodingType)
    {
        // Restrictions on the Virtual Channel(VC) payload size in DisplayPort MST mode (applicable only for 8b/10b channel encoding)
        // In a x1 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 4.
        // In a x2 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 2.
        // In a x4 lane config, each pipe stream on the link must use a VC payload size that is a multiple of 1.
        Remainder = ((pDpLinkData->LinkBwData.MNTUData.DataTU * LaneCount) % 4);
        if (Remainder != 0)
        {
            pDpLinkData->LinkBwData.MNTUData.DataTU += ((4 - Remainder) / LaneCount);
        }
    }

    // a) 8b/10b DPRX Device or 128b/132b DPRX Device supporting multiple streams (MST_CAP(21h[0] == 1))
    // b) 8b/10b DPRX Device or 128b/132b DPRX Device supporting single stream sideband message (MST_CAP(21h[0] == 0) && SS_SB_MSG_SUP(21h[1] == 1))
    // PixelPBN is send as part of ALLOCATE_PAYLOAD sideband message to downstream DP Branch devices.
    if ((TRUE == pDpLinkData->MstBwData.EnableMSTMode) || (TRUE == pDpLinkData->IsSsSbmSupported))
    {
        PBNPerSlotx100 = DpProtocolGetPbnPerSlotx100(LaneCount, (DDU32)LinkRateMbps);
        if (PBNPerSlotx100 == 0)
        {
            DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
            return DDS_UNSUCCESSFUL;
        }

        // Pixel Bandwidth required in Mbps considering 0.6% spread margin = (PixelClockHz * BitsPerPixel * 1.006)/1000000
        // 0.6% margin account for down-spreading overhead of 0.5%, and DP Branch device link reference clock difference of up to 0.06%.
        // While calculating required pixel bandwidth we should be rounding up to the closest integer.
        // Required Pixel Bandwidth in MBps = Ceil((PixelClockHz * BitsPerPixel * 1.006)/(8 * 10^6))
        // Required Pixel Bandwidth in PBN  = Ceil(((PixelClock100Hz * BitsPerPixel * 1.006) / (8 * 10^6)) / (54 / 64))

        // When FEC is enabled, PixelPBN should consider FEC overhead. And when DSC is enabled, PixelPBN needs to consider EOC overhead.
        // As DataM/N considers both FEC/EOC overhead..we can easily determine PixelPBN as below:
        // Required Pixel Bandwidth in PBN = Ceil((DataM/N Ratio) * 64 * PBNPerSlot * 1.006)
        // It is independent of the number of VC Slots.
        // Actual PBN indicates how much Pixel PBN will actually be needed and is the value used in ALLOCATE_PAYLOAD sideband message
        if (TRUE == pDpLinkData->LinkBwData.EnableSpread)
        {
            // When down-spread is enabled...DataM/N factored in the 0.25% overhead in pixel clock. So need to add extra 0.35% overhead while calculating Pixel PBN.
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlot * (1 + (1.006 - 1.025)))
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * (1 + (1.006 - 1.025)))/100
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(100*1000000)
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(100*1000000/64)
            // 1000000/64 => 15625
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * PBNPerSlotx100 * (1000000 + (1006000-DpSscOverheadx1e6)))/(1562500)
            DDU64 Overhead = (DD_MILLION + (1006000 - pDpLinkData->LinkBwData.DpSscOverheadx1e6));
            pDpLinkData->MstBwData.ActualPBN = (DDU32)DD_ROUND_UP_DIV(((DDU64)DataM * (PBNPerSlotx100 * Overhead)), ((DDU64)DataN * 1562500));
        }
        else
        {
            // When down-spread is not enabled...DataM/N didn't factor in the spread overhead. So need to consider it while calculating Pixel PBN.
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlot * 1.006)
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * 1.006)/100
            // Required Pixel Bandwidth in PBN = Ceil((DataM / N Ratio) * 64 * PBNPerSlotx100 * 1006)/(100*1000)
            pDpLinkData->MstBwData.ActualPBN = (DDU32)DD_ROUND_UP_DIV(((DDU64)DataM * (64 * PBNPerSlotx100 * 1006)), ((DDU64)DataN * (DD_1K * DD_CENT)));
        }

        // AllocatedPBN indicates the PBN allocation within the DP Source, which is determined based on the number of slots, link rate, number of lanes
        // and HW restrictions
        pDpLinkData->MstBwData.AllocatedPBN = DD_ROUND_UP_DIV(pDpLinkData->LinkBwData.MNTUData.DataTU * PBNPerSlotx100, DD_CENT);

        // the num slot is updated with TU number
        pDpLinkData->MstBwData.VcPayloadTableNumSlots = (DDU8)pDpLinkData->LinkBwData.MNTUData.DataTU;

        // As ActualPBN contains the rounded-up Pixel PBN value, need to check whether the final rounded value
        // is within the Time-slot limits.
        TuHighEnd = (CH_CODING_128B_132B == DpChannelCodingType) ? TU_HIGH_END : (TU_HIGH_END - 1);
        if ((pDpLinkData->MstBwData.ActualPBN * DD_CENT) > (TuHighEnd * PBNPerSlotx100))
        {
            DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
            return DDS_UNSUCCESSFUL;
        }
    }
    else
    {
        // For DP-SST, 8b/10b Channel Encoding, Size of TU is 64
        // For 128b/132b Channel Encoding, TU must be programmed to actual number of time-slots occupied by the pixel data.
        if (CH_CODING_8B_10B == DpChannelCodingType)
        {
            pDpLinkData->LinkBwData.MNTUData.DataTU = TU_HIGH_END;
        }

        if (pDpLinkData->LinkBwData.IsMsoEnabled)
        {
            // We have to multiply DataM value with number of SST links. (such that Data M/N doesn't change b/w MSO and non-MSO cases)
            DataM *= pDpLinkData->LinkBwData.NumOfLinks;
        }
    }

    pDpLinkData->LinkBwData.MNTUData.DataM = DataM;
    pDpLinkData->LinkBwData.MNTUData.DataN = DataN;

    return DDS_SUCCESS;
}

/***************************************************************
 * @brief Calculate Pixel bandwidth data for given pixel clock in Mbits per sec.
 *
 * @param DotClockInHz
 * @param BitsPerPixel bpp considering DSC.
 * @return DDU32
 ***************************************************************/
DDU64 DpProtocolComputePixelDataBwMbps(DDU64 DotClockInHz, DD_BITS_PER_PIXEL BitsPerPixel)
{
    DDU64 PixelClock100Hz;

    PixelClock100Hz = DotClockInHz / 100;
    // Calculating Pixel Bandwidth in Mbps
    // PixelClockHz * BitsPerPixel => Pixel Bandwidth in Bits Per Sec
    // i.e. ((PixelClockHz/100) * BitsPerPixel) / (10*1000) Mega bits per sec
    // Rounding up to higher integer when dividing to get MbPS.
    return (DD_ROUND_UP_DIV(((PixelClock100Hz * (DDU64)BitsPerPixel.Bppx16) / FIXED_POINT_U6_4_TO_REALVALUE_CONV_DIVISOR), (10 * DD_1K)));
}

/***************************************************************
 * @brief Populate the common Edid and Monitor data for Dp/Hdmi.
 *
 * @param SinkType
 * @param ppEDID
 * @param pTimingInfo
 * @param DtdNum
 * @return void
 ***************************************************************/
void DisplayInfoRoutinePopulateEdidAndMonitorData(DD_VIDEO_OUTPUT_TECHNOLOGY SinkType, EDID_BASE_BLOCK** ppEDID, DD_TIMING_INFO* pTimingInfo, DDU8 DtdNum)
{
    MONITOR_RANGE_LIMITS MonitorRangeLimits;
    DDU8 Temp;
    DDU8 Index;

    DDASSERT(ppEDID);
    DDASSERT(pTimingInfo);

    DD_ZERO_MEM(&MonitorRangeLimits, sizeof(MonitorRangeLimits));
    // Set header
    DD_MEM_COPY_SAFE((*ppEDID)->Header, sizeof((*ppEDID)->Header), BaseEdidHeader, sizeof(BaseEdidHeader));

    // Set vendor/product ID
    DD_MEM_COPY_SAFE((*ppEDID)->PnpID.VendorProductID, sizeof((*ppEDID)->PnpID.VendorProductID), FakeEdidPNPId, sizeof(FakeEdidPNPId));
    (*ppEDID)->Version = 1;

    switch (SinkType)
    {
    case DD_VOT_DISPLAYPORT_EXTERNAL:
        (*ppEDID)->Revision = 4;
        (*ppEDID)->BasicDisplayParams.VideoInput = 0xA5;
        // since Edid_14_MonitorInfo is shifted by 1 index, need to decrement 1 from DtdNum
        (*ppEDID)->EdidTiming.Edid_14_MonitorInfo[DtdNum - 1].MonitorDescHeader.Flag0 = 0;
        (*ppEDID)->EdidTiming.Edid_14_MonitorInfo[DtdNum - 1].MonitorDescHeader.DataTypeTag = 0xFD; // Monitor range limit, BASEEDID_MONITORRANGELIMIT_MDDATATYPE
        (*ppEDID)->EdidTiming.Edid_14_MonitorInfo[DtdNum - 1].MonitorRangeLimits = MonitorRangeLimits;

        break;
    case DD_VOT_HDMI:
        // Set EDID 1.3 version
        (*ppEDID)->Revision = 3;
        // set Video Input to 0x80, Bit 7 indicates its dvi (digital input)
        (*ppEDID)->BasicDisplayParams.VideoInput = 0x80;
        (*ppEDID)->EdidTiming.Edid_13_MonitorInfo[DtdNum].MonitorDescHeader.Flag0 = 0;
        (*ppEDID)->EdidTiming.Edid_13_MonitorInfo[DtdNum].MonitorDescHeader.DataTypeTag = 0xFD; // Monitor range limit, BASEEDID_MONITORRANGELIMIT_MDDATATYPE
        (*ppEDID)->EdidTiming.Edid_13_MonitorInfo[DtdNum].MonitorRangeLimits = MonitorRangeLimits;
        break;
    default:
        DDRLSASSERT_UNEXPECTED(0, "Invalid SinkType Passed");

        break;
    }

    // Set max H/V image size in cm. (Set 0x00 when unknown per EDID spec)
    (*ppEDID)->BasicDisplayParams.MaxHImageSize = 0x00;
    (*ppEDID)->BasicDisplayParams.MaxVImageSize = 0x00;

    // Gamma
    (*ppEDID)->BasicDisplayParams.Gamma = 0xFF; // source: Legacy

    // Feature support (Legacy sets "11101010" -> 0xEA)
    // Note: New code sets GTF support and color space as 1 where as old one doesn't. HCT monitor tests might check for 0's for both of these
    // which is not actually a real bug. There is an known erratum which some customers (like Lenova) use for getting waiver on this.
    (*ppEDID)->BasicDisplayParams.IsGTFSupported = 1;
    (*ppEDID)->BasicDisplayParams.IsPreferredTimingMode = 1; // DTD0 must have valid timings
    (*ppEDID)->BasicDisplayParams.ColorSpace = 1;
    (*ppEDID)->BasicDisplayParams.DispType = 0x01;
    // Assuming active off stands for monitor D3. Done as part of sighting 90950
    //(Note: There is confusion w.r.t EDID spec on whether active off is indeed D3.
    // DVI spec and newer DPM recommends setting this to indicate OFF support)
    (*ppEDID)->BasicDisplayParams.ActiveOff = 1;
    (*ppEDID)->BasicDisplayParams.Suspend = 0; // Usually panel's don't support suspend & standby on it's own. It's either ON/OFF.
    (*ppEDID)->BasicDisplayParams.StandBy = 0;

    // Set Color Bit Depth to 8BPC. As per hdmi Spec, Fail safe mode is 640x480@60Hz @ 24bpp
    (*ppEDID)->BasicDisplayParams.ColorBitDepth = 2; // 8 bpc

    // Chromaticity
    // Don't leave color characteristics offsets [0x19-0x22] as 0. We use a sample
    // color characteristic values from VESA EDID example. These values are not actually
    // used anywhere. Even if they are, using 0 values are just as bad as incorrect values.
    DD_MEM_COPY_SAFE(&(*ppEDID)->ChromaticityData, sizeof((*ppEDID)->ChromaticityData), FakeEdidChroma, sizeof(FakeEdidChroma));

    // Established timings: put 640x480@60 as the only timing.
    (*ppEDID)->EstablishedTimings.EstTiming1 = 0;
    (*ppEDID)->EstablishedTimings.EstTiming2 = 0;
    (*ppEDID)->EstablishedTimings.MfgTimings = 0;
    (*ppEDID)->EstablishedTimings.Supports640x480_60 = 1;
    // Standard timings: None - see reason above
    // Note: Don't leave standard timing id byte fields as 0. If unused, they should be 0x01.
    memset((*ppEDID)->StdTiming, 0x01, sizeof((*ppEDID)->StdTiming));

    for (Index = 0; Index < DtdNum && Index < MAX_EDID_DTD_BLOCKS; Index++)
    {
        // Fill up min/max vertical & horizontal RR's
        if (pTimingInfo[Index].VRoundedRR > MonitorRangeLimits.MinVFrequency)
            MonitorRangeLimits.MinVFrequency = (DDU8)pTimingInfo[Index].VRoundedRR;
        if (pTimingInfo[Index].HRefresh > MonitorRangeLimits.MinHFrequency)
            MonitorRangeLimits.MinHFrequency = (DDU8)pTimingInfo[Index].HRefresh;

        // Fill up max pixel clock value
        Temp = (DDU8)(pTimingInfo[Index].DotClockInHz / (TEN * MEGA_HERTZ)); // in 10MHz
        if ((DDU32)(Temp * (TEN * MEGA_HERTZ)) != pTimingInfo[DtdNum].DotClockInHz)
        {
            // Required as per EDID 1.3 spec
            Temp++;
        }

        if (MonitorRangeLimits.MaxPixelClock < Temp)
            MonitorRangeLimits.MaxPixelClock = Temp;
    }
    // Fill up monitor range limit at the Next descriptor index of the DTDs added
    // Single rate to be supported, Hence copying the same in Max V & H Freq
    MonitorRangeLimits.MaxVFrequency = MonitorRangeLimits.MinVFrequency;
    MonitorRangeLimits.MaxHFrequency = MonitorRangeLimits.MinHFrequency;
    // No extensions
    (*ppEDID)->NumExtBlocks = 0;

    return;
}

/**
 * @brief Method to create CVT timing
 * Reference documents used are   CVT Standard (version 2.0) pdf file & Spread Sheet provided by VESA for CVT timing calculation.
 * This function can be used for generating both RB2 & RB3 versions of timing.
 * @param pCreateCvtTimingArgs
 * @return DDSTATUS
 */
DDSTATUS DisplayInfoRoutinesCreateCVT2Timing(CREATE_CVT_TIMING_ARGS* pCreateCvtTimingArgs)
{
    float FlActHFreq;

    DDASSERT(pCreateCvtTimingArgs);

    pCreateCvtTimingArgs->IsMargin_Req = FALSE; // Not currently supported in CVT descriptor, so forcefully making it zero.

    if ((pCreateCvtTimingArgs->RRate == 0) || (pCreateCvtTimingArgs->XRes == 0) || (pCreateCvtTimingArgs->YRes == 0))
    {
        DISP_DBG_MSG(GFXDBG_NORMAL, "Invalid inputs!");
        DISP_FUNC_EXIT_W_STATUS(DDS_UNSUCCESSFUL);
        return DDS_UNSUCCESSFUL;
    }

    // fixed definations as per VESA spec

    const DDU32 CellGran = (pCreateCvtTimingArgs->RedBlankVersion == RED_BLANK_VER_3) ? 8 : 1;         // cell granularity
    const float AddVFieldRateAdj = (pCreateCvtTimingArgs->RedBlankVersion == RED_BLANK_VER_3) ? 350.0f : 0.0f; // Additional ppm offset adjustment to be added to the requested refresh rate
    // Constants which are same for Both Type of blankings
    const float ClockStep = 0.001f; // Pixel clock freq should be integer multiple of this step.
    const DDU32 H_FPorch = 8;      // Pixels in H_Front Porch
    const DDU32 Min_H_Blank = 80;     // Minimum HBlank duration (in pixels) for RB timings.
    const DDU32 HSyncPixels = 32;     // In RB, H sync width is fixed to 32 pixel clocks.
    const DDU32 RB_Min_VBlank = 460;    // In RB, minimum V Blanking period is 460 uSec.
    const DDU32 VSync = 8;      // VSync duration (in lines)
    const DDU32 Min_V_BPorch = 6;      // minimum default back porch(in lines)
    const DDU32 Min_V_FPorch = 1;      // minimum default front porch(in lines)

    // variable definitions
    DDU32 HBlankPixels = 0; // In RB, Horizontal Blanking is fixed to 160 pixel clocks.
    float HPeriodEst = 0, Total_V_Lines = 0;
    DDU32 V_Blank = 0, TotalPixels = 0;
    float Act_Pixel_Freq = 0;
    float Act_H_Freq = 0, Act_VFrameRate = 0;
    float Refresh_Multiplier = 0;

    DDU32 RB_VB_Lines = 0, RB_Min_VB_Lines = 0, V_Back_Porch = 0, RB_VFrontPorch = 0, Act_VB_Time = 0;

    // Spec defination ends here

    //=========================================================================================
    // CVT-Reduced Blanking Timing Calculation
    //=========================================================================================

    // Step 1: Find the field rate required
    float VFieldRateRqd = (float)(pCreateCvtTimingArgs->RRate * (1 + AddVFieldRateAdj / 1000000));

    // Step 2: Round the Horizontal Resolution to nearest 8 pixel--character cell multiple
    DDU32 HPixels = pCreateCvtTimingArgs->XRes;
    DDU32 ActivePixels = (DDU32)(DD_ROUNDTONEARESTINT((double)HPixels / CellGran) * CellGran);

    // Step 3: Calculate Vertical line rounded to nearest integer
    DDU32 V_Lines_Rnd = (pCreateCvtTimingArgs->YRes);

    // Step 4:Estimate Horizontal Period
    HPeriodEst = (float)(((1000000 / VFieldRateRqd) - RB_Min_VBlank) / (V_Lines_Rnd));

    // Step 5:Find No. of Lines in Vertical Blanking
    Act_VB_Time = (RB_Min_VBlank > pCreateCvtTimingArgs->ReqVblankTime) ? RB_Min_VBlank : pCreateCvtTimingArgs->ReqVblankTime;

    // Step 6: Calc the no of lines in Vblank interval:
    RB_VB_Lines = ((DDU32)(Act_VB_Time / HPeriodEst)) + 1;

    // Step 7:Check Whether Vertical Blanking is sufficient.
    RB_Min_VB_Lines = VSync + Min_V_FPorch + Min_V_BPorch;
    V_Blank = (RB_VB_Lines < RB_Min_VB_Lines) ? RB_Min_VB_Lines : RB_VB_Lines;

    // Step 8 for Both : Find the total number of lines in vertical field
    Total_V_Lines = (float)(V_Lines_Rnd + V_Blank);

    // Step 9: Calc V Back Porch

    if ((pCreateCvtTimingArgs->RedBlankVersion == RED_BLANK_VER_3) && (pCreateCvtTimingArgs->IsEarlyVsync))
    {
        V_Back_Porch = RB_VB_Lines / 2;
    }
    else
    {
        V_Back_Porch = Min_V_BPorch;
    }

    // Step 10: Calc V Front Porch
    RB_VFrontPorch = V_Blank - V_Back_Porch - VSync;

    // Step  11 : Find total number of pixels in one line
    HBlankPixels = (pCreateCvtTimingArgs->RedBlankVersion == RED_BLANK_VER_3) ? pCreateCvtTimingArgs->ReqHblankPixels : Min_H_Blank;
    if (HBlankPixels < Min_H_Blank)
    {
        HBlankPixels = Min_H_Blank;
    }
    TotalPixels = ActivePixels + HBlankPixels;

    // Step 12: Calc H Back Porch,  commented as we dont need it for filling any values
    // RB_HBackPorch = HBlankPixels - H_FPorch - HSyncPixels;

    // Step 13:Find pixel clock freq.
    // REFRESH_MULTIPLIER = IF(AND(I_RED_BLANK_VER=2, I_VIDEO_OPT?="Y"),1000/1001, 1)
    if ((pCreateCvtTimingArgs->RedBlankVersion == RED_BLANK_VER_2) && (pCreateCvtTimingArgs->IsVideoRrReq))
    {
        Refresh_Multiplier = 1000.0f / 1001.0f;
    }
    else
    {
        Refresh_Multiplier = 1.0;
    }
    // ACT_PIXEL_FREQ = C_CLOCK_STEP * IF(I_RED_BLANK_VER = 2,
    //    ROUNDDOWN((V_FIELD_RATE_RQD * TOTAL_V_LINES * TOTAL_PIXELS / 1000000 * REFRESH_MULTIPLIER) / C_CLOCK_STEP, 0),
    //    ROUNDUP((V_FIELD_RATE_RQD * TOTAL_V_LINES * TOTAL_PIXELS / 1000000 * REFRESH_MULTIPLIER) / C_CLOCK_STEP, 0))

    Act_Pixel_Freq = (VFieldRateRqd * Total_V_Lines * TotalPixels / (1000000 * Refresh_Multiplier)) / ClockStep;
    if (pCreateCvtTimingArgs->RedBlankVersion == RED_BLANK_VER_2)
    {
        Act_Pixel_Freq = (float)(DDU32)(Act_Pixel_Freq);
    }
    else
    {
        Act_Pixel_Freq = (float)DD_ROUND_UP_DIV((DDU32)(Act_Pixel_Freq * 10), 10);
    }

    // Flags info
    pCreateCvtTimingArgs->pTimingInfo->HSyncPolarity = FALSE; // Positive
    pCreateCvtTimingArgs->pTimingInfo->VSyncPolarity = TRUE;  // Negative

    // Step 14 : Find actual horizontal Freq.
    // H_Freq calculated here is in KHz.
    Act_H_Freq = (Act_Pixel_Freq / TotalPixels);

    // Step 15 : Find the actual vertical field frequency
    Act_VFrameRate = (1000 * Act_H_Freq / Total_V_Lines);

    // convert the KHz to Hz
    pCreateCvtTimingArgs->pTimingInfo->DotClockInHz = ((DDU64)(Act_Pixel_Freq) * (1000)); // from step 13
    pCreateCvtTimingArgs->pTimingInfo->HTotal = TotalPixels;                        // from step 11
    pCreateCvtTimingArgs->pTimingInfo->HActive = ActivePixels;                       // from step 2

    pCreateCvtTimingArgs->pTimingInfo->HBlankStart = ActivePixels;
    pCreateCvtTimingArgs->pTimingInfo->HBlankEnd = TotalPixels - 1;

    pCreateCvtTimingArgs->pTimingInfo->HSyncStart = ActivePixels + H_FPorch;
    pCreateCvtTimingArgs->pTimingInfo->HSyncEnd = ActivePixels + H_FPorch + HSyncPixels;

    // from step 14 ... Here we need to multiply by 1000 to get H-RR in Hz.
    FlActHFreq = DD_ROUNDTO3DECIMALS(Act_H_Freq * 1000);
    pCreateCvtTimingArgs->pTimingInfo->HRefresh = DD_ROUND_UP_DIV((DDU32)(FlActHFreq * 10), 10);

    // from step 11 in Both
    pCreateCvtTimingArgs->pTimingInfo->VTotal = (DDU32)(DD_ROUNDTO3DECIMALS(Total_V_Lines));
    pCreateCvtTimingArgs->pTimingInfo->VActive = V_Lines_Rnd; // from step 3

    pCreateCvtTimingArgs->pTimingInfo->VBlankStart = V_Lines_Rnd;
    pCreateCvtTimingArgs->pTimingInfo->VBlankEnd = pCreateCvtTimingArgs->pTimingInfo->VTotal - 1;

    pCreateCvtTimingArgs->pTimingInfo->VSyncStart = V_Lines_Rnd + RB_VFrontPorch;
    pCreateCvtTimingArgs->pTimingInfo->VSyncEnd = V_Lines_Rnd + RB_VFrontPorch + VSync;

    pCreateCvtTimingArgs->pTimingInfo->VRoundedRR = (DDU32)DD_ROUNDTONEARESTINT(Act_VFrameRate); // from step 15

    pCreateCvtTimingArgs->pTimingInfo->IsInterlaced = FALSE;
    return DDS_SUCCESS;
}
