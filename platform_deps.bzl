"""Platform-specific dependencies loading"""

def _is_macos(repository_ctx):
    """Check if the current platform is macOS"""
    return repository_ctx.os.name.lower().find("mac") != -1

def _macos_deps_impl(repository_ctx):
    http_archive(
        name = "build_bazel_rules_swift",
        sha256 = "15f7096b41154393da81594909e2db3f5828a5e671b8d873c35788d82f9c97d2",
        url = "https://github.com/bazelbuild/rules_swift/releases/download/2.7.0/rules_swift.2.7.0.tar.gz",
    )

    http_archive(
        name = "rules_xcodeproj",
        integrity = "sha256-hHsx77jVC7+ArceeTRIsc2Ym1oqDYiVNNakGW789Z/Q=",
        url = "https://github.com/MobileNativeFoundation/rules_xcodeproj/releases/download/2.10.0/release.tar.gz",
    )

    load(
        "@rules_xcodeproj//xcodeproj:repositories.bzl",
        "xcodeproj_rules_dependencies",
    )

    xcodeproj_rules_dependencies()

    load("@bazel_features//:deps.bzl", "bazel_features_deps")

    bazel_features_deps()

    load(
        "@build_bazel_rules_apple//apple:repositories.bzl",
        "apple_rules_dependencies",
    )

    apple_rules_dependencies()

    load(
        "@build_bazel_rules_swift//swift:repositories.bzl",
        "swift_rules_dependencies",
    )

    swift_rules_dependencies()

    load(
        "@build_bazel_rules_swift//swift:extras.bzl",
        "swift_rules_extra_dependencies",
    )

    swift_rules_extra_dependencies()

    load(
        "@build_bazel_apple_support//lib:repositories.bzl",
        "apple_support_dependencies",
    )

    apple_support_dependencies()


macos_deps = repository_rule(
    implementation = _macos_deps_impl,
    local = True,
)
