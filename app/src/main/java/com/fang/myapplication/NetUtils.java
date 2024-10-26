package com.fang.myapplication;

import android.util.Log;

import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

public class NetUtils {
    public static String TAG = "NetUtils";
    public static String getLocalMacAddress() {
        try {
            List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
            if (all!=null){
                Log.d(TAG,"getLocalMacAddress all.size="+all.size());
            }
            for (NetworkInterface nif : all) {
                Log.d(TAG,"getLocalMacAddress nif.getName()="+nif.getName());
                if (!nif.getName().equalsIgnoreCase("wlan0")) continue;

                byte[] macBytes = nif.getHardwareAddress();
                if (macBytes == null) {
                    Log.d(TAG,"getLocalMacAddress null");
                    return "";
                }

                StringBuilder res1 = new StringBuilder();
                for (byte b : macBytes) {
                    res1.append(String.format("%02X:", b));
                }

                if (res1.length() > 0) {
                    res1.deleteCharAt(res1.length() - 1);
                }
                return res1.toString();
            }
        } catch (Exception e) {
            e.printStackTrace();
            Log.d(TAG,"getLocalMacAddress e="+e.getMessage());
        }
        return "00:00:00:00:00:00";
    }


}
