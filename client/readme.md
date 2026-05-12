# Desktop сборка

```bash
mkdir build
cmake --build build 
```
# Emscripten сборка
## Предварительно
```bash
sudo apt install ninja-build

git clone https://github.com/conan-io/conan-toolchains.git
conan remote add conan-toolchains ./conan-toolchains
```
## Сборка

```bash

export gl_cv_header_sys_random_h=no

conan install . -pr:h client/profiles/emscripten-wasm -pr:b default -of build/wasm --build=missing

cmake -S . -B build/wasm -DCMAKE_TOOLCHAIN_FILE=build/wasm/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/wasm --parallel

```

```bash

возможно будет нужно
# grep -rn "No getxattr implementation" /home/fasol/.conan2/p/b/glibda62ce7801663/b/ --include="meson.build"
# /home/fasol/.conan2/p/b/glibda62ce7801663/b/src/meson.build:2401:    error('No getxattr implementation found in C library or libxattr')
# fasol@fasol-B760M-GAMING-X-DDR4:~/biom/Biome$ sed -i '2401s/error/# error/' /home/fasol/.conan2/p/b/glibda62ce7801663/b/src/meson.build

```