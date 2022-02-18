// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_CORE_EXPORT_H_
#define CRYPTO_CORE_EXPORT_H_
#include "fldserver/fldserver_config.h"
// Defines CORE_EXPORT so that functionality implemented by the crypto module
// can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(CRYPTO_IMPLEMENTATION)
#define CORE_EXPORT __declspec(dllexport)
#else
#define CORE_EXPORT __declspec(dllimport)
#endif  // defined(CRYPTO_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(CRYPTO_IMPLEMENTATION)
#define CORE_EXPORT __attribute__((visibility("default")))
#else
#define CORE_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define CORE_EXPORT
#endif

#endif  // CRYPTO_CORE_EXPORT_H_
