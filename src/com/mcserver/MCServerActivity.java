package com.mcserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MCServerActivity extends Activity {
	MainThread mThread = null;
	Thread ServerStatusThread = null;
	boolean mbExiting = false;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        Log.e("MCServer", "p id: " + android.os.Process.myPid() );
        
        
        ((Button)findViewById(R.id.start_server)).setOnClickListener( new View.OnClickListener() {
			public void onClick(View v) {
				if( mThread == null || mThread.isAlive() == false ) {
					mThread = new MainThread( (MCServerActivity)v.getContext() );
					mThread.start();
				}
			}
		});
        
        ((Button)findViewById(R.id.stop_server)).setOnClickListener( new View.OnClickListener() {
			public void onClick(View v) {
				NativeCleanUp();
			}
		});
        

        
        ServerStatusThread = new Thread( new Runnable() {
			public void run() {
				for(;;)
				{
					try {
				        runOnUiThread( new Runnable() {
							public void run() {
								UpdateServerStatus();
							}
				        });
				        Thread.sleep(1000);
					} catch (InterruptedException e) {
					}
				}
			}
        });
        ServerStatusThread.start();
        
        
        

        
        

        Thread loggerThread = new Thread( new Runnable() {
			public void run() {
				Process process = null;
				
				SetText( "herpaderpa" );

				try {
					process = Runtime.getRuntime().exec("logcat -v raw *:s MCServer ");// Verbose filter
				} catch (IOException e) {
				}

				BufferedReader reader = null;

				try {
					reader = new BufferedReader(new InputStreamReader(process.getInputStream()));

					String line;

					while( mbExiting == false ) {
						line = reader.readLine();
						if( line != null )
						{
							SetText( line );
						}
					}

					Log.i("MCServer", "Prepping thread for termination");
					reader.close();
					process.destroy();
					process = null;
					reader = null;
				} catch (IOException e) {
				}
			}
        });
        loggerThread.start();

        
        
        
        
        
        ((TextView)findViewById(R.id.ip_address)).setText("Connect to: " + getLocalIpAddress());
    }
    
    
    
    public String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress()) {
                        return inetAddress.getHostAddress().toString();
                    }
                }
            }
        } catch (SocketException ex) {
            Log.e("MCServer", ex.toString());
        }
        return null;
    }
    
    
    
    public void UpdateServerStatus()
    {
		if( NativeIsServerRunning() ) {
			((TextView)findViewById(R.id.server_status_text)).setText(R.string.mcserver_is_running);
			((TextView)findViewById(R.id.server_status_text)).setTextColor(Color.GREEN);
			((Button)findViewById(R.id.stop_server)).setEnabled(true);
			((Button)findViewById(R.id.start_server)).setEnabled(false);
		} else {
			((TextView)findViewById(R.id.server_status_text)).setText(R.string.mcserver_is_not_running);
			((TextView)findViewById(R.id.server_status_text)).setTextColor(Color.RED);
			((Button)findViewById(R.id.stop_server)).setEnabled(false);
			((Button)findViewById(R.id.start_server)).setEnabled(true);
		}
    }
    
    
    
    
    
    public boolean onKeyDown(int keyCode, KeyEvent event) { 
	    if(keyCode==KeyEvent.KEYCODE_BACK) 
	    { 
		    //android.os.Process.killProcess(android.os.Process.myPid());
	    	NativeCleanUp();
		    return super.onKeyDown(keyCode, event); 
	    } 
	    return false; 
    }
    
    
    
    
    public void onDestroy() {
    	mbExiting = true;
    	super.onDestroy();
    }
    
    
    
    
    
    public void AddToLog( String logMessage ) {
    	
    }
    
    
    
    
    
    public void SetText( final String aText ) {
    	//Log.d("MCServer", "in SetText " + aText);
    	/*
    	final MCServerActivity context = this;
		this.runOnUiThread(new Runnable() 
		{
		    public void run() 
		    {
		    	((TextView)context.findViewById(R.id.textView1)).setText(aText);
		    }
		} );
		*/
    	final TextView tv = (TextView)this.findViewById(R.id.textView1);
    	tv.post(new Runnable() {
    		public void run() {
    			tv.setText(aText);
    		}
    	});
    }
    
    
    
    
    
    public void Testtt()
    {
    	//Log.d("MCServer", "in Testtt");
    }
    
    
    
    
    
    static {
        System.loadLibrary("mcserver");
    }
    
    
    public native void    NativeOnCreate();
    public native void    NativeCleanUp();
    public native boolean NativeIsServerRunning();
    
}


class MainThread extends Thread {
	MCServerActivity mContext = null;
	int numlogs = 0;
	
    MainThread( MCServerActivity aContext ) {
    	mContext = aContext;
    }
    
    public void run() {
    	mContext.NativeOnCreate();
    }

}






