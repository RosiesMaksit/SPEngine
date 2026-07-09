#include <game.hpp>


void main() {
    Game game;
    if (!game.Initialize()) {
        game.Run();
    }

}