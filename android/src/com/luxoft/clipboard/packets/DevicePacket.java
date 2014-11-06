package com.luxoft.clipboard.packets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;

public class DevicePacket extends DeleteDevicePacket {
	
	public String login;
	
	public DevicePacket() {
		type = TCPPacket.DEVICE;
	}
	
	public DevicePacket(String login, InetAddress addr) {
		super(addr);
		type = TCPPacket.DEVICE;
		this.login = login;
	}

	@Override
	public void read(DataInputStream in) throws IOException {
		login = readString(in);
		super.read(in);
	}

	@Override
	public void write(DataOutputStream out) throws IOException {
		writeString(out, login);
		super.write(out);
	}

}
