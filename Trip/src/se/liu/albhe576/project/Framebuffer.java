package se.liu.albhe576.project;

import org.lwjgl.BufferUtils;

import java.nio.ByteBuffer;

public class Framebuffer
{
    private final short width;
    private final short height;
    public ByteBuffer buffer;
	public float[] zBuffer;

    public short getWidth(){
	return this.width;
    }
    public short getHeight(){
	return this.height;
    }
    public ByteBuffer getBuffer(){
	return this.buffer;
    }

    public void setPixel(short x, short y, byte [] color){
	int idxOffset = (y * this.width + x) * 4;
	this.buffer.put(idxOffset, color);
    }
    public Framebuffer(short width, short height, ByteBuffer buffer){
	final int bpp = 4;
	this.width = width;
	this.height = height;
	this.buffer = buffer;
    }

    public Framebuffer(short width, short height){
	final int bpp = 4;
	this.width = width;
	this.height = height;
	this.buffer = BufferUtils.createByteBuffer(width * height * bpp);
	for(int i = 0; i < width * height * 4; i+= 4){
	    this.buffer.put(i + 0 , (byte)0);
	    this.buffer.put(i + 1, (byte)0);
	    this.buffer.put(i + 2 , (byte)0);
	    this.buffer.put(i + 3 , (byte)0xFF);
	}
    }
}
