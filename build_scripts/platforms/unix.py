#############################################################################
##
## Copyright (C) 2018 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of Qt for Python.
##
## $QT_BEGIN_LICENSE:LGPL$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU Lesser General Public License Usage
## Alternatively, this file may be used under the terms of the GNU Lesser
## General Public License version 3 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL3 included in the
## packaging of this file. Please review the following information to
## ensure the GNU Lesser General Public License version 3 requirements
## will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 2.0 or (at your option) the GNU General
## Public license version 3 or any later version approved by the KDE Free
## Qt Foundation. The licenses are as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-2.0.html and
## https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################

import fnmatch
import os
import sys

from ..config import config
from ..options import OPTION
from ..utils import copydir, copyfile, makefile
from ..versions import PYSIDE, SHIBOKEN
from .linux import prepare_standalone_package_linux
from .macos import prepare_standalone_package_macos


def _macos_copy_gui_executable(name, _vars=None):
    """macOS helper: Copy a GUI executable from the .app folder and return the
       files"""
    app_name = f"{name.capitalize()}.app"
    return copydir(f"{{install_dir}}/bin/{app_name}",
                   f"{{st_build_dir}}/{{st_package_name}}/{app_name}",
                   _filter=None, recursive=True,
                   force=False, _vars=_vars)


def _unix_copy_gui_executable(name, _vars=None):
    """UNIX helper: Copy a GUI executable and return the files"""
    return copydir("{install_dir}/bin/",
                   "{st_build_dir}/{st_package_name}/",
                   _filter=[name],
                   force=False, _vars=_vars)


def _copy_gui_executable(name, _vars=None):
    """Copy a GUI executable and return the files"""
    if sys.platform == 'darwin':
        return _macos_copy_gui_executable(name, _vars)
    return _unix_copy_gui_executable(name, _vars)


def prepare_packages_posix(self, _vars, cross_build=False):
    executables = []
    libexec_executables = []

    # <install>/lib/site-packages/{st_package_name}/* ->
    # <setup>/{st_package_name}
    # This copies the module .so/.dylib files and various .py files
    # (__init__, config, git version, etc.)
    copydir(
        "{site_packages_dir}/{st_package_name}",
        "{st_build_dir}/{st_package_name}",
        _vars=_vars)

    generated_config = self.get_built_pyside_config(_vars)

    def adjusted_lib_name(name, version):
        postfix = ''
        if sys.platform.startswith('linux'):
            postfix = f".so.{version}"
        elif sys.platform == 'darwin':
            postfix = f".{version}.dylib"
        return name + postfix

    if config.is_internal_shiboken_module_build():
        # <build>/shiboken6/doc/html/* ->
        #   <setup>/{st_package_name}/docs/shiboken6
        copydir(
            f"{{build_dir}}/{SHIBOKEN}/doc/html",
            f"{{st_build_dir}}/{{st_package_name}}/docs/{SHIBOKEN}",
            force=False, _vars=_vars)

        # <install>/lib/lib* -> {st_package_name}/
        copydir(
            "{install_dir}/lib/",
            "{st_build_dir}/{st_package_name}",
            _filter=[
                adjusted_lib_name("libshiboken*",
                                  generated_config['shiboken_library_soversion']),
            ],
            recursive=False, _vars=_vars, force_copy_symlinks=True)

    if config.is_internal_shiboken_generator_build():
        # <install>/bin/* -> {st_package_name}/
        executables.extend(copydir(
            "{install_dir}/bin/",
            "{st_build_dir}/{st_package_name}",
            _filter=[SHIBOKEN],
            recursive=False, _vars=_vars))

        # Used to create scripts directory.
        makefile(
            "{st_build_dir}/{st_package_name}/scripts/shiboken_tool.py",
            _vars=_vars)

        # For setting up setuptools entry points.
        copyfile(
            "{install_dir}/bin/shiboken_tool.py",
            "{st_build_dir}/{st_package_name}/scripts/shiboken_tool.py",
            force=False, _vars=_vars)

    if config.is_internal_shiboken_generator_build() or config.is_internal_pyside_build():
        # <install>/include/* -> <setup>/{st_package_name}/include
        copydir(
            "{install_dir}/include/{cmake_package_name}",
            "{st_build_dir}/{st_package_name}/include",
            _vars=_vars)

    if config.is_internal_pyside_build():
        makefile(
            "{st_build_dir}/{st_package_name}/scripts/__init__.py",
            _vars=_vars)

        # For setting up setuptools entry points
        for script in ("pyside_tool.py", "metaobjectdump.py", "project.py"):
            src = f"{{install_dir}}/bin/{script}"
            target = f"{{st_build_dir}}/{{st_package_name}}/scripts/{script}"
            copyfile(src, target, force=False, _vars=_vars)

        # <install>/bin/* -> {st_package_name}/
        executables.extend(copydir(
            "{install_dir}/bin/",
            "{st_build_dir}/{st_package_name}",
            _filter=[f"{PYSIDE}-lupdate"],
            recursive=False, _vars=_vars))

        lib_exec_filters = []
        if not OPTION['NO_QT_TOOLS']:
            lib_exec_filters.extend(['uic', 'rcc', 'qmltyperegistrar'])
            executables.extend(copydir(
                "{install_dir}/bin/",
                "{st_build_dir}/{st_package_name}",
                _filter=["lrelease", "lupdate", "qmllint"],
                recursive=False, _vars=_vars))
            # Copying assistant/designer
            executables.extend(_copy_gui_executable('assistant', _vars=_vars))
            executables.extend(_copy_gui_executable('designer', _vars=_vars))
            executables.extend(_copy_gui_executable('linguist', _vars=_vars))

        # <qt>/lib/metatypes/* -> <setup>/{st_package_name}/Qt/lib/metatypes
        destination_lib_dir = "{st_build_dir}/{st_package_name}/Qt/lib"
        copydir("{qt_lib_dir}/metatypes", f"{destination_lib_dir}/metatypes",
                _filter=["*.json"],
                recursive=False, _vars=_vars, force_copy_symlinks=True)

        # Copy libexec
        built_modules = self.get_built_pyside_config(_vars)['built_modules']
        if self.is_webengine_built(built_modules):
            lib_exec_filters.append('QtWebEngineProcess')
        if lib_exec_filters:
            libexec_executables.extend(copydir("{qt_lib_execs_dir}",
                                               "{st_build_dir}/{st_package_name}/Qt/libexec",
                                               _filter=lib_exec_filters,
                                               recursive=False,
                                               _vars=_vars))

        # <install>/lib/lib* -> {st_package_name}/
        copydir(
            "{install_dir}/lib/",
            "{st_build_dir}/{st_package_name}",
            _filter=[
                adjusted_lib_name("libpyside*",
                                  generated_config['pyside_library_soversion']),
            ],
            recursive=False, _vars=_vars, force_copy_symlinks=True)

        # <install>/share/{st_package_name}/typesystems/* ->
        #   <setup>/{st_package_name}/typesystems
        copydir(
            "{install_dir}/share/{st_package_name}/typesystems",
            "{st_build_dir}/{st_package_name}/typesystems",
            _vars=_vars)

        # <install>/share/{st_package_name}/glue/* ->
        #   <setup>/{st_package_name}/glue
        copydir(
            "{install_dir}/share/{st_package_name}/glue",
            "{st_build_dir}/{st_package_name}/glue",
            _vars=_vars)

        # <source>/pyside6/{st_package_name}/support/* ->
        #   <setup>/{st_package_name}/support/*
        copydir(
            f"{{build_dir}}/{PYSIDE}/{{st_package_name}}/support",
            "{st_build_dir}/{st_package_name}/support",
            _vars=_vars)

        # <source>/pyside6/{st_package_name}/*.pyi ->
        #   <setup>/{st_package_name}/*.pyi
        copydir(
            f"{{build_dir}}/{PYSIDE}/{{st_package_name}}",
            "{st_build_dir}/{st_package_name}",
            _filter=["*.pyi", "py.typed"],
            _vars=_vars)

        if not OPTION["NOEXAMPLES"]:
            def pycache_dir_filter(dir_name, parent_full_path, dir_full_path):
                if fnmatch.fnmatch(dir_name, "__pycache__"):
                    return False
                return True
            # examples/* -> <setup>/{st_package_name}/examples
            copydir(os.path.join(self.script_dir, "examples"),
                    "{st_build_dir}/{st_package_name}/examples",
                    force=False, _vars=_vars, dir_filter_function=pycache_dir_filter)

    # Copy Qt libs to package
    if OPTION["STANDALONE"]:
        if config.is_internal_pyside_build() or config.is_internal_shiboken_generator_build():
            _vars['built_modules'] = generated_config['built_modules']
            if sys.platform == 'darwin':
                prepare_standalone_package_macos(self, _vars)
            else:
                prepare_standalone_package_linux(self, _vars, cross_build)

        if config.is_internal_shiboken_generator_build():
            # Copy over clang before rpath patching.
            self.prepare_standalone_clang(is_win=False)

    # Update rpath to $ORIGIN
    if sys.platform.startswith('linux') or sys.platform.startswith('darwin'):
        rpath_path = "{st_build_dir}/{st_package_name}".format(**_vars)
        self.update_rpath(rpath_path, executables)
        self.update_rpath(rpath_path, self.package_libraries(rpath_path))
        if libexec_executables:
            self.update_rpath(rpath_path, libexec_executables, libexec=True)
