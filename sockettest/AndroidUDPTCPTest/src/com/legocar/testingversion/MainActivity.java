package com.legocar.testingversion;

import com.legocar.testingversion.utils.CommunicationUtils;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity {

	public static Setting settings;

	EditText ipText;
	EditText portText;
	EditText defaultMessageDelay;
	EditText powerMiddleValueText;
	EditText powerOffsetText;
	EditText steeringMiddleValueText;
	EditText steeringOffsetText;
	EditText powerServoText;
	EditText steeringServoText;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		settings = new Setting();
	}

	@Override
	protected void onStart() {
		super.onStart();
		
		ipText = (EditText) findViewById(R.id.carIP);
		portText = (EditText) findViewById(R.id.carPort);
		defaultMessageDelay = (EditText) findViewById(R.id.defaultMessageDelay);
		powerMiddleValueText = (EditText) findViewById(R.id.middlePowerValue);
		powerOffsetText = (EditText) findViewById(R.id.offsetPowerValue);
		steeringMiddleValueText = (EditText) findViewById(R.id.middleSteeringValue);
		steeringOffsetText = (EditText) findViewById(R.id.offsetSteeringValue);
		powerServoText = (EditText) findViewById(R.id.powerServoValue);
		steeringServoText = (EditText) findViewById(R.id.steeringServoValue);
		
		ipText.setText(Setting.ip);
		portText.setText(Setting.port+"");
		defaultMessageDelay.setText(Setting.defaultMessageDelay+"");
		powerMiddleValueText.setText(Setting.powerMiddleValue+"");
		powerOffsetText.setText(Setting.powerOffset+"");
		steeringMiddleValueText.setText(Setting.steeringMiddleValue+"");
		steeringOffsetText.setText(Setting.steeringOffset+"");
		powerServoText.setText(Setting.powerServo+"");
		steeringServoText.setText(Setting.steeringServo+"");
	};

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	/** Called when the user clicks the Send by UDP button */
	public void sendUDPMessage(View view) {
		EditText editText = (EditText) findViewById(R.id.carIP);
		String message = editText.getText().toString() + "\n";
		CommunicationUtils.sendUDPMessage(message);
	}

	/** Called when the user clicks the Send by TCP button */
	public void sendTCPMessage(View view) {
		EditText editText = (EditText) findViewById(R.id.carIP);
		String message = editText.getText().toString() + "\n";
		CommunicationUtils.sendTCPMessage(message);

	}

	public void startDriving(View view) {
		Intent intent = new Intent(this, DrivingActivity.class);
		setup();
		startActivity(intent);
	}

	private void setup() {
		//		EditText ipText = (EditText) findViewById(R.id.carIP);
		//		EditText portText = (EditText) findViewById(R.id.carPort);
		//		EditText defaultMessageDelay = (EditText) findViewById(R.id.defaultMessageDelay);
		//		EditText powerMiddleValueText = (EditText) findViewById(R.id.middlePowerValue);
		//		EditText powerOffsetText = (EditText) findViewById(R.id.offsetPowerValue);
		//		EditText steeringMiddleValueText = (EditText) findViewById(R.id.middleSteeringValue);
		//		EditText steeringOffsetText = (EditText) findViewById(R.id.offsetSteeringValue);
		//		EditText powerServoText = (EditText) findViewById(R.id.powerServoValue);
		//		EditText steeringServoText = (EditText) findViewById(R.id.steeringServoValue);

		String ip = ipText.getText().toString();
		String port = portText.getText().toString();
		String defaultMessageDelayValue = defaultMessageDelay.getText().toString(); 
		String powerMiddleValue = powerMiddleValueText.getText().toString();
		String powerOffset = powerOffsetText.getText().toString();
		String steeringMiddleValue = steeringMiddleValueText.getText()
				.toString();
		String steeringOffset = steeringOffsetText.getText().toString();
		String powerServo = powerServoText.getText().toString();
		String steeringServo = steeringServoText.getText().toString();

		settings.set(ip, port, defaultMessageDelayValue, powerMiddleValue, powerOffset,
				steeringMiddleValue, steeringOffset, powerServo, steeringServo);

	}

}
