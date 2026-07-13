BUILD_DIR = build

all: build

configure:
	conan install . --output-folder=$(BUILD_DIR) --build=missing

build_release: configure
	cmake -G Ninja -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(BUILD_DIR)/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
	cmake --build $(BUILD_DIR)

build_debug: configure
	cmake -G Ninja -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(BUILD_DIR)/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
	cmake --build $(BUILD_DIR)

test: build_debug
	ctest --output-on-failure
	ctest --output-on-failure --test-dir build

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all configure build clean

