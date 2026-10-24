package io.github.aalezz.pvzandroid;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.view.Gravity;
import android.widget.TextView;

/**
 * Game data lives in /sdcard/PvZ (Android 11+ hides Android/data), which needs
 * "All files access". Ask for it once, then hand over to the SDL activity.
 */
public class PermissionActivity extends Activity {
    private boolean mAsked = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView aText = new TextView(this);
        aText.setText("Activa \"Permitir acceso a todos los archivos\" para esta app y vuelve.\n\n"
                + "Copia main.pak y la carpeta properties a:\n/sdcard/PvZ/");
        aText.setGravity(Gravity.CENTER);
        aText.setPadding(48, 48, 48, 48);
        setContentView(aText);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (Environment.isExternalStorageManager()) {
            startActivity(new Intent(this, PvZActivity.class));
            finish();
        } else if (!mAsked) {
            mAsked = true;
            Intent aIntent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION,
                    Uri.parse("package:" + getPackageName()));
            startActivity(aIntent);
        }
    }
}
