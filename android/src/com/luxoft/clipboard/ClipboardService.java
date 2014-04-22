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
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

public class ClipboardService extends Service {

	private static String LOG_NAME = "service";

	public static final int REGISTER_MESSAGE = 1;

	private final Messenger mMessenger = new Messenger(new IncomingHandler());
	private Messenger mResponseMessenger = null;
	
	private DatagramSocket socket;
	private ClipboardManager clipboard;
	private ClientsManager clients;

	class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case REGISTER_MESSAGE:
				Log.d(LOG_NAME, "Registered Activity's Messenger.");
				mResponseMessenger = msg.replyTo;
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(LOG_NAME, "Service started");
		clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
		try {
			socket = new DatagramSocket(1234);
			clients = new ClientsManager();
			Packet p = new Packet(Packet.LOOKUP, "android");
			new Sender(socket, getBroadcastAddress(), p, true).start();
			new Receiver(this, socket, getLocalAddress()).start();
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return super.onStartCommand(intent, flags, startId);
	}

	@Override
	public IBinder onBind(Intent intent) {
		Log.d(LOG_NAME, "Binding messenger...");
		return mMessenger.getBinder();
	}
	
	public void onReceiveUDP(DatagramPacket datagram) {
		Packet input = new Packet(datagram.getData(), datagram.getOffset(), datagram.getLength());
		switch(input.getType()) {
		case Packet.LOOKUP:
			Log.d(LOG_NAME, "lookup by "+input.getContent());
			sendHello(datagram.getAddress());
			addClient(input.getContent(), datagram.getAddress());
			break;
		case Packet.ARE_YOU_HERE:
			Log.d(LOG_NAME, "are you here? by "+input.getContent());
			sendHello(datagram.getAddress());
			break;
		case Packet.HELLO: 
			Log.d(LOG_NAME, "hello by "+input.getContent());
			addClient(input.getContent(), datagram.getAddress());
			break;
		case Packet.CLIPBOARD:
			Log.d(LOG_NAME, "clipboard: "+input.getContent());
			clipboard.setText(input.getContent());
			break;
		}
 	}
	
	private void sendHello(InetAddress address) {
		Packet output = new Packet(Packet.HELLO, "android");
		new Sender(socket, address, output, false).start();
	}
	
	private void addClient(String client, InetAddress address) {
		if(clients.add(client, address)) {
			Bundle bundle = new Bundle();
			bundle.putString("name", client + ": " + address.getHostAddress());
			sendToActivity(Main.NEW_DEVICE_MESSAGE, bundle);
		}
	}

	private void sendToActivity(int type, Bundle data) {
		if (mResponseMessenger == null) {
			Log.d(LOG_NAME, "Cannot send message to activity");
		} else {
			Log.d(LOG_NAME, "Sending message to activity");
			Message msg = Message.obtain(null, type);
			msg.setData(data);
			try {
				mResponseMessenger.send(msg);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
	}

	private InetAddress getBroadcastAddress() throws IOException {
		WifiManager wifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		byte[] quads = new byte[4];
		int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}
	
	private InetAddress getLocalAddress() throws IOException {
		WifiManager wifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		byte[] quads = new byte[4];
		int ip = dhcp.ipAddress;
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((ip >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}

}
