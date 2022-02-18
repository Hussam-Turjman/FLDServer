#!/usr/bin/env python

# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.

from blitzmanager import Command, Path
import os

if __name__ == "__main__":
    cwd = os.path.dirname(os.path.realpath(__file__))

    source = Path(cwd, "fldserver").path

    files_to_be_formatted = []
    for root, subdirs, files in os.walk(source):

        for file in files:
            if file.endswith(".c") or file.endswith(".h") \
                    or file.endswith(".cpp") or file.endswith(".cc"):
                if Path(Path(root).relative_path(source)).split()[0] == "base":
                    continue
                files_to_be_formatted.append(Path(root, file).path)

    Command("-i", "-style=file",
            *files_to_be_formatted,
            program="clang-format"). \
        execute(cwd=cwd)
