package com.curiologix.pelirlib;

import android.util.Log;

public class IRManager {

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("IRManager");
    }

    public native int findRemoteIdForAcModel(String acModelStr);
    public native String encodeRemoteIrData(String remoteJsonStr);
    public native String decodeRemoteIrData(String encodedIrJsonStr);

    public IRManager() { }
}
