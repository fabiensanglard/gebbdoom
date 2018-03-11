typedef enum {
    ev_keydown,
    ev_keyup,
    ev_mouse,
    ev_joystick
} evtype_t;

typedef struct {
    evtype_t  type;
    int       data1;   // keys/mouse/joystick buttons
    int       data2;   // mouse/joystick x move
    int       data3;   // mouse/joystick y move
} event_t;