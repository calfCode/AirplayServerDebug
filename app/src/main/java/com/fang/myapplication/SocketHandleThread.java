package com.fang.myapplication;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.io.IOException;
public class SocketHandleThread extends Thread{
    private static final String TAG = SocketHandleThread.class.getSimpleName();
    private volatile Socket socket;
    private Context context;
    private volatile boolean isRunning= false;
    private BufferedReader socketReader = null;
    private BufferedWriter socketWriter = null;
    public SocketHandleThread(Socket socket,Context context) {
        this.socket = socket;
        this.context = context;
        setName("SocketHandle");
    }

    @Override
    public void run() {
        super.run();
        isRunning = true;
        try {
            socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            socketWriter = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            String iMessage = null;
            while (isRunning){
                if (socketReader!=null&&!socket.isClosed()) {
                    if ((iMessage = socketReader.readLine()) != null) {
                        Log.d(TAG, "Received message from client: " + iMessage);
                    }
                }
            }
            closeSocket();
        }catch (Exception e){
            try {
                closeSocket();
            } catch (IOException ex) {
                Log.d(TAG,"IOException ex="+ex.getMessage());
            }
            e.printStackTrace();
            Log.d(TAG,"Exception e="+e.getMessage());
        }
    }

    public void stopThread(){
        isRunning = false;
        try {
            closeSocket();
        } catch (IOException e) {
            Log.d(TAG,"stopThread IOException e="+e.getMessage());
        }

    }

    private void closeSocket() throws IOException {
        Log.d(TAG,"closeSocket Thead:"+Thread.currentThread().getName());
        if (socket!=null) {
            socket.close();
            socket = null;
            if (socketReader!=null) {
                socketReader.close();
                socketReader = null;
            }
            if (socketWriter!=null) {
                socketWriter.close();
                socketWriter = null;
            }

        }
    }
}
