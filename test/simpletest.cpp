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
    // Left Button Press
    helper->PressLeftButton();
    helper->NotifyMovePointer(300, 600);
    // Left Button Release
    helper->ReleaseLeftButton();
    Dimensions *dimensions = helper->GetWindowDimensions();
    printf("dimensions: %i x %i\n", dimensions->width, dimensions->height);
    free(dimensions);
    free(helper);

    return 0;
}
