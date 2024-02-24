package se.liu.albhe576.project;

import java.io.IOException;
import java.util.List;

public class GPU
{
    CommandProcessor commandProcessor;

    Rasterizer rasterizer;
    Image image;
    private final GraphicsLayer graphicsLayer;

    public GPU(short imageWidth, short imageHeight){
        this.image = new Image(imageWidth, imageHeight);
        this.rasterizer = new Rasterizer();
        this.commandProcessor = new CommandProcessor();
        this.graphicsLayer = new GraphicsLayer();
    }

    public void draw(){
	// Send of command to commandProcessor
	// 	need some sort of boolean back so we can then do the actual send of with the framebuffer and render the screen
    }
    // list of shader
    // current shader index
    List<Shader> shaders;
    int currentShaderIndex;

    // list of buffers
    // current buffer index

    // list of textures
    // current texture index

    // lookAt
    public static void main(String[] args) throws IOException {
        GPU gpu = new GPU((short)1024, (short)768);
        Thread graphicsLayerThread = new Thread(gpu.graphicsLayer);
        graphicsLayerThread.start();
        Wavefront wavefront = Wavefront.parseWavefrontFromFile("./african_head.obj");
        gpu.rasterizer.rasterWavefront(gpu.image, wavefront);

        while(graphicsLayerThread.isAlive()){
            gpu.graphicsLayer.sendDrawCall(gpu.image);
        }
    }
}
