package se.liu.albhe576.project;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class GPU
{
    private final CommandProcessor commandProcessor;
    private List<Shader> shaders;
    private int shaderIndex;
    public Thread commandProcessorThread;

    public GPU(short imageWidth, short imageHeight){
        this.commandProcessor = new CommandProcessor(imageWidth, imageHeight);
        this.shaders = new ArrayList<Shader>();
        this.shaderIndex = -1;
    }

    public void attachShader(int shaderIndex){
        this.shaderIndex = shaderIndex;
    }

    public void draw(){
        DrawCommand drawCommand = new DrawCommand();
        this.commandProcessor.sendCommmand(drawCommand);
    }

    public int addShader(Shader shader){
        int out = shaders.size();
        shaders.add(shader);
        return out;
    }
    public void runShader(){
        Shader shader = this.shaders.get(this.shaderIndex);
        RunShaderCommand runShaderCommand = new RunShaderCommand(shader);
        this.commandProcessor.sendCommmand(runShaderCommand);
    }
    public void startGPU(){
        this.commandProcessorThread = new Thread(this.commandProcessor);
        this.commandProcessorThread.start();
    }
}
