package com.fang.myapplication;

import android.content.Context;
import android.net.nsd.NsdManager;
import android.net.nsd.NsdServiceInfo;
import android.util.Log;

public class MyNsd {
    private String mMacAddress;
    private String mDeviceName;
    private Context mContext;
    private static final String TAG = MyNsd.class.getSimpleName();

    public MyNsd(Context context) {
        mMacAddress = NetUtils.getLocalMacAddress();
        mDeviceName = "MyAirPlay";
        mContext = context;
    }

    public void registerAirplayService(int airplayPort) {
        NsdServiceInfo serviceInfo = new NsdServiceInfo();
        serviceInfo.setServiceName(mDeviceName);
        serviceInfo.setServiceType("_airplay._tcp");
        serviceInfo.setAttribute("deviceid", mMacAddress);
        serviceInfo.setAttribute("features", "0x5A7FFFF7,0x1E");
        serviceInfo.setAttribute("srcvers", "220.68");
        serviceInfo.setAttribute("flags", "0x4");
        serviceInfo.setAttribute("vv", "2");
        serviceInfo.setAttribute("model", "AppleTV2,1");
        serviceInfo.setAttribute("pw", "false");
        serviceInfo.setAttribute("rhd", "5.6.0.0");
        serviceInfo.setAttribute("pk", "b07727d6f6cd6e08b58ede525ec3cdeaa252ad9f683feb212ef8a205246554e7");
        serviceInfo.setAttribute("pi", "2e388006-13ba-4041-9a67-25dd4a43d536");
        serviceInfo.setPort(airplayPort);
        Log.d(TAG, "registerService airplayPort=" + airplayPort);
        NsdManager nsdManager = (NsdManager) mContext.getApplicationContext().getSystemService(Context.NSD_SERVICE);

        nsdManager.registerService(
                serviceInfo, NsdManager.PROTOCOL_DNS_SD, new NsdManager.RegistrationListener() {
                    @Override
                    public void onRegistrationFailed(NsdServiceInfo nsdServiceInfo, int i) {

                    }

                    @Override
                    public void onUnregistrationFailed(NsdServiceInfo nsdServiceInfo, int i) {

                    }

                    @Override
                    public void onServiceRegistered(NsdServiceInfo nsdServiceInfo) {

                    }

                    @Override
                    public void onServiceUnregistered(NsdServiceInfo nsdServiceInfo) {

                    }
                });

    }

    public void registerRaopService(int raopPort) {
        NsdServiceInfo serviceInfo2 = new NsdServiceInfo();
        serviceInfo2.setServiceName(mDeviceName);
        serviceInfo2.setServiceType("_raop._tcp");
        serviceInfo2.setAttribute("ch", "2");
        serviceInfo2.setAttribute("cn", "0,1,2,3");
        serviceInfo2.setAttribute("da", "true");
        serviceInfo2.setAttribute("et", "0,3,5");
        serviceInfo2.setAttribute("vv", "2");
        serviceInfo2.setAttribute("ft", "0x5A7FFFF7,0x1E");
        serviceInfo2.setAttribute("am", "AppleTV2,1");
        serviceInfo2.setAttribute("md", "0,1,2");
        serviceInfo2.setAttribute("rhd", "5.6.0.0");
        serviceInfo2.setAttribute("pw", "false");
        serviceInfo2.setAttribute("sr", "44100");
        serviceInfo2.setAttribute("ss", "16");
        serviceInfo2.setAttribute("sv", "false");
        serviceInfo2.setAttribute("tp", "UDP");
        serviceInfo2.setAttribute("txtvers", "1");
        serviceInfo2.setAttribute("sf", "0x4");
        serviceInfo2.setAttribute("vs", "220.68");
        serviceInfo2.setAttribute("vn", "65537");
        serviceInfo2.setAttribute("pk", "b07727d6f6cd6e08b58ede525ec3cdeaa252ad9f683feb212ef8a205246554e7");
        serviceInfo2.setPort(raopPort);
        NsdManager nsdManager = (NsdManager) mContext.getApplicationContext().getSystemService(Context.NSD_SERVICE);
        ;
        nsdManager.registerService(
                serviceInfo2, NsdManager.PROTOCOL_DNS_SD, new NsdManager.RegistrationListener() {
                    @Override
                    public void onRegistrationFailed(NsdServiceInfo nsdServiceInfo, int i) {

                    }

                    @Override
                    public void onUnregistrationFailed(NsdServiceInfo nsdServiceInfo, int i) {

                    }

                    @Override
                    public void onServiceRegistered(NsdServiceInfo nsdServiceInfo) {

                    }

                    @Override
                    public void onServiceUnregistered(NsdServiceInfo nsdServiceInfo) {

                    }
                });
        Log.d(TAG, "registerService raopPort=" + raopPort);
    }

    public void changeDeviceName() {

    }

    public String getDeviceName() {
        return mDeviceName;
    }
}
