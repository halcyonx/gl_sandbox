package com.android.glrenderer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainSurfaceView extends GLSurfaceView
{
    AssetManager assetManager;

    public MainSurfaceView(Context context)
    {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);

        assetManager = context.getAssets();
        String apkPath = context.getApplicationInfo().sourceDir;
        setRenderer(new Renderer(assetManager, apkPath));
    }

    public void onDestroy()
    {
        GameJNILib.Shutdown();
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        AssetManager assetMgr;
        String apkPath;

        public Renderer(AssetManager assetManager, String sourceDir)
        {
            assetMgr = assetManager;
            apkPath = sourceDir;
        }

        public void onDrawFrame(GL10 gl)
        {
            GameJNILib.Render();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
            GameJNILib.Resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {
            GameJNILib.Init(assetMgr, apkPath);
        }
    }
}
