package com.legocar.testingversion.communication;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient implements Runnable {
	private String serverIP;
	private int serverPort;
	
	private String message="Lego car?";
	
	public UDPClient(String serverIP, int serverPort){
		this.serverIP=serverIP;
		this.serverPort= serverPort;
	}
    @Override
    public void run() {
        try {
            InetAddress serverAddr = InetAddress.getByName(serverIP);
            DatagramSocket socket = new DatagramSocket();
            byte[] buf = message.getBytes();
            
            DatagramPacket packet = new DatagramPacket(buf, buf.length, serverAddr, serverPort);
            socket.send(packet);
    } catch (Exception e) {
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