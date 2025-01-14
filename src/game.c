#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "sprite.h"

int main() {
    Engine engine;
    if (!engine_init(&engine, 800, 600, "HABe")) {
        fprintf(stderr, "Failed to initialize engine.\n");
        return EXIT_FAILURE;
    }

    Sprite testSprite = create_sprite(
        0.0f, 
        0.0f, 
        0.0f,
        100.0f, 
        100.0f, 
        0.0f, 
        ""
    );

    if (engine_add_sprite(&engine, &testSprite) < 0) {
        fprintf(stderr, "Failed to add sprite to engine.\n");
    }

    engine_run(&engine);
    engine_shutdown(&engine);
    return EXIT_SUCCESS;
}
