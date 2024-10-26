package com.fang.myapplication;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;
public class TCPServerThread extends Thread{

    private static final String TAG = TCPServerThread.class.getSimpleName();
    private Context context;
    private SocketHandleThread socketHandleThread;
    private ServerSocket serverSocket;
    private boolean isRunning = false;

    public Handler getHandler() {
        return handler;
    }

    private Handler handler;
    private HandlerThread handlerThread ;

    private volatile Socket activeSocket;

    public TCPServerThread(Context context) {
        Log.d(TAG," TCPServerThread()");
        this.context = context;
        handlerThread = new HandlerThread("ServerHandler");
        handlerThread.start();
        setName("TCPServer");
    }

    @Override
    public void run() {
        super.run();
        isRunning = true;
        Log.d(TAG," currentThread="+Thread.currentThread().getName());
        try {
            serverSocket = new ServerSocket(7011,100);
            Log.d(TAG,"Server started  at:  " + serverSocket);
            while (isRunning){
                Log.d(TAG,"Wait for connection... ");
                if (serverSocket!=null) {
                    activeSocket = serverSocket.accept();
                    if (activeSocket != null) {
                        Log.d(TAG, "Received a  connection from : " + activeSocket);
                        if (socketHandleThread != null) {
                            socketHandleThread.stopThread();
                        }
                        socketHandleThread = new SocketHandleThread(activeSocket, context);
                        socketHandleThread.setPriority(Thread.MAX_PRIORITY);
                        socketHandleThread.start();
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
            Log.d(TAG,"IOException e="+e.getMessage());
        }
    }

    public void exitThead(){
        Log.d(TAG,"exitThead");
        isRunning = false;
        if (socketHandleThread!=null){
            socketHandleThread.stopThread();
            socketHandleThread = null;
        }
        if (handlerThread!=null){
            handlerThread.quit();
            handlerThread = null;
            handler= null;
        }
        if (serverSocket!=null){
            try {
                Log.d(TAG,"exitThead serverSocket.close()");
                serverSocket.close();
                serverSocket = null;
            } catch (IOException e) {
                Log.d(TAG,"exitThead IOException e="+e.getMessage());
            }
        }

    }


}
