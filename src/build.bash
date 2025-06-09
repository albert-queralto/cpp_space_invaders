g++ src/main.cpp \
    src/spaceship.cpp \
    src/game.cpp \
    src/laser.cpp \
    src/obstacle.cpp \
    src/block.cpp \
    src/alien.cpp \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -o main