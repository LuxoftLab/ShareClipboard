package com.luxoftlab.shareclipboard;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

import android.util.Log;

import com.luxoftlab.shareclipboard.packets.TCPPacket;
import com.luxoftlab.shareclipboard.packets.TCPPacketFactory;
import com.luxoftlab.shareclipboard.packets.TextPacket;

public abstract class Connection extends Thread {
	private static final String LOG = "connection";
	
	private Socket socket;
	private InetAddress host;
	private DataOutputStream out;
	private boolean enabled = false;
	
	public Connection(InetAddress host) {
		this.host = host;
	}
	
	public Connection(Socket socket) {
		this.socket = socket;
	}
	
	@Override
	public void run() {
		try {
			if(socket == null) socket = new Socket(host, TCPServer.PORT);
			DataInputStream in = new DataInputStream(socket.getInputStream());
			out = new DataOutputStream(socket.getOutputStream());
			onStarted();
			enabled = true;
			while(enabled) {
				Log.d(LOG, "reading header...");
				TCPPacket packet = TCPPacketFactory.obtain(in.readByte());
				Log.d(LOG, "got header...");
				if(packet == null) {
					Log.w(LOG, "undefined packet");
					continue;
				}
				Log.d(LOG, "reading packet...");
				packet.read(in);
				Log.d(LOG, "got packet "+packet.type);
				onPacket(packet);
			}
		} catch (IOException e) {
			Log.w(LOG, "exception in "+socket.getInetAddress().getHostAddress());
			onDisconnected(getInetAddress());
			e.printStackTrace();
		}
	}
	
	public void sendPacket(TCPPacket packet) {
		new TCPSender(packet, out).start();
	}
	
	public void sendClipboardText(String text) {
		sendPacket(new TextPacket(text));
	}
	
	public void close() {
		enabled = false;
		try {
			socket.close();
		} catch (IOException e) {
			Log.w(LOG, "cannot close socket");
			e.printStackTrace();
		}
	}
	
	public InetAddress getInetAddress() {
		return socket.getInetAddress();
	}
	
	public void onStarted() {
		
	}
	
	protected abstract void onPacket(TCPPacket packet);
	protected abstract void onDisconnected(InetAddress addr);
	
}
