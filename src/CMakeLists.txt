cmake_minimum_required(VERSION 3.7)

add_executable(life main.c game.c window.c pattern.c menu.c)
file(COPY patterns DESTINATION ../)
target_compile_definitions(life PUBLIC DEBUG)
target_compile_options(life PRIVATE -fsanitize=address)
target_link_options(life PRIVATE -fsanitize=address -lncurses -lSDL2_ttf -lSDL2)
target_compile_options(life PRIVATE -std=c99 -Werror -Wall -Wextra -Wvla -g)
