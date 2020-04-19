package com.android.glrenderer;

import android.app.Activity;
import android.os.Bundle;

public class GLMainActivity extends Activity
{
    MainSurfaceView mView;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        mView = new MainSurfaceView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }

    @Override protected void onDestroy()
    {
        super.onDestroy();
        mView.onDestroy();
    }
}
