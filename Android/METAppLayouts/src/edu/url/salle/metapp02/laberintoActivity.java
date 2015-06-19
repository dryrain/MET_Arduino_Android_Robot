package edu.url.salle.metapp02;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;

import edu.url.salle.metapp02.remotoActivity.SocketListener;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Toast;
/**
*Esta clase es la encargada de mostrar el camino que sigue nuestro robot para resolver 
*el laberinto, ademas tambien mostrara los lugares no explorados y los caminos no validos
*con ayuda de un conjunto de imageView.

*/
public class laberintoActivity extends Activity{
	
	//
	 ImageView a1,a2,a3,a4,a5;
	 ImageView b1,b2,b3,b4,b5;
	 ImageView c1,c2,c3,c4,c5;
	 ImageView d1,d2,d3,d4,d5;
	 ImageView e1,e2,e3,e4,e5;
	 ImageView pa1,pa2,pa3,pa4;
	 ImageView pb1,pb2,pb3,pb4;
	 ImageView pc1,pc2,pc3,pc4;
	 ImageView pd1,pd2,pd3,pd4;
	 ImageView pe1,pe2,pe3,pe4;
	 
	 ImageView pa11,pa12,pa13,pa14,pa15,pa16,pa17,pa18,pa19;
	 ImageView pb11,pb12,pb13,pb14,pb15,pb16,pb17,pb18,pb19;
	 ImageView pc11,pc12,pc13,pc14,pc15,pc16,pc17,pc18,pc19;
	 ImageView pd11,pd12,pd13,pd14,pd15,pd16,pd17,pd18,pd19;
	 ImageView pe11,pe12,pe13,pe14,pe15,pe16,pe17,pe18,pe19;
	 
	 ImageView[][] IMGS= new ImageView[9][9];
	 
	 String log;
	 
	 Thread t1;
	 
	 Activity activityTestLab;
	 
	 String rxPacket;
	 
	int [][]matriz= new int[9][9];
	 
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.laberinto);
        
        t1= new Thread (new SocketListener ());
        
        t1.start();
        activityTestLab=this;
        ImageButton volver;
        ImageButton play,sol,rec,desc;
        
        a1 = (	ImageView) findViewById(R.id.butm1);
		a2 = (	ImageView) findViewById(R.id.butm2);
		a3 = (	ImageView) findViewById(R.id.butm3);
		a4 = (	ImageView) findViewById(R.id.butm4);
		a5 = (	ImageView) findViewById(R.id.butm5);
		b1 = (	ImageView) findViewById(R.id.butm6);
		b2 = (	ImageView) findViewById(R.id.butm7);
		b3 = (	ImageView) findViewById(R.id.butm8);
		b4 = (	ImageView) findViewById(R.id.butm9);
		b5 = (	ImageView) findViewById(R.id.butm10);
		c1 = (	ImageView) findViewById(R.id.butm11);
		c2 = (	ImageView) findViewById(R.id.butm12);
		c3 = (	ImageView) findViewById(R.id.butm13);
		c4 = (	ImageView) findViewById(R.id.butm14);
		c5 = (	ImageView) findViewById(R.id.butm15);
		d1 = (	ImageView) findViewById(R.id.butm16);
		d2 = (	ImageView) findViewById(R.id.butm17);
		d3 = (	ImageView) findViewById(R.id.butm18);
		d4 = (	ImageView) findViewById(R.id.butm19);
		d5 = (	ImageView) findViewById(R.id.butm20);
		e1 = (	ImageView) findViewById(R.id.butm21);
		e2 = (	ImageView) findViewById(R.id.butm22);
		e3 = (	ImageView) findViewById(R.id.butm23);
		e4 = (	ImageView) findViewById(R.id.butm24);
		e5 = (	ImageView) findViewById(R.id.butm25);
		
		
		pa1 = (	ImageView) findViewById(R.id.pared1);
		pa2 = (	ImageView) findViewById(R.id.pared2);
		pa3 = (	ImageView) findViewById(R.id.pared3);
		pa4 = (	ImageView) findViewById(R.id.pared4);
		
		pa11 = (	ImageView) findViewById(R.id.pared5);
		pa12 = (	ImageView) findViewById(R.id.paredC1);
		pa13 = (	ImageView) findViewById(R.id.pared6);
		pa14 = (	ImageView) findViewById(R.id.paredC2);
		pa15 = (	ImageView) findViewById(R.id.pared7);
		pa16 = (	ImageView) findViewById(R.id.paredC3);
		pa17 = (	ImageView) findViewById(R.id.pared8);
		pa18 = (	ImageView) findViewById(R.id.paredC4);
		pa19 = (	ImageView) findViewById(R.id.pared9);
		
		
		pb1 = (	ImageView) findViewById(R.id.pared10);
		pb2 = (	ImageView) findViewById(R.id.pared11);
		pb3 = (	ImageView) findViewById(R.id.pared12);
		pb4 = (	ImageView) findViewById(R.id.pared13);
		
		
		pb11 = (	ImageView) findViewById(R.id.pared14);
		pb12 = (	ImageView) findViewById(R.id.paredC5);
		pb13 = (    ImageView) findViewById(R.id.pared15);
		pb14 = (	ImageView) findViewById(R.id.paredC6);
		pb15 = (	ImageView) findViewById(R.id.pared16);
		pb16 = (	ImageView) findViewById(R.id.paredC7);
		pb17 = (	ImageView) findViewById(R.id.pared17);
		pb18 = (	ImageView) findViewById(R.id.paredC8);
		pb19 = (	ImageView) findViewById(R.id.pared18);
		
		
		pc1 = (	ImageView) findViewById(R.id.pared19);
		pc2 = (	ImageView) findViewById(R.id.pared20);
		pc3 = (	ImageView) findViewById(R.id.pared21);
		pc4 = (	ImageView) findViewById(R.id.pared22);
		
		
		pc11 = (	ImageView) findViewById(R.id.pared23);
		pc12 = (	ImageView) findViewById(R.id.paredC9);
		pc13 = (	ImageView) findViewById(R.id.pared24);
		pc14 = (	ImageView) findViewById(R.id.paredC10);
		pc15 = (	ImageView) findViewById(R.id.pared25);
		pc16 = (	ImageView) findViewById(R.id.paredC11);
		pc17 = (	ImageView) findViewById(R.id.pared26);
		pc18 = (	ImageView) findViewById(R.id.paredC12);
		pc19 = (	ImageView) findViewById(R.id.pared27);
		
		
		pd1 = (	ImageView) findViewById(R.id.pared28);
		pd2 = (	ImageView) findViewById(R.id.pared29);
		pd3 = (	ImageView) findViewById(R.id.pared30);
		pd4 = (	ImageView) findViewById(R.id.pared31);
		
		
		pd11 = (	ImageView) findViewById(R.id.pared32);
		pd12 = (	ImageView) findViewById(R.id.paredC13);
		pd13 = (	ImageView) findViewById(R.id.pared33);
		pd14 = (	ImageView) findViewById(R.id.paredC14);
		pd15 = (	ImageView) findViewById(R.id.pared34);
		pd16 = (	ImageView) findViewById(R.id.paredC15);
		pd17 = (	ImageView) findViewById(R.id.pared35);
		pd18 = (	ImageView) findViewById(R.id.paredC16);
		pd19 = (	ImageView) findViewById(R.id.pared36);
		
		
		
		pe1 = (	ImageView) findViewById(R.id.pared37);
		pe2 = (	ImageView) findViewById(R.id.pared38);
		pe3 = (	ImageView) findViewById(R.id.pared39);
		pe4 = (	ImageView) findViewById(R.id.pared40);

		IMGS[0][0]=a1;
		IMGS[0][1]=pa1;
		IMGS[0][2]=a2;
		IMGS[0][3]=pa2;
		IMGS[0][4]=a3;
		IMGS[0][5]=pa3;
		IMGS[0][6]=a4;
		IMGS[0][7]=pa4;
		IMGS[0][8]=a5;
		
		IMGS[1][0]=pa11;
		IMGS[1][1]=pa12;
		IMGS[1][2]=pa13;
		IMGS[1][3]=pa14;
		IMGS[1][4]=pa15;
		IMGS[1][5]=pa16;
		IMGS[1][6]=pa17;
		IMGS[1][7]=pa18;
		IMGS[1][8]=pa19;
		
		IMGS[2][0]=b1;
		IMGS[2][1]=pb1;
		IMGS[2][2]=b2;
		IMGS[2][3]=pb2;
		IMGS[2][4]=b3;
		IMGS[2][5]=pb3;
		IMGS[2][6]=b4;
		IMGS[2][7]=pb4;
		IMGS[2][8]=b5;
		
		IMGS[3][0]=pb11;
		IMGS[3][1]=pb12;
		IMGS[3][2]=pb13;
		IMGS[3][3]=pb14;
		IMGS[3][4]=pb15;
		IMGS[3][5]=pb16;
		IMGS[3][6]=pb17;
		IMGS[3][7]=pb18;
		IMGS[3][8]=pb19;
		
		IMGS[4][0]=c1;
		IMGS[4][1]=pc1;
		IMGS[4][2]=c2;
		IMGS[4][3]=pc2;
		IMGS[4][4]=c3;
		IMGS[4][5]=pc3;
		IMGS[4][6]=c4;
		IMGS[4][7]=pc4;
		IMGS[4][8]=c5;
		
		IMGS[5][0]=pc11;
		IMGS[5][1]=pc12;
		IMGS[5][2]=pc13;
		IMGS[5][3]=pc14;
		IMGS[5][4]=pc15;
		IMGS[5][5]=pc16;
		IMGS[5][6]=pc17;
		IMGS[5][7]=pc18;
		IMGS[5][8]=pc19;
		
		IMGS[6][0]=d1;
		IMGS[6][1]=pd1;
		IMGS[6][2]=d2;
		IMGS[6][3]=pd2;
		IMGS[6][4]=d3;
		IMGS[6][5]=pd3;
		IMGS[6][6]=d4;
		IMGS[6][7]=pd4;
		IMGS[6][8]=d5;
		
		IMGS[7][0]=pd11;
		IMGS[7][1]=pd12;
		IMGS[7][2]=pd13;
		IMGS[7][3]=pd14;
		IMGS[7][4]=pd15;
		IMGS[7][5]=pd16;
		IMGS[7][6]=pd17;
		IMGS[7][7]=pd18;
		IMGS[7][8]=pd19;
		
		IMGS[8][0]=e1;
		IMGS[8][1]=pe1;
		IMGS[8][2]=e2;
		IMGS[8][3]=pe2;
		IMGS[8][4]=e3;
		IMGS[8][5]=pe3;
		IMGS[8][6]=e4;
		IMGS[8][7]=pe4;
		IMGS[8][8]=e5;
		
		
		
		
    	volver = (ImageButton) findViewById(R.id.button10);
    	volver.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

                // Perform action on click
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
    			//toast1.show();
        	   volver(null);
    		
           }

        });
    	
    	play = (ImageButton) findViewById(R.id.button6);
    	play.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

        	   sendData();
        	   
                // Perform action on click
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Play ", Toast.LENGTH_SHORT);
    			//toast1.show();
        	   pintar();
        	   
           }

        });
    	/*
    	sol = (ImageButton) findViewById(R.id.button11);
    	sol.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

        	   //sendData();
        	   
                // Perform action on click
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Play ", Toast.LENGTH_SHORT);
    			//toast1.show();
        	   pintar();
    		
           }

        });
    	rec = (ImageButton) findViewById(R.id.button12);
    	rec.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

        	   //sendData();
        	   
                // Perform action on click
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Play ", Toast.LENGTH_SHORT);
    			//toast1.show();
        	   pintar();
    		
           }

        });
    	
    	desc = (ImageButton) findViewById(R.id.button13);
    	desc.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

        	   //sendData();
        	   
                // Perform action on click
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Play ", Toast.LENGTH_SHORT);
    			//toast1.show();
        	   pintar();
    			
        	   
           }

        });
    	*/
    	sendData();
    	
    }
	
	
	
	/**
	*La funcion volver ejecutara un intent que nos permitira volver al main principal en cual 
	*se encuentra la pantalla de seleccion de modo.
	*/
	
    	public void volver(View view) {
    		
    		Intent intent = new Intent(this, 
                   MainActivity.class);
    		log=" Tx: XMODLA";
    		ModificarRegistro(); // Esto no iria aqui sino cuando se haga un SendData()
    startActivity(intent);
    t1.interrupt();
    finish();

    }
    	
    /**
    *La funcion onKeyDown nos permite capturar la accion de tocar el boton volver de android, configurando las 
    *acciones que deseamos que lleve a cabo. En nuestro caso queriamos volver a la activity anterior y ademas que
    *destruya la actvity que se esta ejecutando.
    */ 	
    	
    	
    	@Override
    	public boolean onKeyDown(int keyCode, KeyEvent event)
    	{
    		if ((keyCode == KeyEvent.KEYCODE_BACK))
    		{
    			//tramaType='X'; //Kill the arduino process
    			sendData();	
    			volver(null);
    			return true;

    		}
    		
    		return super.onKeyDown(keyCode, event);
    	}   
    	
    	
    	
    	
    	
    	public void pintar() {
 		   
    		   a1.setBackgroundColor(0xFF3333FF);
        	   a2.setBackgroundColor(0xFF3333FF);
        	   b2.setBackgroundColor(0xFF3333FF);
        	   c2.setBackgroundColor(0xFF3333FF);
        	   c3.setBackgroundColor(0xFF3333FF);
        	   c4.setBackgroundColor(0xFF3333FF);
        	   e4.setBackgroundColor(0xFF3333FF);
        	   e5.setBackgroundColor(0xFF3333FF);
    			

    }  
    	

    	public void pintarsolucion() {
 		   /*
    		for(int i = 0; i < 9; i++) {
        		  for(int j =0; j < 9; j++) {
        			 if(matriz[i-1][j-1]==3){
        		    IMGS[i-1][j-1].setBackgroundColor(Color.GREEN);
        			 
        		  }else{
        				IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);	
        				
        			}
        		  }
        		}*/
    			

    }  
    	
    	public void pintarrecorrido() {
  		  /* 
    		for(int i = 0; i < 9; i++) {
        		  for(int j =0; j < 9; j++) {
        			 if(matriz[i-1][j-1]==1){
        		    IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);
        			}else{
        				IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);	
        				
        			}
        		  }
        		}
 			
*/
    		}
    	public void pintardescartados() {
	   /*
    		for(int i = 0; i < 9; i++) {
        		  for(int j =0; j < 9; j++) {
        			 if(matriz[i-1][j-1]==2){
        		    IMGS[i-1][j-1].setBackgroundColor(Color.RED);
        			 }else{
        				IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);	
        				
        			}
        		  }
        		}
		
*/
    		} 
    	/**
       	*La clase parseRXstring es la encargada de traducir la trama recibida por parte 
       	*del sistema arduino y actualizar el estado de nuestros elementos UI como las ImageViews
       	*que nos muestran el camino que esta siguiendo nuestro robot para resolver el laberinto al
       	*que se enfrenta.
       	*@param String data: Es la trama recibida del sistema arduino
       	*@return Retorna False por defecto independientemente de las acciones que realice.
       	*/
    	private boolean parseRXstring(String data)
        {
        	
        	//C34N0M0N
        	//Type-Temp1-Temp0-Led-Colision-M/Auto-Velocidad
    		
        	String type = data.substring(0,1);
        	int count=0;
        	 
        	for(int i = 1; i < 9; i++) {
        		  for(int j =1; j < 9; j++) {
        			  
        		    matriz[i][j] = Integer.parseInt(data.substring(count,count+1));  
        		    count++;
        		    
        		    
        		  }
        		}
        	 System.out.println ("DENTRO1");
        	for(int i = 0; i < 9; i++) {
      		  for(int j =0; j < 9; j++) {
      			 if(matriz[i-1][j-1]==1){
      		    IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);
      			 }
      			else if (matriz[i-1][j-1]==2){
      		    IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);	
      				
      			}else if (matriz[i-1][j-1]==9){
      				IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);	
      				
      			}else{
      				IMGS[i-1][j-1].setBackgroundColor(Color.BLACK);	
      				
      			}
      		  }
      		}
        	
        	
        	 //System.out.println ("DENTRO");
        	 //System.out.println (temperature);
        	//Change temperature
        	
        	
        	 
        	
        	
    		return false;
        }	
    	
    	
    	
    	/**
    	*La clase SendData establece un socket de emision para poder enviar tramas
    	*desde el sistema Android al robot arduino.Ademas realizara el parseado de la
    	*informacion en un formato que sea entendido por nuestro robot.Para poder realizar 
    	*el envio se debera crea un thread. Se utilizara como puerto emisor 55056
    	*/
    	
    	public boolean sendData(){
    		try {
    			final DatagramSocket socket = new DatagramSocket ();
    			
    			socket.setReuseAddress(true);
    			InetAddress address;
    			
    			address = InetAddress.getByName ("172.20.10.9");
    			//address = InetAddress.getByName ("192.168.43.214");
    			
    			byte[] buf = new byte[256];
    	        String s = "L"; //TESTING
    	        buf = s.getBytes ();
    			        
    	        final DatagramPacket packet = new DatagramPacket (buf, buf.length, address, 55056);
    			
    	        new Thread ()
                {
                      public void run ()
                      {
                            try
                            {
                                 System.out.println ("About to send message");
                                  socket.send (packet);
                                  System.out.println ("Sent message");
                            }
                            catch (IOException e1)
                            {
                                  // TODO Auto-generated catch block
                                  e1.printStackTrace();
                            }
                            socket.close();
                      }

                }.start ();
    	        
                return true;
                
    		}catch (SocketException e) {e.printStackTrace();
    		}catch (UnknownHostException e) {e.printStackTrace();
    		}catch (IOException e) {e.printStackTrace();
    		}      
    		return false;
    	}
    	
    	
    	
    	
    	
    	/**
    	*La clase SocketListener establece un socket de recepcion para poder recibir las tramas
    	*procedentes del robot arduino en el sistema Android. Se utilizara como puerto receptor 
    	*el 4560
    	*/  	
    	class SocketListener implements Runnable
        {
              String str;
              public void run()
              {
                    DatagramSocket socket;
                    DatagramPacket packet;
                    byte[] buf = new byte[2];
                    System.out.println ("Thread running");
                    try
                    {
                          socket = new DatagramSocket (4561);
                          //socket.setReuseAddress(true);
                    	 // socket = new DatagramSocket (55056);
                          while (true)
                          {                          
                                packet = new DatagramPacket (buf,2);
                                socket.receive (packet);
                                System.out.println ("Received packet");
                                rxPacket = new String (packet.getData());
                                
                                System.out.println (rxPacket);
                                
                              activityTestLab.runOnUiThread(new Runnable(){
                                	public void run(){
                                		//parseRXstring(rxPacket);
                                		log=" Rx: "+rxPacket;
                                		ModificarRegistro();
                                		
                                		
                                	}
                                		});
                                
                                
                                
                               // String s = new String (packet.getData());
                                //Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                          }
                    }
                    catch (IOException e)
                    {
                          Log.e(getClass().getName(), e.getMessage());
                    }
              }
        }
       
    	
    	
    	
    	public boolean ModificarRegistro(){
    		try
    		{
    		    OutputStreamWriter fout=
    		        new OutputStreamWriter(
    		            openFileOutput("prueba_int.txt", Context.MODE_APPEND));
    		    Date curDate = new Date();
    		    SimpleDateFormat formato = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    		    
    		    String hora = formato.format(curDate);
    		    String esp;
    		    //esp=hora+" Tx: ";
    		    esp=hora;
    		    String cab=esp+log;
    		    cab=cab+"                       \n\n";
    		    
    		    
    		    fout.write(cab+"\n");
    		    fout.close();
    		}
    		catch (Exception ex)
    		{
    		    Log.e("Ficheros", "Error al escribir fichero a memoria interna");
    		}
    		return false;
    		
    		
    		
    		
    	}	



}



