package com.luxoft.clipboard;

import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map.Entry;

public class ClientsManager {
	private DatagramSocket socket;
	private HashMap<String, InetAddress> clients;
	
	public ClientsManager(DatagramSocket socket) {
		this.socket = socket;
		clients = new HashMap<String, InetAddress>();
	}
	
	public boolean add(String client, InetAddress address) {
		return clients.put(client, address) == null;
	}
	
	public void sendToAll(String data) {
		Packet packet = new Packet(Packet.CLIPBOARD, data);
		for(InetAddress address : clients.values()) {
			new Sender(socket, address, packet, false).start();
		}
	}
}
