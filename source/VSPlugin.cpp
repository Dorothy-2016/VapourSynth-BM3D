/*
* BM3D denoising filter - VapourSynth plugin
* Copyright (C) 2015  mawen1250
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "BM3D.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VapourSynth: BM3D_Basic


static void VS_CC BM3D_Basic_Init(VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi)
{
    BM3D_Basic_Data *d = reinterpret_cast<BM3D_Basic_Data *>(*instanceData);

    vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef *VS_CC BM3D_Basic_GetFrame(int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi)
{
    const BM3D_Basic_Data *d = reinterpret_cast<BM3D_Basic_Data *>(*instanceData);

    if (activationReason == arInitial)
    {
        vsapi->requestFrameFilter(n, d->node, frameCtx);
        if (d->rdef) vsapi->requestFrameFilter(n, d->rnode, frameCtx);
    }
    else if (activationReason == arAllFramesReady)
    {
        BM3D_Basic_Process p(*d, n, frameCtx, core, vsapi);

        return p.process();
    }

    return nullptr;
}

static void VS_CC BM3D_Basic_Free(void *instanceData, VSCore *core, const VSAPI *vsapi)
{
    BM3D_Basic_Data *d = reinterpret_cast<BM3D_Basic_Data *>(instanceData);

    delete d;
}


static void VS_CC BM3D_Basic_Create(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi)
{
    BM3D_Basic_Data *d = new BM3D_Basic_Data(vsapi);

    if (d->arguments_process(in, out))
    {
        delete d;
        return;
    }

    // Create filter
    vsapi->createFilter(in, out, "Basic", BM3D_Basic_Init, BM3D_Basic_GetFrame, BM3D_Basic_Free, fmParallel, 0, d, core);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VapourSynth: BM3D_Final


static void VS_CC BM3D_Final_Init(VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi)
{
    BM3D_Final_Data *d = reinterpret_cast<BM3D_Final_Data *>(*instanceData);

    vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef *VS_CC BM3D_Final_GetFrame(int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi)
{
    const BM3D_Final_Data *d = reinterpret_cast<BM3D_Final_Data *>(*instanceData);

    if (activationReason == arInitial)
    {
        vsapi->requestFrameFilter(n, d->node, frameCtx);
        if (d->rdef) vsapi->requestFrameFilter(n, d->rnode, frameCtx);
    }
    else if (activationReason == arAllFramesReady)
    {
        BM3D_Final_Process p(*d, n, frameCtx, core, vsapi);

        return p.process();
    }

    return nullptr;
}

static void VS_CC BM3D_Final_Free(void *instanceData, VSCore *core, const VSAPI *vsapi)
{
    BM3D_Final_Data *d = reinterpret_cast<BM3D_Final_Data *>(instanceData);

    delete d;
}


static void VS_CC BM3D_Final_Create(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi)
{
    BM3D_Final_Data *d = new BM3D_Final_Data(vsapi);

    if (d->arguments_process(in, out))
    {
        delete d;
        return;
    }

    // Create filter
    vsapi->createFilter(in, out, "Final", BM3D_Final_Init, BM3D_Final_GetFrame, BM3D_Final_Free, fmParallel, 0, d, core);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VapourSynth: plugin initialization


VS_EXTERNAL_API(void) VapourSynthPluginInit(VSConfigPlugin configFunc, VSRegisterFunction registerFunc, VSPlugin *plugin)
{
    configFunc("com.vapoursynth.bm3d", "bm3d",
        "Implementation of BM3D denoising filter for VapourSynth.",
        VAPOURSYNTH_API_VERSION, 1, plugin);

    registerFunc("Basic",
        "input:clip;"
        "ref:clip:opt;"
        "profile:data:opt;"
        "sigma:float[]:opt;"
        "block_size:int:opt;"
        "block_step:int:opt;"
        "group_size:int:opt;"
        "bm_range:int:opt;"
        "bm_step:int:opt;"
        "th_mse:float:opt;"
        "lambda:float:opt;"
        "matrix:int:opt;",
        BM3D_Basic_Create, nullptr, plugin);

    registerFunc("Final",
        "input:clip;"
        "ref:clip;"
        "profile:data:opt;"
        "sigma:float[]:opt;"
        "block_size:int:opt;"
        "block_step:int:opt;"
        "group_size:int:opt;"
        "bm_range:int:opt;"
        "bm_step:int:opt;"
        "th_mse:float:opt;"
        "matrix:int:opt;",
        BM3D_Final_Create, nullptr, plugin);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////