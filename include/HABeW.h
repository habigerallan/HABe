#ifndef HABEW_H
#define HABEW_H

#include "HABeM.h"
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>

#define HABEW_MAX_CHILDREN 16
#define HABEW_DEFAULT_WIDTH 800
#define HABEW_DEFAULT_HEIGHT 600

typedef struct HABeW_State HABeW_State;
typedef struct HABeW_Flags HABeW_Flags;
typedef struct HABeW_Thread HABeW_Thread;
typedef struct HABeW_Children HABeW_Children;
typedef struct HABeW_Properties HABeW_Properties;
typedef struct HABeW_Window HABeW_Window;

struct HABeW_State {
    HABeM_Vector2 position;
    HABeM_Vector2 size;
    HABeM_Vector2 previous_position;
    HABeM_Vector2 previous_size;
};

struct HABeW_Flags {
    int is_stopped;
    int is_visible;
};

struct HABeW_Thread {
    HANDLE thread_handle;
    unsigned int thread_id;
    int is_running;
};

struct HABeW_Children {
    HABeW_Window* children[HABEW_MAX_CHILDREN];
    int child_count;
};

struct HABeW_Properties {
    HWND hwnd;
    int (*event_callback)(HABeW_Window*, int, void*);
    HABeW_Window* parent;
    char class_name[256];
};

struct HABeW_Window {
    HABeW_State state;
    HABeW_Flags flags;
    HABeW_Thread thread;
    HABeW_Children child;
    HABeW_Properties properties;
    HANDLE ready_event;
    HANDLE stop_event;
};

HABeW_Window* HABeW_create(const char* class_name);
int HABeW_free(HABeW_Window* window);

int HABeW_show(HABeW_Window* window);
int HABeW_hide(HABeW_Window* window);
int HABeW_stop(HABeW_Window* window);

int HABeW_set_size(HABeW_Window* window, HABeM_Vector2 size);
int HABeW_set_position(HABeW_Window* window, HABeM_Vector2 position);
int HABeW_set_title(HABeW_Window* window, const char* title);
int HABeW_set_cursor(HABeW_Window* window, HCURSOR  cursor);
int HABeW_set_event_callback(HABeW_Window* window, int (*callback)(HABeW_Window*, int, void*));

HABeM_Vector2 HABeW_get_size(HABeW_Window* window);
HABeM_Vector2 HABeW_get_position(HABeW_Window* window);
char* HABeW_get_title(HABeW_Window* window);
HCURSOR HABeW_get_cursor(HABeW_Window* window);
HABeW_Window* HABeW_get_parent(HABeW_Window* window);
int (*HABeW_get_event_callback(HABeW_Window* window))(HABeW_Window*, int, void*);

int HABeW_add_child(HABeW_Window* parent, HABeW_Window* child);
int HABeW_remove_child(HABeW_Window* parent, HABeW_Window* child);

#endif
