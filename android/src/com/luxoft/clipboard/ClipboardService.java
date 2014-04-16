package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.app.Service;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.IBinder;
import android.util.Log;

public class ClipboardService extends Service {

	private static String LOG_NAME = "service";
	
	private DatagramSocket socket;
	private InetAddress address;
	private InetAddress local;
	private boolean own = false;
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(LOG_NAME, "startCommand");
		try {
			socket = new DatagramSocket(1234);
			address = getBroadcastAddress();
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


		final ClipboardManager clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
		new Receiver(socket, clipboard).start();
		clipboard.addPrimaryClipChangedListener(new ClipboardManager.OnPrimaryClipChangedListener() {
			
			@Override
			public void onPrimaryClipChanged() {
				Log.d(LOG_NAME, "cliboard changed");
				if(getOwn()) {
					setOwn(false);
					return;
				}
				Log.d(LOG_NAME, "cliboard: "+((String)clipboard.getText()));
				Log.d(LOG_NAME, "label: "+clipboard.getPrimaryClipDescription().getLabel());
				Log.d(LOG_NAME, "mime: "+clipboard.getPrimaryClipDescription().getMimeType(0));
				new Sender(socket, address, (String)clipboard.getText()).start();
				setOwn(false);
			}
		});
		return super.onStartCommand(intent, flags, startId);
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
					buf.length, address, 1234);
				//for(int i=0; i<5; i++) {
					socket.send(packet);
				//	Thread.sleep(200);
				//}

			} catch (SocketException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} //catch (InterruptedException e) {
				// TODO Auto-generated catch block
			//	e.printStackTrace();
			//}
		}
	}
	
	class Receiver extends Thread {
		DatagramSocket socket;
		ClipboardManager clipboard;
		
		public Receiver(DatagramSocket socket, ClipboardManager clipboard) {
			this.socket = socket;
			this.clipboard = clipboard;
		}
		
		public void run() {
			byte buf[] = new byte[1024];
			DatagramPacket packet = new DatagramPacket(buf, buf.length);
			try {
				while(true) {
					Log.d(LOG_NAME, "wait");
					socket.receive(packet);
					Log.d(LOG_NAME, "received from "+packet.getAddress().getHostAddress());
					String str = new String(packet.getData(), 0, packet.getLength(), "UTF-8");
					Log.d(LOG_NAME, str);
					setOwn(true);
					clipboard.setText(str);
				}
			} catch (SocketException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	@Override
	public IBinder onBind(Intent intent) {
		Log.d(LOG_NAME, "bind");
		return null;
	}
	
	@Override
	public void onDestroy() {
		Log.d(LOG_NAME, "destroy");
		super.onDestroy();
	}
	
	public void setOwn(boolean f) {
		own = f;
	}
	
	public boolean getOwn() {
		return own;
	}
	
	InetAddress getBroadcastAddress() throws IOException {
	    WifiManager wifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
	    DhcpInfo dhcp = wifi.getDhcpInfo();
	    // handle null somehow
	    int ip = dhcp.ipAddress; 
	    byte[] quads = new byte[4];
	    for (int k = 0; k < 4; k++)
	      quads[k] = (byte) ((ip >> k * 8) & 0xFF);
	    local = InetAddress.getByAddress(quads);
	    Log.d("local", InetAddress.getByAddress(quads).getHostAddress());
	    int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
	    for (int k = 0; k < 4; k++)
	      quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
	    Log.d("udp", InetAddress.getByAddress(quads).getHostAddress());
	    return InetAddress.getByAddress(quads);
	}

}
