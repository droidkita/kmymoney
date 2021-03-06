include(ECMInstallIcons)

file(GLOB_RECURSE KMYMONEY_ICONS "kmymoney/*.png")
ecm_install_icons(
  ICONS ${KMYMONEY_ICONS}
  DESTINATION ${ICON_INSTALL_DIR}
)

set (icons_SOURCES
  icons.cpp
)

# This is only needed for Linux/BSD, unless debugging.
# For Windows and macOS, craft automatically sources upstream breeze icons
# into icontheme.rcc and bundles itp with the package.
# More here: https://api.kde.org/frameworks/kiconthemes/html/index.html
if((NOT WIN32 AND NOT APPLE) OR CMAKE_BUILD_TYPE MATCHES Debug)
    # Generate icons.qrc file:
    set(qrcContents "<RCC>\n  <qresource")
    string(APPEND qrcContents " prefix=\"/icons\"")
    string(APPEND qrcContents ">\n")

    file(GLOB_RECURSE icon_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "breeze/*" "breeze-dark/*")
    foreach(icon_file ${icon_files})
        string(APPEND qrcContents "    <file>${icon_file}</file>\n")
    endforeach()
    string(APPEND qrcContents "  </qresource>\n</RCC>\n")

    file(WRITE "icons.qrc" "${qrcContents}")

    # add generated file to resources
    qt5_add_resources(icons_SOURCES icons.qrc)
endif()

add_library(kmm_icons SHARED ${icons_SOURCES})

generate_export_header(kmm_icons)
target_link_libraries(kmm_icons PRIVATE Qt5::Core Qt5::Gui)

set_target_properties(kmm_icons PROPERTIES
  VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS kmm_icons
  ${INSTALL_TARGETS_DEFAULT_ARGS} )
