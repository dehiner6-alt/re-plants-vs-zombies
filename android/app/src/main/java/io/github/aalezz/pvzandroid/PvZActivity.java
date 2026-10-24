package io.github.aalezz.pvzandroid;

import org.libsdl.app.SDLActivity;

public class PvZActivity extends SDLActivity {
    // SDL2 is linked statically (whole-archive) into libmain.so, so only "main" is loaded.
    @Override
    protected String[] getLibraries() {
        return new String[] { "main" };
    }
}
