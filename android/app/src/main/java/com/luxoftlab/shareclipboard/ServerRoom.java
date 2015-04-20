package com.luxoftlab.shareclipboard;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map.Entry;

import android.util.Log;

import com.luxoftlab.shareclipboard.packets.AuthPacket;
import com.luxoftlab.shareclipboard.packets.TextPacket;

public class ServerRoom {
	private static final String LOG = "serverRoom";
	
	private String name, password;
	private TCPServer server;
	private HashMap<InetAddress, ClientConnection> unverifiedDevices;
	private HashMap<InetAddress, ClientConnection> devices;
	
	private String clipboardText = null;
	
	public ServerRoom(String name, String password) throws IOException {
		this.name = name;
		this.password = password;
		unverifiedDevices = new HashMap<InetAddress, ClientConnection>();
		devices = new HashMap<InetAddress, ClientConnection>();
		server = new TCPServer(this);
		server.start();
	}
	
	public void addDevice(Socket socket) {
		Log.d(LOG, "got device "+socket.getInetAddress().getHostAddress());
		ClientConnection connection = new ClientConnection(this, socket);
		connection.start();
		unverifiedDevices.put(socket.getInetAddress(), connection);
	}
	
	public void deleteDevice(InetAddress addr) {
		Log.d(LOG, "disconnect "+addr.getHostAddress());
		ClientConnection connection = unverifiedDevices.remove(addr);
		if(connection != null) return;
		connection = devices.remove(addr);
		if(connection != null) {
			for(Entry<InetAddress, ClientConnection> e : devices.entrySet()) {
				Log.d(LOG, "send remove to"+e.getKey().getHostAddress());
				e.getValue().deleteMember(addr);
			}
		}
	}
	
	public void verifyPassword(InetAddress addr, AuthPacket packet) {
		Log.d(LOG, "pass verification "+addr.getHostAddress()+" "+packet.password);
		ClientConnection connection = unverifiedDevices.get(addr);
		if(connection == null) return;
		if(!password.equals(packet.password)) {
			Log.d(LOG, "fail");
			connection.sendFail();
			return;
		}
		unverifiedDevices.remove(addr);
		devices.put(addr, connection);
		for(Entry<InetAddress, ClientConnection> e : devices.entrySet()) {
			ClientConnection c = e.getValue();
			c.sendMember(packet.login, addr);
			if(c != connection)
				connection.sendMember(c.getLogin(), e.getKey());
		}
		if(clipboardText != null) {
			connection.sendClipboardText(clipboardText);
		}
		connection.setLogin(packet.login);
	}
	
	public void onClipboardUpdated(InetAddress addr, TextPacket packet) {
		ClientConnection connection = devices.get(addr);
		if(connection == null)
			return;
		clipboardText = packet.text;
		for(Entry<InetAddress, ClientConnection> e : devices.entrySet()) {
			ClientConnection c = e.getValue();
			c.sendPacket(packet);
		}
	}
	
	public void close() {
		for(Entry<InetAddress, ClientConnection> e : devices.entrySet()) {
			e.getValue().close();
		}
		for(Entry<InetAddress, ClientConnection> e : unverifiedDevices.entrySet()) {
			e.getValue().close();
		}
		server.close();
	}
	
	public String getName() {
		return name;
	}
}
