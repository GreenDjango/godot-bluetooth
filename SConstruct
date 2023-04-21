#!/usr/bin/env python

import os
import subprocess

env = SConscript("thirdparty/godot-cpp/SConstruct")


def cmake(base_dir):
    cmake_target = "Debug"
    if env["target"] == "template_release":
        cmake_target = "Release"
    if not os.path.isdir(f"{base_dir}/{env['target']}"):
        cmd_args = []
        if env["platform"] == "osx":
            cmd_args.append("-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64")
        subprocess.call(
            [
                "cmake",
                f"-S{base_dir}",
                f"-B{base_dir}/{env['target']}",
                f"-DCMAKE_BUILD_TYPE={cmake_target}",
            ]
            + cmd_args,
        )
    subprocess.call(
        [
            "cmake",
            "--build",
            f"{base_dir}/{env['target']}",
            "--config",
            f"{cmake_target}",
            f"-j{env.GetOption('num_jobs')}",
            "--",
            # "--silent",
        ],
    )
    env.Append(LIBPATH=[env.Dir(f"{base_dir}/{env['target']}/lib/")])


# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

env.Append(
    CPPPATH=[
        "src/",
        "thirdparty/simple-ble/simpleble/include",
        f"thirdparty/simple-ble/simpleble/{env['target']}/export",
    ]
)

if env["platform"] == "macos" or env["platform"] == "ios":
    env.Append(LIBS=["libsimpleble.a"])
elif env["platform"] == "windows":
    env.Append(LIBS=["simpleble.lib"])
elif env["platform"] == "linux":
    env.Append(LIBS=["libdbus-1.so", "libsimpledbus.a", "libsimplebluez.a", "libsimpleble-debug.a"])
    cmake("thirdparty/simple-ble/simpledbus")
    cmake("thirdparty/simple-ble/simplebluez")
elif env["platform"] == "android":
    raise ValueError("Platform android not implemented.")
elif env["platform"] == "javascript":
    raise ValueError("Platform javascript not implemented.")
else:
    raise ValueError(f"Platform not supported: {env['platform']}.")

cmake("thirdparty/simple-ble/simpleble")

sources = Glob("src/**/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        f"addons/blueplug/lib/libblueplug.{env['platform']}.{env['target']}.framework/libblueplug.{env['platform']}.{env['target']}",
        source=sources,
    )
else:
    library = env.SharedLibrary(
        f"addons/blueplug/lib/libblueplug{env['suffix']}{env['SHLIBSUFFIX']}",
        source=sources,
    )

Default(library)
