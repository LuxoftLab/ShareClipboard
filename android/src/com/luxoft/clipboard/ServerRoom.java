package com.luxoft.clipboard;

public class ServerRoom {
	private String name, password;
	
	public ServerRoom(String name, String password) {
		this.name = name;
		this.password = password;
	}
	
	public String getName() {
		return name;
	}
}
