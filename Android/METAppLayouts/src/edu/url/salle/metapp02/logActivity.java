package edu.url.salle.metapp02;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

public class logActivity extends Activity{
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.log);

	ImageButton volver;
	
	volver = (ImageButton) findViewById(R.id.button10);
	volver.setOnClickListener(new View.OnClickListener() {
       public void onClick(View v) {

    	   
            // Perform action on click
        	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
			toast1.show();
    	   volver(null);
			
			
       // 	(null);
       }

    });
	try {
        Process process = Runtime.getRuntime().exec("logcat -d");
        BufferedReader bufferedReader = new BufferedReader(
        new InputStreamReader(process.getInputStream()));

        StringBuilder log=new StringBuilder();
        String line = ""; 
        
        while ((line = bufferedReader.readLine()) != null) {
          log.append(line+"\n");
        }   
        TextView tv = (TextView)findViewById(R.id.text);
        tv.setText(log.toString());
      } catch (IOException e) {
    }
}
	
	
	  
	private CharSequence toString(String line) {
		// TODO Auto-generated method stub
		return null;
	}



	public void volver(View view) {
		   
		Intent intent = new Intent(this, 
               MainActivity.class);
		
startActivity(intent);

}
	
	
	
	
	class SocketListener implements Runnable
    {
          String str;
          public void run()
          {
                DatagramSocket socket;
                DatagramPacket packet;
                byte[] buf = new byte[256];
                System.out.println ("Thread running");
                try
                {
                      //socket = new DatagramSocket (4560);
                	  socket = new DatagramSocket (55056);
                      while (true)
                      {                          
                            packet = new DatagramPacket (buf, buf.length);
                            socket.receive (packet);
                            System.out.println ("Received packet");
                            String s = new String (packet.getData());
                            Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }
}
