//
// Created by Øistein Sletten Løvik on 22/02/2017.
//

#include <cxwin.h>

int main() {
    cxwin_window* window = cxwin_create_window();

    while (true) {
        cxwin_next_event(window);
    }

    cxwin_terminate(window);
    return EXIT_SUCCESS;
}