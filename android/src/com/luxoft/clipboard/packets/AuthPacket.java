package com.luxoft.clipboard.packets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class AuthPacket extends TCPPacket {
	
	public String login, password;
	
	public AuthPacket() {
		super(TCPPacket.AUTH);
	}
	
	public AuthPacket(String login, String password) {
		this();
		this.login = login;
		this.password = password;
	}

	@Override
	public void read(DataInputStream in) throws IOException {
		login = readString(in);
		password = readString(in);
	}

	@Override
	public void write(DataOutputStream out) throws IOException {
		writeString(out, login);
		writeString(out, password);
	}

}
