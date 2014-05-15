package com.luxoft.clipboard;

import java.io.ObjectOutputStream.PutField;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Map.Entry;
import java.util.Set;

import android.util.Log;

public class ClientsManager {
	private static final int DELAY = 45*1000; //45sec
	
	private static final String LOG_NAME = "devices";
	
	private DatagramSocket socket;
	private ClipboardService service;
	private Timer timer;
	private HashMap<InetAddress, Device> devices;
	
	public ClientsManager(DatagramSocket socket, ClipboardService service) {
		Log.d("service", "client manager init");
		this.socket = socket;
		this.service = service;
		timer = new Timer();
		devices = new HashMap<InetAddress, Device>();
	}
	
	public Device add(String client, InetAddress address) {
		Device last = devices.get(address);
		if(last == null) {
			Device temp = new Device(address, client);
			TimerTask task = new AreYouHere(temp);
			timer.schedule(task, DELAY);
			temp.updateTask(task);
			devices.put(address, temp);
			return temp;
		}
		return null;
	}
	
	public Collection<Device> getDevices() {
		return devices.values();
	}
	
	public void sendToAll(String data) {
		Packet packet = new Packet(Packet.CLIPBOARD, data);
		for(InetAddress address : devices.keySet()) {
			new Sender(socket, address, packet, false).start();
		}
	}
	
	public void updateDevice(InetAddress address) {
		Device device = devices.get(address);
		if(device == null) {
			return;
		}
		Log.d(LOG_NAME, "here: "+device.getAddress().getHostAddress());
		TimerTask task = new AreYouHere(device);
		timer.schedule(task, DELAY);
		device.updateTask(task);
	}
	
	public void sendAreYouHere(Device device) {
		Log.d(LOG_NAME, "are you here "+device.getAddress().getHostAddress()+"?");
		Packet packet = new Packet(Packet.ARE_YOU_HERE, "");
		new Sender(socket, device.getAddress(), packet, false).start();
		TimerTask task = new Killer(device);
		timer.schedule(task, DELAY);
		device.updateTask(task);
	}
	
	public void removeDevice(Device device) {
		Log.d(LOG_NAME, "remove "+device.getAddress().getHostAddress()+"?");
		devices.remove(device.getAddress());
		service.removeDevice(device);
	}
	
	class AreYouHere extends TimerTask {
		
		Device device;
		
		public AreYouHere(Device d) {
			super();
			device = d;
		}
		
		public void run() {
			sendAreYouHere(device);
		}
	}
	
	class Killer extends TimerTask {
		Device device;
		
		public Killer(Device d) {
			super();
			device = d;
		}
		
		public void run() {
			removeDevice(device);
		}
	}
	
	
}
