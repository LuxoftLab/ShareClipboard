package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class UDPSender extends Thread {

	public static final int REPETITIONS = 30;
	
	DatagramPacket packet;
	DatagramSocket socket;
	boolean isBroadcast;
	
	public UDPSender(DatagramPacket packet, DatagramSocket socket, boolean isBroadcast) {
		this.packet = packet;
		this.socket = socket;
		this.isBroadcast = isBroadcast;
	}
	
	@Override
	public void run() {
		for(int i=0; i<REPETITIONS; i++) {
			try {
				socket.setBroadcast(isBroadcast);
				socket.send(packet);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	
}
