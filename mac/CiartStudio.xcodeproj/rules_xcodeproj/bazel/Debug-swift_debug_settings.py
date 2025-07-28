#!/usr/bin/python3

"""An lldb module that registers a stop hook to set swift settings."""

import lldb
import re

# Order matters, it needs to be from the most nested to the least
_BUNDLE_EXTENSIONS = [
    ".framework",
    ".xctest",
    ".appex",
    ".bundle",
    ".app",
]

_TRIPLE_MATCH = re.compile(r"([^-]+-[^-]+)(-\D+)[^-]*(-.*)?")

_SETTINGS = {
    "arm64-apple-macosx CiartStudio.app/Contents/MacOS/CiartStudio": {
        "c": "-iquote$(PROJECT_DIR) -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin -iquote$(BAZEL_EXTERNAL)/skia -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia -iquote$(BAZEL_EXTERNAL)/skia_user_config -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia_user_config -iquote$(BAZEL_EXTERNAL)/libpng -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/libpng -iquote$(BAZEL_EXTERNAL)/zlib_skia -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/zlib_skia -iquote$(BAZEL_EXTERNAL)/libjpeg_turbo -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/libjpeg_turbo -iquote$(BAZEL_EXTERNAL)/expat -iquote$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/expat -isystem$(BAZEL_EXTERNAL)/libpng -isystem$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/libpng -isystem$(BAZEL_EXTERNAL)/expat/expat/lib -isystem$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/expat/expat/lib -isystem$(BAZEL_EXTERNAL)/expat/include -isystem$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/expat/include -DSK_USE_BAZEL_CONFIG_HEADER -DSK_GRAPHITE -DSK_METAL -DSK_CODEC_ENCODES_PNG -DSK_CODEC_ENCODES_PNG_WITH_LIBPNG -DSK_DISABLE_LEGACY_SKSHAPER_FUNCTIONS -DSK_CODEC_DECODES_JPEG -DSK_CODEC_DECODES_PNG -DSK_CODEC_DECODES_PNG_WITH_LIBPNG -DXML_STATIC -DSK_XML -DSK_ENABLE_SVG -DSK_FONTMGR_CORETEXT_AVAILABLE -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia/src/gpu/mtl/gpu_mtl_modulemap/_/module.modulemap -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia/src/gpu/graphite/mtl/graphite_native_metal_modulemap/_/module.modulemap -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia/src/utils/mac/mac_utils_modulemap/_/module.modulemap -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia/src/utils/mac/mac_utils_priv_modulemap/_/module.modulemap -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia/src/ports/coretext_typeface_modulemap/_/module.modulemap -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/external/skia/src/ports/fontmgr_coretext_modulemap/_/module.modulemap -fmodule-map-file=$(BAZEL_OUT)/darwin_arm64-dbg-macos-arm64-min15.0-applebin_macos-ST-b07f349e7fd8/bin/core/core_modulemap/_/module.modulemap -std=c++20 -O0 -DDEBUG=1 -fstack-protector -fstack-protector-all",
    },
}

def __lldb_init_module(debugger, _internal_dict):
    # Register the stop hook when this module is loaded in lldb
    ci = debugger.GetCommandInterpreter()
    res = lldb.SBCommandReturnObject()
    ci.HandleCommand(
        "target stop-hook add -P swift_debug_settings.StopHook",
        res,
    )
    if not res.Succeeded():
        print(f"""\
Failed to register Swift debug options stop hook:

{res.GetError()}
Please file a bug report here: \
https://github.com/MobileNativeFoundation/rules_xcodeproj/issues/new?template=bug.md
""")
        return

def _get_relative_executable_path(module):
    for extension in _BUNDLE_EXTENSIONS:
        prefix, _, suffix = module.rpartition(extension)
        if prefix:
            return prefix.split("/")[-1] + extension + suffix
    return module.split("/")[-1]

class StopHook:
    "An lldb stop hook class, that sets swift settings for the current module."

    def __init__(self, _target, _extra_args, _internal_dict):
        pass

    def handle_stop(self, exe_ctx, _stream):
        "Method that is called when the user stops in lldb."
        module = exe_ctx.frame.module
        if not module:
            return

        module_name = module.file.GetDirectory() + "/" + module.file.GetFilename()
        versionless_triple = _TRIPLE_MATCH.sub(r"\1\2\3", module.GetTriple())
        executable_path = _get_relative_executable_path(module_name)
        key = f"{versionless_triple} {executable_path}"

        settings = _SETTINGS.get(key)

        if settings:
            frameworks = " ".join([
                f'"{path}"'
                for path in settings.get("f", [])
            ])
            if frameworks:
                lldb.debugger.HandleCommand(
                    f"settings set -- target.swift-framework-search-paths {frameworks}",
                )
            else:
                lldb.debugger.HandleCommand(
                    "settings clear target.swift-framework-search-paths",
                )

            includes = " ".join([
                f'"{path}"'
                for path in settings.get("s", [])
            ])
            if includes:
                lldb.debugger.HandleCommand(
                    f"settings set -- target.swift-module-search-paths {includes}",
                )
            else:
                lldb.debugger.HandleCommand(
                    "settings clear target.swift-module-search-paths",
                )

            clang = settings.get("c")
            if clang:
                lldb.debugger.HandleCommand(
                    f"settings set -- target.swift-extra-clang-flags '{clang}'",
                )
            else:
                lldb.debugger.HandleCommand(
                    "settings clear target.swift-extra-clang-flags",
                )

        return True
