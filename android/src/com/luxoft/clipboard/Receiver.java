package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.util.Log;

public class Receiver extends Thread {
	
	private ClipboardService service;
	private DatagramSocket socket;
	private InetAddress local;
	private boolean enabled;
	
	public Receiver(ClipboardService service, DatagramSocket socket, InetAddress local) {
		this.service = service;
		this.socket = socket;
		this.local = local;
	}
	
	public void run() {
		Log.d("udp", "run");
		try {
			enabled = true;
			byte buf[] = new byte[1024];
			DatagramPacket packet = new DatagramPacket(buf, buf.length);
			while(enabled) {
				socket.setBroadcast(false);
				Log.d("udp", "wait...");
				socket.receive(packet);
				Log.d("udp", "receive");
				if(!packet.getAddress().equals(local)) {
					service.onReceiveUDP(packet);
				}
			} 
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Log.d("udp", "stop");
	}
	
	public void disable() {
		enabled = false;
	}
}
