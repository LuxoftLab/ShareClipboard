package com.luxoft.clipboard;

import java.net.InetAddress;

public class ClientRoom {
	private String name;
	private InetAddress host;
	
	public ClientRoom(String name, InetAddress host) {
		this.name = name;
		this.host = host;
	}
}
