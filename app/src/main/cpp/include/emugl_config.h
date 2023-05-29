// Copyright 2015 The Android Open Source Project
//
// This software is licensed under the terms of the GNU General Public
// License version 2, as published by the Free Software Foundation, and
// may be copied, distributed, and modified under those terms.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#pragma once
#include "android/utils/compiler.h"

#include <stdbool.h>

ANDROID_BEGIN_HEADER

// Enum tracking all current available renderer backends
// for the emulator.
typedef enum SelectedRenderer {
    SELECTED_RENDERER_UNKNOWN = 0,
    SELECTED_RENDERER_HOST = 1,
    SELECTED_RENDERER_OFF = 2,
    SELECTED_RENDERER_GUEST = 3,
    SELECTED_RENDERER_MESA = 4,
    SELECTED_RENDERER_SWIFTSHADER = 5,
    SELECTED_RENDERER_ANGLE = 6, // ANGLE D3D11 with D3D9 fallback
    SELECTED_RENDERER_ANGLE9 = 7, // ANGLE forced to D3D9
    SELECTED_RENDERER_SWIFTSHADER_INDIRECT = 8,
    SELECTED_RENDERER_ANGLE_INDIRECT = 9,
    SELECTED_RENDERER_ANGLE9_INDIRECT = 10,
    SELECTED_RENDERER_ERROR = 255,
} SelectedRenderer;


ANDROID_END_HEADER
