package com.curiologix.pelirlib;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {

    String TAG = "PELIRLIB";

    IRManager irManager;

    String encodedIrJsonStr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        irManager = new IRManager();

        String barcode = "SQGJCI"; //"SRXARI";
        findRemoteTest(barcode);
        Log.d(TAG, " ");

        encodedIrJsonStr = encodeTest(1000);
        decodeTest(1000, encodedIrJsonStr);
        Log.d(TAG, " ");

        encodedIrJsonStr = encodeTest(1001);
        decodeTest(1001, encodedIrJsonStr);
        Log.d(TAG, " ");

        encodedIrJsonStr = encodeTest(1002);
        decodeTest(1002, encodedIrJsonStr);
        Log.d(TAG, " ");

        encodedIrJsonStr = encodeTest(1003);
        decodeTest(1003, encodedIrJsonStr);
        Log.d(TAG, " ");

        encodedIrJsonStr = encodeTest(1012);
        decodeTest(1012, encodedIrJsonStr);
        Log.d(TAG, " ");

        encodedIrJsonStr = encodeTest(1013);
        decodeTest(1013, encodedIrJsonStr);
        Log.d(TAG, " ");
    }

    void findRemoteTest(String barcode) {
        int foundRemoteId = irManager.findRemoteIdForAcModel(barcode);
        Log.d(TAG, "foundRemoteId: " + foundRemoteId);
    }

    String encodeTest(int rmtId) {
        JSONObject remoteIrJson1 = new JSONObject();
        try {
            remoteIrJson1.put("remoteId", rmtId);
            remoteIrJson1.put("remoteBrand", "Gree");
            remoteIrJson1.put("remoteModel", "Yaa");
            remoteIrJson1.put("powerOnOff", 1);
            remoteIrJson1.put("operationMode", 1);
            remoteIrJson1.put("setTemperature", 16);
            remoteIrJson1.put("fanSpeed", 1);
            remoteIrJson1.put("swingVertical", 0);
            remoteIrJson1.put("swingHorizontal", 0);

            remoteIrJson1.put("turboOnOff", 0);
            remoteIrJson1.put("muteOnOff", 0);
            remoteIrJson1.put("autoEvapOnOff", 0);
            remoteIrJson1.put("displayOnOff", 1);

            String encodedIrJsonStr = irManager.encodeRemoteIrData(remoteIrJson1.toString());
            Log.d(TAG, "encoded(" + rmtId + "): " + encodedIrJsonStr);
            return encodedIrJsonStr;
        } catch (JSONException e) {
            e.printStackTrace();
            return "";
        }
    }

    void decodeTest(int rmtId, String encodedIrJsonStr) {
        try {
            JSONObject encodedIrJson1 = new JSONObject(encodedIrJsonStr);
            JSONObject encodedIrJson2 = new JSONObject();

            String encodedIrData2 = encodedIrJson1.getString("encodedIrData");

            encodedIrJson2.put("remoteId", rmtId);
            encodedIrJson2.put("encodedIrData", encodedIrData2);

            String remoteIrJson2 = irManager.decodeRemoteIrData(encodedIrJson2.toString());

            Log.d(TAG, "decoded(" + rmtId + "): " + remoteIrJson2);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
