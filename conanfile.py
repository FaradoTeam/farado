from conan import ConanFile
from conan.tools.cmake import cmake_layout

class BiomeConanFile(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def configure(self):
        # Для Wasm Qt должен быть статическим и без утилит (они в build-профиле)
        if self.settings.os == "Emscripten":
            self.options["qt/*"].shared = False
            self.options["qt/*"].qttools = False
            self.options["qt/*"].with_pq = False
            self.options["qt/*"].with_md4c = False
            self.options["qt/*"].with_glib = False
            self.options["qt/*"].with_opengl = "no"
            self.options["qt/*"].with_wayland = False
            self.options["qt/*"].with_vulkan = False
            self.options["qt/*"].with_icu = False
            # Принудительно отключаем glib везде, где можно
            self.options["harfbuzz/*"].with_glib = False
            # glib нам не нужен, но если кто-то потянет — запрещаем его сборку (заглушка)
            # Позже, если потребуется, придется заменить рецепт glib на glib-emscripten или пропатчить
            self.options["glib/*"].with_mount = False
        else:
            # Нативные сборки используют shared Qt
            self.options["qt/*"].shared = True
            self.options["qt/*"].qttools = True

        # Опции для QML/Quick – нужны везде, где есть GUI
        self.options["qt/*"].qtdeclarative = True
        self.options["qt/*"].qtquickcontrols2 = True
        self.options["qt/*"].qtshadertools = True

        # libpq shared – только для нативных серверных сборок
        if self.settings.os != "Emscripten":
            self.options["libpq/*"].shared = True

    def build_requirements(self):
        self.tool_requires("m4/1.4.19")

    def requirements(self):
        # Общие зависимости
        self.requires("m4/1.4.19", visible=False, force=True, run=False)
        self.requires("qt/6.10.1")
        self.requires("zlib/1.3.1")

        # Серверные / нативные зависимости не нужны под Wasm
        if self.settings.os != "Emscripten":
            self.requires("boost/1.83.0")
            self.requires("nlohmann_json/3.11.3")
            self.requires("sqlite3/3.51.3")
            self.requires("openssl/3.6.2")
            self.requires("cpprestsdk/2.10.19")
            self.requires("websocketpp/0.8.2")
            self.requires("jwt-cpp/0.7.2")
            self.requires("libpq/17.7")
        # else:
        #     # Если glib всё равно нужен (например, через pcre2), то придется добавить порт:
        #     # self.requires("glib-emscripten/2.75.0")
        #     # Но сначала попробуй без него.

    def layout(self):
        cmake_layout(self)