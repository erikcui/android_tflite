/* ------------------------------------------------ *
 * The MIT License (MIT)
 * Copyright (c) 2020 terryky1220@gmail.com
 * ------------------------------------------------ */
#ifndef __APP_ENGINE_H__
#define __APP_ENGINE_H__

#include <android/native_window.h>
#include <android_native_app_glue.h>
#include <functional>
#include <thread>
#include <GLES2/gl2.h>

#include "camera_manager.h"
#include "tflite_posenet.h"

typedef struct input_tex {
    GLuint texid;
    int    w, h;
    int    draw_x, draw_y, draw_w, draw_h;
} input_tex_t;

typedef struct gles_ctx {
    int initdone;
    int frame_count;

    char *str_glverstion;
    char *str_glvendor;
    char *str_glrender;

    int disp_w, disp_h;

    bool tex_camera_valid;
    input_tex_t tex_static;
    input_tex_t tex_camera;
} gles_ctx_t;


class AppEngine {
public:
    explicit AppEngine(android_app* app);
    ~AppEngine();

    // Interfaces to android application framework
    struct android_app* AndroidApp(void) const;
    void OnAppInitWindow(void);
    void OnAppTermWindow(void);

    // Manage Camera Permission
    void RequestCameraPermission();
    void OnCameraPermission(jboolean granted);

    // Manage NDKCamera Object
    void InitCamera (void);
    void CreateCamera(void);
    void DeleteCamera(void);
    void AcquireCameraFrame (void **cap_buf, int *cap_w, int *cap_h);

    // OpenGLES Render
    void InitGLES (void);
    void TerminateGLES (void);
    
    void LoadInputTexture (input_tex_t *tex, char *fname);
    void UpdateCameraTexture ();
    void AdjustTexture (int win_w, int win_h, int texw, int texh,
                        int *dx, int *dy, int *dw, int *dh);

    void UpdateFrame (void);
    void RenderFrame (void);

    void FeedInputImage (int texid, ssbo_t *ssbo, int win_w, int win_h);
    void DrawTFLiteConfigInfo ();

    // Posenet Specific
    void RenderPosenetResult (int x, int y, int w, int h, posenet_result_t *pose_ret);
    void RenderBone (int ofstx, int ofsty, int drw_w, int drw_h,
             posenet_result_t *pose_ret, int pid,
             enum pose_key_id id0, enum pose_key_id id1, float *col);

private:

    struct android_app  *m_app;

    bool                m_cameraGranted;
    NDKCamera           *m_camera;
    AImageReader        *m_img_reader;

    gles_ctx_t          glctx;
    std::vector<uint8_t> m_tflite_model_buf;

public:
};

AppEngine *GetAppEngine (void);

#endif  // __APP_ENGINE_H__
