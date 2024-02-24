package se.liu.albhe576.project;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class Wavefront
{
    List<Vector<Float>> vertices;
    List<Vector<Float>> textureCoordinates;
    List<Vector<Float>> normals;
    List<WavefrontFace[]> faces;

    private Vector<Float> parseVectorFromLine(String line){
	String[] normals = Arrays.stream(line.split(" ")).filter(x -> !x.isEmpty()).toArray(String[]::new);
	float x  = Float.parseFloat(normals[1]);
	float y  = Float.parseFloat(normals[2]);
	float z  = Float.parseFloat(normals[3]);

	return new Vector<Float>(x,y,z);
    }

    private void parseFace(String line){
	String[] lines = line.split(" ");

	WavefrontFace[] face = new WavefrontFace[]{
		new WavefrontFace(Arrays.stream(lines[1].split("/")).mapToInt(Integer::parseInt).toArray()),
		new WavefrontFace(Arrays.stream(lines[2].split("/")).mapToInt(Integer::parseInt).toArray()),
		new WavefrontFace(Arrays.stream(lines[3].split("/")).mapToInt(Integer::parseInt).toArray())
	};
	this.faces.add(face);

    }
    public static Wavefront parseWavefrontFromFile(String filePath) throws IOException {
	String fileContent = Files.readString(Path.of(filePath));
	String []lines = fileContent.split("\n");
	Wavefront wavefront = new Wavefront();
	for(String line : lines){
	    if(line.isEmpty()){
		continue;
	    }
	    if(line.charAt(0) == 'v' && line.charAt(1) == 't'){
		wavefront.textureCoordinates.add(wavefront.parseVectorFromLine(line));
	    }
	    else if(line.charAt(0) == 'v' && line.charAt(1) == 'n'){
		wavefront.normals.add(wavefront.parseVectorFromLine(line));
	    }else if(line.charAt(0) == 'v'){
		wavefront.vertices.add(wavefront.parseVectorFromLine(line));
	    }else if(line.charAt(0) == 'f'){
		wavefront.parseFace(line);
	    }
	}

	return wavefront;
    }

    public Wavefront(){
	this.vertices 		= new ArrayList<>();
	this.textureCoordinates = new ArrayList<>();
	this.normals		= new ArrayList<>();
	this.faces		= new ArrayList<>();
    }

    public static void main(String[] args) throws IOException {
	Wavefront wavefront = parseWavefrontFromFile("./african_head.obj");
    }
}
