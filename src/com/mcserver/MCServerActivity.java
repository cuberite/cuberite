package com.mcserver;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

public class MCServerActivity extends Activity {
	MainThread mThread = null;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mThread = new MainThread();
        mThread.start();
    }
    
    public boolean onKeyDown(int keyCode, KeyEvent event) { 
	    if(keyCode==KeyEvent.KEYCODE_BACK) 
	    { 
		    //android.os.Process.killProcess(android.os.Process.myPid());
	    	mThread.NativeCleanUp();
		    return super.onKeyDown(keyCode, event); 
	    } 
	    return false; 
    }
    
    public void AddToLog( String logMessage ) {
    	
    }
    
    static {
        System.loadLibrary("mcserver");
    }
    
    
    
    
}


class MainThread extends Thread {
    MainThread() {
    }

    public void AddToLog( String logMessage ) {
    	//Log.d("MCServer", "Add to log: " + logMessage);
    }
    
    public void TestTest(){
    	Log.d("MCServer", "in testtest");
    }
    
    public void run() {
    	NativeOnCreate();
    }
    public native void NativeOnCreate();
    public native void NativeCleanUp();
}






