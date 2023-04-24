#!/usr/bin/env python

import os
import subprocess


def cmake(base_dir, cmd_args=[]):
    cmake_target = "Debug"
    if env["target"] == "template_release":
        cmake_target = "Release"
    build_dir = f"{base_dir}/build{env['suffix']}"

    if not os.path.isdir(build_dir):
        _cmd_args = [] + cmd_args
        if env["platform"] == "macos":
            _cmd_args.append("-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64")
        subprocess.call(
            [
                "cmake",
                f"-S{base_dir}",
                f"-B{build_dir}",
                f"-DCMAKE_BUILD_TYPE={cmake_target}",
            ]
            + _cmd_args,
        )
    subprocess.call(
        [
            "cmake",
            "--build",
            f"{build_dir}",
            "--config",
            f"{cmake_target}",
            f"-j{env.GetOption('num_jobs')}",
            "--",
            "--silent",
        ],
    )
    env.Append(LIBPATH=[env.Dir(f"{build_dir}/lib/")])


# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

env = SConscript("thirdparty/godot-cpp/SConstruct")

env.Append(
    CPPPATH=[
        "src/",
        "thirdparty/simple-ble/simpleble/include",
        f"thirdparty/simple-ble/simpleble/build{env['suffix']}/export",
    ]
)

if env["platform"] == "macos" or env["platform"] == "ios":
    env.Append(LIBS=["libsimpleble.a"])
    cmake("thirdparty/simple-ble/simpleble")

elif env["platform"] == "windows":
    env.Append(LIBS=["simpleble.lib"])
    cmake("thirdparty/simple-ble/simpleble")

elif env["platform"] == "linux":
    env.Append(
        LIBS=[
            "libdbus-1.so",
            "libsimpledbus.a",
            "libsimplebluez.a",
            f"libsimpleble{ '' if env['target'] == 'template_release' else '-debug'}.a",
        ]
    )
    cmake("thirdparty/simple-ble/simpledbus")
    cmake("thirdparty/simple-ble/simplebluez")
    cmake("thirdparty/simple-ble/simpleble")

elif env["platform"] == "android":
    raise ValueError("Platform android not implemented.")

elif env["platform"] == "javascript":
    # cmake("thirdparty/simple-ble/simpleble", ["-DSIMPLEBLE_PLAIN=true"])
    raise ValueError("Platform javascript not implemented.")

else:
    raise ValueError(f"Platform not supported: {env['platform']}.")


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
