package com.mcserver;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

public class MCServerInstaller {
	private MCServerActivity mContext;
	final private String BaseDirectory = "basedir";
	final private String PluginDirectory = "Plugins";
	
	final public String SHARED_PREFS_NAME = "MCSERVER_PREFS";
	final public String PREF_IS_INSTALLED = "IS_INSTALLED";
	final public String PREF_LAST_VERSION = "LAST_VERSION";
	private SharedPreferences mSettings = null;
	
	int thisVersion;
	
	MCServerInstaller( MCServerActivity activity )
	{
		mContext = activity;
		
		mSettings = mContext.getSharedPreferences( SHARED_PREFS_NAME, 0);
		
		try {
			this.thisVersion = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0).versionCode;
		} catch (NameNotFoundException e) {
			Log.e("MCServer", "Could not read version code from manifest!");
			e.printStackTrace();
			this.thisVersion = -1;
		}
	}

	
	public boolean IsInstalled()
	{
		return mSettings.getBoolean(PREF_IS_INSTALLED, false);
	}
	
	
	public boolean NeedsUpdate()
	{
		Log.i("MCServer", "thisVersion: " + this.thisVersion + " pref: " + mSettings.getInt(PREF_LAST_VERSION, 0));
		return mSettings.getInt(PREF_LAST_VERSION, 0) != this.thisVersion;
	}
	
	
	public ArrayList<String> FindFoldersInPath(String path)
	{
		ArrayList<String> allFolders = new ArrayList<String>();
		AssetManager am = mContext.getAssets();
		try {
			String[] allPlugins = am.list(path);
			for(String pluginName : allPlugins)
			{
				InputStream istr = null;
				try
				{
					istr = am.open(path + "/" + pluginName);
				} catch( java.io.FileNotFoundException e ) {
					// It seems to be a folder :D
					allFolders.add(pluginName);
					continue;
				}
				istr.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return allFolders;
	}
	
	
	
	
	public void ExpandAssets( String path )
	{
		AssetManager am = mContext.getAssets();
		try {
			String[] getAssets = am.list(path);
			for(String assetName : getAssets)
			{
				//Log.e("MCServer", path + "/" + imgName);
								
				InputStream istr = null;
				try
				{
					istr = am.open(path + "/" + assetName);
				} catch( java.io.FileNotFoundException e ) {
					//Log.e("MCServer", "Could not open" + path + "/" + imgName );
					ExpandAssets(path + "/" + assetName);
					continue;
				}
				
				String outPath = Environment.getExternalStorageDirectory().getPath() + "/mcserver/" + path + "/" + assetName;
				//Log.e("MCServer", "outPath: " + outPath );
				File f = new File( outPath );
				
				f.getParentFile().mkdirs();
				f.createNewFile();
				OutputStream ostr = new FileOutputStream(f);
				
				byte[] buffer = new byte[1024];
				int length;
				while ((length = istr.read(buffer))>0)
				{
					ostr.write(buffer, 0, length);
				}
				ostr.flush();
				ostr.close();
				istr.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	void ShowFirstRunDialog()
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		//builder.setTitle("blaa");
		builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				dialog.dismiss();
			}
		});
		builder.setMessage("It seems this is the first time you are running MCServer on your Android device or it has been updated! This app comes with a couple of pre-packaged plugins, please take a moment to select the plugins you would like to install.");
		builder.setCancelable(false);
		AlertDialog dialog = builder.create();
		dialog.show();
		
		dialog.setOnDismissListener( new DialogInterface.OnDismissListener(){
			public void onDismiss(DialogInterface dialog) {
				ShowPluginInstallDialog(false);
			}
		});
	}
	
	
	public void ShowPluginInstallDialog(boolean bCancelable)
	{
		final ArrayList<String> allPlugins = FindFoldersInPath( BaseDirectory + "/" + PluginDirectory );
		final CharSequence[] items = allPlugins.toArray(new CharSequence[allPlugins.size()]);
		final boolean[] selected = new boolean[items.length];
		for( int i = 0; i < items.length; ++i )
		{
			if( items[i].toString().contains("Core") )
			{	// Select the core plugin by default
				selected[i] = true;
				items[i] = items[i] + " (Recommended)";
			}
		}
		
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		builder.setTitle("Plugins to install");
		builder.setCancelable(bCancelable);
		builder.setMultiChoiceItems(items, selected, new DialogInterface.OnMultiChoiceClickListener() {
			public void onClick(DialogInterface dialog, int which, boolean isChecked) {
				selected[which] = isChecked;
			}
		});
		builder.setPositiveButton("Install", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				ArrayList<String> toInstall = new ArrayList<String>();
				for( int i = 0; i < selected.length; ++i )
				{
					if( selected[i] )
					{
						toInstall.add(allPlugins.get(i));
					}
				}
				InstallPlugins(toInstall);
			}
		});
		
		AlertDialog dialog2 = builder.create();
		dialog2.show();
	}
	
	
	void InstallPlugins( final ArrayList<String> plugins )
	{
		new AsyncTask<Void, Integer, Boolean>()
		{
			ProgressDialog progressDialog;

			@Override
			protected void onPreExecute()
			{
				/*
				 * This is executed on UI thread before doInBackground(). It is
				 * the perfect place to show the progress dialog.
				 */
				progressDialog = ProgressDialog.show(mContext, "", "Installing...");
				
			}

			@Override
			protected Boolean doInBackground(Void... params)
			{
				if (params == null)
				{
					return false;
				}
				try
				{
					/*
					 * This is run on a background thread, so we can sleep here
					 * or do whatever we want without blocking UI thread. A more
					 * advanced use would download chunks of fixed size and call
					 * publishProgress();
					 */
					for( int i = 0; i < plugins.size(); ++i )
					{
						this.publishProgress((int)(i / (float)plugins.size() * 100), i);
						InstallSinglePlugin(PluginDirectory + "/" + plugins.get(i));
					}
					
					this.publishProgress( 100, -1 );
					InstallExampleSettings();
					
					this.publishProgress( 100, -2 );
					InstallWebAdmin();
					
				}
				catch (Exception e)
				{
					Log.e("tag", e.getMessage());
					/*
					 * The task failed
					 */
					return false;
				}

				/*
				 * The task succeeded
				 */
				return true;
			}
			
			protected void onProgressUpdate(Integer... progress) 
			{
				progressDialog.setProgress(progress[0]);
				if( progress[1] > -1 )
				{
					progressDialog.setMessage("Installing " + plugins.get(progress[1]) + "..." );
				}
				else if( progress[1] == -1 )
				{
					progressDialog.setMessage("Installing default settings...");
				}
				else if( progress[1] == -2 )
				{
					progressDialog.setMessage("Installing WebAdmin...");
				}
			}

			@Override
			protected void onPostExecute(Boolean result)
			{
				progressDialog.dismiss();
				/*
				 * Update here your view objects with content from download. It
				 * is save to dismiss dialogs, update views, etc., since we are
				 * working on UI thread.
				 */
				AlertDialog.Builder b = new AlertDialog.Builder(mContext);
				b.setTitle(android.R.string.dialog_alert_title);
				if (result)
				{
					b.setMessage("Install succeeded");
					
					SharedPreferences.Editor editor = mSettings.edit();
					editor.putBoolean(PREF_IS_INSTALLED, true);
					editor.putInt(PREF_LAST_VERSION, thisVersion);
					editor.commit();
				}
				else
				{
					b.setMessage("Install failed");
				}
				b.setPositiveButton(android.R.string.ok,
						new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog, int which) 
							{
								dialog.dismiss();
							}
						});
				b.create().show();
			}
		}.execute();
	}
	
	
	void InstallExampleSettings()
	{
		AssetManager am = mContext.getAssets();
		try {
			String[] allFiles = am.list(BaseDirectory);
			for(String fileName : allFiles)
			{
				InputStream istr = null;
				try
				{
					istr = am.open(BaseDirectory + "/" + fileName);
				} catch( java.io.FileNotFoundException e ) {
					// Must be a folder :D
					continue;
				}
				
				String outPath = Environment.getExternalStorageDirectory().getPath() + "/mcserver/" + fileName;
				Log.i("MCServer", "outPath: " + outPath );
				File f = new File( outPath );
				
				f.getParentFile().mkdirs();
				f.createNewFile();
				OutputStream ostr = new FileOutputStream(f);
				
				byte[] buffer = new byte[1024];
				int length;
				while ((length = istr.read(buffer))>0)
				{
					ostr.write(buffer, 0, length);
				}
				ostr.flush();
				ostr.close();
				istr.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	void InstallWebAdmin()
	{
		AssetManager am = mContext.getAssets();
		try {
			String[] allFiles = am.list(BaseDirectory + "/webadmin");
			for(String fileName : allFiles)
			{
				InputStream istr = null;
				try
				{
					istr = am.open(BaseDirectory + "/webadmin/" + fileName);
				} catch( java.io.FileNotFoundException e ) {
					// Must be a folder :D
					continue;
				}
				
				String outPath = Environment.getExternalStorageDirectory().getPath() + "/mcserver/webadmin/" + fileName;
				Log.i("MCServer", "outPath: " + outPath );
				File f = new File( outPath );
				
				f.getParentFile().mkdirs();
				f.createNewFile();
				OutputStream ostr = new FileOutputStream(f);
				
				byte[] buffer = new byte[1024];
				int length;
				while ((length = istr.read(buffer))>0)
				{
					ostr.write(buffer, 0, length);
				}
				ostr.flush();
				ostr.close();
				istr.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	void InstallSinglePlugin( String path )
	{
		AssetManager am = mContext.getAssets();
		try {
			String[] getImages = am.list(BaseDirectory + "/" + path);
			for(String imgName : getImages)
			{
				Log.i("MCServer", path + "/" + imgName);
								
				InputStream istr = null;
				try
				{
					istr = am.open(BaseDirectory + "/" + path + "/" + imgName);
				} catch( java.io.FileNotFoundException e ) {
					Log.i("MCServer", "Could not open" + path + "/" + imgName );
					InstallSinglePlugin(path + "/" + imgName);
					continue;
				}
				
				String outPath = Environment.getExternalStorageDirectory().getPath() + "/mcserver/" + path + "/" + imgName;
				Log.i("MCServer", "outPath: " + outPath );
				File f = new File( outPath );
				
				f.getParentFile().mkdirs();
				f.createNewFile();
				OutputStream ostr = new FileOutputStream(f);
				
				byte[] buffer = new byte[1024];
				int length;
				while ((length = istr.read(buffer))>0)
				{
					ostr.write(buffer, 0, length);
				}
				ostr.flush();
				ostr.close();
				istr.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
