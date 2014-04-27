package com.luxoft.clipboard;

import java.util.ArrayList;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class Main extends Activity {

	private static String LOG_NAME = "activity";
	
	public static final int NEW_DEVICE_MESSAGE = 1;

	final Messenger mMessenger = new Messenger(new IncomingHandler());

	private Messenger mService = null;

	private boolean mServiceConnected = false;
	private Intent serviceIntent;
	
	private ListView clientsList;
	private ArrayAdapter<String> adapter;
	private ArrayList<String> devices = new ArrayList<String>();

	class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch(msg.what) {
			case NEW_DEVICE_MESSAGE:
				Main.this.devices.add(msg.getData().getString("name"));
				Main.this.adapter.notifyDataSetChanged();
				Log.d(LOG_NAME, msg.getData().getString("name"));
				break;
			default:
				super.handleMessage(msg);	
			}
		}
	}

	private ServiceConnection mConn = new ServiceConnection() {
		
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			Log.d("MessengerActivity", "Connected to service...");
			mService = new Messenger(service);
			mServiceConnected = true;
			Message msg = Message.obtain(null,
					ClipboardService.REGISTER_MESSAGE);
			msg.replyTo = mMessenger;
			try {
				mService.send(msg);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}

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
		
		clientsList = (ListView) findViewById(R.id.clients);
		adapter = new ArrayAdapter<String>(this, R.layout.line, devices);
		clientsList.setAdapter(adapter);
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
			// stopService(new Intent(this, ClipboardService.class));
			mServiceConnected = false;
		}
	}

}
