#include <stdio.h>
#include <windows.h>
#include "HABeW.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    HABeW_Window* window1 = HABeW_create("window1");
    HABeW_set_title(window1, "window1");

    HABeW_Window* window2 = HABeW_create("window2");
    HABeW_set_title(window2, "window2");

    HABeW_show(window1);
    HABeW_show(window2);

    HABeM_Vector2 size = {1024, 720};
    HABeW_set_size(window1, size);

    printf("Press ENTER to hide window 2...\n");
    getchar();

    HABeW_hide(window2);

    printf("Press ENTER to show the window 2...\n");
    getchar();

    HABeW_show(window2);

    printf("Press ENTER to end the demo...\n");
    getchar();

    HABeW_free(window1);
    HABeW_free(window2);

    return 0;
}
