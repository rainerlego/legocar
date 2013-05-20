package com.legocar.testingversion;

public class Setting {
	protected String ip;
	protected int port;
	protected int powerMiddleValue;
	protected int powerOffset;
	protected int steeringMiddleValue;
	protected int steeringOffset;
	protected int powerServo;
	protected int steeringServo;
	protected int defaultMessageDelay;
	
	public Setting() {
		// TODO Auto-generated constructor stub
	}

	public Setting(String ip, String port, String defaultMessageDelay){
		this();
		this.ip=ip;
		this.port=Integer.valueOf(port);
		this.defaultMessageDelay = Integer.valueOf(defaultMessageDelay);
	}

	public Setting(String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
		this();
		this.powerMiddleValue=Integer.valueOf(powerMiddleValue);
		this.powerOffset=Integer.valueOf(powerOffset);
		this.steeringMiddleValue=Integer.valueOf(steeringMiddleValue);
		this.steeringOffset=Integer.valueOf(steeringOffset);
		this.powerServo = Integer.valueOf(powerServo);
		this.steeringServo = Integer.valueOf(steeringServo);
	}

	public Setting(String ip, String port, String defaultMessageDelay, String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
		this(powerMiddleValue, powerOffset, steeringMiddleValue, steeringOffset, powerServo, steeringServo);
		this.ip=ip;
		this.port=Integer.valueOf(port);
		this.defaultMessageDelay = Integer.valueOf(defaultMessageDelay);

	}


	public void set(String ip, String port, String defaultMessageDelay){
		this.ip=ip;
		this.port=Integer.valueOf(port);
		this.defaultMessageDelay = Integer.valueOf(defaultMessageDelay);
	}

	public void set(String powerMiddleValue, String powerOffset, String steeringMiddleValue, String steeringOffset, String powerServo, String steeringServo){
		this.powerMiddleValue=Integer.valueOf(powerMiddleValue);
		this.powerOffset=Integer.valueOf(powerOffset);
		this.steeringMiddleValue=Integer.valueOf(steeringMiddleValue);
		this.steeringOffset=Integer.valueOf(steeringOffset);
		this.powerServo = Integer.valueOf(powerServo);
		this.steeringServo = Integer.valueOf(steeringServo);
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
