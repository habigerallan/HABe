#include "renderer.h"
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static HWND s_hwnd = NULL;
static HDC  s_hDC  = NULL;
static HGLRC s_hGLRC = NULL;

int InitRenderer(HWND hwnd) {
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;

    s_hwnd = hwnd;

    s_hDC = GetDC(s_hwnd);
    if (!s_hDC) {
        printf("Failed to get device context.\n");
        return 0;
    }

    stbi_set_flip_vertically_on_load(1);

    int pixelFormat = ChoosePixelFormat(s_hDC, &pfd);
    if (!pixelFormat || !SetPixelFormat(s_hDC, pixelFormat, &pfd)) {
        printf("Failed to set pixel format.\n");
        return 0;
    }

    s_hGLRC = wglCreateContext(s_hDC);
    if (!s_hGLRC || !wglMakeCurrent(s_hDC, s_hGLRC)) {
        printf("Failed to create or activate OpenGL context.\n");
        return 0;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    glViewport(0, 0, 800, 600);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return 1;
}

void DestroyRenderer() {
    wglMakeCurrent(NULL, NULL);

    if (s_hGLRC) {
        wglDeleteContext(s_hGLRC);
        s_hGLRC = NULL;
    }

    if (s_hDC && s_hwnd) {
        ReleaseDC(s_hwnd, s_hDC);
        s_hDC = NULL;
    }

    s_hwnd = NULL;
}

unsigned int LoadTexture(const char* filepath) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    int width, height, channels;
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
    if (!data) {
        printf("Failed to load texture: %s\n", filepath);
        return 0;
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}

void DestroyTexture(unsigned int textureID) {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void Render(float x, float y, float z, float width, float height, float rotation,
            const float color[4], unsigned int textureID) {
    glPushMatrix();

    glTranslatef(x + width / 2.0f, y + height / 2.0f, z);

    glRotatef(rotation, 0.0f, 0.0f, 1.0f);

    glTranslatef(-width / 2.0f, -height / 2.0f, 0.0f);

    glColor4f(color[0], color[1], color[2], color[3]);

    if (textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,    0.0f,    0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(width,   0.0f,    0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(width,   height,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f,    height,  0.0f);
        glEnd();
    } else {
        glBegin(GL_QUADS);
            glVertex3f(0.0f,    0.0f,    0.0f);
            glVertex3f(width,   0.0f,    0.0f);
            glVertex3f(width,   height,  0.0f);
            glVertex3f(0.0f,    height,  0.0f);
        glEnd();
    }

    glPopMatrix();

    // Reset color to white
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
