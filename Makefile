# Copyright 2024 James Chen
#
# Environment requirements:
# - Unix-Like OS
# - g++ >= 3.30.0
# - cmake >=3.30.0
# - ninja >=1.12.0
# - SFML >= 2.6.1
#
# Setup CPATH and LIBRARY_PATH so that the compiler can find the header files
# and source files of installed third-libraries.
#
CMAKE = cmake
CMAKE_MAKE_PROGRAM = ninja
CMAKE_DIR = cmake-build-debug
CMAKE_OPTS = -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_MAKE_PROGRAM=$(CMAKE_MAKE_PROGRAM) \
	-G Ninja \
	-S . \
	-B $(CMAKE_DIR)
SIMULATOR_APP = mazemouse_simulator

all: $(SIMULATOR_APP)

$(SIMULATOR_APP):
	$(CMAKE) $(CMAKE_OPTS)
	$(CMAKE) --build $(CMAKE_DIR) --target $(SIMULATOR_APP)

run: $(SIMULATOR_APP)
	cd $(CMAKE_DIR) || exit 1 && ./$(SIMULATOR_APP)

clean:
	rm -rf $(CMAKE_DIR)