load("@bazel_skylib//rules:build_test.bzl", "build_test")
load(
    "@build_bazel_rules_apple//apple:macos.bzl",
    "macos_application",
)
load(
    "@build_bazel_rules_apple//apple:versioning.bzl",
    "apple_bundle_version",
)
load(
    "@build_bazel_rules_swift//swift:swift.bzl",
    "swift_library",
)
load(
    "@rules_xcodeproj//xcodeproj:defs.bzl",
    "top_level_target",
    "xcodeproj",
)

licenses(["notice"])

swift_library(
    name = "sources",
    srcs = glob(["**/*.swift"]),
    #    data = [
    #        "Resources/Main.storyboard",
    #    ],
    copts = ["-cxx-interoperability-mode=default"],
    deps = ["//core"],
)

apple_bundle_version(
    name = "ciart_studio_version",
    build_version = "1.0",
)

macos_application(
    name = "app",
    bundle_name = "CiartStudio",
    #    app_icons = ["Resources:MacAppIcon.xcassets"],
    bundle_id = "com.ciart.studio",
    infoplists = [":Info.plist"],
    minimum_os_version = "12.0",
    version = ":ciart_studio_version",
    visibility = ["//visibility:public"],
    deps = [":sources"],
)

xcodeproj(
   name = "xcodeproj",
   project_name = "CiartStudio",
   tags = ["manual"],
   top_level_targets = [
       top_level_target(
           ":app",
           target_environments = ["device"],
       ),
   ],
)

# Not normally needed, just done for rules_apple's examples so a
# 'bazel test examples/...' ensures all Examples still build.
build_test(
    name = "ExamplesBuildTest",
    targets = [":app"],
)
