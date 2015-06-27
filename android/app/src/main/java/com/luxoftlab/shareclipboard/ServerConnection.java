package com.luxoftlab.shareclipboard;

import java.io.IOException;
import java.net.InetAddress;

import android.util.Log;

import com.luxoftlab.shareclipboard.packets.AuthPacket;
import com.luxoftlab.shareclipboard.packets.DeleteDevicePacket;
import com.luxoftlab.shareclipboard.packets.DevicePacket;
import com.luxoftlab.shareclipboard.packets.ErrorPacket;
import com.luxoftlab.shareclipboard.packets.TCPPacket;
import com.luxoftlab.shareclipboard.packets.TextPacket;

public class ServerConnection extends Connection {
	private static final String LOG = "serverConnection";
	
	private ClientRoom room;
	
	public ServerConnection(InetAddress host, ClientRoom room) throws IOException {
		super(host);
		this.room = room;
	}
	
	public void sendPasswordAndLogin(String login, String password) {
		sendPacket(new AuthPacket(login, password));
	}

	@Override
	protected void onPacket(TCPPacket packet) {
		Log.d(LOG, "got packet "+packet.type);
		switch(packet.type) {
		case TCPPacket.FAIL:
			room.notifyAboutFail((ErrorPacket) packet);
			break;
		case TCPPacket.DEVICE:
			room.addDevice((DevicePacket) packet);
			break;
		case TCPPacket.DELETE_DEVICE:
			room.deleteDevice((DeleteDevicePacket) packet);
			break;
		case TCPPacket.CLIPBOARD_TEXT:
			room.updateClipboard((TextPacket) packet);
			break;
		default:
			Log.w(LOG, "unexpected package");
			break;	
		}
	}

	@Override
	protected void onDisconnected(InetAddress addr) {
		room.onServerDisconnected();
	}
	
	@Override
	public void onStarted() {
		room.onStarted();
	}
}
