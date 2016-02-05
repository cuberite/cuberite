package org.cuberite;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

class CuberiteInstaller {
    private static final String PREF_IS_INSTALLED = "IS_INSTALLED";
    private static final String PREF_LAST_VERSION = "LAST_VERSION";
	
    private int thisVersion;
    private final CuberiteActivity mContext;
    private SharedPreferences mSettings = null;

    CuberiteInstaller(CuberiteActivity activity) {
        mContext = activity;

        String SHARED_PREFS_NAME = "CUBERITE_PREFS";
        mSettings = mContext.getSharedPreferences(SHARED_PREFS_NAME, 0);

        try {
            this.thisVersion = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0).versionCode;
        } catch (NameNotFoundException e) {
            Log.e("Cuberite", "Could not read version code from manifest!");
            e.printStackTrace();
            this.thisVersion = -1;
        }
    }


    public boolean NeedsUpdate() {
        Log.i("Cuberite", "thisVersion: " + this.thisVersion + " pref: " + mSettings.getInt(PREF_LAST_VERSION, 0));
        return mSettings.getInt(PREF_LAST_VERSION, 0) != this.thisVersion;
    }


    void ShowFirstRunDialog() 
	{
		new AsyncTask<Void, Void, Boolean>() {
            ProgressDialog progressDialog;

            @Override
            protected void onPreExecute() {
                /*
                 * This is executed on UI thread before doInBackground(). It is
                 * the perfect place to show the progress dialog.
                 */
                progressDialog = ProgressDialog.show(mContext, "", "Updating assets...");

            }

            @Override
            protected Boolean doInBackground(Void... params) {
                try {
                    /*
                     * This is run on a background thread, so we can sleep here
                     * or do whatever we want without blocking UI thread. A more
                     * advanced use would download chunks of fixed size and call
                     * publishProgress();
                     */
                    
					CopyAllFilesInFolder(mContext.getAssets(), "");
                } catch (Exception e) {
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

            protected void onProgressUpdate(Integer... progress) {
				// TODO: good progress updates
            }

            @Override
            protected void onPostExecute(Boolean result) {
                progressDialog.dismiss();
                /*
                 * Update here your view objects with content from download. It
                 * is save to dismiss dialogs, update views, etc., since we are
                 * working on UI thread.
                 */
                AlertDialog.Builder b = new AlertDialog.Builder(mContext);
                b.setTitle(android.R.string.dialog_alert_title);
                if (result) {
                    b.setMessage("Assets successfully updated!");

                    SharedPreferences.Editor editor = mSettings.edit();
                    editor.putBoolean(PREF_IS_INSTALLED, true);
                    editor.putInt(PREF_LAST_VERSION, thisVersion);
                    editor.commit();
                } else {
                    b.setMessage("An exception occurred when updating assets.");
                }
                b.setPositiveButton(android.R.string.ok,
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                            }
                        });
                b.create().show();
            }
        }.execute();
    }
	
	private static void CopyAllFilesInFolder(AssetManager Manager, String Folder)
	{
		if ((Folder == "webkit") || (Folder == "sounds") || (Folder == "images") || (Folder == "cuberite"))
		{
			// Random files are random :P
			return;
		}
		
		try
		{
            final String[] Files = Manager.list(Folder);
            for (String File : Files)
			{
				final String FilePath = Folder + ("".equals(Folder) ? "" : '/') + File;
                InputStream istr;
                try
				{
                    istr = Manager.open(FilePath);
                }
				catch (java.io.FileNotFoundException e)
				{
                    // Must be a folder :D
					CopyAllFilesInFolder(Manager, FilePath);
                    continue;
                }

                File f = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/Cuberite/" + FilePath);
                f.getParentFile().mkdirs();
                f.createNewFile();
				
                OutputStream ostr = new FileOutputStream(f);
                byte[] buffer = new byte[1024];
                int length;
                while ((length = istr.read(buffer)) > 0)
				{
                    ostr.write(buffer, 0, length);
                }
                ostr.close();
                istr.close();
            }
        }
		catch (IOException e)
		{
            e.printStackTrace();
        }
	}
}
