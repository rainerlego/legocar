package com.legocar.testingversion;

import java.io.IOException;

import com.legocar.testingversion.communication.TCPClient;
import com.legocar.testingversion.communication.TCPConnectTask;
import com.legocar.testingversion.graphics2D.ControlView;
import com.legocar.testingversion.graphics2D.SpeedView;
import com.legocar.testingversion.graphics2D.SteeringView;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import android.support.v4.app.NavUtils;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;

public class DrivingActivity extends Activity implements SensorEventListener{
  private SensorManager sensorManager;
  
  private Sensor gravitySensor;
  
  private TCPClient tClient;
  
  private double zero = 0.0;
  
  private double realAngular=0.0;
  
  private boolean direction=true;

  
  Setting settings;
  
  SpeedView speedView;
  
  TextView speedText;
  
  SteeringView steeringView;
  
  TextView steeringText;

	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		 getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.activity_driving);
		// Show the Up button in the action bar.
		setupActionBar();
		settings = MainActivity.settings;
		tClient = new TCPClient(settings.getIp(), settings.getPort());
		sensorManager= (SensorManager) getSystemService(Context.SENSOR_SERVICE);
	    gravitySensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		
	}
	
	@Override
		protected void onStop() {
			// TODO Auto-generated method stub
			super.onStop();
		}

	@Override
		protected void onDestroy() {
			// TODO Auto-generated method stub
			stop();
			super.onDestroy();
		}

	/**
	 * Set up the {@link android.app.ActionBar}, if the API is available.
	 */
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	private void setupActionBar() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			getActionBar().setDisplayHomeAsUpEnabled(true);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.driving, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			// This ID represents the Home or Up button. In the case of this
			// activity, the Up button is shown. Use NavUtils to allow users
			// to navigate up one level in the application structure. For
			// more details, see the Navigation pattern on Android Design:
			//
			// http://developer.android.com/design/patterns/navigation.html#up-vs-back
			//
			NavUtils.navigateUpFromSameTask(this);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {}

	@Override
	public void onSensorChanged(SensorEvent event) {
		float[] values = event.values;
	    // Movement
	    float x = values[0];
	    float y = values[1];
	    
	    //FIXME care
	    realAngular = Math.atan(y/x);
	    double angular = realAngular-zero;
	    Log.i("Steering", angular+"");
	    if(angular>Math.PI/4){
	    	getSteeringView().updatePercentage((float)100.0, direction);
	    }else if(angular<-Math.PI/4){
	    	getSteeringView().updatePercentage((float)-100.0, direction);	    	
	    }else{
	    	getSteeringView().updatePercentage((float) (400*angular/Math.PI), direction);
	    }
		    
	}

	@Override
		public boolean onTouchEvent(MotionEvent event) {
			// Do like in SpeedView
			return super.onTouchEvent(event);
		}
    
    /** Called when the user clicks the Send by reset steering button */
	public void steeringReset(View view) {
		zero=realAngular;
		getSteeringView().updatePercentage((float) 0.0);
	}
	
	/** Called when the user clicks the Send by start button */
	public void connet(View view) {
        sensorManager.registerListener(this, gravitySensor, SensorManager.SENSOR_DELAY_FASTEST);

        new TCPConnectTask().execute(tClient);
		startControl();	
        
		getSpeedView().updatePercentage(0);
		getSteeringView().updatePercentage((float) 0.0);

	}
	
	/** Called when the user clicks the Send by start button 
	 * @throws IOException */
	public void getperm(View view) throws IOException {
		tClient.sendMessage("servo getperm");
	}

	/** Called when the user clicks the Send by stop button */
	public void disconnet(View view) {	
		stop();
	}
	
	private void stop(){
		getSpeedView().updatePercentage(0);
		getSteeringView().updatePercentage((float) 0.0);

		try {
			tClient.stop();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		sensorManager.unregisterListener(this);
		realAngular = 0.0;
	}

	private void startControl() {
		Log.i("TODO", "startControl");
		//Speed control
	    speedText = (TextView) findViewById(R.id.powerValueTextViewer);
	    setupConnection(getSpeedView(), speedText, ServoType.POWER);	    

	    //Steering control
	    steeringText = (TextView) findViewById(R.id.steeringValueTextView);
	    setupConnection(getSteeringView(), steeringText, ServoType.STEERING);   
	}

	private void setupConnection(ControlView view, TextView textView, ServoType servo) {	
		view.setDisplay(textView);
		view.setTClient(tClient);
		view.setDefaultMessageDelay(settings.getDefaultMessageDelay());
		
		view.setMiddleValue(settings.getMiddleValue(servo));
		view.setOffset(settings.getOffset(servo));
		view.setServo(settings.getServo(servo));
	}
	
	private SteeringView getSteeringView(){
		if (steeringView==null){
		    steeringView = (SteeringView) findViewById(R.id.steeringView);
		}
		return steeringView;
	}
	
	private SpeedView getSpeedView(){
		if (speedView==null){
			speedView = (SpeedView) findViewById(R.id.speedView);
		}
		
		return speedView;
	}
	
	/** Called when the user clicks the Send by direction button */
	public void changeDirection(View view) {	
		direction=!direction;
	}
}
