package com.luxoft.clipboard;

import java.net.InetAddress;
import java.util.HashMap;

public class ClientsManager {
	HashMap<String, InetAddress> clients;
	
	public ClientsManager() {
		clients = new HashMap<String, InetAddress>();
	}
	
	public boolean add(String client, InetAddress address) {
		return clients.put(client, address) == null;
	}
}
