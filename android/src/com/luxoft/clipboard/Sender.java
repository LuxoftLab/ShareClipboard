package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.util.Log;

class Sender extends Thread {
	
	DatagramSocket socket;
	InetAddress address;
	Packet packet;
	boolean broadcast;

	public Sender(DatagramSocket socket, InetAddress adress, Packet packet, boolean broadcast) {
		this.socket = socket;
		this.address = adress;
		this.packet = packet;
		this.broadcast = broadcast;
	}

	public void run() {
		try {
			socket.setBroadcast(broadcast);
			byte buf[] = packet.getData();
			DatagramPacket packet = new DatagramPacket(buf,buf.length, address, 1234);
			Log.d("udp", "sended");
			for(int i=0; i<5; i++) {
				socket.send(packet);
			}
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

