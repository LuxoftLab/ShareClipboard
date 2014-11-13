package com.luxoft.clipboard;

import java.io.IOException;
import java.net.InetAddress;
import java.util.Collection;
import java.util.HashMap;

import android.content.ClipboardManager;
import android.content.Context;

import com.luxoft.clipboard.packets.DeleteDevicePacket;
import com.luxoft.clipboard.packets.DevicePacket;
import com.luxoft.clipboard.packets.ErrorPacket;
import com.luxoft.clipboard.packets.TextPacket;

public class ClientRoom {
	private static final String LOG = "clientRoom";
	
	private String name;
	private InetAddress host;
	private String login, pass;
	private ServerConnection connection;
	private HashMap<InetAddress, Device> devices;
	
	private Controller controller;
	private ClipboardService clipboard;
	
	public ClientRoom(String name, InetAddress host, Controller controller) {
		this.name = name;
		this.host = host;
		this.controller = controller;
		devices = new HashMap<InetAddress, Device>();
	}
	
	public boolean connectToHost(String login, String password) {
		this.login = login;
		this.pass = password;
		try {
			connection = new ServerConnection(host, this);
			connection.start();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public void onStarted() {
		clipboard = new ClipboardService(controller, this);
		connection.sendPasswordAndLogin(login, pass);
	}
	
	public void onServerDisconnected() {
		controller.onDisconnected();
	}
	
	public void onTextCopied(String data) {
		connection.sendClipboardText(data);
	}
	
	public void updateClipboard(TextPacket packet) {
		clipboard.putText(packet.text);
	}
	
	public void addDevice(DevicePacket packet) {
		if(devices.containsKey(packet.addr)) {
			deleteDevice(packet);
		}
		devices.put(packet.addr, new Device(packet.login, packet.addr));
		controller.notifyAboutDevice(packet.login, packet.addr);
	}
	
	public void deleteDevice(DeleteDevicePacket packet) {
		Device device = devices.remove(packet.addr);
		if(device != null)
			controller.notifyAboutDeviceDeletion(packet.addr);
	}
	
	public String getName() {
		return name;
	}

	public InetAddress getHost() {
		return host;
	}
	
	public Collection<Device> getDevice() {
		return devices.values();
	}

	public void notifyAboutFail(ErrorPacket packet) {
		controller.notifyAboutFail(packet.value);
	}
	
	public void disconnect() {
		connection.close();
	}
	
	class Device {
		String login;
		InetAddress addr;
		
		public Device(String login, InetAddress addr) {
			this.login = login;
			this.addr = addr;
		}
	}
}
