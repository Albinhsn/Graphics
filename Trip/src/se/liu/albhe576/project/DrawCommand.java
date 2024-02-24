package se.liu.albhe576.project;

public class DrawCommand implements Command{
    public DrawCommand(){

    }
    @Override
    public void execute(GPUState state) {
        state.graphicsLayer.sendDrawCall(state.framebuffer);
    }
}
