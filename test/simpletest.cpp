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
  
    xcb_connection_t *conn = Connect(display_name, screen_p);
    xcb_screen_t *screen   = GetScreen(conn, screen_number);
    xcb_window_t window    = GetWindowByName(conn, screen, window_name);
    
    if (window)
        printf("Found window!\n");
    NotifyMovePointer(conn, window, 100, 200);
    // Button Press
    Button(conn, window, true);
    NotifyMovePointer(conn, window, 300, 600);
    // Button Release
    Button(conn, window, false);

    // Disconnect
    Disconnect(conn);

    return 0;
}
