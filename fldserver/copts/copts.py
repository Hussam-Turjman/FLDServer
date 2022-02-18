# Auto generated file by blitz 2021-04-22-20:11:59. Blitz is a tool for generating cmake
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

#
# Copyright 2017 The Abseil Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""FLDServer compiler options.

This is the source of truth for FLDServer compiler options.  To modify FLDServer
compilation options:

  (1) Edit the appropriate list in this file based on the platform the flag is
      needed on.
  (2) Run `<path_to_fldserver>/copts/generate_copts.py`.

The generated copts are consumed by configure_copts.bzl and
FLDServerConfigureCopts.cmake.
"""

# /Wall with msvc includes unhelpful warnings such as C4711, C4710, ...
MSVC_BIG_WARNING_FLAGS = [
    "/W3",
]

LLVM_BIG_WARNING_FLAGS = [
    "-Wall",
    "-Wextra",
    "-Weverything",
]

# Docs on single flags is preceded by a comment.
# Docs on groups of flags is preceded by ###.
LLVM_DISABLE_WARNINGS_FLAGS = [
    # FLDServer does not support C++98
    "-Wno-c++98-compat-pedantic",
    # Turns off all implicit conversion warnings. Most are re-enabled below.
    "-Wno-conversion",
    "-Wno-covered-switch-default",
    "-Wno-deprecated",
    "-Wno-disabled-macro-expansion",
    "-Wno-double-promotion",
    ###
    # Turned off as they include valid C++ code.
    "-Wno-comma",
    "-Wno-extra-semi",
    "-Wno-extra-semi-stmt",
    "-Wno-packed",
    "-Wno-padded",
    ###
    # Google style does not use unsigned integers, though STL containers
    # have unsigned types.
    "-Wno-sign-compare",
    ###
    "-Wno-float-conversion",
    "-Wno-float-equal",
    "-Wno-format-nonliteral",
    # Too aggressive: warns on Clang extensions enclosed in Clang-only
    # compilation paths.
    "-Wno-gcc-compat",
    ###
    # Some internal globals are necessary. Don't do this at home.
    "-Wno-global-constructors",
    "-Wno-exit-time-destructors",
    ###
    "-Wno-non-modular-include-in-module",
    "-Wno-old-style-cast",
    # Warns on preferred usage of non-POD types such as string_view
    "-Wno-range-loop-analysis",
    "-Wno-reserved-id-macro",
    "-Wno-shorten-64-to-32",
    "-Wno-switch-enum",
    "-Wno-thread-safety-negative",
    "-Wno-unknown-warning-option",
    "-Wno-unreachable-code",
    # Causes warnings on include guards
    "-Wno-unused-macros",
    "-Wno-weak-vtables",
    # Causes warnings on usage of types/compare.h comparison operators.
    "-Wno-zero-as-null-pointer-constant",
    ###
    # Implicit conversion warnings turned off by -Wno-conversion
    # which are re-enabled below.
    "-Wbitfield-enum-conversion",
    "-Wbool-conversion",
    "-Wconstant-conversion",
    "-Wenum-conversion",
    "-Wint-conversion",
    "-Wliteral-conversion",
    "-Wnon-literal-null-conversion",
    "-Wnull-conversion",
    "-Wobjc-literal-conversion",
    "-Wno-sign-conversion",
    "-Wstring-conversion",
]

LLVM_TEST_DISABLE_WARNINGS_FLAGS = [
    "-Wno-c99-extensions",
    "-Wno-deprecated-declarations",
    "-Wno-missing-noreturn",
    "-Wno-missing-prototypes",
    "-Wno-missing-variable-declarations",
    "-Wno-null-conversion",
    "-Wno-shadow",
    "-Wno-shift-sign-overflow",
    "-Wno-sign-compare",
    "-Wno-unused-function",
    "-Wno-unused-member-function",
    "-Wno-unused-parameter",
    "-Wno-unused-private-field",
    "-Wno-unused-template",
    "-Wno-used-but-marked-unused",
    "-Wno-zero-as-null-pointer-constant",
    # gtest depends on this GNU extension being offered.
    "-Wno-gnu-zero-variadic-macro-arguments",
]

MSVC_DEFINES = [
    "/DNOMINMAX",  # Don't define min and max macros (windows.h)
    # Don't bloat namespace with incompatible winsock versions.
    "/DWIN32_LEAN_AND_MEAN",
    # Don't warn about usage of insecure C functions.
    "/D_CRT_SECURE_NO_WARNINGS",
    "/D_SCL_SECURE_NO_WARNINGS",
    # Introduced in VS 2017 15.8, allow overaligned types in aligned_storage
    "/D_ENABLE_EXTENDED_ALIGNED_STORAGE",
    # Statically links the UCRT into your code.
    "/MT",
]

COPT_VARS = {
    "FLDSERVER_GCC_FLAGS": [
        "-Wall",
        "-Wextra",
        "-Wcast-qual",
        # "-Wconversion-null", is valid for C++/ObjC++ but not for C
        "-Wmissing-declarations",
        "-Woverlength-strings",
        "-Wpointer-arith",
        "-Wundef",
        "-Wunused-local-typedefs",
        "-Wunused-result",
        "-Wvarargs",
        "-Wvla",  # variable-length array
        "-Wwrite-strings",
        # gcc-4.x has spurious missing field initializer warnings.
        # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=36750
        # Remove when gcc-4.x is no longer supported.
        "-Wno-missing-field-initializers",
        # Google style does not use unsigned integers, though STL containers
        # have unsigned types.
        "-Wno-sign-compare",
        # Don't define min and max macros (Build on Windows using gcc)
        "-DNOMINMAX",
        # Ignore narrowing conversion
        "-Wno-narrowing",
    ],
    "FLDSERVER_GCC_TEST_FLAGS": [
        "-Wno-conversion-null",
        "-Wno-deprecated-declarations",
        "-Wno-missing-declarations",
        "-Wno-sign-compare",
        "-Wno-unused-function",
        "-Wno-unused-parameter",
        "-Wno-unused-private-field",
    ],
    "FLDSERVER_LLVM_FLAGS":
        LLVM_BIG_WARNING_FLAGS + LLVM_DISABLE_WARNINGS_FLAGS + [
            # Don't define min and max macros (Build on Windows using clang)
            "-DNOMINMAX",
        ],
    "FLDSERVER_LLVM_TEST_FLAGS":
        LLVM_TEST_DISABLE_WARNINGS_FLAGS,
    "FLDSERVER_CLANG_CL_FLAGS":
        (MSVC_BIG_WARNING_FLAGS + LLVM_DISABLE_WARNINGS_FLAGS + MSVC_DEFINES),
    "FLDSERVER_CLANG_CL_TEST_FLAGS":
        LLVM_TEST_DISABLE_WARNINGS_FLAGS,
    "FLDSERVER_MSVC_FLAGS":
        MSVC_BIG_WARNING_FLAGS + MSVC_DEFINES + [
            # Increase the number of sections available in object files
            "/bigobj",
            "/wd4005",  # macro-redefinition
            "/wd4068",  # unknown pragma
            # qualifier applied to function type has no meaning; ignored
            "/wd4180",
            # conversion from 'type1' to 'type2', possible loss of data
            "/wd4244",
            # conversion from 'size_t' to 'type', possible loss of data
            "/wd4267",
            # The decorated name was longer than the compiler limit
            "/wd4503",
            # forcing value to bool 'true' or 'false' (performance warning)
            "/wd4800",
        ],
    "FLDSERVER_MSVC_TEST_FLAGS": [
        "/wd4018",  # signed/unsigned mismatch
        "/wd4101",  # unreferenced local variable
        "/wd4503",  # decorated name length exceeded, name was truncated
        "/wd4996",  # use of deprecated symbol
        "/DNOMINMAX",  # disable the min() and max() macros from <windows.h>
    ],
    "FLDSERVER_MSVC_LINKOPTS": [
        # Object file doesn't export any previously undefined symbols
        "-ignore:4221",
    ],
    # "HWAES" is an abbreviation for "hardware AES" (AES - Advanced Encryption
    # Standard). These flags are used for detecting whether or not the target
    # architecture has hardware support for AES instructions which can be used
    # to improve performance of some random bit generators.
    "FLDSERVER_RANDOM_HWAES_ARM64_FLAGS": ["-march=armv8-a+crypto"],
    "FLDSERVER_RANDOM_HWAES_ARM32_FLAGS": ["-mfpu=neon"],
    "FLDSERVER_RANDOM_HWAES_X64_FLAGS": [
        "-maes",
        "-msse4.1",
    ],
    "FLDSERVER_RANDOM_HWAES_MSVC_X64_FLAGS": [],
}

# COPT_VARS = {}
if len(COPT_VARS.keys()) == 0:
    print()
    print("#" * 20, " WARNING : ALL COMPILER FLAGS ARE DISABLED ", "#" * 20)
    print()
    print(" TO ENABLE THEM GO TO fldserver/copts/copts.py")
    print()
