#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include "renderer.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void init_renderer(int w, int h) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    stbi_set_flip_vertically_on_load(1);
}

void begin_frame(void) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void end_frame(void) {
    SwapBuffers(wglGetCurrentDC());
}

unsigned int load_texture(const char* filepath) {
    int w, h, c;
    unsigned char* data = stbi_load(filepath, &w, &h, &c, 4);
    if (!data) {
        return 0;
    }
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tex;
}

void free_texture(unsigned int textureID) {
    glDeleteTextures(1, &textureID);
}

void draw_sprite(
    float x,
    float y,
    float z,
    float width,
    float height,
    float rotationRadians,
    unsigned int textureID
) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotationRadians * (180.0f / (float)M_PI), 0, 0, 1);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(width, 0);
    glTexCoord2f(1, 1); glVertex2f(width, height);
    glTexCoord2f(0, 1); glVertex2f(0, height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
