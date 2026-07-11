# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.21)

project(
	PSn00bSDK-template
	LANGUAGES    C CXX ASM
	VERSION      1.0.2
	DESCRIPTION  "PSn00bSDK template"
	HOMEPAGE_URL "http://lameguy64.net/?page=psn00bsdk"
)

psn00bsdk_add_executable(SLPS_057.31 GPREL 
	src/main.cpp
	src/game.cpp

	src/scene.cpp
	src/registry.cpp
	src/scenes/LoadScene.cpp
	src/scenes/MenuScene.cpp
	src/scenes/test.cpp
	src/fx.cpp

	src/render.cpp
	src/geometry.cpp

	src/sound.cpp
	src/pad.cpp

	src/storage/storage.cpp
	src/storage/scratchpad.cpp

	src/parser/cd.cpp

)

target_include_directories(SLPS_057.31 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

psn00bsdk_add_cd_image(
	iso      # Target name
	"Game" # Output file name (= template.bin + template.cue)
	iso.xml  # Path to config file
	DEPENDS SLPS_057.31 system.cnf
)
