package com.luxoftlab.shareclipboard;

import com.luxoftlab.shareclipboard.messages.DeleteItemMessage;
import com.luxoftlab.shareclipboard.messages.NewItemMessage;
import com.luxoftlab.shareclipboard.view.ListAdapter;
import com.luxoftlab.shareclipboard.view.RowData;
import com.luxoftlab.shareclipboard.view.ViewController;

import android.support.v7.app.ActionBarActivity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Messenger;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.Toast;


public class Main extends ActionBarActivity implements ServiceConnection {
	private static final String LOG = "activity";
	
	public static final int MSG_SHOW_ROOMS = 0,
							MSG_SHOW_DEVICES = 1,
							MSG_ADD_ROOM = 2,
							MSG_DELETE_ROOM = 3,
							MSG_SHOW_FAIL = 4,
							MSG_ADD_DEVICE = 5,
							MSG_DELETE_DEVICE = 6;
	
	private MessageManager serviceConnection;
	private Intent serviceIntent;
	
	private ViewController controller;
	private ListAdapter roomsAdapter;
	private ListAdapter devicesAdapter;
	private RelativeLayout mainView, roomView;

	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        serviceIntent = new Intent(this, Controller.class);
        startService(serviceIntent);
        
        initServiceConnection();
        controller = new ViewController(this);
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

    private void initServiceConnection() {
        serviceConnection = new MessageManager();

        serviceConnection.addListener(MSG_SHOW_ROOMS, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                mainView.setVisibility(View.VISIBLE);
                roomView.setVisibility(View.GONE);
            }
        });

        serviceConnection.addListener(MSG_SHOW_DEVICES, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                roomView.setVisibility(View.VISIBLE);
                mainView.setVisibility(View.GONE);
            }
        });

        serviceConnection.addListener(MSG_ADD_ROOM, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                NewItemMessage room = new NewItemMessage(bundle);
                roomsAdapter.put(new RowData(room.name, room.ip));
            }
        });

        serviceConnection.addListener(MSG_DELETE_ROOM, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                DeleteItemMessage item = new DeleteItemMessage(bundle);
                Log.d(LOG, "delete room"+item.ip);
                roomsAdapter.remove(item.ip);
            }
        });

        serviceConnection.addListener(MSG_SHOW_FAIL, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                Toast.makeText(Main.this, "Fail", Toast.LENGTH_LONG).show();
            }
        });

        serviceConnection.addListener(MSG_ADD_DEVICE, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                NewItemMessage room = new NewItemMessage(bundle);
                devicesAdapter.put(new RowData(room.name, room.ip));
            }
        });

        serviceConnection.addListener(MSG_DELETE_DEVICE, new MessageManager.Listener() {
            @Override
            public void onMessage(Bundle bundle) {
                DeleteItemMessage item = new DeleteItemMessage(bundle);
                devicesAdapter.remove(item.ip);
            }
        });
    }

	private void initGUI() {
		mainView = (RelativeLayout)findViewById(R.id.mainView);
		roomView = (RelativeLayout)findViewById(R.id.roomView);
		
        ListView roomsList = (ListView)findViewById(R.id.roomsList);
        roomsAdapter = new ListAdapter(this, R.id.list_item);
        roomsList.setAdapter(roomsAdapter);
        roomsList.setOnItemClickListener(controller);
        
        ListView devicesList = (ListView)findViewById(R.id.devicesList);
        devicesAdapter = new ListAdapter(this, R.id.list_item);
        devicesList.setAdapter(devicesAdapter);
        
        ImageButton addRoom = (ImageButton) findViewById(R.id.addRoom);
        addRoom.setOnClickListener(controller);
        
        ImageButton leave = (ImageButton) findViewById(R.id.leaveRoom);
        leave.setOnClickListener(controller);
	}
}
