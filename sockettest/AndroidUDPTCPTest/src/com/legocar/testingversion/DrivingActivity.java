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
import android.widget.TextView;
import android.support.v4.app.NavUtils;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;

public class DrivingActivity extends Activity implements SensorEventListener{
  private SensorManager sensorManager;
  
  private Sensor gravitySensor;
  
  private TCPClient tClient;
  
  Setting settings;
  
  SpeedView speedView;
  
  TextView speedText;
  
  SteeringView steeringView;
  
  TextView steeringText;
  

	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_driving);
		// Show the Up button in the action bar.
		setupActionBar();
		settings = MainActivity.settings;
		tClient = new TCPClient(settings.getIp(), settings.getPort());
		sensorManager= (SensorManager) getSystemService(Context.SENSOR_SERVICE);
	    gravitySensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		
	}

	@Override
	protected void onStart() {
		super.onStart();
		new TCPConnectTask().execute(tClient);
		startControl();		
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		try {
			tClient.stop();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void startControl() {
		Log.i("TODO", "startControl");
		//Speed control
	    speedView = (SpeedView) findViewById(R.id.speedView);	    
	    speedText = (TextView) findViewById(R.id.textViewPowerValue);
	    setupConnection(speedView, speedText, ServoType.POWER);	    

	    //Steering control
	    steeringView = (SteeringView) findViewById(R.id.steeringView);
	    steeringText = (TextView) findViewById(R.id.textViewSteeringValue);
	    setupConnection(steeringView, steeringText, ServoType.STEERING);

	}

	private void setupConnection(ControlView view, TextView textView, ServoType servo) {
		view.setDisplay(textView);
		view.setTClient(tClient);
		view.setDefaultMessageDelay(settings.getDefaultMessageDelay());
		
		view.setMiddleValue(settings.getMiddleValue(servo));
		view.setOffset(settings.getOffset(servo));
		view.setServo(settings.getServo(servo));
		
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
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		float[] values = event.values;
	    // Movement
	    float x = values[0];
	    float z = values[2];
	    double angular = Math.atan(x/z);
	    Log.i("Steering", angular+"");
	    if(Math.abs(angular)>Math.PI/4){
	    	return;
	    }
	    steeringView.updatePercentage((float) (400*angular/Math.PI));
		    
	}


	protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this, gravitySensor, SensorManager.SENSOR_DELAY_FASTEST);
    }

    protected void onPause() {
        super.onPause();
        sensorManager.unregisterListener(this);
    }
    
}
