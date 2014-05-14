package com.luxoft.clipboard;

import java.net.InetAddress;
import java.util.TimerTask;

public class Device {
	private InetAddress address;
	private String login;
	private TimerTask task;
	
	public Device(InetAddress address, String login) {
		this.address = address;
		this.login = login;
	}
	
	public void updateTask(TimerTask newTask) {
		if(task != null) {
			task.cancel();
		}
		task = newTask;
	}
	
	public InetAddress getAddress() {
		return address;
	}
	
	public String getLogin() {
		return login;
	}
	
	@Override
	public String toString() {
		return login + " " + address.getHostAddress();
	}

}
