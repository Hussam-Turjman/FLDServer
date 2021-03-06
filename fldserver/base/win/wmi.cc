// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/win/wmi.h"

#include "fldserver/base/location.h"
#include "fldserver/base/strings/string_util.h"
#include "fldserver/base/strings/utf_string_conversions.h"
#include "fldserver/base/threading/scoped_thread_priority.h"
#include "fldserver/base/win/scoped_bstr.h"
#include "fldserver/base/win/scoped_variant.h"

#include <objbase.h>
#include <windows.h>

#include <stdint.h>

#include <utility>

using Microsoft::WRL::ComPtr;

namespace base
{
namespace win
{
bool
CreateLocalWmiConnection(bool set_blanket, ComPtr<IWbemServices>* wmi_services)
{
    // Mitigate the issues caused by loading DLLs on a background thread
    // (http://crbug/973868).
    SCOPED_MAY_LOAD_LIBRARY_AT_BACKGROUND_PRIORITY();

    ComPtr<IWbemLocator> wmi_locator;
    HRESULT hr = ::CoCreateInstance(
            CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wmi_locator));
    if (FAILED(hr))
        return false;

    ComPtr<IWbemServices> wmi_services_r;
    hr = wmi_locator->ConnectServer(ScopedBstr(L"ROOT\\CIMV2").Get(),
                                    nullptr,
                                    nullptr,
                                    nullptr,
                                    0,
                                    nullptr,
                                    nullptr,
                                    &wmi_services_r);
    if (FAILED(hr))
        return false;

    if (set_blanket)
    {
        hr = ::CoSetProxyBlanket(wmi_services_r.Get(),
                                 RPC_C_AUTHN_WINNT,
                                 RPC_C_AUTHZ_NONE,
                                 nullptr,
                                 RPC_C_AUTHN_LEVEL_CALL,
                                 RPC_C_IMP_LEVEL_IMPERSONATE,
                                 nullptr,
                                 EOAC_NONE);
        if (FAILED(hr))
            return false;
    }

    *wmi_services = std::move(wmi_services_r);
    return true;
}

bool
CreateWmiClassMethodObject(IWbemServices* wmi_services,
                           WStringPiece class_name,
                           WStringPiece method_name,
                           ComPtr<IWbemClassObject>* class_instance)
{
    // We attempt to instantiate a COM object that represents a WMI object plus
    // a method rolled into one entity.
    ScopedBstr b_class_name(class_name);
    ScopedBstr b_method_name(method_name);
    ComPtr<IWbemClassObject> class_object;
    HRESULT hr;
    hr = wmi_services->GetObject(b_class_name.Get(), 0, nullptr, &class_object, nullptr);
    if (FAILED(hr))
        return false;

    ComPtr<IWbemClassObject> params_def;
    hr = class_object->GetMethod(b_method_name.Get(), 0, &params_def, nullptr);
    if (FAILED(hr))
        return false;

    if (!params_def.Get())
    {
        // You hit this special case if the WMI class is not a CIM class. MSDN
        // sometimes tells you this. Welcome to WMI hell.
        return false;
    }

    hr = params_def->SpawnInstance(0, &(*class_instance));
    return SUCCEEDED(hr);
}

// The code in Launch() basically calls the Create Method of the Win32_Process
// CIM class is documented here:
// http://msdn2.microsoft.com/en-us/library/aa389388(VS.85).aspx
// NOTE: The documentation for the Create method suggests that the ProcessId
// parameter and return value are of type uint32_t, but when we call the method
// the values in the returned out_params, are VT_I4, which is int32_t.
bool
WmiLaunchProcess(const std::wstring& command_line, int* process_id)
{
    ComPtr<IWbemServices> wmi_local;
    if (!CreateLocalWmiConnection(true, &wmi_local))
        return false;

    static constexpr wchar_t class_name[] = L"Win32_Process";
    static constexpr wchar_t method_name[] = L"Create";
    ComPtr<IWbemClassObject> process_create;
    if (!CreateWmiClassMethodObject(wmi_local.Get(), class_name, method_name, &process_create))
    {
        return false;
    }

    ScopedVariant b_command_line(command_line.c_str());

    if (FAILED(process_create->Put(L"CommandLine", 0, b_command_line.AsInput(), 0)))
    {
        return false;
    }

    ComPtr<IWbemClassObject> out_params;
    HRESULT hr = wmi_local->ExecMethod(ScopedBstr(class_name).Get(),
                                       ScopedBstr(method_name).Get(),
                                       0,
                                       nullptr,
                                       process_create.Get(),
                                       &out_params,
                                       nullptr);
    if (FAILED(hr))
        return false;

    // We're only expecting int32_t or uint32_t values, so no need for
    // ScopedVariant.
    VARIANT ret_value = {{{VT_EMPTY}}};
    hr = out_params->Get(L"ReturnValue", 0, &ret_value, nullptr, nullptr);
    if (FAILED(hr) || V_I4(&ret_value) != 0)
        return false;

    VARIANT pid = {{{VT_EMPTY}}};
    hr = out_params->Get(L"ProcessId", 0, &pid, nullptr, nullptr);
    if (FAILED(hr) || V_I4(&pid) == 0)
        return false;

    if (process_id)
        *process_id = V_I4(&pid);

    return true;
}

// static
WmiComputerSystemInfo
WmiComputerSystemInfo::Get()
{
    ComPtr<IWbemServices> services;
    WmiComputerSystemInfo info;

    if (!CreateLocalWmiConnection(true, &services))
        return info;

    info.PopulateSerialNumber(services);

    return info;
}

void
WmiComputerSystemInfo::PopulateSerialNumber(const ComPtr<IWbemServices>& services)
{
    static constexpr WStringPiece query_bios = L"SELECT SerialNumber FROM Win32_Bios";

    ComPtr<IEnumWbemClassObject> enumerator_bios;
    HRESULT hr = services->ExecQuery(ScopedBstr(L"WQL").Get(),
                                     ScopedBstr(query_bios).Get(),
                                     WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                     nullptr,
                                     &enumerator_bios);
    if (FAILED(hr) || !enumerator_bios.Get())
        return;

    ComPtr<IWbemClassObject> class_obj;
    ULONG items_returned = 0;
    hr = enumerator_bios->Next(WBEM_INFINITE, 1, &class_obj, &items_returned);
    if (FAILED(hr) || !items_returned)
        return;

    ScopedVariant serial_number;
    hr = class_obj->Get(L"SerialNumber", 0, serial_number.Receive(), nullptr, nullptr);
    if (SUCCEEDED(hr) && serial_number.type() == VT_BSTR)
    {
        serial_number_.assign(V_BSTR(serial_number.ptr()),
                              ::SysStringLen(V_BSTR(serial_number.ptr())));
    }
}

}  // namespace win
}  // namespace base
