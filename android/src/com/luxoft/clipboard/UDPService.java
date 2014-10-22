package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

public class UDPService extends Thread {
	public static String LOG = "udpservice";
	public static final int PORT = 1234;
	
	Controller controller;
	DatagramSocket socket;
	InetAddress address, broadcast;
	
	boolean enabled;
	
	public UDPService(Controller _controller) throws IOException {
		controller = _controller;
		socket = new DatagramSocket(PORT);
		address = getLocalAddress();
		broadcast = getBroadcastAddress();
	}
	
	public void run() {
		byte buf[] = new byte[1024]; //TODO delete magic numbers
		DatagramPacket packet = new DatagramPacket(buf, buf.length);
		enabled = true;
		while(enabled) {
			try {
				socket.setBroadcast(false);
				socket.receive(packet);
				if(!packet.getAddress().equals(address)) {
					processPacket(packet);
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public void getRooms() {
		UDPPackage output = new UDPPackage(UDPPackage.GET_ROOM);
		new Sender(broadcast, output, true);
	}
	
	public void sendRoom(String name, InetAddress addr) {
		UDPPackage output = new UDPPackage(UDPPackage.NEW_ROOM, name);
		new Sender(addr, output, false);
	}
	
	private void processPacket(DatagramPacket packet) {
		UDPPackage input = new UDPPackage(packet.getData(), packet.getOffset(), packet.getLength());
		//TODO skip package with same id
		switch(input.getType()) {
		case UDPPackage.NEW_ROOM:
			controller.onGetRoom(packet.getAddress());
			break;
		case UDPPackage.GET_ROOM:
			controller.addRoom(packet.getAddress(), input.getContent());
			break;
		default:
			controller.unknownPackage();
		}
	}
	
	private InetAddress getLocalAddress() throws IOException {
		WifiManager wifi = (WifiManager) controller.getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		byte[] quads = new byte[4];
		int ip = dhcp.ipAddress;
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((ip >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}
	
	private InetAddress getBroadcastAddress() throws IOException {
		WifiManager wifi = (WifiManager) controller.getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		byte[] quads = new byte[4];
		int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}
	
	class Sender extends Thread {
		
		InetAddress address;
		UDPPackage packet;
		boolean broadcast;

		public Sender(InetAddress adress, UDPPackage packet, boolean broadcast) {
			this.address = adress;
			this.packet = packet;
			this.broadcast = broadcast;
		}

		public void run() {
			try {
				socket.setBroadcast(broadcast);
				byte buf[] = packet.toBytes();
				DatagramPacket packet = new DatagramPacket(buf,buf.length, address, PORT);
				Log.d(LOG, "send...");
				for(int i=0; i<5; i++) {
					socket.send(packet);
				}
			} catch (SocketException e) {
				Log.d(LOG, "fail to send");
				e.printStackTrace();
			} catch (IOException e) {
				Log.d(LOG, "fail to send");
				e.printStackTrace();
			}
		}
	}
	
}
