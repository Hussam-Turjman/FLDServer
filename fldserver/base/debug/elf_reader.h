// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_ELF_READER_H_
#define BASE_DEBUG_ELF_READER_H_

#include "fldserver/base/containers/span.h"
#include "fldserver/base/hash/sha1.h"
#include "fldserver/base/optional.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/fldserver_config.h"

#include <elf.h>

// Functions for querying metadata from ELF binaries. All functions are signal
// safe and require that the file be fully memory mapped.

#if __SIZEOF_POINTER__ == 4
using Phdr = Elf32_Phdr;
#else
using Phdr = Elf64_Phdr;
#endif

namespace base
{
namespace debug
{
// Hex-encodes the build ID from the ELF binary located at |elf_mapped_base|.
// Returns the length of the build ID in bytes, or zero if the build ID couldn't
// be read.
// When |uppercase| is |true|, the output string is written using uppercase hex
// characters. Otherwise, the output is lowercased.
constexpr size_t kMaxBuildIdStringLength = kSHA1Length * 2;
using ElfBuildIdBuffer = char[kMaxBuildIdStringLength + 1];
size_t CORE_EXPORT
ReadElfBuildId(const void* elf_mapped_base, bool uppercase, ElfBuildIdBuffer build_id);

// Returns the library name from the ELF file mapped at |elf_mapped_base|.
// Returns an empty result if the name could not be read.
Optional<StringPiece> CORE_EXPORT
ReadElfLibraryName(const void* elf_mapped_base);

// Returns a span of ELF program headers for the ELF file mapped at
// |elf_mapped_base|, or an empty span if the header couldn't be read.
span<const Phdr> CORE_EXPORT
GetElfProgramHeaders(const void* elf_mapped_base);

// Returns the offset to add to virtual addresses in the image to compute the
// mapped virtual address. This value must be added to the p_vaddr field in the
// Phdrs to obtain the mapped virtual address.
size_t CORE_EXPORT
GetRelocationOffset(const void* elf_mapped_base);

}  // namespace debug
}  // namespace base

#endif  // BASE_DEBUG_ELF_READER_H_
