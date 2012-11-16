package com.mcserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

public class MCServerActivity extends Activity {
	MainThread mThread = null;
	Thread ServerStatusThread = null;
	boolean mbExiting = false;
	boolean mbEnabledLogging = false;
	
	ArrayList<String> mLogList = new ArrayList<String>();
	ArrayAdapter<String> mAdapter;
	
	MCServerInstaller mInstaller = null;
	
	final private int MENU_REINSTALL = 0;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        //Log.e("MCServer", "p id: " + android.os.Process.myPid() );
        
        
        ((Button)findViewById(R.id.start_server)).setOnClickListener( new View.OnClickListener() {
			public void onClick(View v) {
				mbEnabledLogging = true;
				if( mThread == null || mThread.isAlive() == false ) {
					mThread = new MainThread( (MCServerActivity)v.getContext() );
					mThread.start();
				}
			}
		});
        
        ((Button)findViewById(R.id.stop_server)).setOnClickListener( new View.OnClickListener() {
			public void onClick(View v) {
				mbEnabledLogging = true;
				NativeCleanUp();
			}
		});
        
        ((Button)findViewById(R.id.configure_server)).setOnClickListener( new View.OnClickListener() {
			public void onClick(View v) {
				Intent myIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://localhost:" + NativeGetWebAdminPort() + "/webadmin/"));
				startActivity( myIntent );
			}
		});
        
        
        
        ListView lv = (ListView)this.findViewById(R.id.listView1);
        mAdapter = new ArrayAdapter<String>(this,
        	    R.layout.list_item,
        	    mLogList);
        lv.setAdapter(mAdapter);
        
        
        mLogList.add("---- LOG ----");
        
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

				try {
					process = Runtime.getRuntime().exec("logcat -v raw *:s MCServer ");// Verbose filter
				} catch (IOException e) {
				}

				BufferedReader reader = null;

				try {
					InputStreamReader isr = new InputStreamReader(process.getInputStream());
					reader = new BufferedReader( isr );

					String line;

					while( mbExiting == false ) {
						line = reader.readLine();
						if( mbEnabledLogging == true && line != null )
						{
							AddToLog( line );
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
        
        
        mInstaller = new MCServerInstaller(this);
        if( mInstaller.NeedsUpdate() )
        {
        	mInstaller.ShowFirstRunDialog();
        }
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
			((Button)findViewById(R.id.configure_server)).setEnabled(true);
		} else {
			((TextView)findViewById(R.id.server_status_text)).setText(R.string.mcserver_is_not_running);
			((TextView)findViewById(R.id.server_status_text)).setTextColor(Color.RED);
			((Button)findViewById(R.id.stop_server)).setEnabled(false);
			((Button)findViewById(R.id.start_server)).setEnabled(true);
			((Button)findViewById(R.id.configure_server)).setEnabled(false);
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
    
    
    
    
    
    public void AddToLog( final String logMessage ) {
    	final ListView lv = ((ListView)findViewById(R.id.listView1));
    	lv.post(new Runnable() {
            public void run() {
				//final boolean bAutoscroll = lv.getLastVisiblePosition() >= mAdapter.getCount() - 1 ? true : false;
            	
				mLogList.add(logMessage);
				while( mLogList.size() > 100 ) // only allow 100 messages in the list, otherwise it might slow the GUI down
				{
					mLogList.remove(0);
				}
				mAdapter.notifyDataSetChanged();
				

				// Autoscroll detection is dodgy
				//if( bAutoscroll )
				{
					lv.setSelection(mAdapter.getCount() - 1);
				}
            }
        });
    }
    
    
    
    
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0, MENU_REINSTALL, 0, "Reinstall MCServer" );
        return super.onCreateOptionsMenu(menu);
    }
    
    
    
    
    
	@Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	
    	switch( item.getItemId() )
    	{
    	case MENU_REINSTALL:
    		mInstaller.ShowPluginInstallDialog(true);
    		return true;
    	}
        return false;
    }

    
    
    
    
    static {
        System.loadLibrary("mcserver");
    }
    
    
    public native void    NativeOnCreate();
    public native void    NativeCleanUp();
    public native boolean NativeIsServerRunning();
    public native int     NativeGetWebAdminPort();
    
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






