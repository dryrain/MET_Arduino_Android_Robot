package edu.url.salle.metapp02;




import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

/**
 *La clase MainActivity sera la que nos va a permitir el acesso a los distintos modos de funcionamiento 
 *que tiene el robot y que son los siguientes:
 *- Control Remoto
 *- Reto del Acelerometro
 *- Laberinto
 *- Log de comunicacion

 */



public class MainActivity extends Activity {

	//	Handler myHandler;
	//	Runnable intro;
	
	
static int i=0;
String cab;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		//	Runnable r=new Runable(){Splashscreen();};
		//	myHandler.postDelayed(intro., 250);
		//	myHandler.postDelayed(null, 2000);
		//	myHandler.postDelayed(new Runnable() { public void run(){  }}, 2050);

		Button mBoton_acelerar;
		Button mBoton_laberinto;
		Button mBoton_control;
		Button mBoton_log;
		if(i==0){
	
        CrearRegistro();
        i++;
        }
        
		mBoton_control = (Button) findViewById(R.id.button1);
		mBoton_control.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {


				// Perform action on click
				Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de control remoto ", Toast.LENGTH_SHORT);
				toast1.show();
				pasarremoto(null);


				// 	(null);
			}

		});
		
		mBoton_laberinto = (Button) findViewById(R.id.button3);
		mBoton_laberinto.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// Perform action on click
				Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de laberinto ", Toast.LENGTH_SHORT);
				toast1.show();
				pasarlaberinto(null);
			}
		});

		mBoton_acelerar = (Button) findViewById(R.id.button2);
		mBoton_acelerar.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// Perform action on click
				Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de acelerometro ", Toast.LENGTH_SHORT);
				toast1.show();
				pasaraccel(null);

			}
		});

		mBoton_log = (Button) findViewById(R.id.button4);
		mBoton_log.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// Perform action on click
				Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada de log de comunicaciones ", Toast.LENGTH_SHORT);
				toast1.show();
				pasarlog(null);
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

	/**
	 *La funcion pasarremoto ejecutara un intent que nos permitira acceder al modo de funcionamiento
	 *de control remoto.
	 */	
	public void pasarremoto(View view) {

		Intent intent = new Intent(this, 
				remotoActivity.class);

		startActivity(intent);
		finish();

	}
	/**
	 *La funcion pasaraccel ejecutara un intent que nos permitira acceder al modo de funcionamiento
	 *del reto del acelerometro.
	 */
	public void pasaraccel(View view) {

		Intent intent = new Intent(this, 
				accelActivity.class);
		startActivity(intent);
		
			finish();
			
			
			
	

	}

	/**
	 *La funcion pasarlog ejecutara un intent que nos permitira acceder al modo de funcionamiento
	 *de log de comunicacion.
	 */


	public void pasarlog(View view) {

		Intent intent = new Intent(this, 
				logActivity.class);
		startActivity(intent);
		finish();

	}
	/**
	 *La funcion pasarlaberinto ejecutara un intent que nos permitira acceder al modo de funcionamiento
	 *de laberinto.
	 */
	public void pasarlaberinto(View view) {

		Intent intent = new Intent(this, 
				laberintoActivity.class);
		startActivity(intent);
		finish();

	}	

/**
 * 
 */
public boolean CrearRegistro(){
	try
	{
	    OutputStreamWriter fout=
	        new OutputStreamWriter(
	            openFileOutput("prueba_int.txt", Context.MODE_PRIVATE));
	   
	    Date curDate = new Date();
	    SimpleDateFormat formato = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	    
	    String hora = formato.format(curDate);
	    cab=hora+": Inicio aplicacion ";
	    cab=cab+"                             \n\n";
	    fout.write(cab);
	    fout.close();
	}
	catch (Exception ex)
	{
	    Log.e("Ficheros", "Error al escribir fichero a memoria interna");
	}
	return false;
	
	
	
	
}
/*
public boolean LeerRegitro(){
try
{
    BufferedReader fin =
        new BufferedReader(
            new InputStreamReader(
                openFileInput("prueba_int.txt")));
 
    String texto = fin.readLine();
    System.out.println (texto);
    fin.close();
}
catch (Exception ex)
{
    Log.e("Ficheros", "Error al leer fichero desde memoria interna");
}
return false;
}
*/
}



