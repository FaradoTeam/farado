project(biome-client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

qt_add_resources(${PROJECT_NAME} "qml_resources"
    PREFIX "/"
    FILES
        qml/pages/main.qml
        qml/pages/AuthPage.qml
        qml/components/GearButton.qml
        qml/components/ButtonGroup.qml
        qml/theme/Theme.qml
        qml/theme/qmldir
)

if(EMSCRIPTEN)
    message(STATUS "Configuring ${PROJECT_NAME} for WebAssembly")

    # Используем уже найденный Qt (Conan через toolchain покажет путь)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml QuickControls2)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core Qt6::Gui Qt6::Quick Qt6::Qml Qt6::QuickControls2
    )
    
    # Многопоточность: включаем поддержку pthreads в Emscripten
    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_PTHREADS=1
        # Дополнительно помогает избежать блокировки главного потока
        -sPROXY_TO_PTHREAD=1
        # Если нужен рост памяти (по необходимости)
        -sALLOW_MEMORY_GROWTH=1
        -sINITIAL_MEMORY=64MB
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/wasm_out
        SUFFIX ".html"
    )
else()
    find_package(Qt6 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE qt::qt)
    qt_import_qml_plugins(${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/client
    )
endif()