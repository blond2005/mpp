/*
 * Copyright 2010 Rockchip Electronics S.LSI Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MPP_IMPL_H__
#define __MPP_IMPL_H__

#include "rk_type.h"
#include "rk_mpi.h"
#include "vpu_api.h"

#define MPI_DBG_FUNCTION            (0x00000001)



#define mpi_dbg(flag, fmt, ...)     mpp_dbg(mpi_debug, flag, fmt, ## __VA_ARGS__)

#define MPI_FUNCTION_ENTER()        mpi_dbg(MPI_DBG_FUNCTION, "%s enter\n", __FUNCTION__)
#define MPI_FUNCTION_LEAVE()        mpi_dbg(MPI_DBG_FUNCTION, "%s leave\n", __FUNCTION__)
#define MPI_FUNCTION_LEAVE_OK()     mpi_dbg(MPI_DBG_FUNCTION, "%s success\n", __FUNCTION__)
#define MPI_FUNCTION_LEAVE_FAIL()   mpi_dbg(MPI_DBG_FUNCTION, "%s failed\n", __FUNCTION__)

typedef struct MpiImpl_t MpiImpl;

struct MpiImpl_t {
    MpiImpl     *check;
    MppApi      *api;
    void        *ctx;
};

#ifdef __cplusplus
extern "C" {
#endif

extern RK_U32 mpi_debug;
void get_mpi_debug();

#ifdef __cplusplus
}
#endif

#endif /*__MPP_IMPL_H__*/
