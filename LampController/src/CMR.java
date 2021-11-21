// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;
import java.io.OutputStream;


/// Compact measurement record packet.
public final class CMR {
	public final int type;
	public final byte[] data;

	public final static int	PING	= 0x0000;
	public final static int LAMPNET	= 0xBC07;
	public final static int LAMPNET_MULTIPACKET_BEGIN	= 0xBC08;
	public final static int LAMPNET_MULTIPACKET_PAYLOAD	= 0xBC09;
	public final static int LAMPNET_MULTIPACKET_END		= 0xBC0A;

	/** Maximum payload length for emitting multi-packets instead. */
	public final static int MULTIPACKET_PAYLOAD_THRESHOLD = 0xFF-6;

	/// Construct new CMR.
	public CMR(int type, byte[] data)
	{
		this.type = type;
		this.data = data;
	}

	/// Construct new CMR.
	public CMR(int type, String data)
	{
		this.type = type;
		this.data = data.getBytes();
	}

	/// Static version of emit.
	public final static void emit(OutputStream os, int type, byte[] data) throws java.io.IOException
	{
		if (type == PING) {
			os.write(0x00);
		} else {
			if (type == LAMPNET && data.length>MULTIPACKET_PAYLOAD_THRESHOLD) {
				byte[]	mpacketbuffer = new byte[MULTIPACKET_PAYLOAD_THRESHOLD];
				int		npackets = (data.length + MULTIPACKET_PAYLOAD_THRESHOLD - 1) / MULTIPACKET_PAYLOAD_THRESHOLD;
				int		last_packet_size = data.length % MULTIPACKET_PAYLOAD_THRESHOLD;
				if (last_packet_size == 0) {
					last_packet_size = MULTIPACKET_PAYLOAD_THRESHOLD;
				}

				for (int packetIndex=0; packetIndex<npackets; ++packetIndex) {
					int offset = packetIndex*MULTIPACKET_PAYLOAD_THRESHOLD;
					if (packetIndex+1<npackets) {
						for (int dataIndex=0; dataIndex<MULTIPACKET_PAYLOAD_THRESHOLD; ++dataIndex) {
							mpacketbuffer[dataIndex] = data[offset + dataIndex];
						}
						emit(os, packetIndex==0 ? LAMPNET_MULTIPACKET_BEGIN : LAMPNET_MULTIPACKET_PAYLOAD, mpacketbuffer);
					} else {
						int	this_round = packetIndex+1<npackets ? MULTIPACKET_PAYLOAD_THRESHOLD : last_packet_size;
						if (this_round != mpacketbuffer.length) {
							mpacketbuffer = new byte[this_round];
						}
						for (int dataIndex=0; dataIndex<this_round; ++dataIndex) {
							mpacketbuffer[dataIndex] = data[offset + dataIndex];
						}
						emit(os, LAMPNET_MULTIPACKET_END, mpacketbuffer);
					}
				}
			} else {
				byte[] buffer = new byte[data.length + 6];
				buffer[0] = 0x02; // header
				buffer[1] = (byte)((type & 0xFF00) >> 8);
				buffer[2] = (byte)( type & 0x00FF);
				buffer[3] = (byte)(data.length);

				byte checksum = (byte)(buffer[1] + buffer[2] + buffer[3]);
				int i;
				for (i=0; i<data.length; ++i) {
					buffer[i + 4] = data[i];
					checksum = (byte)(checksum + data[i]);
				}
				buffer[data.length + 4] = checksum;
				buffer[data.length + 5] = 0x03; // trailer
				os.write(buffer, 0, buffer.length);
				os.flush();
			}
		}
	}

	/// Static version of emit.
	public final static void emit(OutputStream os, int type, String data) throws java.io.IOException
	{
		emit(os, type, data.getBytes());
	}

	/// Emit serialized data to outputstream.
	public final void emit(OutputStream os) throws java.io.IOException
	{
		emit(os, type, data);
	}

	public final String toString()
	{
		switch (type) {
		case PING:
			return "PING";
		case LAMPNET:
			{
				StringBuffer	sb = new StringBuffer();

				for (int i=0; i<data.length; ++i) {
					// TODO: detect ASCII chars.
					sb.append((char)data[i]);
				}

				return sb.toString();
			}
		default:
			{
				StringBuffer	sb = new StringBuffer();

				sb.append("[" + type + " : ");

				for (int i=0; i<data.length; ++i) {
					if (i>0) {
						sb.append(' ');
					}
					sb.append(data[i]);
				}

				sb.append(']');
				return sb.toString();
			}
		}
	}
} // class CMR

