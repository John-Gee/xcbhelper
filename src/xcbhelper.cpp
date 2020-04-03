#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xcbhelper.h"


// Open connection to the server
xcb_connection_t *Connect(
    const char *display_name,
    int *screen_p)
{
    xcb_connection_t *conn = xcb_connect(display_name, screen_p);
    if (xcb_connection_has_error(conn)) {
        printf("Error opening display\n");
        exit(1);
    }
    return conn;
}


// Disconnect from the server
void Disconnect(
    xcb_connection_t *conn)
{
    xcb_disconnect(conn);
}

// Get the screen by screen number
xcb_screen_t *GetScreen(
    xcb_connection_t *conn,
    int              screen_number)
{
    const xcb_setup_t *setup = xcb_get_setup(conn);
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(setup);
    if (xcb_setup_roots_length(setup) <= screen_number) {
        printf("Wrong number of screens\n");
        exit(1);
    }
    for(int i = 0; i < screen_number; i++)
        xcb_screen_next(&screen_iter);
    return screen_iter.data;
}

// Get the atom by name
xcb_atom_t GetAtom(
    xcb_connection_t *conn,
    const char *name)
{
    xcb_intern_atom_cookie_t atom_cookie = xcb_intern_atom(conn, false, strlen(name), name);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, atom_cookie, NULL);
    if (reply)
        return reply->atom;
    printf("Error getting the atom\n");
    exit(1);
}

// Get the window's name
const char *GetWindowName(
    xcb_connection_t *conn,
    xcb_window_t window,
    xcb_atom_t net_name,
    const int maxWindows)
{
    xcb_get_property_cookie_t prop_cookie_name = xcb_get_property(conn, false, window, net_name, XCB_GET_PROPERTY_TYPE_ANY, 0, maxWindows);
    xcb_get_property_reply_t *reply_prop_name = xcb_get_property_reply(conn, prop_cookie_name, NULL);
    if (!reply_prop_name)
        return NULL;
    int length2 = xcb_get_property_value_length(reply_prop_name);
    if (!length2)
        return NULL;
    return (const char *) xcb_get_property_value(reply_prop_name);
}

// Get the window by name
xcb_window_t GetWindowByName (
    xcb_connection_t *conn,
    xcb_screen_t *screen,
    const char *required_name)
{
    // Get the window
    xcb_atom_t net_list = GetAtom(conn, "_NET_CLIENT_LIST");
    xcb_atom_t net_name = GetAtom(conn, "_NET_WM_NAME");

    const int maxWindows = 1000;
    xcb_get_property_cookie_t prop_cookie_list = xcb_get_property(conn, false, screen->root, net_list, XCB_GET_PROPERTY_TYPE_ANY, 0, maxWindows);
    xcb_get_property_reply_t *reply_prop_list = xcb_get_property_reply(conn, prop_cookie_list, NULL);
    if (!reply_prop_list)
        return (xcb_window_t) NULL;
    int length = xcb_get_property_value_length(reply_prop_list);
    if (!length)
        return (xcb_window_t) NULL;
    xcb_window_t* windows = (xcb_window_t*) xcb_get_property_value(reply_prop_list);
    for (int i = 0; i < length; i++) {
        const char *name = GetWindowName(conn, windows[i], net_name, maxWindows);
        if (!name)
            continue;
        if (strcmp(name, required_name) == 0)
            return windows[i];
    }
    return (xcb_window_t)NULL;
}

// Press or Release a mouse button
void Button(
    xcb_connection_t *conn,
    xcb_window_t window,
    bool press)
{
    xcb_button_press_event_t *event = (xcb_button_press_event_t *)calloc(32, 1);
    xcb_event_mask_t mask;
    if (press){
        event->response_type = XCB_BUTTON_PRESS;
        mask = XCB_EVENT_MASK_BUTTON_PRESS;
    } else{
        event->response_type = XCB_BUTTON_RELEASE;
        mask = XCB_EVENT_MASK_BUTTON_RELEASE;
    }
    event->event = window;
    
    event->detail = XCB_BUTTON_INDEX_1;
    
    xcb_send_event(conn, false, window, mask, (char*)event);
    xcb_flush(conn);
    free(event);
}

// Warp the actual pointer on screen
void WarpPointer(
    xcb_connection_t *conn,
    xcb_window_t window,
    int x,
    int y)
{
    xcb_warp_pointer(conn, XCB_NONE, window, 0, 0, 0, 0, x, y);
    xcb_flush(conn);
}

// Send a notification the pointer moved
void NotifyMovePointer(
    xcb_connection_t *conn,
    xcb_window_t window,
    int16_t x,
    int16_t y)
{
    xcb_motion_notify_event_t *event = (xcb_motion_notify_event_t *)calloc(32, 1);
    event->response_type = XCB_MOTION_NOTIFY;
    xcb_event_mask_t mask = XCB_EVENT_MASK_POINTER_MOTION;
    event->event = window;
    event->detail = XCB_MOTION_NORMAL;
    event->time = 1;
    event->event_x = x;
    event->event_y = y;
    
    xcb_send_event(conn, false, window, mask, (char*)event);
    xcb_flush(conn);
    free(event);
}
