package se.liu.albhe576.tetris;

import se.liu.albhe576.project.GPU;
import se.liu.albhe576.project.Wavefront;
import se.liu.albhe576.project.WavefrontShader;

import java.io.IOException;

public class Game {
    GPU gpu;

    public Game(short imageWidth, short imageHeight){
        this.gpu = new GPU(imageWidth, imageHeight);
    }

    public void runGame() throws IOException {
        this.gpu.startGPU();
        Wavefront wavefront = Wavefront.parseWavefrontFromFile("./african_head.obj");
        WavefrontShader wavefrontShader = new WavefrontShader(wavefront);

        int shaderIdx = this.gpu.addShader(wavefrontShader);
        this.gpu.attachShader(shaderIdx);
        this.gpu.runShader();
        this.gpu.draw();

        while(this.gpu.commandProcessorThread.isAlive()){

        }
        System.out.println("Exiting game!");
    }


    public static void main(String[] args) throws IOException {
        Game game = new Game((short)1024, (short)768);
        game.runGame();

    }
}
