file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    src/*.c
    src/*.h
    include/*.h
)

add_executable(image-transformer ${sources} "include/bmp.h" "src/rotation.c" "include/image.h" "include/rotation.h" "include/file_io.h" "src/bmp.c" "src/file_io.c" "src/utils.c")
target_include_directories(image-transformer PRIVATE src include)
