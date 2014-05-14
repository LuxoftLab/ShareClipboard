package com.luxoft.clipboard;

import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;

import android.util.Log;

public class ClientsManager {
	private DatagramSocket socket;
	private HashMap<InetAddress, String> clients;
	
	public ClientsManager(DatagramSocket socket) {
		Log.d("service", "client manager init");
		this.socket = socket;
		clients = new HashMap<InetAddress, String>();
	}
	
	public boolean add(String client, InetAddress address) {
		return clients.put(address, client) == null;
	}
	
	public Set<HashMap.Entry<InetAddress, String>> getDevices() {
		return clients.entrySet();
	}
	
	public void sendToAll(String data) {
		Packet packet = new Packet(Packet.CLIPBOARD, data);
		Packet ch = new Packet(packet.getData(), 0, packet.getData().length);
		Log.d("service", ch.getContent());
		for(InetAddress address : clients.keySet()) {
			new Sender(socket, address, packet, false).start();
		}
	}
}
