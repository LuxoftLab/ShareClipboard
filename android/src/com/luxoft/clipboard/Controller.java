package com.luxoft.clipboard;

import java.io.IOException;
import java.net.InetAddress;
import java.util.HashMap;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class Controller extends Service {
	private static String LOG = "service";
	
	private UDPService udp;
	
	private HashMap<InetAddress, ClientRoom> rooms = new HashMap<InetAddress, ClientRoom>();
	private ServerRoom serverRoom;
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(LOG, "id:"+startId);
		if(startId != 1) {
			return START_STICKY;
		}
		try {
			udp = new UDPService(this);
			udp.start();
			udp.getRooms();
		} catch (IOException e) {
			Log.d(LOG, "cannot create udp service");
			e.printStackTrace();
		}
		return START_STICKY;
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
	public void onGetRoom(InetAddress target) {
		if(serverRoom != null) {
			udp.sendRoom(serverRoom.getName(), target);
		}
	}
	
	public void addRoom(InetAddress owner, String name) {
		rooms.put(owner, new ClientRoom(name));
	}
	
	public void unknownPackage() {
		
	}
	
	public void notifyAboutFail() {
		
	}
	

}
