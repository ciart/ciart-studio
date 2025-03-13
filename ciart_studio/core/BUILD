load(
    "@build_bazel_rules_swift//swift:swift.bzl",
    "swift_interop_hint",
)

cc_library(
    name = "core",
    srcs = [
        "workspace.cpp",
    ],
    hdrs = [
        "workspace.h",
    ],
    aspect_hints = [":core_swift_interop"],
    visibility = ["//visibility:public"],
    deps = [
        "@skia//:core",
        "@skia//:png_encode_codec",
        "@skia//:skshaper_core",
        "@skia//:svg_renderer",
    ] + select({
        "@platforms//os:linux": ["@skia//:fontmgr_fontconfig_freetype"],
        "@platforms//os:macos": ["@skia//:fontmgr_coretext"],
        "//conditions:default": [],
    }),
)

swift_interop_hint(
    name = "core_swift_interop",
    module_name = "CiartStudioCore",
)
