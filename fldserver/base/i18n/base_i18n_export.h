// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_CORE_EXPORT_H_
#define BASE_I18N_CORE_EXPORT_H_

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(BASE_I18N_IMPLEMENTATION)
#define CORE_EXPORT __declspec(dllexport)
#else
#define CORE_EXPORT __declspec(dllimport)
#endif  // defined(BASE_I18N_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(BASE_I18N_IMPLEMENTATION)
#define CORE_EXPORT __attribute__((visibility("default")))
#else
#define CORE_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define CORE_EXPORT
#endif

#endif  // BASE_I18N_CORE_EXPORT_H_
