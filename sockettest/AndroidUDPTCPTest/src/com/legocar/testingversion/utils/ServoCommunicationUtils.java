package com.legocar.testingversion.utils;

import android.util.Log;

public class ServoCommunicationUtils {

	public static String getServoControlMessage(int servo, int middleValue, int offset, double percent){
		int value = middleValue+(int)(offset*percent/100.0);
		Log.i("Message", value+"");
		return "servo set "+servo+" "+value+"\n";
	}
}
