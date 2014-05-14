package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Collection;
import java.util.HashMap;
import java.util.Set;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.DhcpInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.util.Log;

public class ClipboardService extends Service {

	private static String LOG_NAME = "service";

	public static final int REGISTER_MESSAGE = 1;
	public static final int CLOSE_MESSAGE = 2;
	public static final int GET_DEVICES = 3;
	
	public static ClipboardService _this = null;

	private final Messenger mMessenger = new Messenger(new IncomingHandler());
	private Messenger mResponseMessenger = null;
	
	private DatagramSocket socket;
	private ClipboardManager clipboard;
	private ClientsManager clients;
	private boolean owner;
	private Packet lastPacket = null;

	class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case REGISTER_MESSAGE:
				Log.d(LOG_NAME, "Registered Activity's Messenger.");
				mResponseMessenger = msg.replyTo;
				Collection<Device> devices = clients.getDevices();
				Log.d(LOG_NAME, "devices size:"+devices.size());
				for(Device device : devices) {
					Bundle bundle = new Bundle();
					bundle.putString("name", device.toString());
					sendToActivity(Main.NEW_DEVICE_MESSAGE, bundle);
				}
				break;
			case GET_DEVICES:
				Log.d(LOG_NAME, "get diveces");
				break;
			case CLOSE_MESSAGE:
				socket.close();
				stopForeground(true);
				stopSelf();
				Log.d(LOG_NAME, "close");
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(LOG_NAME, "id:"+startId);
		if(startId != 1) {
			return START_STICKY;
		}
		Log.d(LOG_NAME, "Service started");
		clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
		clipboard.addPrimaryClipChangedListener(new ClipboardManager.OnPrimaryClipChangedListener() {
			
			@Override
			public void onPrimaryClipChanged() {
				if(!getOwner()) {
					ClipData.Item item = clipboard.getPrimaryClip().getItemAt(0);
					CharSequence result = item.getText();
					if(result == null) {
						Uri resultUri = item.getUri();
						if(resultUri == null) {
							Intent resultIntent = item.getIntent();
							Log.d(LOG_NAME, "clipboard intent");
						} else {
							Log.d(LOG_NAME, "clipboard uri");
						}
					} else {
						String data = result.toString();
						Log.d(LOG_NAME, "clipboard text: "+data);
						clients.sendToAll(data);
					}
				}
				setOwner(false);
			}
		});
		try {
			socket = new DatagramSocket(1234);
			Log.d(LOG_NAME, "init clients");
			clients = new ClientsManager(socket, this);
			Packet p = new Packet(Packet.LOOKUP, Build.DEVICE);
			new Sender(socket, getBroadcastAddress(), p, true).start();
			new Receiver(this, socket, getLocalAddress()).start();
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Intent resultIntent = new Intent(this, Main.class);
		PendingIntent resultPendingIntent = PendingIntent.getActivity(this, 0, resultIntent, 0);
		NotificationCompat.Builder n = 
				new NotificationCompat.Builder(this)
		.setSmallIcon(R.drawable.ic_launcher)
		.setContentTitle("My title")
		.setContentText("My test")
		.setContentIntent(resultPendingIntent);
		Notification nf = n.build();

		nf.flags |= Notification.FLAG_NO_CLEAR;

		startForeground(100, nf);
		return START_STICKY;
	}

	@Override
	public IBinder onBind(Intent intent) {
		Log.d(LOG_NAME, "Binding messenger...");
		return mMessenger.getBinder();
	}
	
	@Override
	public boolean onUnbind(Intent intent) {
		Log.d(LOG_NAME, "Unbinding...");
		return super.onUnbind(intent);
	}
	
	@Override
	public void onDestroy() {
		Log.d(LOG_NAME, "Destroy");
		super.onDestroy();
	}
	
	public void onReceiveUDP(DatagramPacket datagram) {
		Packet input = new Packet(datagram.getData(), datagram.getOffset(), datagram.getLength());
		if(input.equals(lastPacket)) {
			return;
		}
		lastPacket = input;
		switch(input.getType()) {
		case Packet.LOOKUP:
			Log.d(LOG_NAME, "lookup by "+input.getContent());
			sendHello(datagram.getAddress());
			addClient(input.getContent(), datagram.getAddress());
			break;
		case Packet.ARE_YOU_HERE:
			Log.d(LOG_NAME, "are you here? by "+input.getContent());
			sendHello(datagram.getAddress());
			clients.updateDevice(datagram.getAddress());
			break;
		case Packet.HELLO: 
			Log.d(LOG_NAME, "hello by "+input.getContent());
			addClient(input.getContent(), datagram.getAddress());
			clients.updateDevice(datagram.getAddress());
			break;
		case Packet.CLIPBOARD:
			Log.d(LOG_NAME, "clipboard: "+input.getContent());
			setOwner(true);
			clipboard.setText(input.getContent());
			clients.updateDevice(datagram.getAddress());
			break;
		}
 	}
	
	private void sendHello(InetAddress address) {
		Packet output = new Packet(Packet.HELLO, Build.DEVICE);
		new Sender(socket, address, output, false).start();
	}
	
	private void addClient(String client, InetAddress address) {
		Device device = clients.add(client, address);
		if(device != null) {
			Bundle bundle = new Bundle();
			bundle.putString("name", device.toString());
			sendToActivity(Main.NEW_DEVICE_MESSAGE, bundle);
		}
	}
	
	public void removeDevice(Device device) {
		Bundle bundle = new Bundle();
		bundle.putString("name", device.toString());
		sendToActivity(Main.REMOVE_DEVICE, bundle);
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
	
	synchronized private void setOwner(boolean f) {
		owner = f;
	}
	
	synchronized private boolean getOwner() {
		return owner;
	}

}
