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
	
	// Constants describing types of message. Different types of messages can be
		// passed and this identifies them.
		/**
		 * Message type: register the activity's messenger for receiving responses
		 * from Service. We assume only one activity can be registered at one time.
		 */
		public static final int MESSAGE_TYPE_REGISTER = 1;
		/**
		 * Message type: text sent Activity<->Service
		 */
		public static final int MESSAGE_TYPE_TEXT = 2;

		/**
		 * Messenger used for handling incoming messages.
		 */
		final Messenger mMessenger = new Messenger(new IncomingHandler());
		/**
		 * Messenger on Activity side, used for sending messages back to Activity
		 */
		Messenger mResponseMessenger = null;
		DatagramSocket socket;
		
		class IncomingHandler extends Handler {
			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case MESSAGE_TYPE_TEXT:
					Bundle b = msg.getData();
					if (b != null) {
						Log.d(LOG_NAME,
								"Service received message MESSAGE_TYPE_TEXT with: " + b.getCharSequence("data"));
						sendToActivity("Who's there? You wrote: " + b.getCharSequence("data"));
					} else {
						Log.d(LOG_NAME, "Service received message MESSAGE_TYPE_TEXT with empty message");
						sendToActivity("Who's there? Speak!");
					}
					try {
						Packet p = new Packet(Packet.LOOKUP, "android");
						Packet t = new Packet(p.getData());
						Log.d(LOG_NAME, t.getType() + " " + t.getContent());
						new Sender(socket, getBroadcastAddress(), p, true).start();
					} catch (IOException e) {
						Log.d(LOG_NAME, "get adress");
						e.printStackTrace();
					}
					break;
				case MESSAGE_TYPE_REGISTER:
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
			try {
				socket = new DatagramSocket(1234);
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

		/**
		 * Sends message with text stored in bundle extra data ("data" key).
		 * 
		 * @param text
		 *            text to send
		 */
		void sendToActivity(CharSequence text) {
			if (mResponseMessenger == null) {
				Log.d(LOG_NAME, "Cannot send message to activity - no activity registered to this service.");
			} else {
				Log.d(LOG_NAME, "Sending message to activity: " + text);
				Bundle data = new Bundle();
				data.putCharSequence("data", text);
				Message msg = Message.obtain(null, MESSAGE_TYPE_TEXT);
				msg.setData(data);
				try {
					mResponseMessenger.send(msg);
				} catch (RemoteException e) {
					// We always have to trap RemoteException (DeadObjectException
					// is thrown if the target Handler no longer exists)
					e.printStackTrace();
				}
			}
		}
		
		InetAddress getBroadcastAddress() throws IOException {
		    WifiManager wifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		    DhcpInfo dhcp = wifi.getDhcpInfo();
		    // handle null somehow
		    int ip = dhcp.ipAddress; 
		    byte[] quads = new byte[4];
		    for (int k = 0; k < 4; k++)
		      quads[k] = (byte) ((ip >> k * 8) & 0xFF);
		    Log.d("local", InetAddress.getByAddress(quads).getHostAddress());
		    int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
		    for (int k = 0; k < 4; k++)
		      quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
		    Log.d("udp", InetAddress.getByAddress(quads).getHostAddress());
		    return InetAddress.getByAddress(quads);
		}

}
