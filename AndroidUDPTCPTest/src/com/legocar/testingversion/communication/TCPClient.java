package com.legocar.testingversion.communication;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.util.Log;

public class TCPClient implements Runnable {
	private String serverIP;
	private int serverPort;
	
	private String message="Lego car?";
	
	public TCPClient(String serverIP, int serverPort){
		this.serverIP=serverIP;
		this.serverPort= serverPort;
	}
    @Override
    public void run() {
    	 try {
    	        Socket s = new Socket(serverIP, serverPort);
    	        //BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
    	        BufferedWriter out = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
    	        //send output msg
    	        out.write(message);
    	        out.flush();
    	        Log.i("TcpClient", "sent: " + message);
    	        //accept server response
//    	        String inMsg = in.readLine() + System.getProperty("line.separator");
//    	        Log.i("TcpClient", "received: " + inMsg);
    	        //close connection
    	        s.close();
    	    } catch (UnknownHostException e) {
    	        e.printStackTrace();
    	    } catch (IOException e) {
    	        e.printStackTrace();
    	    } 
    }
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		if(message.length()<1){
			return;
		}
		this.message = message;
		
	}
}
