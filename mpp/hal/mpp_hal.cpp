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

#define  MODULE_TAG "mpp_hal"

#include "mpp_mem.h"
#include "mpp_log.h"
#include "mpp_common.h"

#include "mpp.h"
#include "mpp_hal.h"
#include "mpp_frame_impl.h"

#include "hal_h264d_api.h"

/*
 * all hardware api static register here
 */
static const MppHalApi *hw_apis[] = {
    &api_h264d_hal,
};

typedef struct MppHalImpl_t {
    MppCtxType      type;
    MppCodingType   coding;

    void            *ctx;
    const MppHalApi *api;

    HalTaskGroup    tasks;
    RK_U32          task_count;
} MppHalImpl;


MPP_RET mpp_hal_init(MppHal *ctx, MppHalCfg *cfg)
{
    if (NULL == ctx || NULL == cfg) {
        mpp_err_f("found NULL input ctx %p cfg %p\n", ctx, cfg);
        return MPP_ERR_NULL_PTR;
    }
    *ctx = NULL;

    MppHalImpl *p = mpp_calloc(MppHalImpl, 1);
    if (NULL == p) {
        mpp_err_f("malloc failed\n");
        return MPP_ERR_MALLOC;
    }

    RK_U32 i;
    for (i = 0; i < MPP_ARRAY_ELEMS(hw_apis); i++) {
        if (cfg->coding == hw_apis[i]->coding) {
            mpp_assert(cfg->task_count > 0);
            p->type         = cfg->type;
            p->coding       = cfg->coding;
            p->api          = hw_apis[i];
            p->task_count   = cfg->task_count;
            p->ctx          = mpp_malloc_size(void, p->api->ctx_size);
            p->api->init(p->ctx, cfg);

            MPP_RET ret = hal_task_group_init(&p->tasks, p->type, p->task_count);
            if (ret) {
                mpp_err_f("hal_task_group_init failed ret %d\n", ret);
                break;
            }

            cfg->tasks = p->tasks;
            *ctx = p;
            return MPP_OK;
        }
    }

    mpp_err_f("could not found coding type %d\n", cfg->coding);
    if (p->ctx)
        mpp_free(p->ctx);
    mpp_free(p);

    return MPP_NOK;
}

MPP_RET mpp_hal_deinit(MppHal ctx)
{
    if (NULL == ctx) {
        mpp_err_f("found NULL input\n");
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    if (p->ctx)
        mpp_free(p->ctx);
    if (p->tasks)
        hal_task_group_deinit(p->tasks);
    mpp_free(p);
    return MPP_OK;
}

MPP_RET mpp_hal_reg_gen(MppHal ctx, HalTask *task)
{
    if (NULL == ctx || NULL == task) {
        mpp_err_f("found NULL input ctx %p task %p\n", ctx, task);
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    return p->api->reg_gen(p->ctx, task);
}

MPP_RET mpp_hal_hw_start(MppHal ctx, HalTask *task)
{
    if (NULL == ctx || NULL == task) {
        mpp_err_f("found NULL input ctx %p task %p\n", ctx, task);
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    return p->api->start(p->ctx, task);
}

MPP_RET mpp_hal_hw_wait(MppHal ctx, HalTask *task)
{
    if (NULL == ctx || NULL == task) {
        mpp_err_f("found NULL input ctx %p task %p\n", ctx, task);
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    return p->api->wait(p->ctx, task);
}

MPP_RET mpp_hal_reset(MppHal ctx)
{
    if (NULL == ctx) {
        mpp_err_f("found NULL input\n");
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    return p->api->reset(p->ctx);
}

MPP_RET mpp_hal_flush(MppHal ctx)
{
    if (NULL == ctx) {
        mpp_err_f("found NULL input\n");
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    return p->api->flush(p->ctx);
}

MPP_RET mpp_hal_control(MppHal ctx, RK_S32 cmd, void *param)
{
    if (NULL == ctx) {
        mpp_err_f("found NULL input\n");
        return MPP_ERR_NULL_PTR;
    }

    MppHalImpl *p = (MppHalImpl*)ctx;
    return p->api->control(p->ctx, cmd, param);
}

