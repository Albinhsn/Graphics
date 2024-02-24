package se.liu.albhe576.project;

import org.lwjgl.BufferUtils;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;


public class TGAImage
{

    private byte[] fileBytes;
    private int index;
    private TargaHeader targaHeader;
    private byte bpp;
    private class TargaHeader{
	byte []header;

	@Override public String toString() {
	    return String.format(
		    "CIIF: %d\nColorMap: %d\nImageType: %d\nColorMapSpec: %d,%d,%d,%d,%d\nxOrigin: %d\nyOrigin: %d\nWidth: %d\nHeight: %d\nImagePixelSize: %d\nImageDescriptor: %d\n",
		    this.header[0],
		    this.header[1],
		    this.header[2],
		    this.header[3],
		    this.header[4],
		    this.header[5],
		    this.header[6],
		    this.header[7],
		    this.getXOrigin(),
		    this.getYOrigin(),
		    this.getWidth(),
		    this.getHeight(),
		    this.getImagePixelSize(),
		    this.getImageDescriptor()
	    );
	}

	private byte getCharactersInIdenificationField(){
	    return this.header[0];
	}
	private byte getColorMapType(){
	    return this.header[1];
	}
	private byte getImageType(){
	    return this.header[2];
	}
	private byte [] getColorMapSpec(){
	    byte[] colorMapSpec = new byte[]{
		    this.header[3],
		    this.header[4],
		    this.header[5],
		    this.header[6],
		    this.header[7],
	    };
	    return colorMapSpec;
	}
	private short getXOrigin(){
	    return (short)((this.header[8] << 8) | this.header[9]);
	}
	private short getYOrigin(){
	    return (short)((this.header[11] << 8) | this.header[10]);
	}
	private short getWidth(){
	    return (short)(((this.header[13] & 0xFF) << 8)  |   (this.header[12] & 0xFF));
	}
	private short getHeight(){
	    return (short)(((this.header[15] & 0xFF) << 8)  |   (this.header[14] & 0xFF));
	}
	private byte getImagePixelSize(){
	    return this.header[16];
	}
	private byte getImageDescriptor(){
	    return this.header[17];
	}

	private void putByte(byte b, int index){
	    this.header[index] = b;
	}
	private TargaHeader(byte[] image){
	    this.header = new byte[18];
	}
    }
    public static void encodeAndSaveTGAImage(ByteBuffer imageBuffer, String filePath){
	//ToDo actually do this
    }

    private void parseTargaHeader(){
	for(; this.index < 18; this.index++){
	    targaHeader.putByte(this.fileBytes[this.index], this.index);
	}
	this.bpp = (byte) (this.targaHeader.getImagePixelSize() / 8);
    }

    private int getImageSizeInBytes(){
	return 4 * this.targaHeader.getWidth() * this.targaHeader.getHeight();
    }

    private TGAImage(String filePath) throws IOException {
	this.fileBytes = Files.readAllBytes(Path.of(filePath));
	this.index = 0;
	this.targaHeader = new TargaHeader(this.fileBytes);

    }
    private ByteBuffer allocateImageBuffer(){
	int imageSize = getImageSizeInBytes();
	System.out.println("Allocating imagebuffer with size " + imageSize);
	return BufferUtils.createByteBuffer(imageSize);
    }
    private byte[] parseColor(){
	byte[] color = new byte[]{
		this.fileBytes[this.index + 0],
		this.fileBytes[this.index + 1],
		this.fileBytes[this.index + 2],
		this.bpp == 3 ? (byte) 255 : this.fileBytes[this.index + 3]
	};
	this.index += this.bpp;
	return color;
    }
    private byte[] parseRunLengthEncodedImage(){
	int imageSize = this.getImageSizeInBytes();
	byte[] imageData = new byte[imageSize];


	for(int imageIndex = 0; imageIndex < imageSize;){
	    char currentByte = (char) this.fileBytes[this.index];
	    this.index++;

	    if(currentByte > 128){
		byte[] color = this.parseColor();
		int repeatsFor = (int) currentByte - 127;
		for(int i = 0; i < repeatsFor; i++){
		    imageData[imageIndex + i * this.bpp + 0] = color[2];
		    imageData[imageIndex + i * this.bpp + 1] = color[1];
		    imageData[imageIndex + i * this.bpp + 2] = color[0];
		    imageData[imageIndex + i * this.bpp + 3] = color[3];
	       }
		imageIndex += repeatsFor * this.bpp;
	   }else{
		int repeatsFor = currentByte + 1;
		for(int i = 0; i < repeatsFor; i++){
		    byte[] color = this.parseColor();
		    imageData[imageIndex + i * this.bpp + 0] = color[2];
		    imageData[imageIndex + i * this.bpp + 1] = color[1];
		    imageData[imageIndex + i * this.bpp + 2] = color[0];
		    imageData[imageIndex + i * this.bpp + 3] = color[3];
		}
		imageIndex += repeatsFor * this.bpp;
	   }
	}
	return imageData;
    }

    private byte[] parseUncompressed(){
	if(this.bpp == 4){
	    return Arrays.copyOfRange(this.fileBytes, 18, this.fileBytes.length - 1);
	}


	if(this.bpp == 3){
	    int imageSizeBytes = getImageSizeInBytes();
	    int imageSize = imageSizeBytes / 4;
	    byte[] imageData = new byte[imageSizeBytes];
	    for(int idx = 0; idx < imageSize; idx++){
		byte[] color = this.parseColor();
		imageData[idx * 4 + 0] = color[2];
		imageData[idx * 4 + 1] = color[1];
		imageData[idx * 4 + 2] = color[0];
		imageData[idx * 4 + 3] = color[3];
	    }
	    System.out.printf("Last written to %d\n", (imageSize - 1) * 4 + 3);

	    return imageData;

	}
	System.out.printf("Havn't implemented this parsing this bpp yet %d\n", this.bpp);
	System.exit(1);
	return null;

    }
    private ByteBuffer parseImage(){
	this.parseTargaHeader();
	System.out.println(this.index);
	this.index += this.targaHeader.getCharactersInIdenificationField();


	ByteBuffer imageBuffer = this.allocateImageBuffer();
	byte[] imageData = null;

	switch(this.targaHeader.getImageType()){
	    // Uncompressed
	    case 2:{
		System.out.println("Parsing uncompressed");
		imageData = this.parseUncompressed();
		break;
	    }
	    // RLE
	    case 10:{
		System.out.println("Parsing RLE");
		imageData = this.parseRunLengthEncodedImage();
		break;
	    }
	    default:{
		System.out.printf("Don't know how to parse this encoding %d\n", this.targaHeader.getImageType());
		System.exit(2);
	    }
	}
	imageBuffer.put(imageData);
	return imageBuffer;
    }

    public static Framebuffer decodeTGAImageFromFile(String filePath) throws IOException {
	TGAImage tgaImage = new TGAImage(filePath);

	ByteBuffer buffer = tgaImage.parseImage();
	buffer.flip();
	return new Framebuffer(tgaImage.targaHeader.getWidth(), tgaImage.targaHeader.getHeight(), buffer);
    }
}
