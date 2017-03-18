configure_file(Doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
configure_file(footer.html "${CMAKE_CURRENT_BINARY_DIR}/footer.html")
configure_file(header.html "${CMAKE_CURRENT_BINARY_DIR}/header.html")
configure_file(stylesheet.css "${CMAKE_CURRENT_BINARY_DIR}/stylesheet.css")

add_custom_target(doc
    COMMAND 
        ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT
        "Generating documentation."
    SOURCES
        Doxyfile
        footer.html
        header.html
        stylesheet.css
    )
