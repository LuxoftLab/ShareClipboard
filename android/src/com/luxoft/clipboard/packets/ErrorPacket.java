package com.luxoft.clipboard.packets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class ErrorPacket extends TCPPacket {

	public int value;
	
	public ErrorPacket() {
		super(TCPPacket.FAIL);
	}
	
	public ErrorPacket(int val) {
		this();
		value = val;
	}

	@Override
	public void read(DataInputStream in) throws IOException {
		value = in.readInt();
	}

	@Override
	public void write(DataOutputStream out) throws IOException {
		out.writeInt(value);
	}

}
