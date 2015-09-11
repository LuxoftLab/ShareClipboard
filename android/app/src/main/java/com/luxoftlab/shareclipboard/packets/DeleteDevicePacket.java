package com.luxoftlab.shareclipboard.packets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;

public class DeleteDevicePacket extends TCPPacket {

	public InetAddress addr;
	
	public DeleteDevicePacket() {
		super(TCPPacket.DELETE_DEVICE);
	}
	
	public DeleteDevicePacket(InetAddress addr) {
		this();
		this.addr = addr;
	}
	
	@Override
	public void read(DataInputStream in) throws IOException {
		byte[] data = new byte[4];
		in.read(data);
		addr = InetAddress.getByAddress(data);
	}

	@Override
	public void write(DataOutputStream out) throws IOException {
		out.write(addr.getAddress());
	}

}
