package com.legocar.testingversion;

public class Setting {
	protected static String ip = "10.0.0.203";
	protected static int port = 5567;
	protected static int powerMiddleValue = 4000;
	protected static int powerOffset = 4000;
	protected static int steeringMiddleValue = 4000;
	protected static int steeringOffset = 4000;
	protected static int powerServo =0;
	protected static int steeringServo = 1;
	protected static int defaultMessageDelay = 100;
	
	public Setting() {
		// TODO Auto-generated constructor stub
	}
//
//	public Setting(String ip, String port, String defaultMessageDelay){
//		this();
//		Setting.ip=ip;
//		Setting.port=Integer.valueOf(port);
//		Setting.defaultMessageDelay = Integer.valueOf(defaultMessageDelay);
//	}
//
//	public Setting(String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
//		this();
//		Setting.powerMiddleValue=Integer.valueOf(powerMiddleValue);
//		Setting.powerOffset=Integer.valueOf(powerOffset);
//		Setting.steeringMiddleValue=Integer.valueOf(steeringMiddleValue);
//		Setting.steeringOffset=Integer.valueOf(steeringOffset);
//		Setting.powerServo = Integer.valueOf(powerServo);
//		Setting.steeringServo = Integer.valueOf(steeringServo);
//	}
//
//	public Setting(String ip, String port, String defaultMessageDelay, String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
//		this(powerMiddleValue, powerOffset, steeringMiddleValue, steeringOffset, powerServo, steeringServo);
//		Setting.ip=ip;
//		Setting.port=Integer.valueOf(port);
//		Setting.defaultMessageDelay = Integer.valueOf(defaultMessageDelay);
//
//	}


	public void set(String ip, String port, String defaultMessageDelay){
		Setting.ip=ip;
		Setting.port=Integer.valueOf(port);
		Setting.defaultMessageDelay = Integer.valueOf(defaultMessageDelay);
	}

	public void set(String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
		Setting.powerMiddleValue=Integer.valueOf(powerMiddleValue);
		Setting.powerOffset=Integer.valueOf(powerOffset);
		Setting.steeringMiddleValue=Integer.valueOf(steeringMiddleValue);
		Setting.steeringOffset=Integer.valueOf(steeringOffset);
		Setting.powerServo = Integer.valueOf(powerServo);
		Setting.steeringServo = Integer.valueOf(steeringServo);
	}

	public void set(String ip, String port, String defaultMessageDelay, String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
		set(ip, port, defaultMessageDelay);
		set(powerMiddleValue, powerOffset, steeringMiddleValue, steeringOffset, powerServo, steeringServo);
	}

	public String getIp() {
		return ip;
	}

	public int getPort() {
		return port;
	}

	public int getDefaultMessageDelay(){
		return defaultMessageDelay;
	}
	
	public int getMiddleValue(ServoType servo) {
		switch(servo.getCode()){
		case ServoType.POWERCODE:
			return powerMiddleValue;
		case ServoType.STEERINGCODE:
			return steeringMiddleValue;
		default:
			return 0;
		}			
	}

	public int getOffset(ServoType servo) {
		switch(servo.getCode()){
		case ServoType.POWERCODE:
			return powerOffset;
		case ServoType.STEERINGCODE:
			return steeringOffset;
		default:
			return 0;
		}
	}

	public int getServo(ServoType servo) {
		switch(servo.getCode()){
		case ServoType.POWERCODE:
			return powerServo;
		case ServoType.STEERINGCODE:
			return steeringServo;
		default:
			return 0;
		}
	}

}
