package org.cuberite;

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
import android.widget.ListView;
import android.widget.TextView;

public class CuberiteActivity extends Activity {
    private MainThread mThread = null;
    private boolean mbExiting = false;
    private boolean mbEnabledLogging = false;

    private final ArrayList<String> mLogList = new ArrayList<String>();
    private ArrayAdapter<String> mAdapter;

    private CuberiteInstaller mInstaller = null;

    final private int MENU_REINSTALL = 0;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        //Log.e("Cuberite", "p id: " + android.os.Process.myPid() );


        findViewById(R.id.start_server).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mbEnabledLogging = true;
                if (mThread == null || !mThread.isAlive()) {
                    mThread = new MainThread((CuberiteActivity) v.getContext());
                    mThread.start();
                }
            }
        });

        findViewById(R.id.stop_server).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mbEnabledLogging = true;
                NativeCleanUp();
            }
        });

        findViewById(R.id.configure_server).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent myIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://localhost:" + NativeGetWebAdminPort() + "/webadmin/"));
                startActivity(myIntent);
            }
        });



        ListView lv = (ListView)this.findViewById(R.id.listView1);
        mAdapter = new ArrayAdapter<String>(this,
                R.layout.list_item,
                mLogList);
        lv.setAdapter(mAdapter);

        Thread serverStatusThread = new Thread(new Runnable() {
            public void run() {
                for (; ; ) {
                    try {
                        runOnUiThread(new Runnable() {
                            public void run() {
                                UpdateServerStatus();
                            }
                        });
                        Thread.sleep(1000);
                    } catch (InterruptedException ignored) {
                    }
                }
            }
        });
        serverStatusThread.start();







        Thread loggerThread = new Thread( new Runnable() {
            public void run() {
                Process process = null;

                try {
                    process = Runtime.getRuntime().exec("logcat -v raw *:s Cuberite ");// Verbose filter
                } catch (IOException ignored) {
                }

                BufferedReader reader;

                try {
                    InputStreamReader isr = new InputStreamReader(process.getInputStream());
                    reader = new BufferedReader( isr );

                    String line;

                    while(!mbExiting) {
                        line = reader.readLine();
                        if(mbEnabledLogging && line != null )
                        {
                            AddToLog( line );
                        }
                    }

                    Log.i("Cuberite", "Prepping thread for termination");
                    reader.close();
                    process.destroy();
                    process = null;
                    reader = null;
                } catch (IOException ignored) {
                }
            }
        });
        loggerThread.start();

        ((TextView)findViewById(R.id.ip_address)).setText("Connect to: " + getLocalIpAddress());


        mInstaller = new CuberiteInstaller(this);
        if( mInstaller.NeedsUpdate() )
        {
            mInstaller.ShowFirstRunDialog();
        }
    }



    String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress()) {
                        return inetAddress.getHostAddress();
                    }
                }
            }
        } catch (SocketException ex) {
            Log.e("Cuberite", ex.toString());
        }
        return null;
    }



    void UpdateServerStatus()
    {
        if( NativeIsServerRunning() ) {
            ((TextView)findViewById(R.id.server_status_text)).setText(R.string.cuberite_is_running);
            ((TextView)findViewById(R.id.server_status_text)).setTextColor(Color.GREEN);
            findViewById(R.id.stop_server).setEnabled(true);
            findViewById(R.id.start_server).setEnabled(false);
            findViewById(R.id.configure_server).setEnabled(true);
        } else {
            ((TextView)findViewById(R.id.server_status_text)).setText(R.string.cuberite_is_not_running);
            ((TextView)findViewById(R.id.server_status_text)).setTextColor(Color.RED);
            findViewById(R.id.stop_server).setEnabled(false);
            findViewById(R.id.start_server).setEnabled(true);
            findViewById(R.id.configure_server).setEnabled(false);
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





    void AddToLog(final String logMessage) {
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
        menu.add(0, MENU_REINSTALL, 0, "Reinstall Cuberite" );
        return super.onCreateOptionsMenu(menu);
    }





    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch( item.getItemId() )
        {
            case MENU_REINSTALL:
                mInstaller.ShowFirstRunDialog();
                return true;
        }
        return false;
    }





    static {
        System.loadLibrary("Cuberite");
    }


    public native void    NativeOnCreate();
    public native void    NativeCleanUp();
    public native boolean NativeIsServerRunning();
    public native int     NativeGetWebAdminPort();

}


class MainThread extends Thread {
    private CuberiteActivity mContext = null;

    MainThread( CuberiteActivity aContext ) {
        mContext = aContext;
    }

    public void run() {
        mContext.NativeOnCreate();
    }

}






