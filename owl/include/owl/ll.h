// ======================================================================== //
// Copyright 2019 Ingo Wald                                                 //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

/*! \file include/owl/ll.h Defines the dynamically linkable "C-API"
 *  for the low-level owl::ll abstraction layer */

#pragma once

#ifndef _USE_MATH_DEFINES
#  define _USE_MATH_DEFINES
#endif
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#if defined(_MSC_VER)
#  define OWL_LL_DLL_EXPORT __declspec(dllexport)
#  define OWL_LL_DLL_IMPORT __declspec(dllimport)
#elif defined(__clang__) || defined(__GNUC__)
#  define OWL_LL_DLL_EXPORT __attribute__((visibility("default")))
#  define OWL_LL_DLL_IMPORT __attribute__((visibility("default")))
#else
#  define OWL_LL_DLL_EXPORT
#  define OWL_LL_DLL_IMPORT
#endif

#if defined(OWL_LL_DLL_INTERFACE)
#  ifdef llowl_EXPORTS
#    define OWL_LL_INTERFACE OWL_LL_DLL_EXPORT
#  else
#    define OWL_LL_INTERFACE OWL_LL_DLL_IMPORT
#  endif
#else
#  define OWL_LL_INTERFACE __attribute__ ((visibility ("default")))
#endif

#include <iostream>
#include <math.h> // using cmath causes issues under Windows
#include <unistd.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

  typedef struct _LLOContext *LLOContext;

  typedef enum
    {
     /*! no error - api did what it was asked to do */
     LLO_SUCCESS = 0,
     /*! some un-specified error happened. use lloGetLastErrorString
      *  to get a textual description */
     LLO_UNKNOWN_ERROR
    }
    LLOResult;

  /*! creates a new ll-owl device(group) context using the given CUDA
   *  device IDs. An empty list of device IDs is synonymous with "use
   *  all available device". If no context could be crated, the return
   *  value is null, and lloGetLastErrorText should contain an error
   *  message. */
  OWL_LL_INTERFACE
  LLOContext lloCreate(const int32_t *deviceIDs = nullptr,
                       int32_t        numDeviceIDs     = 0);
  
  
  OWL_LL_INTERFACE
  LLOResult lloAllocModules(LLOContext llo,
                            int        numModules);
  
  OWL_LL_INTERFACE
  LLOResult lloModuleCreate(LLOContext  llo,
                            int32_t     moduleID,
                            const char *ptxCode);

  /*! (re-)builds the modules that have been set via
   *  lloModuleCreate */
  OWL_LL_INTERFACE
  LLOResult lloBuildModules(LLOContext llo);
  

  OWL_LL_INTERFACE
  LLOResult lloAllocRayGens(LLOContext llo,
                            int32_t    rayGenProgCount);
  
  OWL_LL_INTERFACE
  LLOResult lloRayGenCreate(LLOContext  llo,
                            /*! ID of ray gen prog to create */
                            int32_t     programID,
                            /*! ID of module in which to look for that program */
                            int32_t     moduleID,
                            /*! name of the program */
                            const char *programName,
                            /*! size of that program's SBT data */
                            size_t      dataSizeOfRayGen);
  
  OWL_LL_INTERFACE
  LLOResult lloBuildPrograms(LLOContext llo);
  
  OWL_LL_INTERFACE
  LLOResult lloCreatePipeline(LLOContext llo);
  
#ifdef __cplusplus
} // extern "C"
#endif

