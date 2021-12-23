package com.kmspofol.game;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.lang.ref.WeakReference;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.kmspofol.lib.Native;

public class App extends AppCompatActivity
{
    public Object getAssetMgr()
    {
        return getAssets();
    }

    Canvas createCanvas(Bitmap bitmap)
    {
        Canvas canvas = new Canvas();
        canvas.setBitmap(bitmap);

        canvas.drawARGB(0, 0, 0, 0);
        Paint paint = null;

        return canvas;
    }

    /*
    static void drawTri(Canvas canvas, float x, float y, float width, float height, Paint paint)
    {
        canvas.drawVertices(Canvas.VertexMode.TRIANGLES,
                6, new float[]{x+width/2, y, x, y+height, x+width, y+height}, 0,
                null, 0,
                new int[]{paint.getColor(), paint.getColor(), paint.getColor(),
                        -0x100000, -0x1000000, -0x1000000}, 0,
                null, 0, 0,
                paint);
    }
    */

    SoundP sp = new SoundP();
    static void loadBgmSound(String path) { app.sp.bgmLoad(path); }
    static void loadSfxSound(String path) { app.sp.sfxLoad(path); }
    static void playBgmSound(int index) { app.sp.bgmPlay(index); }
    static void playSfxSound(int index) { app.sp.sfxPlay(index); }
    static void stopBgmSound(int index) { app.sp.bgmStop(index); }
    static void stopSfxSound(int index) { app.sp.sfxStop(index); }
    static void volumeBgmSound(float volume) { app.sp.bgmVolume(volume); }
    static void volumeSfxSound(float volume) { app.sp.sfxVolume(volume); }
    static void pauseBgmSound(boolean pause) { app.sp.bgmPause(pause);}

    public static App app;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        app = this;

        RelativeLayout layout = new RelativeLayout(getApplicationContext());
        RelativeLayout.LayoutParams parms = new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT);
        //parms.addRule();
        layout.setLayoutParams(parms);

        GLView view = new GLView(getApplicationContext());
        layout.addView(view);

        setContentView(layout);

        /*
        getWindow().setFlags(
                WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN;
        decorView.setSystemUiVisibility(uiOptions);
        */

        View decorView;
        int	uiOption;

        decorView = getWindow().getDecorView();
        uiOption = getWindow().getDecorView().getSystemUiVisibility();
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH )
            uiOption |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN )
            uiOption |= View.SYSTEM_UI_FLAG_FULLSCREEN;
        if( Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT )
            uiOption |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        decorView.setSystemUiVisibility( uiOption );
    }

    MyHandler myHandler = new MyHandler(this);
    public void sendMessage(int what, String data)
    {
        Message msg = new Message();
        msg.what = what;
        msg.obj = new String(data);
        myHandler.sendMessage(msg);
    }

    public void eventMessage(Message msg)
    {
        Log.i("kim", "what = "+msg.what);
        if( msg.what==Msg_Login )
        {
            String strID = (String)msg.obj;
        }
        else if (msg.what==Msg_Logout)
        {

        }
    }

    public final static int Msg_Login = 0;
    public final static int Msg_Logout = 1;
    public final static int Msg_Cloud_Write = 2;
    public final static int Msg_Cloud_Read = 3;

    static class MyHandler extends Handler
    {
        WeakReference<App> handler;

        MyHandler(App app)
        {
            handler = new WeakReference<App>(app);
        }

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            App a = handler.get();
            if( a!=null )
                a.eventMessage(msg);
        }
    }

    @Override
    protected void onStart()
    {
        super.onStart();

        // do something
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();

        Native.freeGame();
    }

    @Override
    protected void onStop()
    {
        super.onStop();

        // do something
    }

    @Override
    protected void onPause()
    {
        super.onPause();

        Native.pauseGame(true);
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        Native.pauseGame(false);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_BACK:
                Native.backKeyDown();
                Log.e("kim", "뒤로 가기 키 down");
                break;
        }
        return false;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_BACK:
                Log.e("kim", "뒤로 가기 키 up");
                break;
        }
        return false;
    }

    @Override
    protected void onUserLeaveHint()
    {
        super.onUserLeaveHint();

        Native.homeButton();
    }
}

class GLView extends GLSurfaceView
{
    public GLView(Context context)
    {
        super(context);

        setEGLContextClientVersion(2);
        setEGLConfigChooser(8, 8, 8, 8, 8, 0);
        getHolder().setFormat(PixelFormat.RGBA_8888);
        setRenderer(new Render(context));
        //setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        //setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        enableMulti = false;
        mi = new int[20];
        mx = new float[20];
        my = new float[20];
    }

    boolean enableMulti;
    int mi[];
    float mx[];
    float my[];

    public void setEnableMulti(boolean enable)
    {
        enableMulti = enable;
    }

    @Override
    public boolean onTouchEvent(MotionEvent e)
    {
        // one
        int state;
        switch(e.getAction()) {
            case MotionEvent.ACTION_DOWN:   state = 0; break;
            case MotionEvent.ACTION_MOVE:   state = 1; break;
            case MotionEvent.ACTION_UP:     state = 2; break;
            default:                        state = e.getAction(); break;
        }

        Native.keyGame(state, e.getX(), e.getY());

        // multi
        int i, j, num = e.getPointerCount(), upIndex = 0xFF;

        if (num < 2)
            return true;

        switch( e.getAction() & MotionEvent.ACTION_MASK ) {
            case MotionEvent.ACTION_CANCEL:
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                upIndex = (e.getAction() & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
                break;
        }

        for(i=0, j=0; i<num; i++)
        {
            if( i==upIndex) continue;

            mi[j] = e.getPointerId(i);
            mx[j] = e.getX(i);
            my[j] = e.getY(i);
            j++;
        }
        Native.multiKeyGame(state, j, mx, my);

        return true;
    }
}

class Render implements GLSurfaceView.Renderer
{
    Context context;
    public Render(Context context) { this.context = context; }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig)
    {
        String apkFilePath = null;
        ApplicationInfo appInfo = null;
        PackageManager packageManager = context.getPackageManager();
        try {
            String packageName = App.app.getApplication().getPackageName();
            appInfo = packageManager.getApplicationInfo(packageName, 0);
        } catch(Exception e)
        {
            e.printStackTrace();
            throw new RuntimeException("경로 찾다가, 프로그램 종료되었습니다.");
        }
        apkFilePath = appInfo.sourceDir;

        String ioPath = context.getFilesDir().getAbsolutePath();

        Native.loadGame(apkFilePath, ioPath);
        //new Audio();
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h)
    {
        Native.resizeGame(w, h);
    }
    @Override
    public void onDrawFrame(GL10 gl10)
    {
        Native.drawGame();
    }


}


