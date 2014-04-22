package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.os.Handler;
import android.util.Log;

public class Receiver extends Thread {
	
	Main m;
	DatagramSocket socket;
	
	public Receiver(Main main, DatagramSocket s) {
		m = main;
		socket = s;
	}
	
	public void run() {
		Log.d("udp", "run");
		try {
			socket.setBroadcast(false);
			byte buf[] = new byte[64];
			DatagramPacket packet = new DatagramPacket(buf, buf.length);
			//m.send("wait");
			socket.receive(packet);
			//m.send("received");
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Log.d("udp", "stop");
	}
}
