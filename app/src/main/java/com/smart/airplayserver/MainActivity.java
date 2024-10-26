package com.smart.airplayserver;

import android.app.Activity;
import android.app.Instrumentation;
import android.content.Context;
import android.graphics.Color;
import android.net.nsd.NsdManager;
import android.net.nsd.NsdServiceInfo;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.fang.myapplication.AirPlayServer;
import com.fang.myapplication.DNSNotify;
import com.fang.myapplication.MyNsd;
import com.fang.myapplication.RaopServer;
import com.fang.myapplication.TCPServerThread;


public class MainActivity extends Activity implements View.OnClickListener {

    public static String TAG = "MainActivity";

    private AirPlayServer mAirPlayServer;
    private RaopServer mRaopServer;
    private DNSNotify mDNSNotify;

    private SurfaceView mSurfaceView;
    private Button mBtnControl;
    private TextView mTxtDevice;
    private boolean mIsStart = false;
    private MyNsd myNsd;

    private int screenWidth = 0;
    private int screenHeight = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
        getSystemService(Context.NSD_SERVICE);
//        mBtnControl = findViewById(R.id.btn_control);
//        mTxtDevice = findViewById(R.id.txt_device);
//        mBtnControl.setOnClickListener(this);
//        mSurfaceView = findViewById(R.id.surface);

        FrameLayout layout_root = new FrameLayout(this);
        layout_root.setLayoutParams(new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
        layout_root.setBackgroundColor(Color.BLACK);

        setContentView(layout_root);
        mSurfaceView = new SurfaceView(this);
        mSurfaceView.setVisibility(View.VISIBLE);
        // surfaceView.setZOrderOnTop(true);
        mSurfaceView.setLayoutParams(new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
        layout_root.addView(mSurfaceView);
        mAirPlayServer = new AirPlayServer();
        mRaopServer = new RaopServer(mSurfaceView);
        mDNSNotify = new DNSNotify();
        myNsd = new MyNsd(this);
        DisplayMetrics outMetrics = new DisplayMetrics();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            getDisplay().getMetrics(outMetrics);
            screenWidth = outMetrics.widthPixels;
            screenHeight = outMetrics.heightPixels;
            Log.d(TAG,"screenWidth="+screenWidth+";screenHeight="+screenHeight);
        }




    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.btn_control) {
            if (!mIsStart) {
                startServer();
                int airplayPort = mAirPlayServer.getPort();
                int raopPort = mRaopServer.getPort();
                mTxtDevice.setText("设备名称:" + mDNSNotify.getDeviceName() + ";airplayPort=" + airplayPort + ";raopPort=" + raopPort);
            } else {
                stopServer();
                mTxtDevice.setText("未启动");
            }
            mIsStart = !mIsStart;
            mBtnControl.setText(mIsStart ? "结束" : "开始");
        }
    }

    private void startServer() {
        mDNSNotify.changeDeviceName();
        mAirPlayServer.startServer();

        mRaopServer.startServer();
        int raopPort = mRaopServer.getPort();
        if (raopPort == 0) {
            Toast.makeText(this.getApplicationContext(), "启动raop服务失败", Toast.LENGTH_SHORT).show();
        } else {
            mDNSNotify.registerRaop(raopPort);
        }
        int airplayPort = mRaopServer.getAirplayPort();
        if (airplayPort == 0) {
            Toast.makeText(this.getApplicationContext(), "启动airplay服务失败", Toast.LENGTH_SHORT).show();
        } else {
            mDNSNotify.registerAirplay(airplayPort);
        }
        Log.d(TAG, "airplayPort = " + airplayPort + ", raopPort = " + raopPort);
    }

    private void stopServer() {
        mDNSNotify.stop();
        mAirPlayServer.stopServer();
        mRaopServer.stopServer();
    }

    @Override
    protected void onResume() {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        super.onResume();
        RaopServer.setVideoListener(new RaopServer.VideoListener() {
            @Override
            public void videoSize(int width, int height) {
                Log.d(TAG,"videoSize width="+width+";height="+height);
                if (mSurfaceView!=null){
                    float rateX = (float) ((float) screenWidth / width);
                    float rateY = (float) ((float) screenHeight / height);
                    Log.d(TAG,"videoSize width="+width+";height="+height);
                    float rate = (rateX<=rateY)?rateX:rateY;
                    Log.d(TAG,"videoSize rateX="+rateX+";rateY="+rateY+";rate="+rate);
                    int videoWidth = (int)(width*rate);
                    int videoHeight = (int)(height*rate);
                    Log.d(TAG,"videoSize videoWidth="+videoWidth+";videoHeight="+videoHeight);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(
                                    videoWidth, videoHeight, Gravity.CENTER);
                            lp.setMargins(0, 0, 0, 0);
                            mSurfaceView.setVisibility(View.VISIBLE);
                            mSurfaceView.setLayoutParams(lp);
                        }
                    });
                }
            }
        });
        startServer();
    }
}


