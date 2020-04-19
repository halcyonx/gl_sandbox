package com.android.glrenderer;

// Wrapper for native library

import android.content.res.AssetManager;

public class GameJNILib
{
     static
     {
          System.loadLibrary("glrenderer");
     }

     public static native void Init(AssetManager assetManager);
     public static native void Resize(int width, int height);
     public static native void Render();
     public static native void Shutdown();
}
