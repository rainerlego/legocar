package com.legocar.testingversion.communication;

import android.os.AsyncTask;

public class TCPConnectTask extends AsyncTask<TCPClient, String, Boolean>{

	@Override
	protected Boolean doInBackground(TCPClient... clients) {
		//we create a TCPClient object and
        for(TCPClient tcpClient: clients){
        	tcpClient.start();
        }

        return true;
	}
}
