package com.luxoft.clipboard;

import com.luxoft.clipboard.messages.DeleteItemMessage;
import com.luxoft.clipboard.messages.NewItemMessage;
import com.luxoft.clipboard.view.ListAdapter;
import com.luxoft.clipboard.view.RowData;
import com.luxoft.clipboard.view.ViewController;

import android.support.v7.app.ActionBarActivity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Toast;


public class Main extends ActionBarActivity implements MessageManager.Listener, ServiceConnection {
	private static final String LOG = "activity";
	
	public static final int MSG_SHOW_ROOMS = 0,
							MSG_SHOW_MEMBERS = 1,
							MSG_ADD_ROOM = 2,
							MSG_DELETE_ROOM = 3,
							MSG_SHOW_FAIL = 4;
	
	private MessageManager serviceConnection;
	private Intent serviceIntent;
	
	private ListView roomsList;
	private ListAdapter roomsAdapter;
	
	private ImageButton addRoom;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        serviceIntent = new Intent(this, Controller.class);
        startService(serviceIntent);
        
        serviceConnection = new MessageManager(this);
        
        initGUI();
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
		serviceConnection.send(Controller.MSG_CLOSE, null);
		super.onBackPressed();
	}

	@Override
	public boolean onMessage(Message msg) {
		Bundle data = msg.getData();
		switch(msg.what) {
			case MSG_SHOW_ROOMS:
				Log.d(LOG, "show rooms");
				break;
			case MSG_SHOW_MEMBERS:
				break;
			case MSG_ADD_ROOM:
				NewItemMessage room = new NewItemMessage(data);
				roomsAdapter.add(new RowData(room.name, room.ip));
				break;
			case MSG_DELETE_ROOM:
				DeleteItemMessage item = new DeleteItemMessage(data);
				roomsAdapter.remove(item.ip);
				break;
			case MSG_SHOW_FAIL:
				Toast.makeText(this, "Fail", Toast.LENGTH_LONG).show();
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
		serviceConnection.send(Controller.MSG_CONNECTION, null);
	}

	@Override
	public void onServiceDisconnected(ComponentName name) {
		Log.d(LOG, "service disconnected");
		serviceConnection.setTarget(null);
	}
	
	public MessageManager getServiceConnection() {
		return serviceConnection;
	}

	private void initGUI() {
		ViewController controller = new ViewController(this);
		
        roomsList = (ListView)findViewById(R.id.roomsList);
        roomsAdapter = new ListAdapter(this, R.id.list_item);
        roomsList.setAdapter(roomsAdapter);
        
        addRoom = (ImageButton) findViewById(R.id.addRoom);
        addRoom.setOnClickListener(controller);
	}
}
