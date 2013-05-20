package com.legocar.testingversion;

public enum ServoType {

	POWER(0), STEERING(1);
	
	public final static int POWERCODE=0;
	public final static int STEERINGCODE=1;

	private int code;

	ServoType(int c) {
		code = c;
	}

	public int getCode() {
		return code;
	}

}
