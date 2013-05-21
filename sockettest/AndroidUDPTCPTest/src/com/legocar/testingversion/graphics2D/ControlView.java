package com.legocar.testingversion.graphics2D;

import static com.legocar.testingversion.utils.ServoCommunicationUtils.getServoControlMessage;

import java.io.IOException;

import com.legocar.testingversion.communication.TCPClient;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

public abstract class ControlView extends View{

	final static protected float LINEWIDTH = (float) 5.0;

	protected float percentage = 0;

	protected TextView display = null;

	protected TCPClient tClient = null;

	protected int middleValue=4000;

	protected int offset=3000;

	protected int servo=0;

	protected int defaultMessageDelay = 50;

	protected long lastTimeMillis=-1;

	protected long currentTimeMillis;


	public ControlView(Context context) {
		super(context);
	}

	public ControlView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}


	public void updatePercentage(float percentage){
		setPercentage(percentage);
		invalidate();

		currentTimeMillis=System.currentTimeMillis();
		if(lastTimeMillis<0 || currentTimeMillis-lastTimeMillis>defaultMessageDelay){
			lastTimeMillis = currentTimeMillis;

			if(display!=null){
				display.setText(String.format("%.02f", percentage)+"%");
			}
			if(tClient!=null){

				try {
					tClient.sendMessage(getServoControlMessage(servo, middleValue, offset, percentage));
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}	
		}
	}

	protected float getCanvasWidth(){
		return (float) (getWidth()-2.0*LINEWIDTH);
	}

	protected float getCanvasHeight(){
		return (float) (getHeight()-2.0*LINEWIDTH);
	}

	public float getPercentage() {
		return percentage;
	}


	protected void setPercentage(float percentage) {
		this.percentage = percentage;
	}

	public void setDisplay(TextView display) {
		this.display = display;
	}

	public void setTClient(TCPClient tClient) {
		this.tClient = tClient;
	}


	public void setMiddleValue(int middleValue) {
		this.middleValue = middleValue;
	}

	public void setOffset(int offset) {
		this.offset = offset;
	}

	public void setServo(int servo) {
		this.servo = servo;
	}

	public void setDefaultMessageDelay(int defaultMessageDelay){
		this.defaultMessageDelay=defaultMessageDelay;
	}


}
