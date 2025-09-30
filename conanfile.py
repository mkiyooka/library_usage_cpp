from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain


class TemplateCLICppConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("cli11/2.5.0")  # CLI parsing library
        self.requires("fmt/11.2.0")  # Formatting library
        self.requires("tomlplusplus/3.4.0")  # TOML configuration library
        self.requires("nlohmann_json/3.12.0")  # JSON library
        self.requires("yyjson/0.10.0")  # JSON library
        # self.requires("glaze/5.4.1")  # JSON library C++23
        self.requires("indicators/2.3")  # Progress bar library
        self.requires("quill/10.0.1")  # Logging library
        self.requires("nanobench/4.3.11")  # Benchmarking library
        self.requires("doctest/2.4.11")  # Testing framework

        # OS に応じて cppzmq/zeromq を有効化
        if self.settings.os == "Linux":
            self.requires("cppzmq/4.11.0")  # C++ binding for ZeroMQ (using FetchContent)
            # self.requires("zeromq/4.3.5")  # ZeroMQ library (using FetchContent)

    def layout(self):
        # ルートディレクトリのbuildフォルダを使用
        self.folders.build = "."
        self.folders.generators = "generators"

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        # CMakeUserPresets.jsonの生成を無効化
        tc.user_presets_path = False
        tc.generate()

    def configure(self):
        pass
