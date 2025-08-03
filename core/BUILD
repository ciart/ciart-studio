load(
    "@build_bazel_rules_swift//swift:swift.bzl",
    "swift_interop_hint",
)

cc_library(
    name = "core",
    srcs = glob(["**/*.cpp"]),
    hdrs = glob(["**/*.h"]),
    aspect_hints = select({
        "@platforms//os:macos": [":core_swift_interop"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "@skia//:core",
        "@skia//:png_encode_codec",
        "@skia//:skshaper_core",
        "@skia//:svg_renderer",
    ] + select({
        "@platforms//os:windows": ["@rules_vulkan//vulkan:vulkan_cc_library", "@skia//:graphite_native_vulkan"],
        "@platforms//os:linux": ["@skia//:graphite_native_metal", "@skia//:fontmgr_fontconfig_freetype"],
        "@platforms//os:macos": ["@skia//:fontmgr_coretext"],
        "//conditions:default": [],
    }),
)

swift_interop_hint(
    name = "core_swift_interop",
    module_name = "CiartStudioCore",
)
