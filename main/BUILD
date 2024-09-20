cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cpp"],
    copts = [
        "-Iexternal/skia/include/core",
    ],
    #    includes = [
    #        "skia/include/core",
    #    ],
    #    linkopts = [
    #        "-lpthread",
    #    ],
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
