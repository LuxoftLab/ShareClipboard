package com.luxoft.clipboard;

import java.util.ArrayList;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.os.SystemClock;
import android.text.Editable;
import android.text.method.KeyListener;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

public class Main extends Activity {

	private static String LOG_NAME = "activity";
	
	public static final int NEW_DEVICE_MESSAGE = 1;
	public static final int REMOVE_DEVICE = 2;

	final Messenger mMessenger = new Messenger(new IncomingHandler());

	private Messenger mService = null;

	private boolean mServiceConnected = false;
	private Intent serviceIntent;
	
	private ListView clientsList;
	private ArrayAdapter<String> adapter;
	private ArrayList<String> devices = new ArrayList<String>();
	private Menu menu;
	private String name;

	class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch(msg.what) {
			case NEW_DEVICE_MESSAGE:
				Main.this.devices.add(msg.getData().getString("name"));
				Main.this.adapter.notifyDataSetChanged();
				Log.d(LOG_NAME, msg.getData().getString("name"));
				break;
			case REMOVE_DEVICE:
				Main.this.devices.remove(msg.getData().getString("name"));
				Main.this.adapter.notifyDataSetChanged();
				Log.d(LOG_NAME, "remove: "  + msg.getData().getString("name"));
			default:
				super.handleMessage(msg);	
			}
		}
	}

	private ServiceConnection mConn = new ServiceConnection() {
		
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			adapter.clear();
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
		getActionBar().setTitle("");
		setContentView(R.layout.main);
		
		serviceIntent = new Intent(this, ClipboardService.class);
		startService(serviceIntent);
		
		name = Build.DEVICE;
		
		clientsList = (ListView) findViewById(R.id.clients);
		adapter = new ArrayAdapter<String>(this, R.layout.line, devices);
		clientsList.setAdapter(adapter);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		this.menu = menu;
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.bar, menu);
		menu.findItem(R.id.name).setTitle(name);
		EditText edit = (EditText)menu.findItem(R.id.editName).getActionView();
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		int id = item.getItemId();
		switch(id) {
		case R.id.on:
			menu.findItem(R.id.off).setVisible(true);
			item.setVisible(false);
			break;
		case R.id.off:
			menu.findItem(R.id.on).setVisible(true);
			item.setVisible(false);
			break;
		case R.id.edit:
			EditText edit = (EditText)menu.findItem(R.id.editName).getActionView();
			menu.findItem(R.id.close).setVisible(true);
			menu.findItem(R.id.name).setVisible(false);
			edit.setText(name);
			edit.requestFocus();
			menu.findItem(R.id.editName).setVisible(true);
			item.setVisible(false);
			break;
		case R.id.close:
			menu.findItem(R.id.edit).setVisible(true);
			menu.findItem(R.id.editName).setVisible(false);
			menu.findItem(R.id.name).setVisible(true);
			item.setVisible(false);
			break;
		}
		return super.onOptionsItemSelected(item);
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
	
	@Override
	public void onBackPressed() {
		sendToService(ClipboardService.CLOSE_MESSAGE, new Bundle());
		super.onBackPressed();
	}
	
	void sendToService(int type, Bundle data) {
		if (mServiceConnected) {
			Log.d(LOG_NAME, "Sending message to service: ");
			Message msg = Message.obtain(null, type);
			msg.setData(data);
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

}
