package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import javax.net.ssl.HandshakeCompletedListener;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Main extends Activity implements Handler.Callback {
	DatagramSocket socket;
	Context mContext;
	byte buf[];
	TextView text;
	Handler handler;
	Main instance;
	Intent intent;
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    setContentView(R.layout.main);
	    intent = new Intent(this, ClipboardService.class);
	    startService(intent);
	    
	    mContext = this;
	    buf = new byte[1024];
	    Button s = ((Button)findViewById(R.id.send));
	    Button r = ((Button)findViewById(R.id.receive));
	    Button o = ((Button)findViewById(R.id.off));
	    text = ((TextView)findViewById(R.id.text));
	    handler = new Handler(getMainLooper(), this);
	    instance = this;
	    try {
			socket = new DatagramSocket(1235);
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    o.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				instance.stop();
			}
		});
	    s.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				try {
					new Sender(socket, getBroadcastAddress(), "test").start();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}



			}
		});
	    r.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
					new Receiver(instance, socket).start();
			}
		});
	}
	
	public void stop() {
		stopService(intent);
	}
	
	public void send(final String msg) {
		handler.post(new Runnable() {
			
			@Override
			public void run() {
				text.setText(msg);
				
			}
		});
	}
	
	public boolean handleMessage(android.os.Message arg0) {
		if(arg0.what == 7) {
			text.setText((String)arg0.obj);
			return true;
		}
		return false;
	};
	
	InetAddress getBroadcastAddress() throws IOException {
	    WifiManager wifi = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
	    DhcpInfo dhcp = wifi.getDhcpInfo();
	    // handle null somehow

	    int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
	    byte[] quads = new byte[4];
	    for (int k = 0; k < 4; k++)
	      quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
	    Log.d("udp", InetAddress.getByAddress(quads).getHostAddress());
	    return InetAddress.getByAddress(quads);
	}
	
	class Sender extends Thread {
		DatagramSocket socket;
		InetAddress address;
		String data;
		
		public Sender(DatagramSocket socket, InetAddress adr, String msg) {
			this.socket = socket;
			address = adr;
			data = msg;
		}
		
		public void run() {
			try {
				socket.setBroadcast(true);
				byte buf[] = data.getBytes("UTF-8");
				DatagramPacket packet = new DatagramPacket(buf,
					buf.length, address, 1235);

					socket.send(packet);


			} catch (SocketException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
