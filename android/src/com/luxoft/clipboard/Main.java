package com.luxoft.clipboard;

import android.support.v7.app.ActionBarActivity;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.BluetoothProfile.ServiceListener;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;


public class Main extends ActionBarActivity implements MessageManager.Listener, ServiceConnection {
	private static final String LOG = "activity";
	
	public static final int MSG_SHOW_ROOMS = 0,
							MSG_SHOW_MEMBERS = 1,
							MSG_ADD_ROOM = 2,
							MSG_DELETE_ROOM = 3,
							MSG_SHOW_FAIL = 4;
	
	private MessageManager serviceConnection;
	private Intent serviceIntent;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        serviceIntent = new Intent(this, Controller.class);
        startService(serviceIntent);
        
        serviceConnection = new MessageManager(this);
        Log.d(LOG, "created");
    }
    
    @Override
    protected void onStart() {
    	super.onStart();
    	bindService(serviceIntent, this, BIND_AUTO_CREATE);
    	Log.d(LOG, "started");
    }
    
    @Override
    protected void onStop() {
    	super.onStop();
    	unbindService(this);
    	Log.d(LOG, "stoped");
    }
    
    @Override
	public void onBackPressed() {
		serviceConnection.send(Controller.MSG_CLOSE, new Bundle());
		super.onBackPressed();
	}

	@Override
	public boolean onMessage(Message msg) {
		switch(msg.what) {
			case MSG_SHOW_ROOMS:
				Log.d(LOG, "show rooms");
				break;
			case MSG_SHOW_MEMBERS:
				break;
			case MSG_ADD_ROOM:
				break;
			case MSG_DELETE_ROOM:
				break;
			case MSG_SHOW_FAIL:
				break;
			default:
				Log.w(LOG, "undefined message");
				return false;
		}
		return true;
	}

	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		Log.d(LOG, "service connected");
		serviceConnection.setTarget(new Messenger(service));
		serviceConnection.send(Controller.MSG_CONNECTION, new Bundle());
	}

	@Override
	public void onServiceDisconnected(ComponentName name) {
		Log.d(LOG, "service disconnected");
		serviceConnection.setTarget(null);
	}

}
