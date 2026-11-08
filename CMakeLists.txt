cmake_minimum_required(VERSION 3.21)
project(PSn00bSDK-template LANGUAGES C CXX ASM VERSION 1.0.2)

file(GLOB ALL_ENGINE_SCENES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/scenes/*.cpp")

set(ENGINE_SOURCES
    src/game.cpp src/scene.cpp src/registry.cpp src/fx.cpp
    src/render.cpp src/geometry.cpp src/sound.cpp src/pad.cpp
    src/storage/storage.cpp src/storage/scratchpad.cpp
    src/parser/cd.cpp src/parser/memory_card.cpp src/parser/sound.cpp
)

psn00bsdk_add_executable(SLPS_057.31 GPREL src/main.cpp ${ENGINE_SOURCES})
target_include_directories(SLPS_057.31 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_source_files_properties(${ENGINE_SOURCES} PROPERTIES COMPILE_OPTIONS "-fno-function-sections;-fno-data-sections")

target_link_options(SLPS_057.31 PRIVATE 
    "-Wl,--export-dynamic"
    "-Wl,--section-start=.overlay_space=0x800C0000"
    "-Wl,-u,_ZTV5Scene"
    "-Wl,-u,GetTimInfo"
    "-Wl,-u,LoadImage"
    "-Wl,-u,memset"
)

set(OVERLAY_LD "${CMAKE_BINARY_DIR}/overlay.ld")
file(WRITE ${OVERLAY_LD} "
SECTIONS {
    . = 0x800C0000;
    .text : {
        *(.text.start_scene_init)
        *(.text*)
    }
    .rodata : {
        *(.rodata*)
        *(.rdata*)
    }
    .data : {
        *(.data*)
        *(.sdata*)
    }
}
")

set(SCN_LIST "")
foreach(SCENE_PATH ${ALL_ENGINE_SCENES})
    get_filename_component(SCENE_NAME "${SCENE_PATH}" NAME_WE)
    
    if(NOT "${SCENE_PATH}" MATCHES ".*\\.cpp$")
        continue()
    endif()

    set(SCENE_TARGET "target_${SCENE_NAME}")
    
    set(BOOTSTRAP_FILE "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${SCENE_TARGET}.dir/bootstrap_${SCENE_NAME}.cpp")
    
    file(WRITE ${BOOTSTRAP_FILE} "
#include <game.hpp>
#include <scenes/${SCENE_NAME}.hpp>

extern \"C\" {
    __attribute__((section(\".text.start_scene_init\"), noinline)) 
    Scene* start_scene_init(void* ram_buffer, uint32_t engine_gp) {
        __asm__ __volatile__(
            \"move $gp, %0\" 
            : 
            : \"r\"(engine_gp) 
            : \"memory\"
        );
        return ::new (ram_buffer) SCENE_CLASS_NAME();
    }
}
")
    
    add_executable(${SCENE_TARGET} "${SCENE_PATH}" "${BOOTSTRAP_FILE}")
    
    # СВЯЗЫВАЕМ ИМЯ ФАЙЛА И ИМЯ КЛАССА ВНУТРИ НЕГО
    set(ACTUAL_CLASS_NAME "${SCENE_NAME}")
    
    if("${SCENE_NAME}" STREQUAL "test")
        set(ACTUAL_CLASS_NAME "TestScene")
    elseif("${SCENE_NAME}" STREQUAL "loading")
        set(ACTUAL_CLASS_NAME "LoadingScene")
    elseif("${SCENE_NAME}" STREQUAL "menu")
        set(ACTUAL_CLASS_NAME "MenuScene")
    endif()
    
    # Передаем имя класса в компилятор как макрос
    target_compile_definitions(${SCENE_TARGET} PRIVATE "SCENE_CLASS_NAME=${ACTUAL_CLASS_NAME}")
    
    target_include_directories(${SCENE_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_options(${SCENE_TARGET} PRIVATE "-mlong-calls" "-G0" "-mno-gpopt")
    
    target_link_options(${SCENE_TARGET} PRIVATE 
        "-Wl,-R,$<TARGET_FILE:SLPS_057.31>"
        "-Wl,-T,${OVERLAY_LD}"
        "-Wl,--entry=start_scene_init"
        "-Wl,-u,start_scene_init"
        "-Wl,-N"
        "-G0"
    )
    add_dependencies(${SCENE_TARGET} SLPS_057.31)
    set_target_properties(${SCENE_TARGET} PROPERTIES LINK_DEPENDS $<TARGET_FILE:SLPS_057.31>)
    
    # имя выходного файла всегда строго совпадает с именем файла на диске (test.SCN, loading.SCN)
    set(SCN_OUTPUT "${CMAKE_BINARY_DIR}/${SCENE_NAME}.SCN")
    list(APPEND SCN_LIST "${SCN_OUTPUT}")

    add_custom_command(
        OUTPUT "${SCN_OUTPUT}"
        COMMAND ${CMAKE_OBJCOPY} -O binary --only-section=.text* --only-section=.rodata* --only-section=.rdata* --only-section=.data* --only-section=.sdata* --strip-all $<TARGET_FILE:${SCENE_TARGET}> "${SCN_OUTPUT}"
        DEPENDS ${SCENE_TARGET}
        COMMENT "Generating overlay binary for ${SCENE_NAME}"
        VERBATIM
    )
endforeach()

list(REMOVE_DUPLICATES SCN_LIST)

add_custom_target(compile_all_scenes DEPENDS ${SCN_LIST})

psn00bsdk_add_cd_image(iso "Game" iso.xml DEPENDS SLPS_057.31 compile_all_scenes ${SCN_LIST} system.cnf)
