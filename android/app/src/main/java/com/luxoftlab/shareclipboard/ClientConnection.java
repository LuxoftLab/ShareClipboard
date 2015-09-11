package com.luxoftlab.shareclipboard;

import java.net.InetAddress;
import java.net.Socket;

import com.luxoftlab.shareclipboard.packets.AuthPacket;
import com.luxoftlab.shareclipboard.packets.DeleteDevicePacket;
import com.luxoftlab.shareclipboard.packets.DevicePacket;
import com.luxoftlab.shareclipboard.packets.ErrorPacket;
import com.luxoftlab.shareclipboard.packets.TCPPacket;
import com.luxoftlab.shareclipboard.packets.TextPacket;

import android.util.Log;

public class ClientConnection extends Connection {
	private static final String LOG = "clientConnection";
	
	private ServerRoom room;
	private String login;
	
	public ClientConnection(ServerRoom room, Socket socket) {
		super(socket);
		this.room = room;
	}
	
	public void setLogin(String login) {
		this.login = login;
	}
	
	public String getLogin() {
		return login;
	}
	
	public void sendFail() {
		sendPacket(new ErrorPacket(0));
	}
	
	public void sendMember(String login, InetAddress addr) {
		sendPacket(new DevicePacket(login, addr));
	}
	
	public void deleteMember(InetAddress addr) {
		sendPacket(new DeleteDevicePacket(addr));
	}
	
	@Override
	protected void onPacket(TCPPacket packet) {
		switch(packet.type) {
		case TCPPacket.AUTH:
			room.verifyPassword(getInetAddress(), (AuthPacket)packet);
			break;
		case TCPPacket.CLIPBOARD_TEXT:
			room.onClipboardUpdated(getInetAddress(), (TextPacket)packet);
			break;
		default:
			Log.w(LOG, "unexpected package");
			break;
		}
	}

	@Override
	protected void onDisconnected(InetAddress addr) {
		room.deleteDevice(addr);	
	}
	
}
