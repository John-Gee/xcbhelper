#include <stdio.h>
#include <stdlib.h>
#include "../src/xcbhelper.h"

int main(int argc, char **argv)
{
    if (argc <= 1) {
        printf("No window name passed\n");
        exit(0);
    }
    const char *window_name   = argv[1];
    const char *display_name  = NULL;
    int        *screen_p      = NULL;
    const int   screen_number = 0;
  
    XCBHelper    *helper   = new XCBHelper(display_name, screen_p);
    xcb_screen_t *screen   = helper->GetScreen(screen_number);
    xcb_window_t window    = helper->GetWindowByName(screen, window_name);
    
    if (window)
        printf("Found window!\n");
    helper->NotifyMovePointer(window, 100, 200);
    // Button Press
    helper->Button(window, true);
    helper->NotifyMovePointer(window, 300, 600);
    // Button Release
    helper->Button(window, false);

    free(helper);

    return 0;
}
