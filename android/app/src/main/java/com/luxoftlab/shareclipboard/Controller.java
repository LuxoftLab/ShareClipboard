package com.luxoftlab.shareclipboard;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.HashMap;

import com.luxoftlab.shareclipboard.ClientRoom.Device;
import com.luxoftlab.shareclipboard.messages.CreateRoomMessage;
import com.luxoftlab.shareclipboard.messages.DeleteItemMessage;
import com.luxoftlab.shareclipboard.messages.JoinRoomMessage;
import com.luxoftlab.shareclipboard.messages.NewItemMessage;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
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
					  MSG_LEAVE_ROOM = 4,
					  MSG_CLOSE = 5;
	
	
	private String login = Build.MODEL;
	private UDPService udp;
	private ServerRoom server;
	private ClientRoom currentRoom;
	private HashMap<InetAddress, ClientRoom> rooms;
	
	private MessageManager guiConnection = new MessageManager(this);
	private ClipboardManager clipboardManager;
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.w(LOG, "startcommand");
		if(startId != 1) {
			return START_STICKY;
		}
		Log.w(LOG, "started");
		currentRoom = null;
		rooms = new HashMap<InetAddress, ClientRoom>();
		 clipboardManager = (ClipboardManager)getSystemService(Context.CLIPBOARD_SERVICE);
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
		Log.d(LOG, "bind");
		return guiConnection.getBinder();
	}
	
	@Override
	public boolean onUnbind(Intent intent) {
		guiConnection.setTarget(null);
		return super.onUnbind(intent);
	}
	
	@Override
	public void onDestroy() {
		Log.d(LOG, "destroyed");
		super.onDestroy();
	}
	
	@Override
	public boolean onMessage(Message msg) {
		Bundle data = msg.getData();
		switch(msg.what) {
		case MSG_CONNECTION:
			Log.d(LOG, "gui connected");
			guiConnection.setTarget(msg.replyTo);
			synchronizeGUI();
			break;
		case MSG_CREATE_ROOM:
			CreateRoomMessage createRoom = new CreateRoomMessage(data);
			if(!createRoom(createRoom.name, createRoom.password)) {
				guiConnection.send(Main.MSG_SHOW_FAIL, null);
			}
			guiConnection.send(Main.MSG_SHOW_DEVICES, null);
			break;
		case MSG_JOIN_ROOM:
			JoinRoomMessage joinRoom = new JoinRoomMessage(data);
			if(!joinRoom(joinRoom.ip, joinRoom.password)) {
				guiConnection.send(Main.MSG_SHOW_FAIL, null);
			}
			guiConnection.send(Main.MSG_SHOW_DEVICES, null);
			break;
		case MSG_LEAVE_ROOM:
			leaveRoom();
			break;
		case MSG_CLOSE:
			udp.close();
			leaveRoom();
			stopForeground(true);
			stopSelf();
			Log.d(LOG, "closed");
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
		Log.d(LOG, "got room"+name);
		if(rooms.containsKey(host)) {
			deleteRoom(host);
		}
		rooms.put(host, new ClientRoom(name, host, this));
		notifyAboutRoom(name, host);
	}
	
	public void deleteRoom(InetAddress host) {
		Log.d(LOG, "delete room"+host.getHostAddress());
		rooms.remove(host);
		DeleteItemMessage msg = new DeleteItemMessage(host.getHostAddress());
		guiConnection.send(Main.MSG_DELETE_ROOM, msg);
	}
	
	public void notifyAboutFail(int error) {
		guiConnection.send(Main.MSG_SHOW_FAIL, null);
	}
	
	public void notifyAboutDevice(String login, InetAddress addr) {
		NewItemMessage msg = new NewItemMessage(login, addr.getHostAddress());
		guiConnection.send(Main.MSG_ADD_DEVICE, msg);
	}
	
	public void notifyAboutDeviceDeletion(InetAddress addr) {
		DeleteItemMessage msg = new DeleteItemMessage(addr.getHostAddress());
		guiConnection.send(Main.MSG_DELETE_DEVICE, msg);
	}

	public void onDisconnected() {
		currentRoom = null;
		guiConnection.send(Main.MSG_SHOW_ROOMS, null);
	}
	
	public ClipboardManager getClipboardManager() {
		return clipboardManager;
	}
	
	private void leaveRoom() {
		if(currentRoom != null) {
			currentRoom.disconnect();
			onDisconnected();
		}
		if(server != null) {
			deleteRoom(udp.getLocalAddress());
			udp.deleteRoom();
			server.close();
			server = null;
		}
	}
	
	private boolean createRoom(String name, String pass) {
		if(server != null || currentRoom != null)
			return false;
		try {
			server = new ServerRoom(name, pass);
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		InetAddress local = udp.getLocalAddress();
		addRoom(name, local);
		udp.notifyAboutRoom(name);
		return joinRoom(local.getHostAddress(), pass);
	}
	
	private boolean joinRoom(String ip, String pass) {
		if(currentRoom != null)
			return false;
		InetAddress host;
		try {
			host = InetAddress.getByName(ip);
		} catch (UnknownHostException e) {
			e.printStackTrace();
			return false;
		}
		ClientRoom room = rooms.get(host);
		if(room == null)
			return false;
		currentRoom = room;
		Log.d(LOG, "join with "+login+" "+pass);
		return room.connectToHost(login, pass);
	}
	
	private void synchronizeGUI() {
		for(ClientRoom room : rooms.values())
			notifyAboutRoom(room.getName(), room.getHost());
		if(currentRoom == null) 
			guiConnection.send(Main.MSG_SHOW_ROOMS, null);
		else {
			guiConnection.send(Main.MSG_SHOW_DEVICES, null);
			for(Device device : currentRoom.getDevice()) {
				notifyAboutDevice(device.login, device.addr);
			}
		}
	}
	
	private void notifyAboutRoom(String name, InetAddress host) {
		NewItemMessage msg = new NewItemMessage(name, host.getHostAddress());
		guiConnection.send(Main.MSG_ADD_ROOM, msg);
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
