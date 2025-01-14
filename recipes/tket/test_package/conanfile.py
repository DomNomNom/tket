# Copyright 2019-2021 Cambridge Quantum Computing
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
from shutil import copyfile
import platform

from conans import ConanFile, CMake, tools


class TketTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    keep_imports = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*", src="@bindirs", dst="bin")
        self.copy("*", src="@libdirs", dst="lib")

    def test(self):
        if not tools.cross_building(self):
            libname = {
                "Linux": "libtket.so",
                "Darwin": "libtket.dylib",
                "Windows": "tket.dll",
            }
            tket_lib = libname[platform.system()]
            copyfile(
                os.path.join(self.install_folder, "lib", tket_lib),
                os.path.join("bin", tket_lib),
            )
            os.chdir("bin")
            self.run(os.path.join(os.curdir, "test"))
