package com.luxoft.clipboard;

import java.io.DataOutputStream;
import java.io.IOException;

import com.luxoft.clipboard.packets.TCPPacket;

public class TCPSender extends Thread {
	
	DataOutputStream stream;
	TCPPacket packet;
	
	public TCPSender(TCPPacket packet, DataOutputStream stream) {
		this.stream = stream;
		this.packet = packet;
	}
	
	public void run() {
		try {
			stream.writeByte(packet.type);
			packet.write(stream);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
