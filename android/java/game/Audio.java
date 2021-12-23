package com.kmspofol.game;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

import java.io.InputStream;

public class Audio implements Runnable {
    AudioTrack audioTrack;
    boolean isPlaying;

    byte[] bgm;
    int offBgm;

    byte[] eff;
    int offEFf;

    boolean toggle;

    int bufferSize;

    public Audio() {
        final int sampleRate = 22050;//44100;
        int channelCount = AudioFormat.CHANNEL_IN_STEREO;
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        bufferSize = AudioTrack.getMinBufferSize(sampleRate, channelCount, audioFormat);

        //Log.e("kim", "" + bufferSize);

        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
                sampleRate,
                channelCount,
                audioFormat,
                bufferSize,
                AudioTrack.MODE_STREAM);

        isPlaying = true;

        bgm = getStream("Sound/bgm.wav");
        offBgm = 0;

        eff = getStream("");
        offEFf = 0;

        new Thread(this).start();
    }

    byte[] getStream(String fileName)
    {
        byte[] b = null;

        try {
            InputStream is = App.app.getAssets().open(fileName);
            int len = is.available();

            b = new byte[len];
            is.read(b);
            is.close();
        } catch (Exception e) {return null;}
        return b;
    }

    public void run()
    {
        audioTrack.play();
        while(isPlaying)
        {
            try{
                if(toggle)
                {
                    int len = audioTrack.write(bgm, offBgm, bufferSize);
                    offBgm += len;
                    if(offBgm >= bgm.length)
                        offBgm = 0;
                }
                else
                {
                    int len = audioTrack.write(bgm, offEFf, bufferSize);
                    offEFf += len;
                    if(offEFf >= bgm.length)
                        offEFf = 0;
                }
                toggle = !toggle;
            }catch (Exception e) {}
        }

        audioTrack.stop();
        audioTrack.release();
    }
}
