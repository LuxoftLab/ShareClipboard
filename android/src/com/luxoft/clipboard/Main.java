package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Main extends Activity implements Handler.Callback {
	/**
	 * Messenger used for receiving responses from service.
	 */
	
	private static String LOG_NAME = "activity";
	
	final Messenger mMessenger = new Messenger(new IncomingHandler());
	/**
	 * Messenger used for communicating with service.
	 */
	Messenger mService = null;

	boolean mServiceConnected = false;
	Intent serviceIntent;
	
	
	class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			if (msg.what == ClipboardService.MESSAGE_TYPE_TEXT) {
				Bundle b = msg.getData();
				CharSequence text = null;
				if (b != null) {
					text = b.getCharSequence("data");
				} else {
					text = "Service responded with empty message";
				}
				Log.d(LOG_NAME, "Response: " + text);
				//final TextView responseFromService = (TextView) findViewById(R.id.responseFromService);
				//responseFromService.setText(text);
			} else {
				super.handleMessage(msg);
			}
		}
	}

	/**
	 * Class for interacting with the main interface of the service.
	 */
	private ServiceConnection mConn = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			Log.d("MessengerActivity", "Connected to service. Registering our Messenger in the Service...");
			mService = new Messenger(service);
			mServiceConnected = true;

			// Register our messenger also on Service side:
			Message msg = Message.obtain(null, ClipboardService.MESSAGE_TYPE_REGISTER);
			msg.replyTo = mMessenger;
			try {
				mService.send(msg);
			} catch (RemoteException e) {
				// We always have to trap RemoteException (DeadObjectException
				// is thrown if the target Handler no longer exists)
				e.printStackTrace();
			}
		}

		/**
		 * Connection dropped.
		 */
		@Override
		public void onServiceDisconnected(ComponentName className) {
			Log.d("MessengerActivity", "Disconnected from service.");
			mService = null;
			mServiceConnected = false;
		}
	};

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		serviceIntent = new Intent(this, ClipboardService.class);
		startService(serviceIntent);
		Button send = (Button) findViewById(R.id.send);
		// Message handling - to service:
		//final EditText textToService = (EditText) findViewById(R.id.textToService);
		//final Button sendTextToService = (Button) findViewById(R.id.sendTextToService);
		send.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				sendToService("testData");
			}
		});
	}

	@Override
	public void onStart() {
		super.onStart();
		bindService(serviceIntent, mConn, Context.BIND_AUTO_CREATE);
	}

	@Override
	protected void onStop() {
		super.onStop();
		if (mServiceConnected) {
			unbindService(mConn);
			//stopService(new Intent(this, ClipboardService.class));
			mServiceConnected = false;
		}
	}

	/**
	 * Sends message with text stored in bundle extra data ("data" key).
	 * 
	 * @param text
	 *            text to send
	 */
	void sendToService(CharSequence text) {
		if (mServiceConnected) {
			Log.d(LOG_NAME, "Sending message to service: " + text);
			Message msg = Message.obtain(null, ClipboardService.MESSAGE_TYPE_TEXT);
			Bundle b = new Bundle();
			b.putCharSequence("data", text);
			msg.setData(b);
			try {
				mService.send(msg);
			} catch (RemoteException e) {
				// We always have to trap RemoteException (DeadObjectException
				// is thrown if the target Handler no longer exists)
				e.printStackTrace();
			}
		} else {
			Log.d(LOG_NAME, "Cannot send - not connected to service.");
		}
	}

	@Override
	public boolean handleMessage(Message arg0) {
		// TODO Auto-generated method stub
		return false;
	}

}
