#!/usr/bin/env python

# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.
import shutil

from blitzmanager import BlitzManager, Path, SupportedManagers, logger
from blitzmanager.platform import PLATFORM
from blitzmanager.command import Command
import os
import json

cwd = os.path.dirname(os.path.realpath(__file__))


class DepsCache(object):
    def __init__(self):

        self.output_path = None
        self.deps = {}

    def add_dep(self, dep: str, success: bool):
        """

        :param dep:
        :param success:
        :return:
        """
        self.deps[dep] = success

    def load(self, output_dir: Path):
        """

        :return:
        """
        self.output_path = Path(output_dir.path, "deps_cache.json")
        if self.output_path.exists():
            with open(self.output_path.path, "r") as f:
                self.deps = json.load(f)
                f.close()

    def check(self, dep: str) -> bool:
        """

        :param dep:
        :return:
        """
        if dep not in self.deps.keys():
            return False
        return self.deps[dep]

    def export(self):
        """

        :return:
        """
        with open(self.output_path.path, "w+") as f:
            json.dump(self.deps, f, indent=4)
            f.close()


class PkgConfigCreator(object):
    def __init__(self):
        """

        """
        self.config = {}

    def create(self, config: dict):
        """

        :param config:
        :return:
        """
        self.config = config
        return self

    def install(self, target_path: Path):
        """

        :param target_path:
        :return:
        """

        with open(target_path.path, "w+") as f:
            for key in self.config.keys():
                if key[0].islower():
                    f.write(f"{key}={self.config[key]}{os.linesep}")
                else:
                    f.write(f"{key}: {self.config[key]}{os.linesep}")
            f.close()

        return self


class PreBuiltDep(object):
    def __init__(self, name: str,
                 lib_source: Path,
                 lib_target: Path,
                 include_source: Path,
                 include_target: Path):
        self.name = name
        self.lib_source = lib_source
        self.lib_target = lib_target
        self.include_source = include_source
        self.include_target = include_target

    def create_pkg_config(self, config: dict, install_path: Path):
        """

        :return:
        """

        PkgConfigCreator().create(config).install(install_path)
        return self

    def copy(self):
        assert self.lib_source.exists()
        assert self.include_source.exists()

        def lib_callback(root, _, files):
            to_copy = []

            for f in files:
                if PLATFORM.is_linux():
                    if ".so" in f or ".a" in f:
                        to_copy.append(f)
                elif PLATFORM.is_windows():
                    if ".dll" in f or ".lib" in f:
                        to_copy.append(f)
                else:
                    raise NotImplementedError()

            for f in to_copy:
                Path(root, f).copy_to(self.lib_target.copy().join(f))

        assert self.lib_source.walk(lib_callback, recursive=True)
        assert self.include_source.copy_to(self.include_target)


def generate_cmake_arguments():
    """

    :return:
    """
    cmake_args = {
        "fldserver": ["-DBUILD_SHARED_LIBS=OFF",
                      "-DCORE_ENABLE_INSTALL=ON",
                      "-DBUILD_DLL=OFF",
                      "-DCORE_RUN_TESTS=OFF",
                      ],
        "opencv": ["-DWITH_FFMPEG=ON",
                   "-DWITH_GTK=ON",
                   "-DBUILD_SHARED_LIBS=OFF",
                   "-DBUILD_opencv_apps=OFF",
                   "-DBUILD_TESTS=OFF",
                   "-DBUILD_DOCS=OFF",
                   "-DBUILD_EXAMPLES=OFF",
                   "-DBUILD_JAVA=OFF",
                   "-DBUILD_ZLIB=ON",
                   "-DBUILD_TIFF=ON",
                   "-DBUILD_TBB=ON",
                   ],

        "OpenFace": ["-DBUILD_SHARED_LIBS=OFF",
                     "-DBUILD_EXAMPLES=OFF",
                     "-DBUILD_DOCS=OFF",
                     "-DBUILD_TESTS=OFF", ],

        "libevent": ["-DEVENT__DISABLE_DEBUG_MODE=ON",
                     "-DEVENT__DISABLE_BENCHMARK=ON",
                     "-DEVENT__DISABLE_TESTS=ON",
                     "-DEVENT__DISABLE_REGRESS=ON",
                     "-DEVENT__DISABLE_SAMPLES=ON",
                     "-DEVENT__LIBRARY_TYPE=STATIC",
                     "-DEVENT__DISABLE_OPENSSL=ON", ],
        "abseil": [],
        "googletest": [],
        "Dummy": [],

    }
    if PLATFORM.is_linux():
        cmake_args["fldserver"].append("-DCMAKE_C_COMPILER=gcc-10")
        cmake_args["fldserver"].append("-DCMAKE_CXX_COMPILER=g++-10")
    elif PLATFORM.is_darwin():
        raise NotImplementedError("Darwin is currently not supported.")
    return cmake_args


def package_manager_deps(deps_cache: DepsCache):
    """

    :return:
    """

    _deps = ["protobuf",
             "libxml2",
             "sqlite3",
             "zlib",
             "boost",
             "tbb",
             "tiff",
             "libpng",
             "libjpeg-turbo",
             "ffmpeg",
             "dlib",
             "icu",
             ]
    to_remove = []
    for _d in _deps:
        if deps_cache.check(_d):
            to_remove.append(_d)
    for t in to_remove:
        _deps.remove(t)
    return _deps


class Tools(object):
    def __init__(self, manager: BlitzManager, supported_manager: SupportedManagers):
        binary_name = "protoc.exe" if PLATFORM.is_windows() else "protoc"
        if supported_manager is not SupportedManagers.NONE:
            self.protobuf = Path(manager.manager_install_path.path, "tools", "protobuf", binary_name)
        else:
            path = shutil.which("protoc")
            if path is not None:
                self.protobuf = Path(path)
            else:
                self.protobuf = None
        logger.info(f"Using protoc from : {self.protobuf}")


def compile_protocol_buffers_files(root_path: Path, protoc_path: Path):
    if PLATFORM.is_linux():
        if protoc_path is not None:
            Command("+x", protoc_path.path, program="chmod").execute()

    protocols = {

    }
    for p in protocols.keys():
        if protoc_path is None:
            logger.error("Protocol buffer executable path is invalid.")
            continue
        for v in protocols[p]:
            if Path(root_path.path, *p, f"{v}.pb.h").exists() and \
                    Path(root_path.path, *p, f"{v}.pb.cc").exists():
                continue
            node = Path(root_path.path, *p, f"{v}.proto")
            protocol_dir = Path(root_path.path, *p)
            protocol_out_dir = Path(root_path.path, *p)

            Command("--cpp_out={}".format(protocol_out_dir.path),
                    "--proto_path={}".format(protocol_dir.path),
                    node.path,
                    program=protoc_path.path). \
                execute(cwd=protocol_dir.path)


def build_fldserver(manager: BlitzManager, cmake_args: list, source_dir: Path, install_dir: Path,
                    supported_manager: SupportedManagers):
    """

    :param supported_manager:
    :param manager:
    :param cmake_args:
    :param source_dir:
    :param install_dir:
    :return:
    """
    tools = Tools(manager, supported_manager)
    compile_protocol_buffers_files(source_dir, tools.protobuf)

    out = Path(source_dir.path, "fldserver", "copts").path
    Command("generate_copts.py", program="python3").execute(cwd=out)
    manager.clear_dependencies()
    manager.build_from_source("fldserver", *cmake_args, install_path=install_dir, input_dir=source_dir)
    manager.build_dependencies()
    _, args, _ = manager.dependencies["fldserver"]
    with open(Path(source_dir.path, "CMakeArguments.txt").path, "w+") as f:
        f.write(str(args))
        f.close()
    user_args = f"-DCMAKE_PREFIX_PATH={install_dir.path}"
    print()
    print()
    print(f"The FLDServer has been successfully installed here : {install_dir}")
    user_info = f"""
    
    
    
    
    
    """
    print(user_info)
    if manager["disable_dev_mode"]:
        pass
    else:
        p = Path(install_dir.path, "include", "fldserver")
        p.remove(ignore_errors=False)


def run():
    manager = BlitzManager()
    manager.add_flags({
        "--build_dir": {
            "default": Path(cwd, "out").path,
            "help": "Build directory for all third_party dependencies and the FLDServer.",
            "required": False,
            "type": str
        },
        "--install_dir": {
            "default": Path(cwd, "out", "install").path,
            "help": "Install directory.",
            "required": False,
            "type": str,
        },
        "--disable_package_manager": {
            "default": False,
            "help": "Assume third_party deps are already installed and disable the package manager.",
            "required": False,
            "action": "store_true"
        },
        "--disable_dev_mode": {
            "default": False,
            "help": "Keep FLDServer include directory after install.",
            "required": False,
            "action": "store_true"
        }
    })
    manager.parse_arguments()
    build_path = Path(manager["build_dir"])
    root_path = Path(cwd)
    install_path = Path(manager["install_dir"])
    manager_output_path = Path(cwd, "out")
    deps_cache = DepsCache()
    deps_cache.load(build_path)
    supported_manager = SupportedManagers.NONE if manager["disable_package_manager"] else SupportedManagers.VCPKG
    manager.initialize(manager_output_path, build_path, install_path, supported_manager)
    manager.initialize_managers()
    pkg_deps = package_manager_deps(deps_cache)
    manager.build_via_package_manager(pkg_deps)

    cmake_arguments = generate_cmake_arguments()
    fldserver_cmake_arguments = cmake_arguments.pop("fldserver")
    built_deps = []
    built_deps += pkg_deps
    for dep in cmake_arguments.keys():
        if deps_cache.check(dep):
            continue
        manager.build_from_source(dep, *cmake_arguments[dep], input_dir=Path(root_path.path, "third_party", dep))
        built_deps.append(dep)

    manager.build_dependencies()

    for d in built_deps:
        deps_cache.add_dep(d, True)

    deps_cache.export()

    build_fldserver(manager, fldserver_cmake_arguments, root_path, install_path, supported_manager)


if __name__ == "__main__":
    run()
