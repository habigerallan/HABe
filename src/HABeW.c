#include "HABeW.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HABeW_Window* window = (HABeW_Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (window && window->properties.event_callback) {
        int result = window->properties.event_callback(window, uMsg, NULL);
        if (result) {
            return result;
        }
    }

    switch (uMsg) {
        case WM_MOVE:
            if (window) {
                window->state.position.x = (float)(short)LOWORD(lParam);
                window->state.position.y = (float)(short)HIWORD(lParam);
            }
            break;

        case WM_SIZE:
            if (window) {
                window->state.size.x = (float)LOWORD(lParam);
                window->state.size.y = (float)HIWORD(lParam);
            }
            break;

        case WM_CLOSE:
            if (window) {
                HABeW_free(window);
            }
            return 0;

        case SW_HIDE:
            if (window) {
                HABeW_hide(window);
            }
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

unsigned int __stdcall HABeW_window_thread(void* param) {
    HABeW_Window* window = (HABeW_Window*)param;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wchar_t wclass_name[256];
    MultiByteToWideChar(CP_UTF8, 0, window->properties.class_name, -1, wclass_name, 256);

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = wclass_name;

    if (!RegisterClassW(&wc)) {
        return 1;
    }

    window->properties.hwnd = CreateWindowExW(
        0, wclass_name, L"", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, HABEW_DEFAULT_WIDTH, HABEW_DEFAULT_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!window->properties.hwnd) {
        return 1;
    }

    SetWindowLongPtr(window->properties.hwnd, GWLP_USERDATA, (LONG_PTR)window);

    SetEvent(window->ready_event);

    MSG msg;
    while (1) {
        DWORD waitResult = WaitForSingleObject(window->stop_event, 0);
        if (waitResult == WAIT_OBJECT_0) {
            window->flags.is_stopped = 1;

            break;
        }

        if (PeekMessage(&msg, window->properties.hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}


HABeW_Window* HABeW_create(const char* class_name) {
    HABeW_Window* window = (HABeW_Window*)malloc(sizeof(HABeW_Window));
    if (!window) {
        return NULL;
    }

    memset(window, 0, sizeof(HABeW_Window));

    window->state.position.x = 0;
    window->state.position.y = 0;
    window->state.size.x = HABEW_DEFAULT_WIDTH;
    window->state.size.y = HABEW_DEFAULT_HEIGHT;

    window->state.previous_position.x = 0;
    window->state.previous_position.y = 0;
    window->state.previous_size.x = HABEW_DEFAULT_WIDTH;
    window->state.previous_size.y = HABEW_DEFAULT_HEIGHT;

    window->flags.is_stopped = 0;
    window->flags.is_visible = 0;

    window->thread.thread_handle = NULL;
    window->thread.thread_id = 0;
    window->thread.is_running = 0;

    window->child.child_count = 0;
    for (int i = 0; i < HABEW_MAX_CHILDREN; i++) {
        window->child.children[i] = NULL;
    }

    window->properties.event_callback = NULL;
    window->properties.parent = NULL;
    strncpy(window->properties.class_name, class_name, sizeof(window->properties.class_name) - 1);

    window->ready_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    window->stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (!window->ready_event || !window->stop_event) {
        if (window->ready_event) {
            CloseHandle(window->ready_event);
        }

        if (window->stop_event) {
            CloseHandle(window->stop_event);
        }

        HABeW_free(window);
        return NULL;
    }

    window->thread.thread_handle = (HANDLE)_beginthreadex(NULL, 0, HABeW_window_thread, window, 0, &window->thread.thread_id);

    if (!window->thread.thread_handle) {
        CloseHandle(window->ready_event);
        CloseHandle(window->stop_event);

        HABeW_free(window);
        return NULL;
    }

    WaitForSingleObject(window->ready_event, INFINITE);
    CloseHandle(window->ready_event);

    return window;
}


int HABeW_free(HABeW_Window* window) {
    if (!window) {
        return 1;
    }

    if (!window->flags.is_stopped) {
        HABeW_stop(window);
    }

    if (window->thread.thread_handle) {
        CloseHandle(window->thread.thread_handle);
    }

    if (window->properties.parent) {
        HABeW_remove_child(window->properties.parent, window);
    }

    for (int i = 0; i < window->child.child_count; i++) {
        HABeW_free(window->child.children[i]);
    }

    if (window->ready_event) {
        CloseHandle(window->ready_event);
    }

    if (window->stop_event) {
        CloseHandle(window->stop_event);
    }

    free(window);

    return 0;
}




int HABeW_show(HABeW_Window* window) {
    if (!window || window->flags.is_visible) {
        return 1;
    }

    ShowWindow(window->properties.hwnd, SW_SHOW);
    window->flags.is_visible = 1;

    return 0;
}


int HABeW_hide(HABeW_Window* window) {
    if (!window || !window->flags.is_visible) {
        return 1;
    }

    ShowWindow(window->properties.hwnd, SW_HIDE);

    HWND nextWindow = GetNextWindow(window->properties.hwnd, GW_HWNDNEXT);
    if (!nextWindow) {
        nextWindow = GetNextWindow(window->properties.hwnd, GW_HWNDPREV);
    }

    if (nextWindow) {
        SetForegroundWindow(nextWindow);
    }

    window->flags.is_visible = 0;
    return 0;
}


int HABeW_stop(HABeW_Window* window) {
    if (!window) {
        return 1;
    }
    
    if (window->flags.is_visible) {
        HABeW_hide(window);
    }

    SetEvent(window->stop_event);

    return 0;
}




int HABeW_set_size(HABeW_Window* window, HABeM_Vector2 size) {
    if (!window) {
        return 1;
    }

    if (!SetWindowPos(window->properties.hwnd, NULL, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOZORDER)) {
        return 1;
    }
    
    window->state.size = size;
    return 0;
}


int HABeW_set_position(HABeW_Window* window, HABeM_Vector2 position) {
    if (!window) {
        return 1;
    }

    if (!SetWindowPos(window->properties.hwnd, NULL, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER)) {
        return 1;
    }

    return 0;
}


int HABeW_set_title(HABeW_Window* window, const char* title) {
    if (!window || !title) {
        return 1;
    }

    wchar_t wtitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, 256);

    if (!SetWindowTextW(window->properties.hwnd, wtitle)) {
        return 1;
    }

    return 0;
}


int HABeW_set_cursor(HABeW_Window* window, HCURSOR cursor) {
    if (!window) {
        return 1;
    }

    SetClassLongPtr(window->properties.hwnd, GCLP_HCURSOR, (LONG_PTR)cursor);

    return 0;
}


int HABeW_set_event_callback(HABeW_Window* window, int (*callback)(HABeW_Window*, int, void*)) {
    if (!window || !callback) {
        return 1;
    }

    window->properties.event_callback = callback;

    return 0;
}




HABeM_Vector2 HABeW_get_size(HABeW_Window* window) {
    if (!window) {
        HABeM_Vector2 size = {0, 0};
        return size;
    }

    return window->state.size;
}


HABeM_Vector2 HABeW_get_position(HABeW_Window* window) {
    HABeM_Vector2 position = {0, 0};
    if (!window) {
        return position;
    }

    RECT windowRect;
    if (!GetWindowRect(window->properties.hwnd, &windowRect)) {
        return position;
    }

    position.x = windowRect.left;
    position.y = windowRect.top;

    return position;
}


char* HABeW_get_title(HABeW_Window* window) {
    if (!window) {
        return "";
    }

    wchar_t wtitle[256];
    if (GetWindowTextW(window->properties.hwnd, wtitle, sizeof(wtitle)/sizeof(wchar_t)) == 0) {
        return "";
    }

    static char title[256];
    WideCharToMultiByte(CP_UTF8, 0, wtitle, -1, title, sizeof(title), NULL, NULL);
    
    return title;
}


HCURSOR HABeW_get_cursor(HABeW_Window* window) {
    if (!window) {
        return NULL;
    }

    HCURSOR cursor = (HCURSOR)GetClassLongPtr(window->properties.hwnd, GCLP_HCURSOR);

    if (!cursor) {
        return NULL;
    }

    return cursor;
}


HABeW_Window* HABeW_get_parent(HABeW_Window* window) {
    if (!window) {
        return NULL;
    }

    return window->properties.parent;
}


int (*HABeW_get_event_callback(HABeW_Window* window))(HABeW_Window*, int, void*) {
    if (!window) {
        return NULL;
    }

    return window->properties.event_callback;
}




int HABeW_add_child(HABeW_Window* parent, HABeW_Window* child) {
    if (!parent || !child || child->properties.parent) { 
        return 1;
    }

    if (parent->child.child_count >= HABEW_MAX_CHILDREN) {
        return 1;
    }

    parent->child.children[parent->child.child_count] = child;
    parent->child.child_count++;

    child->properties.parent = parent;

    return 0;
}


int HABeW_remove_child(HABeW_Window* parent, HABeW_Window* child) {
    if (!parent || !child || child->properties.parent != parent) { 
        return 1;
    }

    for (int i = 0; i < parent->child.child_count; i++) {
        if (parent->child.children[i] == child) {
            parent->child.children[i] = parent->child.children[--parent->child.child_count];
            child->properties.parent = NULL;
            return 0;
        }
    }

    return 1;
}
