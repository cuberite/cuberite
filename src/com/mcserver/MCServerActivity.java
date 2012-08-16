package com.mcserver;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;

public class MCServerActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        MainThread p = new MainThread();
        p.start();
    }
    
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    { 
	    if(keyCode==KeyEvent.KEYCODE_BACK) 
	    { 
		    //android.os.Process.killProcess(android.os.Process.myPid());
	    	NativeCleanUp();
		    return super.onKeyDown(keyCode, event); 
	    } 
	    return false; 
    }
    
    static {
        System.loadLibrary("mcserver");
    }
    
    private static native void NativeCleanUp();
    
}


class MainThread extends Thread {
    MainThread() {
    }

    public void run() {
    	NativeOnCreate();
    }
    
    private static native void NativeOnCreate();
}