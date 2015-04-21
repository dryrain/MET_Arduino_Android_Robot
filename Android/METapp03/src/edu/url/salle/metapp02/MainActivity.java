package edu.url.salle.metapp02;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity {
	
//	Handler myHandler;
//	Runnable intro;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Button mBoton_acelerar;
		Button mBoton_laberinto;
		Button mBoton_control;
		Button mBoton_log;
		
		mBoton_control = (Button) findViewById(R.id.button1);
		mBoton_control.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de control remoto ", Toast.LENGTH_SHORT);
				toast1.show();
			    Intent mainIntent = new Intent().setClass(MainActivity.this, WifiActivity.class);
                startActivity(mainIntent);
           // 	nuevaPantalla(null);
	       }
        });
		
		mBoton_laberinto = (Button) findViewById(R.id.button3);
		mBoton_laberinto.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de laberinto ", Toast.LENGTH_SHORT);
				toast1.show();
				Intent mainIntent = new Intent().setClass(MainActivity.this, WifiChatActivity.class);
                startActivity(mainIntent);
           // 	nuevaPantalla(null);
	       }
        });
		
		mBoton_acelerar = (Button) findViewById(R.id.button2);
		mBoton_acelerar.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de acelerometro ", Toast.LENGTH_SHORT);
				toast1.show();
           // 	nuevaPantalla(null);
	       }
        });
		
		mBoton_log = (Button) findViewById(R.id.button4);
		mBoton_log.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de log de comunicaciones ", Toast.LENGTH_SHORT);
				toast1.show();
           // 	nuevaPantalla(null);
	       }
        });
		
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);		
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
		
				
	}
	
	private boolean Splashscreen() {
		
		Toast toast1 =Toast.makeText(getApplicationContext(),"Splash screen", Toast.LENGTH_SHORT);
		toast1.show();
		return true;

	}
	
	
	
}



