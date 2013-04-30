package com.legocar.testingversion;

import com.legocar.testingversion.utils.CommunicationUtils;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    /** Called when the user clicks the Send by UDP button */
    public void sendUDPMessage(View view) {
        EditText editText = (EditText) findViewById(R.id.packageMessage);
        String message = editText.getText().toString()+"\n";
        CommunicationUtils.sendUDPMessage(message);
    }
    
    /** Called when the user clicks the Send by TCP button */
    public void sendTCPMessage(View view) {
        EditText editText = (EditText) findViewById(R.id.packageMessage);
        String message = editText.getText().toString()+"\n";
        CommunicationUtils.sendTCPMessage(message);
        
    }
    
}
