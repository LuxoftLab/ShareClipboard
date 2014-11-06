package com.luxoft.clipboard;

import java.io.IOException;
import java.net.InetAddress;
import java.util.HashMap;

import com.luxoft.clipboard.messages.CreateRoomMessage;
import com.luxoft.clipboard.messages.DeleteItemMessage;
import com.luxoft.clipboard.messages.NewItemMessage;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Message;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

public class Controller extends Service implements MessageManager.Listener {
	private static final String LOG = "controller";
	
	private static final int NOTIFICATION_ID = 100;
	
	public static final int MSG_CONNECTION = 0,
					  MSG_CREATE_ROOM = 1,
					  MSG_JOIN_ROOM = 2,
					  MSG_DELETE_ROOM = 3,
					  MSG_LEAVE_ROOM = 4,
					  MSG_CLOSE = 5;
	
	private UDPService udp;
	private ServerRoom server;
	private ClientRoom currentRoom;
	private HashMap<InetAddress, ClientRoom> rooms;
	
	private MessageManager guiConnection = new MessageManager(this);
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.w(LOG, "startcommand");
		if(startId != 1) {
			return START_STICKY;
		}
		Log.w(LOG, "started");
		rooms = new HashMap<InetAddress, ClientRoom>();
		try {
			udp = new UDPService(this);
			udp.start();
			udp.getRooms();
			startForeground(NOTIFICATION_ID, createNotification());
		} catch (IOException e) {
			e.printStackTrace();
		}
		return START_STICKY;
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		Log.w(LOG, "bind");
		return guiConnection.getBinder();
	}
	
	@Override
	public void onDestroy() {
		Log.w(LOG, "destroyed");
		super.onDestroy();
	}
	
	@Override
	public boolean onMessage(Message msg) {
		Bundle data = msg.getData();
		switch(msg.what) {
		case MSG_CONNECTION:
			Log.w(LOG, "gui connected");
			guiConnection.setTarget(msg.replyTo);
			guiConnection.send(Main.MSG_SHOW_ROOMS, null);
			break;
		case MSG_CREATE_ROOM:
			CreateRoomMessage room = new CreateRoomMessage(data);
			if(!createRoom(room.name, room.password)) {
				guiConnection.send(Main.MSG_SHOW_FAIL, null);
			}
			break;
		case MSG_JOIN_ROOM:
			break;
		case MSG_DELETE_ROOM:
			break;
		case MSG_LEAVE_ROOM:
			break;
		case MSG_CLOSE:
			udp.close();
			stopForeground(true);
			stopSelf();
			Log.w(LOG, "closed");
			break;
		default:
			Log.w(LOG, "undefined message");
			return false;
		}
		return true;
	}
	
	public void getRoom(InetAddress addr) {
		if(server != null) {
			udp.sendRoom(addr, server.getName());
		}
	}
	
	public void addRoom(String name, InetAddress host) {
		Log.w(LOG, "got room"+name);
		if(rooms.containsKey(host)) {
			deleteRoom(host);
		}
		rooms.put(host, new ClientRoom(name, host));
		NewItemMessage msg = new NewItemMessage(name, host.getHostAddress());
		guiConnection.send(Main.MSG_ADD_ROOM, msg);
	}
	
	public void deleteRoom(InetAddress host) {
		rooms.remove(host);
		DeleteItemMessage msg = new DeleteItemMessage(host.getHostAddress());
		guiConnection.send(Main.MSG_DELETE_ROOM, msg);
	}
	
	private boolean createRoom(String name, String pass) {
		if(server != null || currentRoom != null)
			return false;
		server = new ServerRoom(name, pass);
		InetAddress local = udp.getLocalAddress();
		addRoom(name, local);
		udp.notifyAboutRoom(name);
		return joinRoom(local, pass);
	}
	
	private boolean joinRoom(InetAddress host, String pass) {
		if(currentRoom != null)
			return false;
		ClientRoom room = rooms.get(host);
		if(room == null)
			return false;
		currentRoom = room;
		//TODO connect to host
		return true;
	}
	
	private Notification createNotification() {
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
		return nf;
	}

}
