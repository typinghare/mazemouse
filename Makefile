# Copyright 2024 James Chen
#
# Environment requirements:
# - Unix-Like OS
# - g++ >= 3.30.0
# - cmake >=3.30.0
# - ninja >=1.12.0
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

run:
	$(CMAKE_DIR)/$(SIMULATOR_APP)

clean:
	rm -rf $(CMAKE_DIR)