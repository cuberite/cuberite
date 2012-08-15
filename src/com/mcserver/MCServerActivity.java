package com.mcserver;

import android.app.Activity;
import android.os.Bundle;

public class MCServerActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        MainThread p = new MainThread();
        p.start();
        
        //NativeOnCreate();
    }
    
    
    static {
        System.loadLibrary("mcserver");
    }
    
    private static native void NativeOnCreate();
}


class MainThread extends Thread {
    MainThread() {
    }

    public void run() {
    	NativeOnCreate();
    }
    
    private static native void NativeOnCreate();
}