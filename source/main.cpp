#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <tgmath.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

int oob = 0;
int count = 0;

// Recursive function to provide the iterative every 100th
// f^n (0) for every pixel on the screen.
int Mandle(double _Complex c, double _Complex t = 0, int counter = 0) {
    // To eliminate out of bound values.
    if (cabs(t) > 4) {
        C2D_DrawLine(creal(c) * SCREEN_HEIGHT / 2 + SCREEN_WIDTH / 2,
                     cimag(c) * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2,
                     C2D_Color32(0, 255, 255, 255),
//                     C2D_Color32(128 - 128 * cabs(t) / cabs(c),
//                           128 - 128 * cabs(t) / cabs(c),
//                           128 - 128 * cabs(t) / cabs(c),
//                           255),
                     C2D_Color32(0, 255, 255, 255),
                     creal(c) * SCREEN_HEIGHT / 2 + SCREEN_WIDTH / 2,
                     cimag(c) * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2,
//                C2D_Color32(128 - 128 * cabs(t) / cabs(c),
//                           128 - 128 * cabs(t) / cabs(c),
//                           128 - 128 * cabs(t) / cabs(c),
//                           255),
                           1, 1);

        oob++;
        return 0;
    }

    // To put about the end of the fractal,
    // the higher the value of the counter,
    // The more accurate the fractal is generated,
    // however, higher values cause
    // more processing time.
    if (counter == 1) {
        C2D_DrawLine(creal(c) * SCREEN_HEIGHT / 2 + SCREEN_WIDTH / 2,
                     cimag(c) * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2,
//                     C2D_Color32(255 * (cabs((t * t)) / cabs((t - c) * c)), 0, 0, 255),
                     C2D_Color32(255, 255, 255, 255),
                     creal(c) * SCREEN_HEIGHT / 2 + SCREEN_WIDTH / 2,
                     cimag(c) * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2,
//                     C2D_Color32(255 * (cabs((t * t)) / cabs((t - c) * c)), 0, 0, 255),
                     C2D_Color32(255, 255, 255, 255),
                     1, 1);

        count++;
        return 0;
    }

    // printf("\x1b[1;1HDebug: %zu/%zu\x1b[K", oob, count);

    // recursively calling Mandle with increased counter
    // and passing the value of the squares of t into it.
    Mandle(c, cpow(t, 2) + c, counter + 1);

    return 0;
}

// Calling Mandle function for every point on the screen.
int MandleSet() {

    for (double x = -2; x < 2; x += 0.0015) {
        for (double y = -1; y < 1; y += 0.0015) {
            double _Complex temp = x + y * _Complex_I;
            printf("\x1b[16;1HDebug: %zu/%zu\x1b[K", oob, count);
            Mandle(temp);
        }
    }
    return 0;
}

// recursion machine go brrrrrrr
int draw(int x, int y, int side, int num) {
    int sub = side / 3;

    C2D_DrawRectSolid (x + sub, y + sub, 1, sub, sub, C2D_Color32(255, 255, 255, 255));

    count ++;

    if (sub >= 3) {
        // Draw 8 surrounding squares
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (j != 1 || i != 1)
                    draw(x + i * sub, y + j * sub, sub, num++);
            }
        }
    }

    return 0;
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
    printf("1. initializing... "); // this line doesn't print but i have it here anyways
    // Init libs
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, NULL);

    printf("initialized!\n");


    // Create screens
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    printf("2. created screen\n");

    // Main loop
    while (aptMainLoop()) {
        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break; // break in order to return to hbmenu

        // printf("\x1b[16;1HDebug: %zu/%zu\x1b[K", oob, count);


        // Render the scene
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
        C2D_SceneBegin(top);

        // MandleSet();
        draw(0, 0, 243, 0);

        C3D_FrameEnd(0);
    }

    // Deinit libs
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
