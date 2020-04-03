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
  
    XCBHelper *helper   = new XCBHelper(display_name, screen_p, screen_number, window_name);
    helper->NotifyMovePointer(100, 200);
    // Button Press
    helper->Button(true);
    helper->NotifyMovePointer(300, 600);
    // Button Release
    helper->Button(false);
    free(helper);

    return 0;
}
