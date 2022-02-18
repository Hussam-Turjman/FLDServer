#!/usr/bin/env python

# Auto generated file by blitz 2021-04-29-22:22:31. Blitz is a tool for generating cmake 
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

# Copyright (c) 2021 The Blitz project Authors. All rights reserved. Use of this source 
# code is governed by a BSD-style license that can be found in the LICENSE file.

import os
import subprocess

def mkdir(path: str):
    """

    :param path:
    :return:
    """
    os.makedirs(path, exist_ok=True)

if __name__ == "__main__":
    out = os.path.join(os.path.dirname(os.path.realpath(__file__)), "out")
    mkdir(out)
    subprocess.run(
        ["cmake",
         "-DCMAKE_INSTALL_PREFIX=install",
         "-GNinja", ".."],
        cwd=out, check=True)

    subprocess.run(
        ["ninja", "-j","8"],
        cwd=out, check=True)

    subprocess.run(
        ["ninja", "install"],
        cwd=out, check=True)