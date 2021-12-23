package com.kmspofol.lib;

import android.util.Log;

public class Native
{
    static {
        System.loadLibrary("native-lib");
    }

    public static native void loadGame(String ipkFilePath, String ioPath);
    public static native void freeGame();
    public static native void drawGame();
    public static native void keyGame(int state, float x, float y);
    public static native void multiKeyGame(int state, int num, float[] mx, float[] my);
    public static native void resizeGame(int width, int height);
    public static native void pauseGame(boolean pause);
    public static native void backKeyDown();
    public static native void homeButton();
}

