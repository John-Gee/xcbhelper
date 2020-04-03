#include <xcb/xcb.h>

#ifndef XBCHELPER
#define XBCHELPER

class XCBHelper {
    xcb_connection_t *m_conn;
    xcb_window_t m_window;

    // Open connection to the server
    xcb_connection_t *Connect(
        const char *display_name,
        int *screen_p);

    // Disconnect from the server
    void Disconnect();

    // Get the screen by screen number
    xcb_screen_t *GetScreen(
        int screen_number);

    // Get the atom by name
    xcb_atom_t GetAtom(
        const char *name);

    // Get the window's name
    const char *GetWindowName(
        xcb_window_t window,
        xcb_atom_t net_name,
        const int maxWindows);

    // Get the window by name
    xcb_window_t GetWindowByName (
        xcb_screen_t *screen,
        const char *required_name);

public:
    XCBHelper(
        const char *display_name,
        int *screen_p,
        int screen_number,
        const char* window_name);

    ~XCBHelper();

    // Send a notification a mouse button was pressed or released
    void Button(
        bool press);

    // Warp the actual pointer on screen
    void WarpPointer(
        int x,
        int y);

    // Send a notification the pointer moved
    void NotifyMovePointer(
        int16_t x,
        int16_t y);
};

#endif
