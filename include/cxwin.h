//
// Created by Øistein Sletten Løvik on 22/02/2017.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CXWIN_alloc
#include <stdlib.h>
#define CXWIN_FREE(x) (free(x))
#define CXWIN_CALLOC(x) (calloc(1, sizeof(x)))
#endif

#ifdef __APPLE__
    #define CXWIN_APPLE 1
#elif __linux__
    #define CXWIN_LINUX 1
#elif _WIN32
    #define CXWIN_WINDOWS 1
#endif

typedef struct cxwin_window cxwin_window;

cxwin_window* cxwin_create_window();
void cxwin_terminate(cxwin_window* win);

#pragma region macos
#if defined(__APPLE__)

#include <assert.h>

struct cxwin_window {
    uint16_t width;
    uint16_t height;
};

cxwin_window* cxwin_create_window() {

    cxwin_window* win = CXWIN_CALLOC(sizeof(cxwin_window));

    return win;
}

void cxwin_terminate(cxwin_window* win) {
    assert(win);

    CXWIN_FREE(win);
}

#endif
#pragma endregion macos

#ifdef __cplusplus
}
#endif
