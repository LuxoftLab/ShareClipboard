package com.luxoft.clipboard.packets;

public class TCPPacketFactory {

	public static TCPPacket obtain(byte type) {
		switch (type) {
		case TCPPacket.AUTH:
			return new AuthPacket();
		case TCPPacket.DEVICE:
			return new DevicePacket();
		case TCPPacket.FAIL:
			return new ErrorPacket();
		case TCPPacket.DELETE_DEVICE:
			return new DeleteDevicePacket();
		default:
			return null;
		}
	}
}
