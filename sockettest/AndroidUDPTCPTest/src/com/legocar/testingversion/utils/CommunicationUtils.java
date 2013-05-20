package com.legocar.testingversion.utils;

import com.legocar.testingversion.communication.TCPClient;
import com.legocar.testingversion.communication.UDPClient;

public class CommunicationUtils {
	
	public static final String DEFAULTSERVERIP="10.0.0.7";
	public static final int DEFAULTSERVERPORT=4363;
	
	 /** Called when the user clicks the Send by UDP button */
    public static void sendUDPMessage(String message) {
       UDPClient client=new UDPClient(DEFAULTSERVERIP, DEFAULTSERVERPORT);
       client.setMessage(message);
       new Thread(client).start();
    }
    
    /** Called when the user clicks the Send by TCP button */
    public static void sendTCPMessage(String message) {
//    	TCPClient client=new TCPClient(DEFAULTSERVERIP, DEFAULTSERVERPORT);
//        client.setMessage(message);
//        new Thread(client).start();
        
    }

}
