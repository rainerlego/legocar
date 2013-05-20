package com.legocar.testingversion.communication;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.util.Log;

public class TCPClient {
	private String serverIP;
	private int serverPort;
	
	private Socket sendSocket;
	private BufferedWriter out;
	
	private String message="Lego car?";
	
	public TCPClient(String serverIP, int serverPort){
		this.serverIP=serverIP;
		this.serverPort= serverPort;
	}
	
	public void sendMessage(String message) throws IOException{
		try{
			out.write(message);
			out.flush();
			Log.i("TcpClient", "sent: " + message);
		}catch (NullPointerException e) {
			// TODO: handle exception
			Log.e("TcpClient", "error: " + e.getMessage());
		}
	}
	
	public void stop() throws IOException{
		try{
			
			out.close();
			sendSocket.close();
		}catch (NullPointerException e) {
			// TODO: handle exception
			Log.e("TcpClient", "error: " + e.getMessage());
		}
	}

	public void start() {
    	 try {
    		 	sendSocket = new Socket(serverIP, serverPort);
    		 	sendSocket.setTcpNoDelay(true);
    		    out = new BufferedWriter(new OutputStreamWriter(sendSocket.getOutputStream()));
    		
    	        //BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
    	       
    	        //accept server response
    	        //String inMsg = in.readLine() + System.getProperty("line.separator");
    	        //Log.i("TcpClient", "received: " + inMsg);
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
